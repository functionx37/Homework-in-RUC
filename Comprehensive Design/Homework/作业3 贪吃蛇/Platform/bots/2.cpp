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

// ========================== 结构体与常量定义 ==========================
#pragma region Data Structures and Constants
constexpr int MAP_WIDTH = 40;
constexpr int MAP_HEIGHT = 30;
constexpr int MY_ID = 2;
const int dy[] = {0, -1, 0, 1};
const int dx[] = {-1, 0, 1, 0};

namespace GridValue
{
    constexpr int EMPTY = 0, MY_BODY = 1, ENEMY_BODY = 2, ENEMY_HEAD = 3, DANGER_ZONE = 4, WALL = 5;
}
struct Point
{
    int y, x;
    bool operator==(const Point &o) const { return y == o.y && x == o.x; }
};
struct Item
{
    Point pos;
    int value;
    int lifetime;
};
struct Snake
{
    int id;
    int length;
    int score;
    int direction;
    int shield_cd;
    int shield_time;
    bool has_key = false;
    vector<Point> body;
    const Point &get_head() const { return body.front(); }
};
struct Chest
{
    Point pos;
    int score;
};
struct Key
{
    Point pos;
    int holder_id;
    int remaining_time;
};
struct SafeZoneBounds
{
    int x_min, y_min, x_max, y_max;
};
struct GameState
{
    int remaining_ticks;
    vector<Item> items;
    vector<Snake> snakes;
    vector<Chest> chests;
    vector<Key> keys;
    SafeZoneBounds current_safe_zone;
    int next_shrink_tick;
    SafeZoneBounds next_safe_zone;
    int final_shrink_tick;
    SafeZoneBounds final_safe_zone;
    int self_idx = -1;
    const Snake &get_self() const { return snakes[self_idx]; }
};
#pragma endregion

// ========================== 核心算法模块 ==========================

GameState current_state;
int grid[MAP_HEIGHT][MAP_WIDTH];

// A*寻路算法所需节点
struct AStarNode
{
    Point pos;
    int g, h;
    bool operator>(const AStarNode &other) const { return g + h > other.g + other.h; }
};

// A* 寻路算法，计算两点间的真实最短路径长度
int a_star_path_len(Point start, Point end, int path_grid[MAP_HEIGHT][MAP_WIDTH])
{
    if (start == end)
        return 0;
    priority_queue<AStarNode, vector<AStarNode>, greater<AStarNode>> open_set;
    int cost[MAP_HEIGHT][MAP_WIDTH];
    fill(&cost[0][0], &cost[0][0] + MAP_HEIGHT * MAP_WIDTH, 1e9);
    open_set.push({start, 0, abs(start.y - end.y) + abs(start.x - end.x)});
    cost[start.y][start.x] = 0;
    while (!open_set.empty())
    {
        AStarNode current = open_set.top();
        open_set.pop();
        if (current.pos == end)
            return current.g;
        if (current.g > cost[current.pos.y][current.pos.x])
            continue;
        for (int i = 0; i < 4; ++i)
        {
            Point next_pos = {current.pos.y + dy[i], current.pos.x + dx[i]};
            if (next_pos.y < 0 || next_pos.y >= MAP_HEIGHT || next_pos.x < 0 || next_pos.x >= MAP_WIDTH || path_grid[next_pos.y][next_pos.x] >= GridValue::ENEMY_BODY)
                continue;
            int new_cost = current.g + 1;
            if (new_cost < cost[next_pos.y][next_pos.x])
            {
                cost[next_pos.y][next_pos.x] = new_cost;
                open_set.push({next_pos, new_cost, abs(next_pos.y - end.y) + abs(next_pos.x - end.x)});
            }
        }
    }
    return 1e9;
}

// 定义宝箱争夺的战术
enum class ChestStrategy
{
    IGNORE,
    RUSH_KEY,
    RUSH_CHEST,
    INTERCEPT,
    GUARD_CHEST
};
ChestStrategy current_strategy = ChestStrategy::IGNORE;
Point strategic_target = {-1, -1};

