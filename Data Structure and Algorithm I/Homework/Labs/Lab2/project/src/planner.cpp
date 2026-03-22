#include "planner.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <queue>
#include <set>
#include <map>
#include <functional>

// 辅助函数：将时间字符串 "M/D/YYYY H:MM" 解析为 time_t
// time_t 是一个整数类型，表示自 1970 年 1 月 1 日以来的秒数，方便比较时间的先后。
time_t Planner::parse_time(const std::string& time_str) {
    std::tm tm = {}; // 初始化 tm 结构体
    std::istringstream ss(time_str);
    char delimiter;
    int month, day, year, hour, minute;
    
    // 假设输入格式为: 5/5/2017 12:20
    // 使用输入流提取运算符 >> 按顺序读取 月、分隔符、日、分隔符、年、时、分隔符、分
    if (ss >> month >> delimiter >> day >> delimiter >> year >> hour >> delimiter >> minute) {
        tm.tm_mon = month - 1;   // tm_mon 范围是 0-11，所以要减 1
        tm.tm_mday = day;        // tm_mday 范围是 1-31
        tm.tm_year = year - 1900; // tm_year 是自 1900 年起的年份
        tm.tm_hour = hour;
        tm.tm_min = minute;
        tm.tm_sec = 0;           // 秒数默认为 0
        tm.tm_isdst = -1;        // 让 mktime 自动判断夏令时
        return std::mktime(&tm); // 将 tm 结构体转换为 time_t 并返回
    }
    return 0; // 解析失败返回 0
}

// Planner 类的构造函数
// 参数 data_path: 航班数据 CSV 文件的路径
Planner::Planner(const std::string& data_path) {
    std::ifstream file(data_path); // 打开文件
    std::string line;
    
    // 跳过第一行（表头）
    std::getline(file, line);

    // 逐行读取文件内容
    while (std::getline(file, line)) {
        if (line.empty()) continue; // 跳过空行
        std::stringstream ss(line);
        std::string segment;
        Vector<std::string> tokens;
        
        // 使用逗号分割每一行的数据
        while (std::getline(ss, segment, ',')) {
            tokens.push_back(segment);
        }

        // 确保一行至少有 11 个字段，否则视为无效行
        if (tokens.size() < 11) continue;

        Flight f;
        // 依次解析字段并将字符串转换为相应类型（int 或 string）
        f.flight_id = std::stoi(tokens[0]);
        f.departure_date = tokens[1];
        f.type = tokens[2];
        f.flight_no = tokens[3];
        f.departure_airport = std::stoi(tokens[4]);
        f.arrival_airport = std::stoi(tokens[5]);
        f.departure_time = tokens[6];
        f.arrival_time = tokens[7];
        f.airplane_id = std::stoi(tokens[8]);
        f.airplane_model = tokens[9];
        f.air_fares = std::stoi(tokens[10]);
        
        // 解析出发和到达时间为 time_t，方便后续计算
        f.dep_time_t = parse_time(f.departure_time);
        f.arr_time_t = parse_time(f.arrival_time);

        // 将航班添加到航班列表中
        flights.push_back(f);
        
        // 更新最大机场 ID，以便后续构建邻接表时确定大小
        if (f.departure_airport > max_airport_id) max_airport_id = f.departure_airport;
        if (f.arrival_airport > max_airport_id) max_airport_id = f.arrival_airport;
    }

    // 数据加载完毕后，构建图结构（邻接表）
    build_graph();
}

// 构建图的邻接表
void Planner::build_graph() {
    // 根据最大机场 ID 调整邻接表的大小
    // 我们假设机场 ID 是连续的或者是比较小的整数
    // 重新创建邻接表，每个元素是一个空的 List<int>
    adj = Vector<List<int>>();
    for(int i=0; i <= max_airport_id; ++i) {
        adj.push_back(List<int>());
    }

    // 遍历所有航班，将航班索引加入到出发机场对应的列表中
    for (size_t i = 0; i < flights.size(); ++i) {
        int u = flights[i].departure_airport;
        adj[u].push_back(i); // 存储航班在 flights 向量中的下标，而不是航班 ID
    }
}

