#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <utility>
#include <limits>
#include <map>
#include <stack> // 引入 stack 头文件

using namespace std;

class Cube
{
public:
    char back[3][3];
    char up[3][3];
    char front[3][3];
    char down[3][3];
    char right[3][3];
    char left[3][3];

    string hash;
    string operation;

    double time;

    Cube *parent;

    Cube(string str)
    {
        hash = str;
        int idx = 0;

        time = 0.0;
        operation = "";

        // back
        for (int i = 0; i < 9; i++, idx++)
            back[i / 3][i % 3] = str[idx];

        // up
        for (int i = 0; i < 9; i++, idx++)
            up[i / 3][i % 3] = str[idx];

        // front
        for (int i = 0; i < 9; i++, idx++)
            front[i / 3][i % 3] = str[idx];

        // down
        for (int i = 0; i < 9; i++, idx++)
            down[i / 3][i % 3] = str[idx];

        // right
        for (int i = 0; i < 9; i++, idx++)
            right[i / 3][i % 3] = str[idx];

        // left
        for (int i = 0; i < 9; i++, idx++)
            left[i / 3][i % 3] = str[idx];
    }

    Cube(const Cube &other) = default;

    void getHash()
    {
        int idx = 0;
        hash.resize(54);
        // back
        for (int i = 0; i < 9; i++, idx++)
            hash[idx] = back[i / 3][i % 3];
        // up
        for (int i = 0; i < 9; i++, idx++)
            hash[idx] = up[i / 3][i % 3];
        // front
        for (int i = 0; i < 9; i++, idx++)
            hash[idx] = front[i / 3][i % 3];
        // down
        for (int i = 0; i < 9; i++, idx++)
            hash[idx] = down[i / 3][i % 3];
        // right
        for (int i = 0; i < 9; i++, idx++)
            hash[idx] = right[i / 3][i % 3];
        // left
        for (int i = 0; i < 9; i++, idx++)
            hash[idx] = left[i / 3][i % 3];
    }

    // =================================================================
    // 这里是唯一的修改点
    // =================================================================
    bool isSolved()
    {
        getHash(); // 更新 hash

        auto allEqual = [](char face[3][3]) -> bool
        {
            char c = face[1][1];
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    if (face[i][j] != c)
                        return false;
            return true;
        };

        return allEqual(back) && allEqual(up) && allEqual(front) &&
               allEqual(down) && allEqual(right) && allEqual(left);
    }
    // =================================================================
    // 修改结束
    // =================================================================

