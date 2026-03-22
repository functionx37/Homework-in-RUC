
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <unordered_map>
#include <cmath>
#include <limits>
#include <algorithm>
#include <sstream>
#include <queue>
#include <map>
#include <memory>

using namespace std;

// --- 常量与枚举定义 ---
constexpr int MAP_HEIGHT = 30;
constexpr int MAP_WIDTH = 40;
constexpr int MAX_TICKS = 256;
constexpr int MYID = 2024201559;
constexpr long long TIME_LIMIT_MS = 950;
constexpr double LETHAL_RISK_THRESHOLD = 4500.0; 

enum class Strategy { EARLY_GROWTH, MID_COMPETE, LATE_SURVIVAL };
enum class TargetType { NONE, FOOD, GROWTH_BEAN, KEY, CHEST, SAFE_CENTER };

// --- 结构体定义 ---
struct Point { int y, x; };
bool operator==(const Point& a, const Point& b) { return a.y == b.y && a.x == b.x; }
bool operator<(const Point& a, const Point& b) { return a.y < b.y || (a.y == b.y && a.x < b.x); }
struct Item{ Point pos; int value; int lifetime; };
struct Snake{ int id; int length; int score; int direction; int shield_cd; int shield_time; bool has_key; vector<Point> body; const Point &get_head() const { return body.front(); } };
struct Chest{ Point pos; int score; };
struct Key{ Point pos; int holder_id; int remaining_time; };
struct SafeZoneBounds{ int x_min, y_min, x_max, y_max; };
struct GameState { int remaining_ticks; vector<Item> items; vector<Snake> snakes; vector<Chest> chests; vector<Key> keys; SafeZoneBounds current_safe_zone; int next_shrink_tick; SafeZoneBounds next_safe_zone; int final_shrink_tick; SafeZoneBounds final_safe_zone; int self_idx; string memory_buffer; const Snake &get_self() const { return snakes[self_idx]; } };
struct Target { TargetType type = TargetType::NONE; Point pos = {-1, -1}; double value = 0.0; double score = 0.0; };

// --- 函数声明 ---
void read_game_state(GameState &s);
void write_memory(const string &data);
Target parse_memory(const string& memory_buffer);
Strategy determine_strategy(int remaining_ticks);
void update_risk_map(double (&risk_map)[MAP_HEIGHT][MAP_WIDTH], const GameState &state);
vector<int> get_safe_moves(const GameState& state, const double (&risk_map)[MAP_HEIGHT][MAP_WIDTH]);
int find_path_bfs(const Point &start, const Point &end, const double (&risk_map)[MAP_HEIGHT][MAP_WIDTH]);
Target find_best_target(const GameState& state, Strategy strategy, const Target& last_target, const double (&risk_map)[MAP_HEIGHT][MAP_WIDTH]);
int failsafe_move(const GameState& state, const vector<int>& safe_moves, const double (&risk_map)[MAP_HEIGHT][MAP_WIDTH]);
int count_reachable_cells(Point start_pos, const double (&risk_map)[MAP_HEIGHT][MAP_WIDTH], int limit);
void add_territory_risk(const GameState& state, const vector<int>& candidate_moves, Strategy strategy, double (&risk_map)[MAP_HEIGHT][MAP_WIDTH]);

// --- 主函数 ---
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    GameState state;
    read_game_state(state);
    
    Strategy strategy = determine_strategy(state.remaining_ticks);
    Target last_target = parse_memory(state.memory_buffer);

    static double risk_map[MAP_HEIGHT][MAP_WIDTH];
    update_risk_map(risk_map, state);

    vector<int> candidate_moves = get_safe_moves(state, risk_map);

    if (candidate_moves.empty()) {
        cout << state.get_self().direction << endl;
        write_memory("STATUS:TRAPPED_EARLY");
        return 0;
    }

    add_territory_risk(state, candidate_moves, strategy, risk_map);
    
    vector<int> final_safe_moves = get_safe_moves(state, risk_map);
    
    if (final_safe_moves.empty()) {
        cout << state.get_self().direction << endl;
        write_memory("STATUS:TRAPPED_BY_TERRAIN");
        return 0;
    }
    
    Target current_target = find_best_target(state, strategy, last_target, risk_map);
    int planned_move = -1;
    if (current_target.type != TargetType::NONE) {
        planned_move = find_path_bfs(state.get_self().get_head(), current_target.pos, risk_map);
    }
    
    int final_decision = -1;
    bool is_plan_safe = find(final_safe_moves.begin(), final_safe_moves.end(), planned_move) != final_safe_moves.end();

    if (is_plan_safe) {
        final_decision = planned_move;
    } else {
        final_decision = failsafe_move(state, final_safe_moves, risk_map);
    }
    
    cout << final_decision << endl;
    
    stringstream memory_stream;
    memory_stream << static_cast<int>(current_target.type) << ":" 
                  << current_target.pos.y << " " << current_target.pos.x << " " 
                  << current_target.score;
    write_memory(memory_stream.str());

    return 0;
}

