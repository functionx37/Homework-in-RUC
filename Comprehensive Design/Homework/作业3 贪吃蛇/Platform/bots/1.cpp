#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <random>
#include <queue>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

// ========================== 常量定义 ==========================

// 地图尺寸
constexpr int MAP_WIDTH = 40;
constexpr int MAP_HEIGHT = 30;

// 游戏总刻数
constexpr int MAX_TICKS = 256;

// !! 重要：这是你的学号 !!
constexpr int MY_ID = 1;

// 方向向量: 0-左, 1-上, 2-右, 3-下
const int dy[] = {0, -1, 0, 1};
const int dx[] = {-1, 0, 1, 0};

// 网格中不同元素的值，用于BFS和危险评估
namespace GridValue {
    constexpr int EMPTY = 0;        // 空地
    constexpr int MY_BODY = 1;      // 自己的身体（可穿越）
    constexpr int ENEMY_BODY = 2;   // 敌人身体
    constexpr int ENEMY_HEAD = 3;   // 敌人头部
    constexpr int DANGER_ZONE = 4;  // 危险区域（紧邻敌人头部）
    constexpr int WALL = 5;         // 墙、出界、致命障碍物
};

// ========================== 数据结构 (与模板一致) ==========================

struct Point {
    int y, x;
    bool operator==(const Point& other) const { return y == other.y && x == other.x; }
};

struct Item { Point pos; int value; int lifetime; };
struct Snake {
    int id; int length; int score; int direction;
    int shield_cd; int shield_time; bool has_key = false;
    vector<Point> body;
    const Point& get_head() const { return body.front(); }
};
struct Chest { Point pos; int score; };
struct Key { Point pos; int holder_id; int remaining_time; };
struct SafeZoneBounds { int x_min, y_min, x_max, y_max; };

struct GameState {
    int remaining_ticks;
    vector<Item> items;
    vector<Snake> snakes;
    vector<Chest> chests;
    vector<Key> keys;
    SafeZoneBounds current_safe_zone;
    int next_shrink_tick; SafeZoneBounds next_safe_zone;
    int final_shrink_tick; SafeZoneBounds final_safe_zone;
    int self_idx = -1;
    const Snake& get_self() const { return snakes[self_idx]; }
};

// ========================== 核心算法逻辑 ==========================

// 全局游戏状态和辅助数据
GameState current_state;
int grid[MAP_HEIGHT][MAP_WIDTH];

// 广度优先搜索(BFS)计算可达空间大小
int calculate_freedom(Point start_pos, const Snake& snake) {
    // 检查起点是否合法
    if (start_pos.y < 0 || start_pos.y >= MAP_HEIGHT || start_pos.x < 0 || start_pos.x >= MAP_WIDTH ||
        grid[start_pos.y][start_pos.x] >= GridValue::ENEMY_BODY) {
        return 0;
    }

    queue<Point> q;
    q.push(start_pos);

    bool visited[MAP_HEIGHT][MAP_WIDTH] = {false};
    visited[start_pos.y][start_pos.x] = true;
    int count = 0;

    while (!q.empty()) {
        Point curr = q.front();
        q.pop();
        count++;

        for (int i = 0; i < 4; ++i) {
            Point next = {curr.y + dy[i], curr.x + dx[i]};
            if (next.y >= 0 && next.y < MAP_HEIGHT && next.x >= 0 && next.x < MAP_WIDTH &&
                !visited[next.y][next.x] && grid[next.y][next.x] < GridValue::ENEMY_BODY) { // 任何高于等于ENEMY_BODY的都不可走
                visited[next.y][next.x] = true;
                q.push(next);
            }
        }
    }
    return count;
}