    void p0()
    {
        char temp[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp[i][j] = left[i][j];
        left[0][0] = temp[2][0];
        left[1][0] = temp[2][1];
        left[2][0] = temp[2][2];
        left[2][1] = temp[1][2];
        left[2][2] = temp[0][2];
        left[1][2] = temp[0][1];
        left[0][2] = temp[0][0];
        left[0][1] = temp[1][0];
        char p1 = back[0][0];
        char p2 = back[1][0];
        char p3 = back[2][0];
        back[0][0] = down[0][0];
        back[1][0] = down[1][0];
        back[2][0] = down[2][0];
        down[0][0] = front[0][0];
        down[1][0] = front[1][0];
        down[2][0] = front[2][0];
        front[0][0] = up[0][0];
        front[1][0] = up[1][0];
        front[2][0] = up[2][0];
        up[0][0] = p1;
        up[1][0] = p2;
        up[2][0] = p3;
    }
    void n0()
    {
        p0();
        p0();
        p0();
    }
    void p1()
    {
        char p1 = back[0][1];
        char p2 = back[1][1];
        char p3 = back[2][1];
        back[0][1] = down[0][1];
        back[1][1] = down[1][1];
        back[2][1] = down[2][1];
        down[0][1] = front[0][1];
        down[1][1] = front[1][1];
        down[2][1] = front[2][1];
        front[0][1] = up[0][1];
        front[1][1] = up[1][1];
        front[2][1] = up[2][1];
        up[0][1] = p1;
        up[1][1] = p2;
        up[2][1] = p3;
    }
    void n1()
    {
        p1();
        p1();
        p1();
    }
    void p2()
    {
        char temp[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp[i][j] = right[i][j];
        right[0][0] = temp[0][2];
        right[0][1] = temp[1][2];
        right[0][2] = temp[2][2];
        right[1][2] = temp[2][1];
        right[2][2] = temp[2][0];
        right[2][1] = temp[1][0];
        right[2][0] = temp[0][0];
        right[1][0] = temp[0][1];
        char p1 = front[0][2];
        char p2 = front[1][2];
        char p3 = front[2][2];
        front[0][2] = up[0][2];
        front[1][2] = up[1][2];
        front[2][2] = up[2][2];
        up[0][2] = back[0][2];
        up[1][2] = back[1][2];
        up[2][2] = back[2][2];
        back[0][2] = down[0][2];
        back[1][2] = down[1][2];
        back[2][2] = down[2][2];
        down[0][2] = p1;
        down[1][2] = p2;
        down[2][2] = p3;
    }
    void n2()
    {
        p2();
        p2();
        p2();
    }
    void p3()
    {
        char p1 = front[2][0];
        char p2 = front[2][1];
        char p3 = front[2][2];
        front[2][0] = right[2][2];
        front[2][1] = right[1][2];
        front[2][2] = right[0][2];
        right[2][2] = back[0][2];
        right[1][2] = back[0][1];
        right[0][2] = back[0][0];
        back[0][2] = left[0][0];
        back[0][1] = left[1][0];
        back[0][0] = left[2][0];
        left[2][0] = p3;
        left[1][0] = p2;
        left[0][0] = p1;
        char temp[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp[i][j] = down[i][j];
        down[0][0] = temp[0][2];
        down[0][1] = temp[1][2];
        down[0][2] = temp[2][2];
        down[1][2] = temp[2][1];
        down[2][2] = temp[2][0];
        down[2][1] = temp[1][0];
        down[2][0] = temp[0][0];
        down[1][0] = temp[0][1];
    }
    void n3()
    {
        p3();
        p3();
        p3();
    }
    void p4()
    {
        char p1 = front[1][0];
        char p2 = front[1][1];
        char p3 = front[1][2];
        front[1][0] = right[2][1];
        front[1][1] = right[1][1];
        front[1][2] = right[0][1];
        right[2][1] = back[1][2];
        right[1][1] = back[1][1];
        right[0][1] = back[1][0];
        back[1][0] = left[2][1];
        back[1][1] = left[1][1];
        back[1][2] = left[0][1];
        left[2][1] = p3;
        left[1][1] = p2;
        left[0][1] = p1;
    }
    void n4()
    {
        p4();
        p4();
        p4();
    }
    void p5()
    {
        char p1 = front[0][0];
        char p2 = front[0][1];
        char p3 = front[0][2];
        front[0][0] = right[2][0];
        front[0][1] = right[1][0];
        front[0][2] = right[0][0];
        right[2][0] = back[2][2];
        right[1][0] = back[2][1];
        right[0][0] = back[2][0];
        back[2][0] = left[2][2];
        back[2][1] = left[1][2];
        back[2][2] = left[0][2];
        left[2][2] = p3;
        left[1][2] = p2;
        left[0][2] = p1;
        char temp[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp[i][j] = up[i][j];
        up[0][0] = temp[2][0];
        up[0][1] = temp[1][0];
        up[0][2] = temp[0][0];
        up[1][2] = temp[0][1];
        up[2][2] = temp[0][2];
        up[2][1] = temp[1][2];
        up[2][0] = temp[2][2];
        up[1][0] = temp[2][1];
    }
    void n5()
    {
        p5();
        p5();
        p5();
    }
    void p6()
    {
        char p1 = up[2][0];
        char p2 = up[2][1];
        char p3 = up[2][2];
        up[2][0] = left[2][0];
        up[2][1] = left[2][1];
        up[2][2] = left[2][2];
        left[2][0] = down[0][2];
        left[2][1] = down[0][1];
        left[2][2] = down[0][0];
        down[0][0] = right[2][2];
        down[0][1] = right[2][1];
        down[0][2] = right[2][0];
        right[2][0] = p1;
        right[2][1] = p2;
        right[2][2] = p3;
        char temp[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp[i][j] = front[i][j];
        front[0][0] = temp[2][0];
        front[0][1] = temp[1][0];
        front[0][2] = temp[0][0];
        front[1][2] = temp[0][1];
        front[2][2] = temp[0][2];
        front[2][1] = temp[1][2];
        front[2][0] = temp[2][2];
        front[1][0] = temp[2][1];
    }
    void n6()
    {
        p6();
        p6();
        p6();
    }
    void p7()
    {
        char p1 = up[1][0];
        char p2 = up[1][1];
        char p3 = up[1][2];
        up[1][0] = left[1][0];
        up[1][1] = left[1][1];
        up[1][2] = left[1][2];
        left[1][0] = down[1][2];
        left[1][1] = down[1][1];
        left[1][2] = down[1][0];
        down[1][0] = right[1][2];
        down[1][1] = right[1][1];
        down[1][2] = right[1][0];
        right[1][0] = p1;
        right[1][1] = p2;
        right[1][2] = p3;
    }
    void n7()
    {
        p7();
        p7();
        p7();
    }
    void p8()
    {
        char p1 = up[0][0];
        char p2 = up[0][1];
        char p3 = up[0][2];
        up[0][0] = left[0][0];
        up[0][1] = left[0][1];
        up[0][2] = left[0][2];
        left[0][2] = down[2][0];
        left[0][1] = down[2][1];
        left[0][0] = down[2][2];
        down[2][0] = right[0][2];
        down[2][1] = right[0][1];
        down[2][2] = right[0][0];
        right[0][0] = p1;
        right[0][1] = p2;
        right[0][2] = p3;
        char temp[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp[i][j] = back[i][j];
        back[0][0] = temp[0][2];
        back[0][1] = temp[1][2];
        back[0][2] = temp[2][2];
        back[1][2] = temp[2][1];
        back[2][2] = temp[2][0];
        back[2][1] = temp[1][0];
        back[2][0] = temp[0][0];
        back[1][0] = temp[0][1];
    }
    void n8()
    {
        p8();
        p8();
        p8();
    }
};

map<string, double> time_table;

string read()
{
    string begin;
    for (int i = 0; i < 6; i++)
    {
        string x;
        cin >> x;
        for (int j = 0; j < 9; j++)
        {
            char y;
            cin >> y;
            begin += y;
        }
    }

    int k = 0;
    cin >> k;
    while (k--)
    {
        string x;
        double y;
        cin >> x >> y;
        time_table[x] = y;
    }
    return begin;
}

double min_time = numeric_limits<double>::infinity();
string best_operation = "";

double calculate_time_increase(const string &current_operation, const string &next_move, const map<string, double> &time_table)
{
    if (current_operation.empty())
    {
        return 1.0;
    }

    const int move_len = 4;
    string last_move = current_operation.substr(current_operation.length() - move_len);
    string x = last_move.substr(1, 2);
    string y = next_move.substr(1, 2);
    string key = x + y;

    auto it = time_table.find(key);
    if (it != time_table.end())
    {
        return it->second;
    }

    return 1.0;
}

// =================================================================
// START: 使用栈的迭代版本替换递归版本
// =================================================================
void solve_stack(Cube start_cube, const vector<pair<string, void (Cube::*)()>> &moves, const map<string, double> &time_table)
{
    stack<Cube> s;
    s.push(start_cube);

    while (!s.empty())
    {
        Cube current_cube = s.top();
        s.pop();

        // 剪枝条件：如果当前路径的耗时已经超过已找到的最优解
        if (current_cube.time >= min_time)
        {
            continue;
        }

        // 找到一个解
        if (current_cube.isSolved())
        {
            if (current_cube.time < min_time)
            {
                min_time = current_cube.time;
                best_operation = current_cube.operation;
            }
            continue; // 找到解后，无需从此状态继续深入
        }

        // 深度限制剪枝
        if (current_cube.operation.length() / 4 >= 5)
        {
            continue;
        }

        // 探索下一步
        // 反向遍历moves，确保搜索顺序与原始递归版本一致
        for (auto it = moves.rbegin(); it != moves.rend(); ++it)
        {
            const auto &move = *it;
            Cube next_cube = current_cube;

            (next_cube.*move.second)();

            next_cube.time += calculate_time_increase(current_cube.operation, move.first, time_table);
            next_cube.operation += move.first;

            // 将新状态压入栈中，待后续处理
            s.push(next_cube);
        }
    }
}
// =================================================================
// END: 替换完成
// =================================================================

int main()
{
    vector<pair<string, void (Cube::*)()>> moves = {
        {"'0+'", &Cube::p0}, {"'0-'", &Cube::n0}, {"'1+'", &Cube::p1}, {"'1-'", &Cube::n1}, {"'2+'", &Cube::p2}, {"'2-'", &Cube::n2}, {"'3+'", &Cube::p3}, {"'3-'", &Cube::n3}, {"'4+'", &Cube::p4}, {"'4-'", &Cube::n4}, {"'5+'", &Cube::p5}, {"'5-'", &Cube::n5}, {"'6+'", &Cube::p6}, {"'6-'", &Cube::n6}, {"'7+'", &Cube::p7}, {"'7-'", &Cube::n7}, {"'8+'", &Cube::p8}, {"'8-'", &Cube::n8}};

    cout << "Please provide the initial state for the cube, followed by the number of time table entries, and then the entries themselves:" << endl;

    Cube start_cube = Cube(read());

    if (start_cube.isSolved())
    {
        cout << "The initial state is already solved." << endl;
        cout << "Operation: (empty)" << endl;
        return 0;
    }

    cout << "Starting DFS search up to 5 moves..." << endl;

    // 调用新的基于栈的函数
    solve_stack(start_cube, moves, time_table);

    if (best_operation.empty())
    {
        cout << "No solution found within 5 moves." << endl;
    }
    else
    {
        cout << "Solution found!" << endl;
        cout << "Time: " << min_time << endl;
        cout << "Operation: " << best_operation << endl;
    }

    return 0;
}