// --- 辅助函数实现 ---

void update_risk_map(double (&risk_map)[MAP_HEIGHT][MAP_WIDTH], const GameState &state) {
    for (int y = 0; y < MAP_HEIGHT; ++y) fill(risk_map[y], risk_map[y] + MAP_WIDTH, 0.0);
    const auto &self = state.get_self();
    for (int y = 0; y < MAP_HEIGHT; ++y) for (int x = 0; x < MAP_WIDTH; ++x) if (x < state.current_safe_zone.x_min || x > state.current_safe_zone.x_max || y < state.current_safe_zone.y_min || y > state.current_safe_zone.y_max) risk_map[y][x] = numeric_limits<double>::infinity();
    
    // [核心修改] 新增收缩预警机制
    int current_tick = MAX_TICKS - state.remaining_ticks;
    if (state.next_shrink_tick != -1 && current_tick >= state.next_shrink_tick - 2) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int x = 0; x < MAP_WIDTH; ++x) {
                if (x < state.next_safe_zone.x_min || x > state.next_safe_zone.x_max ||
                    y < state.next_safe_zone.y_min || y > state.next_safe_zone.y_max) {
                    risk_map[y][x] = numeric_limits<double>::infinity();
                }
            }
        }
    }

    for (const auto &snake : state.snakes) { if (snake.id == MYID) continue; for (const auto &body_part : snake.body) if (body_part.y >= 0 && body_part.y < MAP_HEIGHT && body_part.x >= 0 && body_part.x < MAP_WIDTH) risk_map[body_part.y][body_part.x] += 5000.0; Point head = snake.body.front(); if (head.y >= 0 && head.y < MAP_HEIGHT && head.x >= 0 && head.x < MAP_WIDTH) risk_map[head.y][head.x] += 10000.0; int dy[] = {0, -1, 0, 1}, dx[] = {-1, 0, 1, 0}; for (int i = 0; i < 4; ++i) { if (snake.length > 1 && (snake.direction + 2) % 4 == i) continue; int ny = head.y + dy[i], nx = head.x + dx[i]; if (ny >= 0 && ny < MAP_HEIGHT && nx >= 0 && nx < MAP_WIDTH) risk_map[ny][nx] += (snake.length >= self.length) ? 4000.0 : 2000.0; } }
    for(const auto& item : state.items) if(item.value == -2) risk_map[item.pos.y][item.pos.x] += 2500.0;
    if (!self.has_key) for (const auto &chest : state.chests) risk_map[chest.pos.y][chest.pos.x] = numeric_limits<double>::infinity();
}

Target find_best_target(const GameState& state, Strategy strategy, const Target& last_target, const double (&risk_map)[MAP_HEIGHT][MAP_WIDTH]) {
    const auto& self = state.get_self(); Point head = self.get_head(); vector<Target> potential_targets;

    if (self.has_key && !state.chests.empty()) {
        const auto& chest = state.chests[0];
        potential_targets.push_back({TargetType::CHEST, chest.pos, (double)chest.score * 3.0});
    }
    
    for (const auto& item : state.items) {
        TargetType type = TargetType::NONE; double value = 0.0; double bonus = 1.0; int lifetime = item.lifetime;
        if (item.value > 0) {
            type = TargetType::FOOD; value = item.value;
            double surrounding_risk = risk_map[item.pos.y][item.pos.x];
            int dy[] = {0, -1, 0, 1}, dx[] = {-1, 0, 1, 0};
            for (int i=0; i<4; ++i) { int ny = item.pos.y + dy[i], nx = item.pos.x + dx[i]; if (ny >= 0 && ny < MAP_HEIGHT && nx >= 0 && nx < MAP_WIDTH) surrounding_risk += risk_map[ny][nx]; }
            // [修改] 早期奖励更高
            if (surrounding_risk < 100.0) {
                bonus = (strategy == Strategy::EARLY_GROWTH) ? 4.0 : 3.0;
            }
        }
        else if (item.value == -1) { type = TargetType::GROWTH_BEAN; value = (strategy == Strategy::EARLY_GROWTH) ? 10.0 : 5.0; }
        else if (item.value == -3 && !state.chests.empty()) { type = TargetType::KEY; value = 15.0 + state.chests[0].score * 0.5; }
        if (type != TargetType::NONE) potential_targets.push_back({type, item.pos, value * bonus, (double)lifetime});
    }

    Target best_target; double max_score = -1.0;

    for (auto& target : potential_targets) {
        int dist = abs(target.pos.y - head.y) + abs(target.pos.x - head.x); if (dist == 0) dist = 1;
        int lifetime = (int)target.score;

        if (dist >= state.remaining_ticks) continue;
        if (lifetime != -1 && dist >= lifetime) continue;

        double final_score;
        if (strategy == Strategy::EARLY_GROWTH) {
            // 早期采用更纯粹的贪婪公式
            final_score = target.value / pow(dist + 1, 1.2);
        } else {
            // 中后期考虑时间回报率
            double time_factor = (double)(state.remaining_ticks - dist) / state.remaining_ticks;
            final_score = (target.value / pow(dist + 1, 1.2)) * time_factor;
        }

        if (dist <= 2) final_score *= 3.0;
        
        target.score = final_score;
        if (target.score > max_score) { max_score = target.score; best_target = target; }
    }

    if (last_target.type != TargetType::NONE) {
        bool last_target_exists = any_of(potential_targets.begin(), potential_targets.end(), [&](const Target& t){ return t.pos == last_target.pos; });
        if (last_target_exists && best_target.score < last_target.score * 1.5) { return last_target; }
    }
    
    if (best_target.type == TargetType::NONE) {
        Point center = { (state.next_safe_zone.y_min + state.next_safe_zone.y_max) / 2, (state.next_safe_zone.x_min + state.next_safe_zone.x_max) / 2 };
        best_target = {TargetType::SAFE_CENTER, center, 1.0};
    }

    return best_target;
}