// 预处理函数，构建包含危险区域信息的游戏网格
void prepare_grid() {
    const auto& self = current_state.get_self();
    fill(&grid[0][0], &grid[0][0] + MAP_HEIGHT * MAP_WIDTH, GridValue::EMPTY);

    // 1. 标记安全区外的区域为墙
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            if (j < current_state.current_safe_zone.x_min || j > current_state.current_safe_zone.x_max ||
                i < current_state.current_safe_zone.y_min || i > current_state.current_safe_zone.y_max) {
                grid[i][j] = GridValue::WALL;
            }
        }
    }

    // 2. 标记宝箱（无钥匙时是致命障碍）
    if (!self.has_key) {
        for (const auto& chest : current_state.chests) {
            grid[chest.pos.y][chest.pos.x] = GridValue::WALL;
        }
    }

    // 3. 标记所有蛇
    for (const auto& snake : current_state.snakes) {
        if (snake.id == MY_ID) { // 自己的蛇
            for (size_t i = 1; i < snake.body.size(); ++i) {
                 grid[snake.body[i].y][snake.body[i].x] = GridValue::MY_BODY;
            }
        } else { // 敌人的蛇
            for (size_t i = 0; i < snake.body.size(); ++i) {
                const auto& p = snake.body[i];
                if (i == 0) { // 敌人头部
                     grid[p.y][p.x] = GridValue::ENEMY_HEAD;
                } else { // 敌人身体
                     grid[p.y][p.x] = GridValue::ENEMY_BODY;
                }
            }
            // 4. 标记敌人头部周围的“危险区域”，以规避U型陷阱
            if(snake.length > 2) { // 短蛇威胁不大
                Point enemy_head = snake.get_head();
                for(int i = 0; i < 4; ++i) {
                    Point danger_pos = {enemy_head.y + dy[i], enemy_head.x + dx[i]};
                    if (danger_pos.y >= 0 && danger_pos.y < MAP_HEIGHT && danger_pos.x >= 0 && danger_pos.x < MAP_WIDTH &&
                        grid[danger_pos.y][danger_pos.x] < GridValue::ENEMY_BODY) { // 只标记空地
                        grid[danger_pos.y][danger_pos.x] = GridValue::DANGER_ZONE;
                    }
                }
            }
        }
    }
}

// 读取游戏状态 (与模板一致)
void read_game_state() {
    cin >> current_state.remaining_ticks;
    int item_count; cin >> item_count;
    current_state.items.resize(item_count);
    for (auto& item : current_state.items) cin >> item.pos.y >> item.pos.x >> item.value >> item.lifetime;

    int snake_count; cin >> snake_count;
    current_state.snakes.resize(snake_count);
    unordered_map<int, int> id2idx;
    for (int i = 0; i < snake_count; ++i) {
        auto &sn = current_state.snakes[i];
        cin >> sn.id >> sn.length >> sn.score >> sn.direction >> sn.shield_cd >> sn.shield_time;
        sn.body.resize(sn.length);
        for (auto& p : sn.body) cin >> p.y >> p.x;
        if (sn.id == MY_ID) current_state.self_idx = i;
        id2idx[sn.id] = i;
        sn.has_key = false;
    }
    if (current_state.self_idx == -1) exit(0);

    int chest_count; cin >> chest_count;
    current_state.chests.resize(chest_count);
    for(auto& chest : current_state.chests) cin >> chest.pos.y >> chest.pos.x >> chest.score;

    int key_count; cin >> key_count;
    current_state.keys.resize(key_count);
    for (auto& key : current_state.keys) {
        cin >> key.pos.y >> key.pos.x >> key.holder_id >> key.remaining_time;
        if (key.holder_id != -1) {
            if (id2idx.count(key.holder_id)) current_state.snakes[id2idx[key.holder_id]].has_key = true;
        }
    }

    cin >> current_state.current_safe_zone.x_min >> current_state.current_safe_zone.y_min >> current_state.current_safe_zone.x_max >> current_state.current_safe_zone.y_max;
    cin >> current_state.next_shrink_tick >> current_state.next_safe_zone.x_min >> current_state.next_safe_zone.y_min >> current_state.next_safe_zone.x_max >> current_state.next_safe_zone.y_max;
    cin >> current_state.final_shrink_tick >> current_state.final_safe_zone.x_min >> current_state.final_safe_zone.y_min >> current_state.final_safe_zone.x_max >> current_state.final_safe_zone.y_max;
}