// 深度优先搜索（DFS）查询
// 目标：遍历所有可到达的机场，优先选择出发时间早的航班，其次选择目标机场 ID 小的航班
DFSResult Planner::query_dfs(int airport_id, std::string start_time) {
    DFSResult result;
    time_t start_t = parse_time(start_time);
    
    std::set<int> visited; // 用于记录已经访问过的机场，防止循环和重复访问
    
    // 我们使用递归来实现 DFS
    // Helper function (Lambda 表达式)
    std::function<void(int, time_t)> dfs_helper = [&](int u, time_t t) {
        visited.insert(u); // 标记当前机场为已访问
        result.airport_sequence.push_back(u); // 将当前机场加入结果序列
        
        // 收集所有从当前机场出发的候选航班
        struct Candidate {
            int flight_idx; // 航班索引
            time_t dep;     // 出发时间
            int target;     // 目标机场
        };
        Vector<Candidate> candidates;
        
        if (u <= max_airport_id) {
            // 遍历当前机场的所有出发航班
            for (auto it = adj[u].begin(); it != adj[u].end(); ++it) {
                int f_idx = *it;
                const auto& f = flights[f_idx];
                // 筛选条件：航班出发时间必须晚于或等于当前到达时间 t
                if (f.dep_time_t >= t) {
                    // 且目标机场未被访问过
                    if (visited.find(f.arrival_airport) == visited.end()) {
                        candidates.push_back({f_idx, f.dep_time_t, f.arrival_airport});
                    }
                }
            }
        }
        
        // 排序候选项，以满足题目要求的遍历顺序
        // 规则：先按出发时间排序，如果时间相同，则按目标机场 ID 排序
        std::sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
            if (a.dep != b.dep) return a.dep < b.dep;
            return a.target < b.target;
        });
        
        // 按排序后的顺序递归访问下一个机场
        for (const auto& cand : candidates) {
            // 再次检查 visited，因为在递归过程中可能已经被其他分支访问过了
            if (visited.find(cand.target) == visited.end()) {
                const auto& f = flights[cand.flight_idx];
                // 递归调用，更新到达时间为该航班的到达时间
                dfs_helper(cand.target, f.arr_time_t);
            }
        }
    };
    
    // 从起始机场开始 DFS
    dfs_helper(airport_id, start_t);
    return result;
}

// 广度优先搜索（BFS）查询
// 目标：层序遍历，原则类似
BFSResult Planner::query_bfs(int airport_id, std::string start_time) {
    BFSResult result;
    time_t start_t = parse_time(start_time);
    
    std::set<int> visited; // 记录已访问机场
    std::queue<std::pair<int, time_t>> q; // BFS 队列，存储 {机场 ID, 到达该机场的时间}
    
    // 初始化：将起始机场加入队列
    q.push({airport_id, start_t});
    visited.insert(airport_id); // 标记为已访问
    
    while (!q.empty()) {
        auto [u, t] = q.front();
        q.pop();
        result.airport_sequence.push_back(u); // 记录访问顺序
        
        // 收集候选航班
        struct Candidate {
            int flight_idx;
            time_t dep;
            int target;
        };
        Vector<Candidate> candidates;
        
        if (u <= max_airport_id) {
            for (auto it = adj[u].begin(); it != adj[u].end(); ++it) {
                int f_idx = *it;
                const auto& f = flights[f_idx];
                // 筛选条件：出发时间 >= 当前到达时间
                if (f.dep_time_t >= t) {
                    // 如果目标机场未被访问
                    if (visited.find(f.arrival_airport) == visited.end()) {
                        candidates.push_back({f_idx, f.dep_time_t, f.arrival_airport});
                    }
                }
            }
        }
        
        // 排序：先时间，后 ID
        std::sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
            if (a.dep != b.dep) return a.dep < b.dep;
            return a.target < b.target;
        });
        
        // 将排序后的邻居加入队列
        for (const auto& cand : candidates) {
            // 再次检查是否已访问（因为可能被同层的其他节点抢先访问了）
            if (visited.find(cand.target) == visited.end()) {
                visited.insert(cand.target); // 加入队列时即标记访问
                q.push({cand.target, flights[cand.flight_idx].arr_time_t});
            }
        }
    }
    
    return result;
}

// 查询连通性：是否存在从 airport_1 到 airport_2 的路径（直达或一次中转）
ConnectivityResult Planner::query_connectivity(int airport_1, int airport_2) {
    ConnectivityResult result;
    result.connected = false;
    
    // 1. 检查直达航班
    if (airport_1 <= max_airport_id) {
        for (auto it = adj[airport_1].begin(); it != adj[airport_1].end(); ++it) {
            const auto& f = flights[*it];
            if (f.arrival_airport == airport_2) {
                result.connected = true;
                result.flight_ids.push_back(f.flight_id);
                return result; // 找到直达即可返回
            }
        }
    }
    
    // 2. 检查一次中转的情况：A -> X -> B
    // 我们需要找到两趟航班 f1 (A->X) 和 f2 (X->B)，且时间上衔接（f2.dep >= f1.arr）
    // 注意：题目可能没有严格要求时间衔接，但根据常理和代码逻辑应考虑。
    // 这里我们假设需要满足时间衔接。但由于没有给定起始时间，我们寻找任意一对满足衔接条件的航班。
    
    if (airport_1 <= max_airport_id) {
        for (auto it1 = adj[airport_1].begin(); it1 != adj[airport_1].end(); ++it1) {
            const auto& f1 = flights[*it1];
            int x = f1.arrival_airport; // 中转机场 X
            if (x <= max_airport_id) {
                // 遍历从中转机场 X 出发的所有航班
                for (auto it2 = adj[x].begin(); it2 != adj[x].end(); ++it2) {
                    const auto& f2 = flights[*it2];
                    if (f2.arrival_airport == airport_2) {
                        // 检查时间衔接
                        if (f2.dep_time_t >= f1.arr_time_t) {
                            result.connected = true;
                            result.flight_ids.push_back(f1.flight_id);
                            result.flight_ids.push_back(f2.flight_id);
                            return result;
                        }
                    }
                }
            }
        }
    }
    
    return result;
}

