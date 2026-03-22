#include "GameState.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <stdexcept>
#include <random>
#include <numeric>
#include <algorithm>

// --- 跨平台进程管理 ---
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>
#include <poll.h>
#endif

// BotProcess 结构体，用于存储从命令行解析出的ID和路径
struct BotProcess {
    long long snake_id;
    std::string path;
};

// --- 游戏逻辑辅助函数 ---
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

Point get_random_empty_pos(const GameState& state) {
    std::set<Point> occupied;
    for(const auto& pair : state.snakes) if(pair.second.is_alive) for(const auto& p : pair.second.body) occupied.insert(p);
    for(const auto& item : state.items) occupied.insert(item.pos);
    for(const auto& key : state.keys) occupied.insert(key.pos);
    for(const auto& chest : state.chests) occupied.insert(chest.pos);
    
    Point p;
    do {
        p = { (int)(rng() % state.map_height), (int)(rng() % state.map_width) };
    } while (occupied.count(p) || p.x < state.current_safe_zone.x_min || p.x > state.current_safe_zone.x_max || p.y < state.current_safe_zone.y_min || p.y > state.current_safe_zone.y_max);
    return p;
}

void handle_item_spawning(GameState& state) {
    if (state.current_tick > 1 && state.current_tick % 3 != 0) return;
    size_t living_snakes = 0;
    for(const auto& pair : state.snakes) if(pair.second.is_alive) living_snakes++;
    if (state.items.size() >= living_snakes * 10 + 5) return;

    double food_chance = 0.0, grow_chance = 0.0, trap_chance = 0.0;
    int min_food_val = 1, max_food_val = 3;

    if (state.current_tick <= 80) { food_chance = 0.8; grow_chance = 0.2; } 
    else if (state.current_tick <= 200) { food_chance = 0.78; grow_chance = 0.12; trap_chance = 0.10; max_food_val = 5; } 
    else { food_chance = 0.80; grow_chance = 0.05; trap_chance = 0.15; max_food_val = 5; }

    double r = (double)rng() / rng.max();
    Point pos = get_random_empty_pos(state);
    if (r < food_chance) {
        int value = min_food_val + rng() % (max_food_val - min_food_val + 1);
        state.items.push_back({pos, value, 60, ItemType::Food});
    } else if (r < food_chance + grow_chance) {
        state.items.push_back({pos, 0, 80, ItemType::GrowBean});
    } else if (r < food_chance + grow_chance + trap_chance) {
        state.items.push_back({pos, 0, 80, ItemType::Trap});
    }
}

void handle_chest_spawning(GameState& state) {
    if (state.chests.size() >= 2) return;
    bool should_spawn = false;
    if (state.current_tick >= 81 && state.current_tick <= 200 && state.chests.empty()) { should_spawn = (rng() % 50) == 0; } 
    else if (state.current_tick >= 201 && state.chests.size() == 1) { should_spawn = (rng() % 50) == 0; }
    if (!should_spawn) return;

    std::vector<int> scores; int first_place_score = 0;
    for(const auto& pair : state.snakes) if(pair.second.is_alive) { scores.push_back(pair.second.score); if(pair.second.score > first_place_score) first_place_score = pair.second.score; }
    if (scores.empty()) return;

    double non_first_avg = 0;
    long long sum = 0; int count = 0;
    for(int s : scores) if(s != first_place_score) { sum += s; count++; }
    if(count > 0) non_first_avg = (double)sum / count;
    int chest_score = 30 + (first_place_score - non_first_avg) * 0.6;
    chest_score = std::max(30, std::min(75, chest_score));
    state.chests.push_back({get_random_empty_pos(state), chest_score});

    size_t living_snakes = scores.size();
    int key_count = std::min(4, std::max(2, (int)floor(living_snakes / 2.0)));
    for(int i=0; i<key_count; ++i) state.keys.push_back({get_random_empty_pos(state), -1, 0});
}

