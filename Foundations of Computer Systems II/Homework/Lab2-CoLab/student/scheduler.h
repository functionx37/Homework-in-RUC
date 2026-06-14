#pragma once

#include <cstdint>
#include <mutex>
#include <optional>
#include <unordered_map>
#include <vector>

#include "schedlab/scheduler.hpp"

namespace student {

class Scheduler final : public schedlab::Scheduler {
 public:
  void init(const schedlab::SystemView& system) override;

  int select_worker(
      const schedlab::TaskView& task,
      schedlab::ReadyContext ctx,
      const schedlab::SystemView& system) override;

  std::optional<uint64_t> pick_next(
      int worker_id,
      schedlab::QueueView candidates,
      const schedlab::SystemView& system) override;

  schedlab::TickAction on_tick(
      const schedlab::TaskView& current,
      int worker_id,
      const schedlab::SystemView& system) override;

  bool should_preempt(
      const schedlab::TaskView& waking,
      const schedlab::TaskView& current,
      int worker_id,
      const schedlab::SystemView& system) override;

  std::optional<StealResult> steal(
      int thief_worker_id,
      const schedlab::SystemView& system) override;

  void on_task_preempted(const schedlab::TaskView& task, int worker_id) override;
  void on_task_blocked(const schedlab::TaskView& task, int worker_id) override;
  void on_task_exited(const schedlab::TaskView& task, int worker_id) override;

 private:
  struct GroupState {
    uint64_t weight = 1;
    double service = 0.0;
    uint32_t runnable_count = 0;
  };

  struct WorkerChoice {
    int worker_id = 0;
    uint32_t load = 0;
  };

  GroupState& group_state_locked(const schedlab::TaskView& task);
  const GroupState* find_group_locked(uint64_t group_id) const;
  double active_floor_locked() const;
  uint64_t active_weight_locked() const;
  double normalized_service_locked(const schedlab::TaskView& task) const;
  double normalized_service_locked(uint64_t group_id, uint64_t fallback_weight) const;
  double live_normalized_service_locked(
      const schedlab::TaskView& task,
      const schedlab::SystemView& system) const;
  uint32_t running_count_locked(uint64_t group_id, const schedlab::SystemView& system) const;
  uint32_t total_running_locked(const schedlab::SystemView& system) const;
  uint32_t fair_parallel_limit_locked(const schedlab::SystemView& system) const;
  double dispatch_score_locked(
      const schedlab::TaskView& task,
      const schedlab::SystemView& system) const;
  uint64_t quantum_for(const schedlab::TaskView& task, const schedlab::SystemView& system) const;
  WorkerChoice best_worker_for_locked(
      const schedlab::TaskView& task,
      schedlab::ReadyContext ctx,
      const schedlab::SystemView& system) const;
  void account_stop_locked(const schedlab::TaskView& task);
  void mark_runnable_locked(const schedlab::TaskView& task);
  void mark_not_runnable_locked(const schedlab::TaskView& task);

  mutable std::mutex mu_;
  std::unordered_map<uint64_t, GroupState> groups_;
  std::vector<uint64_t> task_accounted_runtime_;
  std::vector<uint8_t> task_runnable_;
};

}  // namespace student