// 查询最短路径（时间最短）
// 使用 Dijkstra 算法的变体。这里的“权重”是到达时间。我们想尽早到达。
ShortestPathResult Planner::query_shortest_path(int airport_1, int airport_2, std::string start_time, std::string end_time) {
    ShortestPathResult result;
    result.found = false;
    result.total_time_minutes = -1;
    
    time_t start_t = parse_time(start_time);
    time_t end_t = parse_time(end_time);
    
    // 定义搜索节点
    struct Node {
        time_t time;    // 当前到达时间
        int u;          // 当前所在机场
        int flight_idx; // 到达该机场所乘坐的航班索引（用于回溯路径）
        std::shared_ptr<Node> parent; // 父节点指针（用于回溯路径）
        
        // 优先队列总是弹出最小的元素，所以我们要重载 > 运算符来实现最小堆
        bool operator>(const Node& other) const {
            return time > other.time; // 时间越早优先级越高
        }
    };
    
    // 优先队列（最小堆）
    std::priority_queue<Node, Vector<Node>, std::greater<Node>> pq;
    
    // 初始状态：我们在 airport_1，时间是 start_t
    pq.push(Node{start_t, airport_1, -1, nullptr});
    
    // 记录到达每个机场的最早时间，用于剪枝
    std::map<int, time_t> min_arrival;
    
    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();
        
        // 如果当前到达时间已经超过了限制的结束时间，则忽略
        if (current.time > end_t) continue;
        
        // 剪枝：如果之前已经以更早或相同的时间到达过该机场，则无需再处理
        if (min_arrival.count(current.u) && min_arrival[current.u] <= current.time) {
            continue;
        }
        min_arrival[current.u] = current.time;
        
        // 如果到达目标机场
        if (current.u == airport_2) {
            result.found = true;
            result.total_time_minutes = (current.time - start_t) / 60; // 计算总耗时（分钟）
            
            // 回溯路径
            Node* p = &current;
            while (p->parent) {
                // 将航班 ID 插入到路径列表的前面，因为我们是倒序回溯的
                result.flight_ids.insert(result.flight_ids.begin(), flights[p->flight_idx].flight_id);
                p = p->parent.get();
            }
            return result;
        }
        
        // 拓展节点
        if (current.u <= max_airport_id) {
            for (auto it = adj[current.u].begin(); it != adj[current.u].end(); ++it) {
                int f_idx = *it;
                const auto& f = flights[f_idx];
                // 检查时间约束：
                // 1. 航班出发时间 >= 当前到达时间
                // 2. 航班到达时间 <= 全局允许的最晚结束时间
                if (f.dep_time_t >= current.time && f.arr_time_t <= end_t) {
                    // 将新状态加入队列
                    pq.push(Node{f.arr_time_t, f.arrival_airport, f_idx, std::make_shared<Node>(current)});
                }
            }
        }
    }
    
    return result;
}

