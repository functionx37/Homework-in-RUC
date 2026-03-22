#include "planner.hpp" // 包含 Planner 类的定义
#include <iostream>    // 包含标准输入输出流（std::cout, std::cin）
#include <sstream>     // 包含字符串流，用于处理字符串分割
#include <string>      // 包含字符串类

// 打印帮助信息函数
// 告诉用户支持哪些命令以及如何使用
void print_help() {
    std::cout << "Commands:\n";
    std::cout << "  dfs <airport_id> <start_time>\n";
    std::cout << "  bfs <airport_id> <start_time>\n";
    std::cout << "  connectivity <airport1> <airport2>\n";
    std::cout << "  shortest <airport1> <airport2> <start_time> <end_time>\n";
    std::cout << "  min_cost <airport1> <airport2> <start_time> <end_time>\n";
    std::cout << "  all_paths <airport1> <airport2> <start_time> <end_time>\n";
    std::cout << "  exit\n";
    std::cout << "Time format: M/D/YYYY H:MM (e.g., 5/5/2017 12:20)\n";
    std::cout << "Note: If time contains space, enclose it in quotes or just type it naturally, the parser handles it.\n";
}

// 辅助函数：解析命令行输入
// 将用户输入的一行字符串按空格分割成多个参数
Vector<std::string> parse_args(const std::string& line) {
    Vector<std::string> args;
    std::stringstream ss(line);
    std::string temp;
    while (ss >> temp) {
        args.push_back(temp);
    }
    return args;
}

// 辅助函数：打印航班详细信息
void print_path(Planner& planner, const Vector<int>& flight_ids) {
    if (flight_ids.empty()) return;
    for (size_t i = 0; i < flight_ids.size(); ++i) {
        int fid = flight_ids[i];
        Flight f;
        if (planner.get_flight_by_id(fid, f)) {
            std::cout << "\n    Step " << i + 1 << ": Flight " << f.flight_no 
                      << " (ID:" << f.flight_id << ")"
                      << " From " << f.departure_airport << " To " << f.arrival_airport
                      << " Time: " << f.departure_time << " - " << f.arrival_time
                      << " Cost: " << f.air_fares;
        } else {
            std::cout << "\n    Step " << i + 1 << ": Flight ID " << fid << " (Info not found)";
        }
    }
    std::cout << std::endl;
}