void update_safe_zone(GameState& state) {
    for (const auto& event : state.safe_zone_schedule) {
        if (state.current_tick >= event.tick_start && state.current_tick <= event.tick_end) {
            double progress = (double)(state.current_tick - event.tick_start) / (event.tick_end - event.tick_start);
            state.current_safe_zone.x_min = event.start_zone.x_min + (event.target_zone.x_min - event.start_zone.x_min) * progress;
            state.current_safe_zone.y_min = event.start_zone.y_min + (event.target_zone.y_min - event.start_zone.y_min) * progress;
            state.current_safe_zone.x_max = event.start_zone.x_max - (event.start_zone.x_max - event.target_zone.x_max) * progress;
            state.current_safe_zone.y_max = event.start_zone.y_max - (event.start_zone.y_max - event.target_zone.y_max) * progress;
            state.items.erase(std::remove_if(state.items.begin(), state.items.end(), [&](const Item& i){ return i.pos.x < state.current_safe_zone.x_min || i.pos.x > state.current_safe_zone.x_max || i.pos.y < state.current_safe_zone.y_min || i.pos.y > state.current_safe_zone.y_max; }), state.items.end());
            return;
        }
    }
}

void convert_snake_to_food(GameState& state, long long dead_snake_id) {
    Snake& snake = state.snakes.at(dead_snake_id);
    int score_to_convert = snake.score;
    for (const auto& p : snake.body) {
        if (score_to_convert <= 0) break;
        int food_value = std::min(20, score_to_convert);
        state.items.push_back({p, food_value, 60, ItemType::Food});
        score_to_convert -= food_value;
    }
}