// 查询最小花费路径
// 同样使用 Dijkstra 算法。这次的“权重”是累积的票价。
MinimumCostPathResult Planner::query_minimum_cost_path(int airport_1, int airport_2, std::string start_time, std::string end_time) {
    MinimumCostPathResult result;
    result.found = false;
    result.total_cost = -1;
    
    time_t start_t = parse_time(start_time);
    time_t end_t = parse_time(end_time);
    
    struct Node {
        int cost;           // 当前总花费
        int u;              // 当前所在机场
        time_t current_time;// 当前到达时间
        int flight_idx;     // 航班索引
        std::shared_ptr<Node> parent;
        
        // 按花费排序，花费越少越优先
        bool operator>(const Node& other) const {
            return cost > other.cost;
        }
    };
    
    std::priority_queue<Node, Vector<Node>, std::greater<Node>> pq;
    pq.push(Node{0, airport_1, start_t, -1, nullptr});
    
    // 剪枝状态记录：记录到达某个机场时的 {花费, 到达时间} 对
    // 因为这不仅仅是求最短路，还有时间限制。
    // 如果我们以更高的花费到达同一个点，但这使得时间更早（从而能赶上后续某趟便宜的航班），那么这个状态也是有价值的。
    // 这是一个多目标优化的简化版（帕累托最优）。
    // 这里我们简单记录所有非支配状态。
    std::map<int, Vector<std::pair<int, time_t>>> visited_states; 
    
    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();
        
        // 剪枝检查：是否被支配？
        // 如果存在一个状态 s，使得 s.cost <= current.cost 且 s.time <= current.time，那么 current 就是无用的（除了相等的情况）。
        // 实际上，只要 cost 更小，且时间足够赶上所有后续航班，就更好。
        // 但为了简化，我们只判断：如果之前有更少花费且更早（或由同一时间）到达的，则当前状态被支配。
        bool dominated = false;
        if (visited_states.count(current.u)) {
            for (const auto& state : visited_states[current.u]) {
                if (state.first <= current.cost && state.second <= current.current_time) {
                    dominated = true;
                    break;
                }
            }
        }
        if (dominated) continue;
        visited_states[current.u].push_back({current.cost, current.current_time});
        
        // 找到目标
        if (current.u == airport_2) {
            result.found = true;
            result.total_cost = current.cost;
            
            Node* p = &current;
            while (p->parent) {
                result.flight_ids.insert(result.flight_ids.begin(), flights[p->flight_idx].flight_id);
                p = p->parent.get();
            }
            return result;
        }
        
        // 拓展
        if (current.u <= max_airport_id) {
            for (auto it = adj[current.u].begin(); it != adj[current.u].end(); ++it) {
                int f_idx = *it;
                const auto& f = flights[f_idx];
                // 检查时间是否有效
                if (f.dep_time_t >= current.current_time && f.arr_time_t <= end_t) {
                    pq.push(Node{current.cost + f.air_fares, f.arrival_airport, f.arr_time_t, f_idx, std::make_shared<Node>(current)});
                }
            }
        }
    }
    
    return result;
}

// 查询所有路径（直达或一次中转）
// 这个函数名 query_all_paths 听起来像查找所有可能的路径，但根据题目上面 connecticity 的逻辑，通常限制为最多 1 次中转，
// 或者题目有特定限制。在这个实现中，我们只查找直达和 1 次中转的路径。
AllPathsRes Planner::query_all_paths(int airport_1, int airport_2, std::string start_time, std::string end_time) {
    AllPathsRes result;
    time_t start_t = parse_time(start_time);
    time_t end_t = parse_time(end_time);
    
    // 1. 查找直达路径
    if (airport_1 <= max_airport_id) {
        for (auto it = adj[airport_1].begin(); it != adj[airport_1].end(); ++it) {
            const auto& f = flights[*it];
            // 检查时间窗口
            if (f.dep_time_t >= start_t && f.arr_time_t <= end_t) {
                if (f.arrival_airport == airport_2) {
                    Vector<int> path;
                    path.push_back(f.flight_id);
                    result.all_paths_list.push_back(path);
                }
            }
        }
    }
    
    // 2. 查找 1 次中转路径
    if (airport_1 <= max_airport_id) {
        for (auto it1 = adj[airport_1].begin(); it1 != adj[airport_1].end(); ++it1) {
            const auto& f1 = flights[*it1];
            // 第一段行程的时间检查
            if (f1.dep_time_t >= start_t && f1.arr_time_t <= end_t) {
                int x = f1.arrival_airport;
                if (x <= max_airport_id) {
                    for (auto it2 = adj[x].begin(); it2 != adj[x].end(); ++it2) {
                        const auto& f2 = flights[*it2];
                        // 第二段行程的时间检查（衔接 + 截止）
                        if (f2.dep_time_t >= f1.arr_time_t && f2.arr_time_t <= end_t) {
                            if (f2.arrival_airport == airport_2) {
                                Vector<int> path;
                                path.push_back(f1.flight_id);
                                path.push_back(f2.flight_id);
                                result.all_paths_list.push_back(path);
                            }
                        }
                    }
                }
            }
        }
    }
    
    return result;
}

// 实现辅助函数：根据 ID 查找航班
bool Planner::get_flight_by_id(int id, Flight& f) {
    // 这里使用简单的线性搜索。
    // 如果航班数量巨大，应该建立一个 map<int, Flight> 或 map<int, int> (id -> index) 的索引。
    // 但鉴于这是一个实验室项目，且前面代码未建立此索引，我们保持简单。
    for (size_t i = 0; i < flights.size(); ++i) {
        if (flights[i].flight_id == id) {
            f = flights[i];
            return true;
        }
    }
    return false;
}
