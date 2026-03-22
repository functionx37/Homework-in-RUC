#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cmath>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

// 定义棋盘状态节点
struct Node {
    vector<vector<int>> board; // 棋盘布局
    int g;                     // g(n): 从初始状态到当前状态的实际代价（步数）
    int h;                     // h(n): 从当前状态到目标状态的启发式评估代价（曼哈顿距离）
    shared_ptr<Node> parent;   // 指向父节点，用于回溯路径

    Node(const vector<vector<int>>& b, int g_val, int h_val, shared_ptr<Node> p)
        : board(b), g(g_val), h(h_val), parent(p) {}

    // f(n) = g(n) + h(n)
    int f() const {
        return g + h;
    }
};

// 用于优先队列的自定义比较器，使得f(n)值最小的节点拥有最高优先级
struct CompareNodes {
    bool operator()(const shared_ptr<Node>& a, const shared_ptr<Node>& b) const {
        // 主要按f值排序
        if (a->f() != b->f()) {
            return a->f() > b->f();
        }
        // 如果f值相同，优先选择h值较小的（更接近目标）
        return a->h > b->h;
    }
};

// 打印棋盘
void printBoard(const vector<vector<int>>& board) {
    for (const auto& row : board) {
        for (int val : row) {
            cout << setw(3) << val << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// 查找特定数值在棋盘上的位置
pair<int, int> findPosition(const vector<vector<int>>& board, int value) {
    int n = board.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (board[i][j] == value) {
                return {i, j};
            }
        }
    }
    return {-1, -1}; // 理论上对于0不会发生
}

// 计算曼哈顿距离启发函数
int calculateManhattanDistance(const vector<vector<int>>& board, const map<int, pair<int, int>>& goal_positions) {
    int distance = 0;
    int n = board.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int value = board[i][j];
            // 只计算非空格、非障碍的数字
            if (value != 0 && value != -1) {
                auto it = goal_positions.find(value);
                if (it != goal_positions.end()) {
                    distance += abs(i - it->second.first) + abs(j - it->second.second);
                }
            }
        }
    }
    return distance;
}

// A* 算法求解主函数
void solvePuzzle(const vector<vector<int>>& start_state, const vector<vector<int>>& goal_state) {
    int n = start_state.size();

    // 预先计算目标状态中各数值的位置，方便快速查找
    map<int, pair<int, int>> goal_positions;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (goal_state[i][j] != -1) {
                goal_positions[goal_state[i][j]] = {i, j};
            }
        }
    }

    // Open List (优先队列) 和 Closed List (集合)
    priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, CompareNodes> open_list;
    set<vector<vector<int>>> closed_list;

    // 创建起始节点
    int start_h = calculateManhattanDistance(start_state, goal_positions);
    auto start_node = make_shared<Node>(start_state, 0, start_h, nullptr);
    open_list.push(start_node);

    // 定义四个移动方向: 上, 下, 左, 右
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    shared_ptr<Node> goal_node = nullptr;

    while (!open_list.empty()) {
        shared_ptr<Node> current_node = open_list.top();
        open_list.pop();

        // 如果当前状态已在Closed List中，则跳过
        if (closed_list.count(current_node->board)) {
            continue;
        }
        closed_list.insert(current_node->board);

        // 检查是否达到目标状态
        if (current_node->board == goal_state) {
            goal_node = current_node;
            break;
        }

        // 找到空格(0)的位置
        pair<int, int> zero_pos = findPosition(current_node->board, 0);
        int r = zero_pos.first;
        int c = zero_pos.second;

        // 尝试向四个方向移动
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // 检查新位置是否合法（在边界内且不是障碍物）
            if (nr >= 0 && nr < n && nc >= 0 && nc < n && current_node->board[nr][nc] != -1) {
                vector<vector<int>> new_board = current_node->board;
                swap(new_board[r][c], new_board[nr][nc]);

                if (!closed_list.count(new_board)) {
                    int g = current_node->g + 1;
                    int h = calculateManhattanDistance(new_board, goal_positions);
                    auto new_node = make_shared<Node>(new_board, g, h, current_node);
                    open_list.push(new_node);
                }
            }
        }
    }

    // 如果找到解，则回溯路径并输出
    if (goal_node) {
        vector<shared_ptr<Node>> path;
        shared_ptr<Node> current = goal_node;
        while (current) {
            path.push_back(current);
            current = current->parent;
        }
        reverse(path.begin(), path.end());

        cout << path.size() - 1 << endl;
        for (const auto& node : path) {
            printBoard(node->board);
        }
    } else {
        cout << "No solution found." << endl;
    }
}

// 从标准输入读取一个完整的棋盘
vector<vector<int>> readBoard() {
    vector<vector<int>> board;
    string line;
    int n = 0;
    // 循环读取，直到读到一个完整的N*N矩阵
    while (getline(cin, line) && !line.empty()) {
        stringstream ss(line);
        vector<int> row;
        int num;
        while (ss >> num) {
            row.push_back(num);
        }
        // 如果是第一行，确定矩阵维度N
        if (n == 0) {
            n = row.size();
        }
        board.push_back(row);
        if (board.size() == n) {
            break;
        }
    }
    return board;
}


int main() {
    // 优化C++的输入输出速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 读取初始状态
    vector<vector<int>> start_state = readBoard();
    // 读取目标状态
    vector<vector<int>> goal_state = readBoard();
    
    // 确保输入合法
    if (start_state.empty() || goal_state.empty() || start_state.size() != goal_state.size()) {
        // 如果输入不符合预期，则静默退出
        return 1;
    }

    solvePuzzle(start_state, goal_state);

    return 0;
}