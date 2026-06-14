#include "scheduler.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace student {

namespace {

constexpr uint64_t kMinQuantumUs = 3;
constexpr uint64_t kMaxQuantumUs = 18;
constexpr double kPreemptDebtMarginUs = 1.5;
constexpr double kWakeupPlacementDebtMarginUs = 6.0;
constexpr double kSleeperCarryMarginUs = 12.0;
constexpr double kParallelPenaltyUs = 10.0;
constexpr double kLiveSliceServiceFactor = 0.5;
constexpr double kFairParallelSlack = 0.15;

uint64_t safe_weight(uint64_t weight) noexcept {
  return weight == 0 ? 1 : weight;
}

std::size_t task_index(uint64_t task_id) noexcept {
  return static_cast<std::size_t>(task_id);
}

}  // namespace

void Scheduler::init(const schedlab::SystemView& system) {
  const std::lock_guard<std::mutex> lock(mu_);
  groups_.clear();
  task_accounted_runtime_.clear();
  task_runnable_.clear();
  (void)system;
}

Scheduler::GroupState& Scheduler::group_state_locked(const schedlab::TaskView& task) {
  GroupState& state = groups_[task.group_id];
  state.weight = safe_weight(task.weight);
  const std::size_t index = task_index(task.task_id);
  if (task_accounted_runtime_.size() <= index) {
    task_accounted_runtime_.resize(index + 1, 0);
  }
  if (task_runnable_.size() <= index) {
    task_runnable_.resize(index + 1, 0);
  }
  return state;
}

const Scheduler::GroupState* Scheduler::find_group_locked(uint64_t group_id) const {
  const auto it = groups_.find(group_id);
  if (it == groups_.end()) {
    return nullptr;
  }
  return &it->second;
}

double Scheduler::active_floor_locked() const {
  double floor = std::numeric_limits<double>::infinity();
  for (const auto& [group_id, state] : groups_) {
    (void)group_id;
    if (state.runnable_count == 0) {
      continue;
    }
    floor = std::min(floor, state.service / static_cast<double>(safe_weight(state.weight)));
  }
  if (!std::isfinite(floor)) {
    return 0.0;
  }
  return floor;
}

uint64_t Scheduler::active_weight_locked() const {
  uint64_t total = 0;
  for (const auto& [group_id, state] : groups_) {
    (void)group_id;
    if (state.runnable_count == 0) {
      continue;
    }
    total += safe_weight(state.weight);
  }
  return total;
}

double Scheduler::normalized_service_locked(const schedlab::TaskView& task) const {
  return normalized_service_locked(task.group_id, task.weight);
}

double Scheduler::normalized_service_locked(uint64_t group_id, uint64_t fallback_weight) const {
  const GroupState* state = find_group_locked(group_id);
  const uint64_t weight = state == nullptr ? safe_weight(fallback_weight) : safe_weight(state->weight);
  const double service = state == nullptr ? 0.0 : state->service;
  return service / static_cast<double>(weight);
}

double Scheduler::live_normalized_service_locked(
    const schedlab::TaskView& task,
    const schedlab::SystemView& system) const {
  double service = 0.0;
  if (const GroupState* state = find_group_locked(task.group_id); state != nullptr) {
    service = state->service;
  }
  for (const auto& worker : system.worker_states()) {
    if (worker.is_idle || !worker.running_task_id.has_value()) {
      continue;
    }
    const schedlab::TaskViewRef running = system.running_task(worker.worker_id);
    if (running && running->group_id == task.group_id) {
      service += kLiveSliceServiceFactor * static_cast<double>(running->current_slice_runtime_us);
    }
  }
  return service / static_cast<double>(safe_weight(task.weight));
}

uint32_t Scheduler::running_count_locked(uint64_t group_id,
                                         const schedlab::SystemView& system) const {
  uint32_t running = 0;
  for (const auto& worker : system.worker_states()) {
    if (worker.is_idle || !worker.running_task_id.has_value()) {
      continue;
    }
    const schedlab::TaskViewRef task = system.running_task(worker.worker_id);
    if (task && task->group_id == group_id) {
      ++running;
    }
  }
  return running;
}

uint32_t Scheduler::total_running_locked(const schedlab::SystemView& system) const {
  uint32_t total = 0;
  for (const auto& worker : system.worker_states()) {
    if (!worker.is_idle && worker.running_task_id.has_value()) {
      ++total;
    }
  }
  return total;
}