// 主函数：程序的入口点
int main(int argc, char* argv[]) {
    // 默认数据文件路径
    std::string data_path = "../project/data/flight-data.csv";
    // 如果命令行提供了参数，则使用提供的路径（例如 ./main data.csv）
    if (argc > 1) {
        data_path = argv[1];
    }

    std::cout << "Loading data from " << data_path << "..." << std::endl;
    // 创建 Planner 对象，这会触发数据加载和图的构建
    Planner planner(data_path);
    std::cout << "Data loaded." << std::endl;

    // 打印帮助菜单
    print_help();

    std::string line;
    // 进入主循环，不断等待用户输入命令
    while (true) {
        std::cout << "> "; // 打印提示符
        if (!std::getline(std::cin, line)) break; // 读取一行输入，如果遇到 EOF（如 Ctrl+D/Z）则退出
        if (line.empty()) continue; // 如果输入为空行，则继续下一次循环

        // 解析参数
        auto args = parse_args(line);
        if (args.empty()) continue;

        // 获取第一个参数作为命令名
        std::string cmd = args[0];

        // 根据命令分发处理逻辑
        if (cmd == "exit") {
            break; // 退出循环，结束程序
        } else if (cmd == "dfs") {
            // 处理 DFS 命令
            if (args.size() < 4) {
                std::cout << "Usage: dfs <id> <date> <time>" << std::endl;
                continue;
            }
            int id = std::stoi(args[1]); // 解析机场 ID
            std::string time = args[2] + " " + args[3]; // 拼接日期和时间
            auto res = planner.query_dfs(id, time); // 调用 Planner 的查询方法
            
            // DFS/BFS 输出的是机场序列，不是航班序列，保持原样
            std::cout << "DFS Result (Airport Sequence): ";
            for (size_t i = 0; i < res.airport_sequence.size(); ++i) {
                std::cout << res.airport_sequence[i] << (i == res.airport_sequence.size() - 1 ? "" : " -> ");
            }
            std::cout << std::endl;
        } else if (cmd == "bfs") {
            // 处理 BFS 命令
            if (args.size() < 4) {
                std::cout << "Usage: bfs <id> <date> <time>" << std::endl;
                continue;
            }
            int id = std::stoi(args[1]);
            std::string time = args[2] + " " + args[3];
            auto res = planner.query_bfs(id, time);
            std::cout << "BFS Result (Airport Sequence): ";
            for (size_t i = 0; i < res.airport_sequence.size(); ++i) {
                std::cout << res.airport_sequence[i] << (i == res.airport_sequence.size() - 1 ? "" : " -> ");
            }
            std::cout << std::endl;
        } else if (cmd == "connectivity") {
            // 处理连通性查询
            if (args.size() < 3) {
                std::cout << "Usage: connectivity <id1> <id2>" << std::endl;
                continue;
            }
            int id1 = std::stoi(args[1]);
            int id2 = std::stoi(args[2]);
            auto res = planner.query_connectivity(id1, id2);
            if (res.connected) {
                std::cout << "Connected: Yes. Path details:";
                print_path(planner, res.flight_ids);
            } else {
                std::cout << "Connected: No" << std::endl;
            }
        } else if (cmd == "shortest") {
            // 处理最短路径查询
            if (args.size() < 7) {
                std::cout << "Usage: shortest <id1> <id2> <start_date> <start_time> <end_date> <end_time>" << std::endl;
                continue;
            }
            int id1 = std::stoi(args[1]);
            int id2 = std::stoi(args[2]);
            std::string start = args[3] + " " + args[4];
            std::string end = args[5] + " " + args[6];
            auto res = planner.query_shortest_path(id1, id2, start, end);
            if (res.found) {
                std::cout << "Found. Total Time: " << res.total_time_minutes << " mins. Path details:";
                print_path(planner, res.flight_ids);
            } else {
                std::cout << "Not found." << std::endl;
            }
        } else if (cmd == "min_cost") {
            // 处理最小花费查询
            if (args.size() < 7) {
                std::cout << "Usage: min_cost <id1> <id2> <start_date> <start_time> <end_date> <end_time>" << std::endl;
                continue;
            }
            int id1 = std::stoi(args[1]);
            int id2 = std::stoi(args[2]);
            std::string start = args[3] + " " + args[4];
            std::string end = args[5] + " " + args[6];
            auto res = planner.query_minimum_cost_path(id1, id2, start, end);
            if (res.found) {
                std::cout << "Found. Total Cost: " << res.total_cost << ". Path details:";
                print_path(planner, res.flight_ids);
            } else {
                std::cout << "Not found." << std::endl;
            }
        } else if (cmd == "all_paths") {
            // 处理所有路径查询
            if (args.size() < 7) {
                std::cout << "Usage: all_paths <id1> <id2> <start_date> <start_time> <end_date> <end_time>" << std::endl;
                continue;
            }
            int id1 = std::stoi(args[1]);
            int id2 = std::stoi(args[2]);
            std::string start = args[3] + " " + args[4];
            std::string end = args[5] + " " + args[6];
            auto res = planner.query_all_paths(id1, id2, start, end);
            std::cout << "Found " << res.all_paths_list.size() << " paths:" << std::endl;
            for (size_t i = 0; i < res.all_paths_list.size(); ++i) {
                std::cout << "Path " << i + 1 << ":";
                print_path(planner, res.all_paths_list[i]);
            }
        } else {
            std::cout << "Unknown command." << std::endl;
        }
    }

    return 0; // 程序正常退出
}
