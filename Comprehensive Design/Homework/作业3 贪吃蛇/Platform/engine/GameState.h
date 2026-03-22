#pragma once

#include <vector>
#include <string>
#include <deque>
#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>

// 坐标结构
struct Point {
    int y, x;
    bool operator==(const Point& other) const { return y == other.y && x == other.x; }
    bool operator<(const Point& other) const {
        if (y != other.y) return y < other.y;
        return x < other.x;
    }
};

// 物品类型枚举 (内部使用)
enum class ItemType { Food, GrowBean, Trap };

// 场上基础物品
struct Item {
    Point pos;
    int value;
    int lifetime;
    ItemType type;
};

// 宝箱
struct Chest {
    Point pos;
    int score;
};

// 钥匙
struct Key {
    Point pos;
    long long holder_id = -1;
    int remaining_time = 0; // 作为持有者自动掉落的计时器
};

// 蛇
struct Snake {
    long long id;
    int score = 0;
    std::deque<Point> body;
    int direction = 2; // 0:左, 1:上, 2:右, 3:下
    int shield_cd = 0;
    int shield_time = 10; // 初始护盾
    bool has_key = false;
    bool is_alive = true;
    int decision;
    std::string memory;
    int growth_pending = 0; // 待增长长度
    int score_at_last_growth = 0; // 用于计算分数增长
};

// 安全区
struct SafeZone {
    int x_min, y_min, x_max, y_max;
};

// 安全区收缩事件
struct SafeZoneEvent {
    int tick_start;
    int tick_end;
    SafeZone start_zone;
    SafeZone target_zone;
};

// 游戏主状态
class GameState {
public:
    int current_tick = 1;
    const int total_ticks = 256;
    const int map_width = 40;
    const int map_height = 30;

    std::map<long long, Snake> snakes;
    std::vector<Item> items;
    std::vector<Chest> chests;
    std::vector<Key> keys;

    SafeZone current_safe_zone = {0, 0, 39, 29};
    std::vector<SafeZoneEvent> safe_zone_schedule;

    // 此函数严格按照您的AI模板中的 read_game_state 函数的读取顺序来生成数据
    std::string to_bot_string(long long snake_id) {
        std::stringstream ss;
        ss << total_ticks - current_tick << std::endl;

        std::vector<std::string> item_lines;
        for (const auto& item : items) {
            int val = 0;
            if (item.type == ItemType::Food) val = item.value;
            else if (item.type == ItemType::GrowBean) val = -1;
            else if (item.type == ItemType::Trap) val = -2;
            item_lines.push_back(std::to_string(item.pos.y) + " " + std::to_string(item.pos.x) + " " + std::to_string(val) + " " + std::to_string(item.lifetime));
        }
        for (const auto& key : keys) if (key.holder_id == -1) item_lines.push_back(std::to_string(key.pos.y) + " " + std::to_string(key.pos.x) + " -3 -1");
        for (const auto& chest : chests) item_lines.push_back(std::to_string(chest.pos.y) + " " + std::to_string(chest.pos.x) + " -5 -1");
        ss << item_lines.size() << std::endl;
        for (const auto& line : item_lines) ss << line << std::endl;

        std::vector<const Snake*> living_snakes;
        for(const auto& pair : snakes) if(pair.second.is_alive) living_snakes.push_back(&pair.second);
        ss << living_snakes.size() << std::endl;
        for (const auto* snake_ptr : living_snakes) {
            const auto& snake = *snake_ptr;
            ss << snake.id << " " << snake.body.size() << " " << snake.score << " " << snake.direction << " " << snake.shield_cd << " " << snake.shield_time << std::endl;
            for (const auto& p : snake.body) ss << p.y << " " << p.x << std::endl;
        }

        ss << chests.size() << std::endl;
        for(const auto& chest : chests) ss << chest.pos.y << " " << chest.pos.x << " " << chest.score << std::endl;

        ss << keys.size() << std::endl;
        for (const auto& key : keys) {
            int time_val = (key.holder_id != -1) ? key.remaining_time : 0;
            ss << key.pos.y << " " << key.pos.x << " " << key.holder_id << " " << time_val << std::endl;
        }
        
        ss << current_safe_zone.x_min << " " << current_safe_zone.y_min << " " << current_safe_zone.x_max << " " << current_safe_zone.y_max << std::endl;
        
        int next_shrink_tick = -1; SafeZone next_target_zone = {-1, -1, -1, -1};
        for (const auto& event : safe_zone_schedule) {
            if (event.tick_start > current_tick) {
                next_shrink_tick = event.tick_start; next_target_zone = event.target_zone; break;
            }
        }
        int final_shrink_tick = -1; SafeZone final_target_zone = {-1, -1, -1, -1};
        bool in_shrink = false;
        for (const auto& event : safe_zone_schedule) {
            if (current_tick >= event.tick_start && current_tick <= event.tick_end) {
                final_shrink_tick = event.tick_end; final_target_zone = event.target_zone; in_shrink = true; break;
            }
        }
        if (!in_shrink && next_shrink_tick != -1) {
             for(const auto& event : safe_zone_schedule) {
                 if (event.tick_start == next_shrink_tick) {
                     final_shrink_tick = event.tick_end; final_target_zone = event.target_zone; break;
                 }
             }
        }
        
        if (next_shrink_tick != -1) ss << next_shrink_tick << " " << next_target_zone.x_min << " " << next_target_zone.y_min << " " << next_target_zone.x_max << " " << next_target_zone.y_max << std::endl;
        else ss << "-1 -1 -1 -1 -1" << std::endl;
        if (final_shrink_tick != -1) ss << final_shrink_tick << " " << final_target_zone.x_min << " " << final_target_zone.y_min << " " << final_target_zone.x_max << " " << final_target_zone.y_max << std::endl;
        else ss << "-1 -1 -1 -1 -1" << std::endl;

        ss << snakes.at(snake_id).memory << std::endl;
        return ss.str();
    }