void run_game_tick(GameState& state) {
    for(auto it = state.items.begin(); it != state.items.end(); ) { it->lifetime--; if(it->lifetime == 0) it = state.items.erase(it); else ++it; }
    for(auto& pair : state.snakes) {
        if(!pair.second.is_alive) continue;
        if(pair.second.shield_cd > 0) pair.second.shield_cd--;
        if(pair.second.shield_time > 0) pair.second.shield_time--;
        if(pair.second.has_key) {
            for(auto& key : state.keys) {
                if(key.holder_id == pair.first) {
                    key.remaining_time--;
                    if(key.remaining_time == 0) { key.holder_id = -1; key.pos = pair.second.body.front(); pair.second.has_key = false; }
                    break;
                }
            }
        }
    }
    
    update_safe_zone(state);
    handle_item_spawning(state);
    handle_chest_spawning(state);

    std::map<Point, std::vector<long long>> next_head_positions;
    for(auto& pair : state.snakes) {
        Snake& snake = pair.second; if(!snake.is_alive) continue;
        if(snake.decision == 4) {
            if(snake.score >= 20 && snake.shield_cd == 0) { snake.score -= 20; snake.shield_time = 5; snake.shield_cd = 30; }
            next_head_positions[snake.body.front()].push_back(snake.id);
        } else {
            int dx[] = {-1, 0, 1, 0}, dy[] = {0, -1, 0, 1};
            if(snake.body.size() <= 1 || (snake.direction + 2) % 4 != snake.decision) snake.direction = snake.decision;
            Point new_head = {snake.body.front().y + dy[snake.direction], snake.body.front().x + dx[snake.direction]};
            snake.body.push_front(new_head);
            next_head_positions[new_head].push_back(snake.id);
        }
    }

    std::set<long long> dead_snakes;
    for(auto& pair : state.snakes) {
        Snake& snake = pair.second; if(!snake.is_alive) continue;
        Point head = snake.body.front();

        if(head.x < 0 || head.x >= state.map_width || head.y < 0 || head.y >= state.map_height) { dead_snakes.insert(snake.id); continue; }
        if(head.x < state.current_safe_zone.x_min || head.x > state.current_safe_zone.x_max || head.y < state.current_safe_zone.y_min || head.y > state.current_safe_zone.y_max) {
             if(snake.shield_time <= 0) { dead_snakes.insert(snake.id); continue; }
        }
        for(const auto& chest : state.chests) if(head == chest.pos && !snake.has_key) { dead_snakes.insert(snake.id); goto next_snake_loop; }
        
        state.items.erase(std::remove_if(state.items.begin(), state.items.end(), [&](Item& item){
            if(item.pos == head) {
                if(item.type == ItemType::Food) {
                    snake.score += item.value;
                    if(snake.score / 20 > snake.score_at_last_growth / 20) {
                        snake.growth_pending += (snake.score / 20 - snake.score_at_last_growth / 20);
                        snake.score_at_last_growth = snake.score / 20 * 20;
                    }
                } else if(item.type == ItemType::GrowBean) snake.growth_pending += 2;
                else if(item.type == ItemType::Trap) snake.score -= 10;
                return true;
            } return false;
        }), state.items.end());
        for(auto& key : state.keys) {
            if(key.holder_id == -1 && head == key.pos && !snake.has_key) {
                snake.has_key = true; key.holder_id = snake.id; key.remaining_time = 31; break;
            }
        }
        
        // ==========================================================
        //  *** 核心修正点在这里 ***
        // ==========================================================
        if (snake.has_key) {
            size_t chests_before = state.chests.size();
            state.chests.erase(std::remove_if(state.chests.begin(), state.chests.end(), [&](Chest& chest){
                if(head == chest.pos) { 
                    snake.score += chest.score; 
                    return true; 
                } 
                return false;
            }), state.chests.end());
            
            if (state.chests.size() < chests_before) { // 检查宝箱数量是否减少
                snake.has_key = false; 
                state.keys.clear(); // 所有钥匙消失
            }
        }
        
        if(next_head_positions[head].size() > 1) {
            for(long long other_id : next_head_positions[head]) if(snake.id != other_id && snake.shield_time <= 0) dead_snakes.insert(snake.id);
        }
        for(const auto& other_pair : state.snakes) {
            if(snake.id == other_pair.first || !other_pair.second.is_alive) continue;
            for(size_t i = 1; i < other_pair.second.body.size(); ++i) if(head == other_pair.second.body[i] && snake.shield_time <= 0) dead_snakes.insert(snake.id);
        }
        next_snake_loop:;
    }

    for(long long id : dead_snakes) {
        if(state.snakes.at(id).is_alive) {
            state.snakes.at(id).is_alive = false;
            if(state.snakes.at(id).has_key) {
                for(auto& key : state.keys) if(key.holder_id == id) { key.holder_id = -1; key.pos = state.snakes.at(id).body.front(); }
            }
            convert_snake_to_food(state, id);
        }
    }
    for(auto& pair : state.snakes) {
        if(!pair.second.is_alive) continue;
        if(pair.second.decision != 4) {
            if(pair.second.growth_pending > 0) pair.second.growth_pending--;
            else pair.second.body.pop_back();
        }
    }
}