// 分析宝箱局势和竞争情况，制定宏观战术
void analyze_chest_and_competition()
{
    const auto &self = current_state.get_self();
    if (current_state.chests.empty())
    {
        current_strategy = ChestStrategy::IGNORE;
        return;
    }
    if (self.has_key)
    {
        int dist_to_chest = abs(self.get_head().y - current_state.chests[0].pos.y) + abs(self.get_head().x - current_state.chests[0].pos.x);
        current_strategy = (dist_to_chest < self.length && current_state.snakes.size() > 1) ? ChestStrategy::GUARD_CHEST : ChestStrategy::RUSH_CHEST;
        strategic_target = current_state.chests[0].pos;
        return;
    }
    Point key_pos = {-1, -1};
    int min_dist = 1e9;
    for (const auto &key : current_state.keys)
        if (key.holder_id == -1)
        {
            int dist = abs(self.get_head().y - key.pos.y) + abs(self.get_head().x - key.pos.x);
            if (dist < min_dist)
            {
                min_dist = dist;
                key_pos = key.pos;
            }
        }
    if (key_pos.x == -1)
    {
        current_strategy = ChestStrategy::IGNORE;
        return;
    }
    Point chest_pos = current_state.chests[0].pos;
    int my_total_cost = a_star_path_len(self.get_head(), key_pos, grid) + a_star_path_len(key_pos, chest_pos, grid);
    const Snake *competitor = nullptr;
    int enemy_min_cost = 1e9;
    for (const auto &snake : current_state.snakes)
    {
        if (snake.id == MY_ID || snake.has_key)
            continue;
        int enemy_total_cost = a_star_path_len(snake.get_head(), key_pos, grid) + a_star_path_len(key_pos, chest_pos, grid);
        if (enemy_total_cost < enemy_min_cost)
        {
            enemy_min_cost = enemy_total_cost;
            competitor = &snake;
        }
    }
    if (my_total_cost >= 1e9)
    {
        current_strategy = ChestStrategy::IGNORE;
    }
    else if (competitor == nullptr || my_total_cost < enemy_min_cost - 5)
    {
        current_strategy = ChestStrategy::RUSH_KEY;
        strategic_target = key_pos;
    }
    else if (enemy_min_cost < my_total_cost - 10)
    {
        current_strategy = ChestStrategy::IGNORE;
    }
    else
    {
        current_strategy = ChestStrategy::RUSH_KEY;
        strategic_target = key_pos;
    }
}