void add_territory_risk(const GameState& state, const vector<int>& candidate_moves, Strategy strategy, double (&risk_map)[MAP_HEIGHT][MAP_WIDTH]) {
    const auto& self = state.get_self(); Point head = self.get_head();
    int dy[] = {0, -1, 0, 1}, dx[] = {-1, 0, 1, 0};

    // [修改] 根据不同策略，设定不同的空间要求
    double required_freedom;
    if (strategy == Strategy::EARLY_GROWTH) {
        required_freedom = self.length + 5; // 早期更胆大
    } else {
        required_freedom = self.length * 1.5 + 5; // 中后期更谨慎
    }

    for (int dir : candidate_moves) {
        Point next_pos = {head.y + dy[dir], head.x + dx[dir]};
        int freedom = count_reachable_cells(next_pos, risk_map, self.length * 3);

        if (freedom < self.length) {
            risk_map[next_pos.y][next_pos.x] = numeric_limits<double>::infinity();
        } 
        else if (freedom < required_freedom) {
            risk_map[next_pos.y][next_pos.x] += 50000.0 / (1.0 + freedom);
        }
    }
}

void read_game_state(GameState &s){cin>>s.remaining_ticks;int item_count;cin>>item_count;s.items.resize(item_count);for(int i=0;i<item_count;++i){cin>>s.items[i].pos.y>>s.items[i].pos.x>>s.items[i].value>>s.items[i].lifetime;}int snake_count;cin>>snake_count;s.snakes.resize(snake_count);unordered_map<int,int> id2idx;id2idx.reserve(snake_count*2);for(int i=0;i<snake_count;++i){auto &sn=s.snakes[i];cin>>sn.id>>sn.length>>sn.score>>sn.direction>>sn.shield_cd>>sn.shield_time;sn.has_key=false;sn.body.resize(sn.length);for(int j=0;j<sn.length;++j){cin>>sn.body[j].y>>sn.body[j].x;}if(sn.id==MYID)s.self_idx=i;id2idx[sn.id]=i;}int chest_count;cin>>chest_count;s.chests.resize(chest_count);for(int i=0;i<chest_count;++i){cin>>s.chests[i].pos.y>>s.chests[i].pos.x>>s.chests[i].score;}int key_count;cin>>key_count;s.keys.resize(key_count);for(int i=0;i<key_count;++i){auto &key=s.keys[i];cin>>key.pos.y>>key.pos.x>>key.holder_id>>key.remaining_time;if(key.holder_id!=-1){auto it=id2idx.find(key.holder_id);if(it!=id2idx.end()){s.snakes[it->second].has_key=true;}}}cin>>s.current_safe_zone.x_min>>s.current_safe_zone.y_min>>s.current_safe_zone.x_max>>s.current_safe_zone.y_max;cin>>s.next_shrink_tick>>s.next_safe_zone.x_min>>s.next_safe_zone.y_min>>s.next_safe_zone.x_max>>s.next_safe_zone.y_max;cin>>s.final_shrink_tick>>s.final_safe_zone.x_min>>s.final_safe_zone.y_min>>s.final_safe_zone.x_max>>s.final_safe_zone.y_max;string line;if(getline(cin>>ws,line)){s.memory_buffer=line;}}
void write_memory(const string &data){cout<<data<<endl;}
Target parse_memory(const string& memory_buffer){Target t;stringstream ss(memory_buffer);int type_int;char colon;if(ss>>type_int>>colon&&colon==':'&&ss>>t.pos.y>>t.pos.x>>t.score){if(type_int>=0&&type_int<=static_cast<int>(TargetType::SAFE_CENTER)){t.type=static_cast<TargetType>(type_int);}}return t;}
Strategy determine_strategy(int remaining_ticks){int current_tick=MAX_TICKS-remaining_ticks;if(current_tick<=80)return Strategy::EARLY_GROWTH;if(current_tick<=200)return Strategy::MID_COMPETE;return Strategy::LATE_SURVIVAL;}
vector<int> get_safe_moves(const GameState& state, const double (&risk_map)[MAP_HEIGHT][MAP_WIDTH]) { vector<int> safe_moves; const auto& self = state.get_self(); const Point head = self.get_head(); const int current_dir = self.direction; const int dy[] = {0, -1, 0, 1}, dx[] = {-1, 0, 1, 0}; const int relative_dirs[] = {(current_dir + 3) % 4, current_dir, (current_dir + 1) % 4}; for (int move_dir : relative_dirs) { Point next_pos = {head.y + dy[move_dir], head.x + dx[move_dir]}; if (next_pos.y >= 0 && next_pos.y < MAP_HEIGHT && next_pos.x >= 0 && next_pos.x < MAP_WIDTH) { if (risk_map[next_pos.y][next_pos.x] < LETHAL_RISK_THRESHOLD) safe_moves.push_back(move_dir); } } return safe_moves; }
int find_path_bfs(const Point &start, const Point &end, const double (&risk_map)[MAP_HEIGHT][MAP_WIDTH]) { if (start == end) return -1; map<Point, Point> parent; queue<Point> q; q.push(start); map<Point, bool> visited; visited[start] = true; int dy[] = {0, -1, 0, 1}, dx[] = {-1, 0, 1, 0}; while (!q.empty()) { Point curr = q.front(); q.pop(); if (curr == end) { Point step = end; while (parent.count(step) && !(parent[step] == start)) step = parent[step]; if (step.y - start.y == 1) return 3; if (step.y - start.y == -1) return 1; if (step.x - start.x == 1) return 2; if (step.x - start.x == -1) return 0; return -1; } for (int i = 0; i < 4; ++i) { Point next = {curr.y + dy[i], curr.x + dx[i]}; if (next.y >= 0 && next.y < MAP_HEIGHT && next.x >= 0 && next.x < MAP_WIDTH && !visited[next] && risk_map[next.y][next.x] < 1500.0) { visited[next] = true; parent[next] = curr; q.push(next); } } } return -1; }
int failsafe_move(const GameState& state, const vector<int>& safe_moves, const double (&risk_map)[MAP_HEIGHT][MAP_WIDTH]) { if (safe_moves.empty()) return state.get_self().direction; const auto& self = state.get_self(); Point head = self.get_head(); int dy[] = {0, -1, 0, 1}, dx[] = {-1, 0, 1, 0}; double min_risk = numeric_limits<double>::infinity(); vector<int> best_risk_moves; for (int dir : safe_moves) { Point next_pos = {head.y + dy[dir], head.x + dx[dir]}; double current_risk = risk_map[next_pos.y][next_pos.x]; if (current_risk < min_risk) { min_risk = current_risk; best_risk_moves.clear(); best_risk_moves.push_back(dir); } else if (abs(current_risk - min_risk) < 1e-6) best_risk_moves.push_back(dir); } if (best_risk_moves.size() == 1) return best_risk_moves[0]; Point center = { (state.next_safe_zone.y_min + state.next_safe_zone.y_max) / 2, (state.next_safe_zone.x_min + state.next_safe_zone.x_max) / 2 }; int final_decision = best_risk_moves[0]; int min_dist_to_center = numeric_limits<int>::max(); for (int dir : best_risk_moves) { Point next = {head.y + dy[dir], head.x + dx[dir]}; int dist = abs(next.y - center.y) + abs(next.x - center.x); if (dist < min_dist_to_center) { min_dist_to_center = dist; final_decision = dir; } } return final_decision; }
int count_reachable_cells(Point start_pos, const double (&risk_map)[MAP_HEIGHT][MAP_WIDTH], int limit) { queue<Point> q; q.push(start_pos); map<Point, bool> visited; visited[start_pos] = true; int count = 0; int dy[] = {0, -1, 0, 1}, dx[] = {-1, 0, 1, 0}; while (!q.empty() && count < limit) { Point curr = q.front(); q.pop(); count++; for (int i = 0; i < 4; ++i) { Point next = {curr.y + dy[i], curr.x + dx[i]}; if (next.y >= 0 && next.y < MAP_HEIGHT && next.x >= 0 && next.x < MAP_WIDTH && !visited[next] && risk_map[next.y][next.x] < LETHAL_RISK_THRESHOLD) { visited[next] = true; q.push(next); } } } return count; }