void get_bot_decisions(GameState& state, std::vector<BotProcess>& bots) {
    for (auto& bot_proc : bots) {
        if (!state.snakes.count(bot_proc.snake_id) || !state.snakes.at(bot_proc.snake_id).is_alive) continue;
        std::string bot_input = state.to_bot_string(bot_proc.snake_id); std::string bot_output; bool success = false;
#ifdef _WIN32
        HANDLE h_child_stdin_read=0, h_child_stdin_write=0, h_child_stdout_read=0, h_child_stdout_write=0;
        PROCESS_INFORMATION pi; SECURITY_ATTRIBUTES sa; sa.nLength=sizeof(SECURITY_ATTRIBUTES); sa.bInheritHandle=TRUE; sa.lpSecurityDescriptor=NULL;
        CreatePipe(&h_child_stdout_read, &h_child_stdout_write, &sa, 0); SetHandleInformation(h_child_stdout_read, HANDLE_FLAG_INHERIT, 0);
        CreatePipe(&h_child_stdin_read, &h_child_stdin_write, &sa, 0); SetHandleInformation(h_child_stdin_write, HANDLE_FLAG_INHERIT, 0);
        STARTUPINFOA si; ZeroMemory(&si, sizeof(STARTUPINFOA)); si.cb=sizeof(STARTUPINFOA); si.hStdError=GetStdHandle(STD_ERROR_HANDLE);
        si.hStdOutput=h_child_stdout_write; si.hStdInput=h_child_stdin_read; si.dwFlags|=STARTF_USESTDHANDLES;
        ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
        if (CreateProcessA(NULL, (char*)bot_proc.path.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            DWORD bytes_written; WriteFile(h_child_stdin_write, bot_input.c_str(), bot_input.length(), &bytes_written, NULL); CloseHandle(h_child_stdin_write);
            if (WaitForSingleObject(pi.hProcess, 1000) == WAIT_TIMEOUT) TerminateProcess(pi.hProcess, 1);
            else {
                char buffer[4096]; DWORD bytes_read = 0;
                if (ReadFile(h_child_stdout_read, buffer, sizeof(buffer) - 1, &bytes_read, NULL) && bytes_read > 0) { buffer[bytes_read] = '\0'; bot_output = buffer; success = true; }
            }
            CloseHandle(pi.hProcess); CloseHandle(pi.hThread); CloseHandle(h_child_stdout_read); CloseHandle(h_child_stdout_write);
        }
#else
        // Linux/macOS implementation would go here
#endif
        if (!success) { state.snakes.at(bot_proc.snake_id).is_alive = false; std::cerr << "Bot " << bot_proc.snake_id << " timed out or crashed. Killing." << std::endl; continue; }
        std::stringstream ss(bot_output); int decision = -1; ss >> decision;
        if (decision < 0 || decision > 4) { state.snakes.at(bot_proc.snake_id).is_alive = false; std::cerr << "Bot " << bot_proc.snake_id << " gave invalid decision '" << decision << "'. Killing." << std::endl;
        } else {
             state.snakes.at(bot_proc.snake_id).decision = decision;
             std::string mem; std::getline(ss, mem); 
             if (std::getline(ss, mem)) { if(!mem.empty() && mem.back() == '\r') mem.pop_back(); state.snakes.at(bot_proc.snake_id).memory = mem; }
             else { state.snakes.at(bot_proc.snake_id).memory = ""; }
        }
    }
}

void initialize_game(GameState& state, std::vector<BotProcess>& bots) {
    for(const auto& bot : bots) {
        Snake s; s.id = bot.snake_id;
        Point start_pos = {(int)(rng() % (state.map_height-10))+5, (int)(rng() % (state.map_width-10))+5};
        s.body.push_front(start_pos);
        for(int j=1; j<5; ++j) s.body.push_back({start_pos.y, std::max(0, start_pos.x - j)});
        state.snakes[s.id] = s;
    }
    state.safe_zone_schedule.push_back({81, 100, {0,0,39,29}, {3,2,36,27}});
    state.safe_zone_schedule.push_back({161, 180, {3,2,36,27}, {7,5,32,24}});
    state.safe_zone_schedule.push_back({221, 240, {7,5,32,24}, {10,7,29,22}});
    for(int i=0; i<15; ++i) handle_item_spawning(state);
}

int main(int argc, char* argv[]) {
    if (argc < 3 || (argc - 1) % 2 != 0) { std::cerr << "Usage: " << argv[0] << " <ID1> <bot1_path> ..." << std::endl; return 1; }
    
    std::vector<BotProcess> bots;
    for (int i = 1; i < argc; i += 2) bots.push_back({std::stoll(argv[i]), argv[i+1]});

    GameState state;
    initialize_game(state, bots);
    
    for (int tick = 1; tick <= state.total_ticks; ++tick) {
        state.current_tick = tick;
        get_bot_decisions(state, bots);
        run_game_tick(state);
        std::cout << state.to_json() << std::endl;
        
        size_t living_snakes = 0;
        for(const auto& pair : state.snakes) if(pair.second.is_alive) living_snakes++;
        if (living_snakes == 0 && state.current_tick > 1) {
            std::cerr << "All snakes are dead. Game over at tick " << tick << std::endl;
            break;
        }
    }
    std::cerr << "Game finished. Log saved to game.log" << std::endl;
    return 0;
}