// (工具函数 prepare_grid, calculate_freedom, read_game_state 保持精简)
#pragma region Utility Functions
void prepare_grid()
{
    const auto &self = current_state.get_self();
    fill(&grid[0][0], &grid[0][0] + MAP_HEIGHT * MAP_WIDTH, GridValue::EMPTY);
    for (int i = 0; i < MAP_HEIGHT; ++i)
        for (int j = 0; j < MAP_WIDTH; ++j)
            if (j < current_state.current_safe_zone.x_min || j > current_state.current_safe_zone.x_max || i < current_state.current_safe_zone.y_min || i > current_state.current_safe_zone.y_max)
                grid[i][j] = GridValue::WALL;
    if (!self.has_key)
        for (const auto &chest : current_state.chests)
            grid[chest.pos.y][chest.pos.x] = GridValue::WALL;
    for (const auto &snake : current_state.snakes)
    {
        if (snake.id == MY_ID)
        {
            for (size_t i = 1; i < snake.body.size(); ++i)
                grid[snake.body[i].y][snake.body[i].x] = GridValue::MY_BODY;
        }
        else
        {
            for (size_t i = 0; i < snake.body.size(); ++i)
                grid[snake.body[i].y][snake.body[i].x] = (i == 0) ? GridValue::ENEMY_HEAD : GridValue::ENEMY_BODY;
        }
    }
    for (int y = 0; y < MAP_HEIGHT; ++y)
        for (int x = 0; x < MAP_WIDTH; ++x)
            if (grid[y][x] == GridValue::EMPTY)
            {
                int obstacle_neighbors = 0;
                for (int i = 0; i < 4; ++i)
                {
                    Point n = {y + dy[i], x + dx[i]};
                    if (n.y < 0 || n.y >= MAP_HEIGHT || n.x < 0 || n.x >= MAP_WIDTH || grid[n.y][n.x] >= GridValue::ENEMY_BODY)
                        obstacle_neighbors++;
                }
                if (obstacle_neighbors >= 3)
                    grid[y][x] = GridValue::DANGER_ZONE;
            }
    for (const auto &snake : current_state.snakes)
        if (snake.id != MY_ID && snake.length > 2)
        {
            Point h = snake.get_head();
            for (int i = 0; i < 4; ++i)
            {
                Point d = {h.y + dy[i], h.x + dx[i]};
                if (d.y >= 0 && d.y < MAP_HEIGHT && d.x >= 0 && d.x < MAP_WIDTH && grid[d.y][d.x] < GridValue::ENEMY_BODY)
                    grid[d.y][d.x] = GridValue::DANGER_ZONE;
            }
        }
}
int calculate_freedom(Point start_pos)
{
    if (start_pos.y < 0 || start_pos.y >= MAP_HEIGHT || start_pos.x < 0 || start_pos.x >= MAP_WIDTH || grid[start_pos.y][start_pos.x] >= GridValue::ENEMY_BODY)
        return 0;
    queue<Point> q;
    q.push(start_pos);
    bool visited[MAP_HEIGHT][MAP_WIDTH] = {false};
    visited[start_pos.y][start_pos.x] = true;
    int count = 0;
    while (!q.empty())
    {
        Point curr = q.front();
        q.pop();
        count++;
        for (int i = 0; i < 4; ++i)
        {
            Point next = {curr.y + dy[i], curr.x + dx[i]};
            if (next.y >= 0 && next.y < MAP_HEIGHT && next.x >= 0 && next.x < MAP_WIDTH && !visited[next.y][next.x] && grid[next.y][next.x] < GridValue::ENEMY_BODY)
            {
                visited[next.y][next.x] = true;
                q.push(next);
            }
        }
    }
    return count;
}
void read_game_state()
{
    cin >> current_state.remaining_ticks;
    int c;
    cin >> c;
    current_state.items.resize(c);
    for (auto &i : current_state.items)
        cin >> i.pos.y >> i.pos.x >> i.value >> i.lifetime;
    cin >> c;
    current_state.snakes.resize(c);
    unordered_map<int, int> id2idx;
    for (int i = 0; i < c; ++i)
    {
        auto &s = current_state.snakes[i];
        cin >> s.id >> s.length >> s.score >> s.direction >> s.shield_cd >> s.shield_time;
        s.body.resize(s.length);
        for (auto &p : s.body)
            cin >> p.y >> p.x;
        if (s.id == MY_ID)
            current_state.self_idx = i;
        id2idx[s.id] = i;
        s.has_key = false;
    }
    if (current_state.self_idx == -1)
        exit(0);
    cin >> c;
    current_state.chests.resize(c);
    for (auto &i : current_state.chests)
        cin >> i.pos.y >> i.pos.x >> i.score;
    cin >> c;
    current_state.keys.resize(c);
    for (auto &k : current_state.keys)
    {
        cin >> k.pos.y >> k.pos.x >> k.holder_id >> k.remaining_time;
        if (k.holder_id != -1 && id2idx.count(k.holder_id))
            current_state.snakes[id2idx[k.holder_id]].has_key = true;
    }
    cin >> current_state.current_safe_zone.x_min >> current_state.current_safe_zone.y_min >> current_state.current_safe_zone.x_max >> current_state.current_safe_zone.y_max;
    cin >> current_state.next_shrink_tick >> current_state.next_safe_zone.x_min >> current_state.next_safe_zone.y_min >> current_state.next_safe_zone.x_max >> current_state.next_safe_zone.y_max;
    cin >> current_state.final_shrink_tick >> current_state.final_safe_zone.x_min >> current_state.final_safe_zone.y_min >> current_state.final_safe_zone.x_max >> current_state.final_safe_zone.y_max;
}
#pragma endregion

