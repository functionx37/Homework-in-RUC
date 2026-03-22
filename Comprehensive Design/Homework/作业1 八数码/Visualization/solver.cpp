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
#include <string>

// Node 结构体, CompareNodes, findPosition, calculateManhattanDistance 等函数保持不变
// 它们已经足够通用，无需修改
using namespace std;

struct Node {
    vector<vector<int>> board;
    int g;
    int h;
    shared_ptr<Node> parent;
    Node(const vector<vector<int>>& b, int g_val, int h_val, shared_ptr<Node> p)
        : board(b), g(g_val), h(h_val), parent(p) {}
    int f() const { return g + h; }
};

struct CompareNodes {
    bool operator()(const shared_ptr<Node>& a, const shared_ptr<Node>& b) const {
        if (a->f() != b->f()) return a->f() > b->f();
        return a->h > b->h;
    }
};

pair<int, int> findPosition(const vector<vector<int>>& board, int value) {
    int n = board.size();
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (board[i][j] == value) return {i, j};
    return {-1, -1};
}

int calculateManhattanDistance(const vector<vector<int>>& board, const map<int, pair<int, int>>& goal_positions) {
    int distance = 0;
    int n = board.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int value = board[i][j];
            if (value > 0) { // 只计算正数牌的距离, 自动忽略 0 和 -1
                auto it = goal_positions.find(value);
                if (it != goal_positions.end()) {
                    distance += abs(i - it->second.first) + abs(j - it->second.second);
                }
            }
        }
    }
    return distance;
}

vector<shared_ptr<Node>> get_solution_path(const vector<vector<int>>& start_state, const vector<vector<int>>& goal_state) {
    int n = start_state.size();
    map<int, pair<int, int>> goal_positions;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (goal_state[i][j] > 0) { // 目标位置只记录正数牌
                goal_positions[goal_state[i][j]] = {i, j};
            }
        }
    }

    priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, CompareNodes> open_list;
    set<vector<vector<int>>> closed_list;
    int start_h = calculateManhattanDistance(start_state, goal_positions);
    auto start_node = make_shared<Node>(start_state, 0, start_h, nullptr);
    open_list.push(start_node);

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    shared_ptr<Node> goal_node = nullptr;

    while (!open_list.empty()) {
        shared_ptr<Node> current_node = open_list.top();
        open_list.pop();
        if (closed_list.count(current_node->board)) continue;
        closed_list.insert(current_node->board);
        if (current_node->board == goal_state) {
            goal_node = current_node;
            break;
        }
        pair<int, int> zero_pos = findPosition(current_node->board, 0);
        int r = zero_pos.first, c = zero_pos.second;
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i], nc = c + dc[i];
            
            // ============== 关键修改处 ==============
            // 检查新位置是否在界内，并且不是障碍物(-1)
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

    vector<shared_ptr<Node>> path;
    if (goal_node) {
        shared_ptr<Node> current = goal_node;
        while (current) {
            path.push_back(current);
            current = current->parent;
        }
        reverse(path.begin(), path.end());
    }
    return path;
}

// main 函数无需修改，它已经足够通用
int main(int argc, char* argv[]) {
    int total_args = argc - 1;
    if (total_args == 0) {
        cerr << "Error: No puzzle input provided." << endl;
        return 1;
    }
    int board_size_flat = total_args / 2;
    int n = static_cast<int>(round(sqrt(board_size_flat)));
    if (n * n != board_size_flat || total_args % 2 != 0) {
        cerr << "Error: Invalid number of arguments. Expected 2 * N*N numbers." << endl;
        return 1;
    }
    vector<vector<int>> start_state(n, vector<int>(n));
    for (int i = 0; i < board_size_flat; ++i) {
        start_state[i / n][i % n] = stoi(argv[i + 1]);
    }
    vector<vector<int>> goal_state(n, vector<int>(n));
    for (int i = 0; i < board_size_flat; ++i) {
        goal_state[i / n][i % n] = stoi(argv[i + 1 + board_size_flat]);
    }
    vector<shared_ptr<Node>> path = get_solution_path(start_state, goal_state);
    if (!path.empty()) {
        for (const auto& node : path) {
            for (int r = 0; r < n; ++r) {
                for (int c = 0; c < n; ++c) {
                    cout << node->board[r][c] << (c == n - 1 ? "" : " ");
                }
                 if (r < n - 1) { cout << " "; }
            }
            cout << endl;
        }
    } else {
        cerr << "No solution found." << endl;
    }
    return 0;
}