uint32_t Scheduler::fair_parallel_limit_locked(const schedlab::SystemView& system) const {
  const uint32_t worker_count = system.total_worker_count();
  if (worker_count <= 1) {
    return worker_count;
  }

  const uint64_t active_weight = active_weight_locked();
  if (active_weight == 0) {
    return worker_count;
  }

  uint32_t limited_parallelism = worker_count;
  bool saw_limited_group = false;
  for (const auto& [group_id, state] : groups_) {
    if (state.runnable_count == 0) {
      continue;
    }
    if (state.runnable_count >= worker_count) {
      continue;
    }
    const double ideal_parallel =
        static_cast<double>(worker_count) *
        (static_cast<double>(safe_weight(state.weight)) / static_cast<double>(active_weight));
    if (ideal_parallel <= static_cast<double>(state.runnable_count) + kFairParallelSlack) {
      continue;
    }
    limited_parallelism =
        std::min<uint32_t>(limited_parallelism,
                           static_cast<uint32_t>(state.runnable_count + 1));
    saw_limited_group = true;
  }

  if (!saw_limited_group) {
    return worker_count;
  }
  return std::max<uint32_t>(1, limited_parallelism);
}

double Scheduler::dispatch_score_locked(const schedlab::TaskView& task,
                                        const schedlab::SystemView& system) const {
  const double parallel =
      static_cast<double>(running_count_locked(task.group_id, system)) /
      static_cast<double>(safe_weight(task.weight));
  return live_normalized_service_locked(task, system) + kParallelPenaltyUs * parallel;
}

uint64_t Scheduler::quantum_for(const schedlab::TaskView& task,
                                const schedlab::SystemView& system) const {
  const uint64_t switch_cost = system.switch_cost();
  const uint64_t base = std::max<uint64_t>(kMinQuantumUs, switch_cost + 1);

  // Fairness scoring uses windowed group service.  Keep slices short enough to
  // correct skew quickly, while avoiding the baseline's very expensive 20us RR.
  if (task.voluntary_block_count > 0 || task.avg_slice_us > 0) {
    return std::min<uint64_t>(kMaxQuantumUs, std::max<uint64_t>(base, 5));
  }
  return std::min<uint64_t>(kMaxQuantumUs, std::max<uint64_t>(base, 6));
}

Scheduler::WorkerChoice Scheduler::best_worker_for_locked(
    const schedlab::TaskView& task,
    schedlab::ReadyContext ctx,
    const schedlab::SystemView& system) const {
  const uint32_t workers = system.total_worker_count();
  if (workers == 0) {
    return WorkerChoice{};
  }

  auto load_of = [&](const schedlab::SystemView::WorkerState& state) {
    return state.local_queue_size + (state.is_idle ? 0u : 1u);
  };

  WorkerChoice best{
      .worker_id = 0,
      .load = std::numeric_limits<uint32_t>::max(),
  };

  const int affinity_worker =
      ctx.source_worker_id >= 0 ? ctx.source_worker_id : ctx.previous_worker_id;

  for (const auto& state : system.worker_states()) {
    if (state.worker_id < 0) {
      continue;
    }
    uint32_t load = load_of(state);
    if (affinity_worker >= 0 && state.worker_id != affinity_worker) {
      load += static_cast<uint32_t>(std::min<uint64_t>(system.migration_cost(affinity_worker,
                                                                             state.worker_id),
                                                      16));
    }
    if (load < best.load || (load == best.load && state.worker_id < best.worker_id)) {
      best = WorkerChoice{
          .worker_id = state.worker_id,
          .load = load,
      };
    }
  }

  if (best.load == std::numeric_limits<uint32_t>::max()) {
    best = WorkerChoice{};
  }

  if (affinity_worker >= 0 && affinity_worker < static_cast<int>(workers)) {
    const auto& states = system.worker_states();
    const auto affinity_it =
        std::find_if(states.begin(), states.end(),
                     [&](const schedlab::SystemView::WorkerState& state) {
                       return state.worker_id == affinity_worker;
                     });
    if (affinity_it != states.end()) {
      const uint32_t affinity_load = load_of(*affinity_it);
      const double task_norm = dispatch_score_locked(task, system);
      const schedlab::TaskViewRef current = system.running_task(affinity_worker);
      const bool can_fix_skew =
          !current ||
          dispatch_score_locked(*current, system) > task_norm + kWakeupPlacementDebtMarginUs;
      if (affinity_load <= best.load + 1 && can_fix_skew) {
        return WorkerChoice{
            .worker_id = affinity_worker,
            .load = affinity_load,
        };
      }
    }
  }

  return best;
}