    // to_json 函数用于可视化
    std::string to_json() {
        std::stringstream ss;
        ss << "{";
        ss << "\"tick\":" << current_tick << ",";
        ss << "\"snakes\":[";
        bool first = true;
        for(const auto& pair : snakes) {
            if(!pair.second.is_alive) continue;
            if(!first) ss << ",";
            const auto& s = pair.second;
            ss << "{\"id\":" << s.id << ",\"score\":" << s.score << ",\"has_shield\":" << (s.shield_time > 0) << ",\"has_key\":" << s.has_key << ",\"body\":[";
            bool first_body = true;
            for(const auto& p : s.body) {
                if(!first_body) ss << ",";
                ss << "[" << p.y << "," << p.x << "]";
                first_body = false;
            }
            ss << "]}";
            first = false;
        }
        ss << "],";
        ss << "\"items\":[";
        first = true;
        for(const auto& i : items) {
            if(!first) ss << ",";
            ss << "{\"pos\":[" << i.pos.y << "," << i.pos.x << "],\"type\":" << (int)i.type << ",\"value\":" << i.value << "}";
            first = false;
        }
        ss << "],";
        ss << "\"keys\":[";
        first = true;
        for(const auto& k : keys) {
            if(k.holder_id != -1) continue;
            if(!first) ss << ",";
            ss << "{\"pos\":[" << k.pos.y << "," << k.pos.x << "]}";
            first = false;
        }
        ss << "],";
        ss << "\"chests\":[";
        first = true;
        for(const auto& c : chests) {
            if(!first) ss << ",";
            ss << "{\"pos\":[" << c.pos.y << "," << c.pos.x << "],\"score\":" << c.score << "}";
            first = false;
        }
        ss << "],";
        ss << "\"safe_zone\":{\"xmin\":" << current_safe_zone.x_min << ",\"ymin\":" << current_safe_zone.y_min
           << ",\"xmax\":" << current_safe_zone.x_max << ",\"ymax\":" << current_safe_zone.y_max << "}";
        ss << "}";
        return ss.str();
    }
};