// 主决策函数
int decide() {
    const auto& self = current_state.get_self();
    Point head = self.get_head();

    // 1. 预处理网格，标记墙、蛇、危险区等
    prepare_grid();

    // 2. 确定战略目标
    Point target_pos = {-1, -1};
    if (self.has_key && !current_state.chests.empty()) {
        target_pos = current_state.chests[0].pos; // 目标：宝箱
    } else {
        int min_key_dist = 10000;
        // 寻找最近的在地上的钥匙
        if (!current_state.chests.empty()) {
            for (const auto& key : current_state.keys) {
                if (key.holder_id == -1) {
                    int dist = abs(head.y - key.pos.y) + abs(head.x - key.pos.x);
                    if (dist < min_key_dist) {
                        min_key_dist = dist;
                        target_pos = key.pos;
                    }
                }
            }
        }
        // 如果没有钥匙目标，寻找最佳食物
        if (target_pos.x == -1) {
            double max_food_score = -1.0;
            for (const auto& item : current_state.items) {
                if (item.value > 0) {
                    int dist = abs(head.y - item.pos.y) + abs(head.x - item.pos.x) + 1;
                    double score = (double)item.value * 100.0 / dist; // 价值*100/距离
                    if (score > max_food_score) {
                        max_food_score = score;
                        target_pos = item.pos;
                    }
                }
            }
        }
    }

    // 3. 寻找最有威胁的敌人（用于进攻性策略评估）
    const Snake* threat = nullptr;
    int min_dist_to_enemy = 10000;
    for(const auto& snake : current_state.snakes) {
        if(snake.id == MY_ID) continue;
        int dist = abs(head.y - snake.get_head().y) + abs(head.x - snake.get_head().x);
        if(dist < min_dist_to_enemy) {
            min_dist_to_enemy = dist;
            threat = &snake;
        }
    }
    
    // 4. 评估四个方向
    vector<double> direction_scores(4, -numeric_limits<double>::infinity());
    int enemy_original_freedom = 0;
    if (threat) {
        enemy_original_freedom = calculate_freedom(threat->get_head(), *threat);
    }
    
    for (int i = 0; i < 4; ++i) {
        Point next_head = {head.y + dy[i], head.x + dx[i]};

        // ========== A. 绝对安全检查 (Veto Power) ==========
        // 不能180度掉头
        if (self.length > 1 && next_head == self.body[1]) continue;
        // 检查地图边界
        if (next_head.x < 0 || next_head.x >= MAP_WIDTH || next_head.y < 0 || next_head.y >= MAP_HEIGHT) continue;
        // 检查墙和致命障碍
        if (grid[next_head.y][next_head.x] == GridValue::WALL) continue;
        // 无护盾时，检查是否撞上敌人
        if (self.shield_time <= 0 && grid[next_head.y][next_head.x] >= GridValue::ENEMY_BODY) continue;

        double current_score = 0.0;

        // ========== B. 启发式评分 ==========
        // 1. 生存空间 (Freedom Score) - 基础生存保障
        int freedom = calculate_freedom(next_head, self);
        if (freedom < self.length) current_score -= 1e9; // 如果空间不够长，这是死路，给予巨大惩罚
        else current_score += freedom * 50.0;

        // 2. 目标导向 (Target Score) - 核心进攻欲望
        if (target_pos.x != -1) {
            int dist_to_target = abs(next_head.y - target_pos.y) + abs(next_head.x - target_pos.x);
            current_score += (MAP_WIDTH + MAP_HEIGHT - dist_to_target) * 500.0;
        }

        // 3. 风险规避 (Risk Score) - 远离危险区域
        if (grid[next_head.y][next_head.x] == GridValue::DANGER_ZONE) {
            current_score -= 1e7; // 进入紧邻敌方蛇头的区域，巨大惩罚
        }

        // 4. 机会收益 (Opportunity Score) - 吃掉脚下物品
        for (const auto& item : current_state.items) {
            if (item.pos == next_head) {
                if (item.value > 0) current_score += item.value * 1000.0; // 食物
                else if (item.value == -1) current_score += 1500.0; // 增长豆
                else if (item.value == -2) current_score -= 2000.0; // 陷阱
                else if (item.value == -3) current_score += 20000.0; // 钥匙
            }
        }

        // 5. 进攻性 (Offensive Score) - 压缩敌人空间
        if (threat && min_dist_to_enemy < self.length + threat->length) {
            // 模拟我方移动后，敌人的生存空间
            grid[head.y][head.x] = GridValue::MY_BODY; // 旧头变身体
            grid[next_head.y][next_head.x] = GridValue::WALL; // 新头位置对敌人来说是障碍
            int enemy_new_freedom = calculate_freedom(threat->get_head(), *threat);
            grid[head.y][head.x] = GridValue::EMPTY; // 恢复
            grid[next_head.y][next_head.x] = GridValue::EMPTY;
            
            if (enemy_new_freedom < enemy_original_freedom) {
                current_score += (enemy_original_freedom - enemy_new_freedom) * 300.0; // 封锁敌人奖励
            }
        }
        direction_scores[i] = current_score;
    }

    // 5. 作出最终决策
    int best_direction = -1;
    double max_score = -numeric_limits<double>::infinity();
    for (int i = 0; i < 4; ++i) {
        if (direction_scores[i] > max_score) {
            max_score = direction_scores[i];
            best_direction = i;
        }
    }
    
    // 如果所有方向都不安全（被完全堵死），为了不决策失败，随便选一个不会立刻撞上自己脖子的方向
    if (best_direction == -1) {
        for (int i = 0; i < 4; ++i) {
             if (self.length > 1 && (head.y + dy[i] == self.body[1].y && head.x + dx[i] == self.body[1].x)) continue;
             return i;
        }
        return 0; // 最终的保底策略
    }

    return best_direction;
}

int main() {
    // 读取当前 tick 的所有游戏状态
    read_game_state();
    // 调用决策函数获取决策
    int decision = decide();
    // 输出决策
    cout << decision << endl;
    return 0;
}