void Scheduler::account_stop_locked(const schedlab::TaskView& task) {
  GroupState& state = group_state_locked(task);
  const std::size_t index = task_index(task.task_id);
  if (task_accounted_runtime_.size() <= index) {
    task_accounted_runtime_.resize(index + 1, 0);
  }
  const uint64_t previous = task_accounted_runtime_[index];
  if (task.total_runtime_us > previous) {
    state.service += static_cast<double>(task.total_runtime_us - previous);
    task_accounted_runtime_[index] = task.total_runtime_us;
  }
  if (task.last_stop_reason == schedlab::StopReason::Blocked ||
      task.last_stop_reason == schedlab::StopReason::Exited) {
    mark_not_runnable_locked(task);
  }
}

void Scheduler::mark_runnable_locked(const schedlab::TaskView& task) {
  GroupState& state = group_state_locked(task);
  const std::size_t index = task_index(task.task_id);
  if (task_runnable_.size() <= index) {
    task_runnable_.resize(index + 1, 0);
  }
  if (task_runnable_[index] != 0) {
    return;
  }
  if (state.runnable_count == 0) {
    const double floor = active_floor_locked();
    const double capped_norm =
        std::min(std::max(normalized_service_locked(task), floor), floor + kSleeperCarryMarginUs);
    state.service =
        capped_norm * static_cast<double>(safe_weight(state.weight));
  }
  task_runnable_[index] = 1;
  ++state.runnable_count;
}

void Scheduler::mark_not_runnable_locked(const schedlab::TaskView& task) {
  GroupState& state = group_state_locked(task);
  const std::size_t index = task_index(task.task_id);
  if (task_runnable_.size() <= index || task_runnable_[index] == 0) {
    return;
  }
  task_runnable_[index] = 0;
  if (state.runnable_count > 0) {
    --state.runnable_count;
  }
}

int Scheduler::select_worker(const schedlab::TaskView& task, schedlab::ReadyContext ctx,
                             const schedlab::SystemView& system) {
  const std::lock_guard<std::mutex> lock(mu_);
  mark_runnable_locked(task);
  return best_worker_for_locked(task, ctx, system).worker_id;
}

std::optional<uint64_t> Scheduler::pick_next(int worker_id, schedlab::QueueView candidates,
                                             const schedlab::SystemView& system) {
  if (candidates.empty()) {
    return std::nullopt;
  }

  const std::lock_guard<std::mutex> lock(mu_);
  const schedlab::TaskViewRef current = system.running_task(worker_id);
  const double current_norm =
      current ? dispatch_score_locked(*current, system) : std::numeric_limits<double>::infinity();
  const uint32_t fair_parallel_limit = fair_parallel_limit_locked(system);
  const uint32_t running_total = total_running_locked(system);

  const schedlab::TaskView* best = nullptr;
  double best_score = std::numeric_limits<double>::infinity();
  for (const schedlab::TaskView& task : candidates) {
    group_state_locked(task);
    double score = dispatch_score_locked(task, system);
    if (current && task.group_id == current->group_id) {
      score += 0.25;
    }
    if (task.last_worker_id >= 0 && task.last_worker_id != worker_id) {
      score += static_cast<double>(std::min<uint64_t>(system.migration_cost(task.last_worker_id,
                                                                            worker_id),
                                                     12)) /
               16.0;
    }
    if (best == nullptr || score < best_score ||
        (score == best_score && task.task_id < best->task_id)) {
      best = &task;
      best_score = score;
    }
  }

  if (best == nullptr) {
    return std::nullopt;
  }

  if (!current && running_total >= fair_parallel_limit && best->weight == 1 &&
      running_count_locked(best->group_id, system) > 0) {
    return std::nullopt;
  }

  if (current && current_norm + kPreemptDebtMarginUs < best_score &&
      current->current_slice_runtime_us < quantum_for(*current, system)) {
    return std::nullopt;
  }

  return best->task_id;
}