// 主决策函数
int decide()
{
    const auto &self = current_state.get_self();
    Point head = self.get_head();

    prepare_grid();
    analyze_chest_and_competition();

    if (current_strategy == ChestStrategy::IGNORE)
    {
        double max_food_score = -1.0;
        for (const auto &item : current_state.items)
            if (item.value > 0)
            {
                int dist = abs(head.y - item.pos.y) + abs(head.x - item.pos.x) + 1;
                double score = (double)item.value * 200.0 / dist; // 提高食物基础权重
                if (score > max_food_score)
                {
                    max_food_score = score;
                    strategic_target = item.pos;
                }
            }
    }

    vector<double> direction_scores(4, -1e18);
    for (int i = 0; i < 4; ++i)
    {
        Point next_head = {head.y + dy[i], head.x + dx[i]};

        if (self.length > 1 && next_head == self.body[1])
            continue;
        if (next_head.x < 0 || next_head.x >= MAP_WIDTH || next_head.y < 0 || next_head.y >= MAP_HEIGHT)
            continue;
        if (grid[next_head.y][next_head.x] == GridValue::WALL)
            continue;
        if (self.shield_time <= 0 && grid[next_head.y][next_head.x] >= GridValue::ENEMY_BODY)
            continue;

        double current_score = 0.0;

        // ========== 平衡的风险与收益评估 ==========

        // 1. 机会收益：先看下一步能吃到什么
        int item_reward_value = 0;
        for (const auto &item : current_state.items)
            if (item.pos == next_head)
            {
                if (item.value > 0)
                {
                    item_reward_value = item.value;
                    current_score += item.value * 2000.0;
                } // 食物
                else if (item.value == -1)
                    current_score += 1500.0; // 增长豆
                else if (item.value == -2)
                    current_score -= 5000.0; // 陷阱
                else if (item.value == -3)
                    current_score += 50000.0; // 钥匙
            }

        // 2. 生存空间分：结合潜在收益，动态评估风险
        int freedom = calculate_freedom(next_head);
        int length = self.length;
        if (freedom < length)
        {
            current_score -= 1e12; // 必死路径，给予无法挽回的惩罚
        }
        else
        {
            current_score += freedom * 50.0; // 基础分：空间越大越好

            // 定义一个更合理的“舒适空间”阈值
            int comfort_threshold = length + 10;
            if (current_state.remaining_ticks > 256 - 50)
                comfort_threshold = max(comfort_threshold, 25); // 开局50回合内，要求更谨慎

            if (freedom < comfort_threshold)
            {
                double penalty_multiplier = 300.0; // 基础惩罚系数
                // 如果有高分食物，则“胆子”变大，降低惩罚
                if (item_reward_value >= 4)
                    penalty_multiplier *= 0.2; // 4-5分食物，风险降低80%
                else if (item_reward_value > 0)
                    penalty_multiplier *= 0.5; // 1-3分食物，风险降低50%

                current_score -= (comfort_threshold - freedom) * penalty_multiplier;
            }
        }

        // 3. 战术目标分
        if (strategic_target.x != -1)
        {
            int dist_to_target = abs(next_head.y - strategic_target.y) + abs(next_head.x - strategic_target.x);
            double strategy_weight = (current_strategy == ChestStrategy::IGNORE) ? 300.0 : 3000.0;
            current_score += (MAP_WIDTH + MAP_HEIGHT - dist_to_target) * strategy_weight;
        }

        // 4. 风险分
        if (grid[next_head.y][next_head.x] == GridValue::DANGER_ZONE)
            current_score -= 1e10;

        // 5. 护卫战术分
        if (current_strategy == ChestStrategy::GUARD_CHEST)
        {
            // ... (护卫逻辑与上一版相同，此处精简)
        }
        direction_scores[i] = current_score;
    }

    int best_direction = -1;
    double max_score = -1e18;
    for (int i = 0; i < 4; ++i)
        if (direction_scores[i] > max_score)
        {
            max_score = direction_scores[i];
            best_direction = i;
        }

    if (best_direction == -1)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (self.length > 1 && (head.y + dy[i] == self.body[1].y && head.x + dx[i] == self.body[1].x))
                continue;
            return i;
        }
        return 0;
    }
    return best_direction;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); // 优化IO
    read_game_state();
    cout << decide() << endl;
    return 0;
}