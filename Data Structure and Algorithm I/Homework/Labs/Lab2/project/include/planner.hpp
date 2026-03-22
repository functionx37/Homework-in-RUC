#pragma once
// #pragma once 是一个预处理指令，确保这个头文件在编译过程中只被包含一次，防止重复定义错误。

#include <miniSTL/vector.hpp> // 引入 miniSTL 库中的 Vector 容器，类似于 std::vector
#include <miniSTL/list.hpp>   // 引入 miniSTL 库中的 List 容器，类似于 std::list
#include <string>             // 引入标准库的字符串类
#include <tuple>              // 引入 tuple（元组），用于存储多个不同类型的值

// Flight 结构体用于存储单个航班的详细信息
struct Flight {
    int flight_id;                  // 航班的唯一标识符 ID
    std::string departure_date;     // 出发日期，例如 "2024-01-01"
    std::string type;               // 航班类型，例如 "Intl" (国际) 或 "Dome" (国内)
    std::string flight_no;          // 航班号，例如 "CA1234"
    int departure_airport;          // 出发机场的 ID
    int arrival_airport;            // 到达机场的 ID
    std::string departure_time;     // 计划出发时间，例如 "08:00:00"
    std::string arrival_time;       // 计划到达时间，例如 "10:30:00"
    int airplane_id;                // 飞机的 ID
    std::string airplane_model;     // 飞机型号，例如 "Boeing 737"
    int air_fares;                  // 机票价格
    
    // Parsed times for easier calculation
    // 为了方便计算，我们将字符串格式的时间转换成了 time_t 类型（通常是自 1970 年 1 月 1 日以来的秒数）
    time_t dep_time_t; // 转换后的出发时间
    time_t arr_time_t; // 转换后的到达时间
};

// DFSResult 结构体用于存储深度优先搜索（DFS）的结果
struct DFSResult {
    // 存储访问过的机场 ID 序列
    Vector<int> airport_sequence;
};

// BFSResult 结构体用于存储广度优先搜索（BFS）的结果
struct BFSResult {
    // 存储访问过的机场 ID 序列
    Vector<int> airport_sequence;
};

// ConnectivityResult 结构体用于存储连通性查询的结果
struct ConnectivityResult {
    bool connected;             // 两个机场是否连通（即是否存在路径）
    Vector<int> flight_ids;     // 如果连通，存储一条可能的飞行路径（由航班 ID 组成的列表）
};

// ShortestPathResult 结构体用于存储最短路径查询的结果
struct ShortestPathResult {
    bool found;                 // 是否找到了路径
    Vector<int> flight_ids;     // 构成最短路径的航班 ID 列表
    int total_time_minutes;     // 总耗时（以分钟为单位）
};

// MinimumCostPathResult 结构体用于存储最小花费路径查询的结果
struct MinimumCostPathResult {
    bool found;                 // 是否找到了路径
    Vector<int> flight_ids;     // 构成最小花费路径的航班 ID 列表
    int total_cost;             // 总花费金额
};

// AllPathsRes 结构体用于存储所有可能路径的查询结果
struct AllPathsRes {
    // 存储所有找到的路径列表。
    // 外层的 miniSTL Vector 存储具体的每一条路径，
    // 内层的 miniSTL Vector<int> 存储一条路径中的航班 ID 序列。
    Vector<Vector<int>> all_paths_list; 
};

// Planner 类是核心类，负责管理航班数据并处理各种查询
class Planner {
public:
    // 构造函数：初始化 Planner 对象
    // 参数 data_path: 包含航班数据的 CSV 文件路径
    Planner(const std::string& data_path);

    // 查询 DFS 遍历序列
    // 参数 airport_id: 起始机场 ID
    // 参数 start_time: 限制出发时间（可选，视具体实现逻辑而定）
    DFSResult query_dfs(int airport_id, std::string start_time);

    // 查询 BFS 遍历序列
    // 参数 airport_id: 起始机场 ID
    // 参数 start_time: 限制出发时间
    BFSResult query_bfs(int airport_id, std::string start_time);

    // 查询两个机场之间是否连通
    // 参数 airport_1: 起始机场 ID
    // 参数 airport_2: 目标机场 ID
    ConnectivityResult query_connectivity(int airport_1, int airport_2);

    // 查询两个机场之间的最短路径（时间最短）
    // 参数 airport_1: 起始机场 ID
    // 参数 airport_2: 目标机场 ID
    // 参数 start_time: 允许的最早出发时间
    // 参数 end_time: 允许的最晚到达时间
    ShortestPathResult query_shortest_path(int airport_1, int airport_2, std::string start_time, std::string end_time);

    // 查询两个机场之间的最小花费路径
    // 参数同上
    MinimumCostPathResult query_minimum_cost_path(int airport_1, int airport_2, std::string start_time, std::string end_time);

    // 查询两个机场之间的所有可行路径
    // 参数同上
    AllPathsRes query_all_paths(int airport_1, int airport_2, std::string start_time, std::string end_time);

    // 辅助函数：根据航班 ID 获取航班详细信息
    // 如果找到返回 true，并将信息填充到 f 中；否则返回 false
    bool get_flight_by_id(int id, Flight& f);

private:
    // 存储所有加载的航班对象
    Vector<Flight> flights;
    
    // 邻接表 (Adjacency list) 用于表示图结构
    // 这是一个 Vector，其中每个元素对应一个机场（索引为 airport_id）
    // 每个元素是一个 List<int>，存储从该机场出发的所有航班在 'flights' 向量中的下标索引
    // 假设机场 ID 分布比较密集，或者我们通过某种方式映射了 ID。
    // 这里我们假设机场 ID 是整数且最大值在可控范围内。
    Vector<List<int>> adj; 
    
    // 记录最大的机场 ID，用于确定邻接表的大小
    int max_airport_id = 0;

    // 辅助函数：构建图
    // 根据加载的航班数据，填充邻接表 'adj'
    void build_graph();

    // 辅助函数：解析时间字符串
    // 将 "YYYY-MM-DD HH:MM:SS" 格式的字符串转换为 time_t 类型
    time_t parse_time(const std::string& time_str);
};