schedlab::TickAction Scheduler::on_tick(const schedlab::TaskView& current, int worker_id,
                                        const schedlab::SystemView& system) {
  const uint64_t quantum = quantum_for(current, system);
  if (current.current_slice_runtime_us < quantum) {
    return schedlab::TickAction::Continue;
  }

  const std::lock_guard<std::mutex> lock(mu_);
  group_state_locked(current);
  const double current_norm =
      (groups_[current.group_id].service + static_cast<double>(current.current_slice_runtime_us)) /
      static_cast<double>(safe_weight(current.weight));

  double best_waiter_norm = std::numeric_limits<double>::infinity();
  for (const schedlab::TaskView& task : system.queue(worker_id)) {
    group_state_locked(task);
    best_waiter_norm = std::min(best_waiter_norm, dispatch_score_locked(task, system));
  }
  for (const auto& state : system.worker_states()) {
    if (state.worker_id == worker_id || state.is_idle || !state.running_task_id.has_value()) {
      continue;
    }
    const schedlab::TaskViewRef running = system.running_task(state.worker_id);
    if (running && running->group_id != current.group_id) {
      best_waiter_norm = std::min(best_waiter_norm, dispatch_score_locked(*running, system));
    }
  }

  if (best_waiter_norm + kPreemptDebtMarginUs < current_norm) {
    return schedlab::TickAction::RequestResched;
  }
  if (!system.queue(worker_id).empty() &&
      current.current_slice_runtime_us >= std::min<uint64_t>(kMaxQuantumUs, quantum + 3)) {
    return schedlab::TickAction::RequestResched;
  }
  return schedlab::TickAction::Continue;
}

bool Scheduler::should_preempt(const schedlab::TaskView& waking,
                               const schedlab::TaskView& current,
                               int worker_id,
                               const schedlab::SystemView& system) {
  (void)worker_id;
  const std::lock_guard<std::mutex> lock(mu_);
  group_state_locked(waking);
  group_state_locked(current);

  const double waking_norm = dispatch_score_locked(waking, system);
  const double current_norm =
      (groups_[current.group_id].service + static_cast<double>(current.current_slice_runtime_us)) /
      static_cast<double>(safe_weight(current.weight));
  if (waking_norm + kPreemptDebtMarginUs < current_norm) {
    return true;
  }
  return current.current_slice_runtime_us >= quantum_for(current, system) &&
         waking.group_id != current.group_id;
}

std::optional<schedlab::Scheduler::StealResult> Scheduler::steal(
    int thief_worker_id,
    const schedlab::SystemView& system) {
  const std::lock_guard<std::mutex> lock(mu_);

  const schedlab::TaskView* best = nullptr;
  int best_victim = -1;
  double best_score = std::numeric_limits<double>::infinity();

  for (const auto& state : system.worker_states()) {
    if (state.worker_id == thief_worker_id || state.local_queue_size == 0) {
      continue;
    }
    const schedlab::QueueView& queue = system.queue(state.worker_id);
    for (const schedlab::TaskView& task : queue) {
      group_state_locked(task);
      double score = dispatch_score_locked(task, system);
      if (task.last_worker_id >= 0 && task.last_worker_id != thief_worker_id) {
        score += static_cast<double>(std::min<uint64_t>(system.migration_cost(task.last_worker_id,
                                                                              thief_worker_id),
                                                       12)) /
                 16.0;
      }
      if (best == nullptr || score < best_score ||
          (score == best_score && task.task_id < best->task_id)) {
        best = &task;
        best_victim = state.worker_id;
        best_score = score;
      }
    }
  }

  if (best == nullptr || best_victim < 0) {
    return std::nullopt;
  }
  return StealResult{
      .victim_worker_id = best_victim,
      .task_id = best->task_id,
  };
}

void Scheduler::on_task_preempted(const schedlab::TaskView& task, int worker_id) {
  (void)worker_id;
  const std::lock_guard<std::mutex> lock(mu_);
  account_stop_locked(task);
}

void Scheduler::on_task_blocked(const schedlab::TaskView& task, int worker_id) {
  (void)worker_id;
  const std::lock_guard<std::mutex> lock(mu_);
  account_stop_locked(task);
}

void Scheduler::on_task_exited(const schedlab::TaskView& task, int worker_id) {
  (void)worker_id;
  const std::lock_guard<std::mutex> lock(mu_);
  account_stop_locked(task);
}

}  // namespace student
