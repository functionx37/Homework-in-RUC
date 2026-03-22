#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <utility>

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

    Cube *parent;

    Cube(string str)
    {
        hash = str;
        int idx = 0;

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

    string operation;

    void p0()
    {
        // 临时保存 left 面
        char temp[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp[i][j] = left[i][j];

        // 顺时针旋转
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
        // 临时保存 right 面
        char temp[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp[i][j] = right[i][j];

        // 逆时针旋转 right 面（正确赋值顺序）
        right[0][0] = temp[0][2];
        right[0][1] = temp[1][2];
        right[0][2] = temp[2][2];
        right[1][2] = temp[2][1];
        right[2][2] = temp[2][0];
        right[2][1] = temp[1][0];
        right[2][0] = temp[0][0];
        right[1][0] = temp[0][1];

        // 保存 front 右列
        char p1 = front[0][2];
        char p2 = front[1][2];
        char p3 = front[2][2];

        // 逆时针轮换四列
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
        // 保存 front 底行
        char p1 = front[2][0];
        char p2 = front[2][1];
        char p3 = front[2][2];

        // front[2][*] ← right[*,2]
        front[2][0] = right[2][2]; // 右下 → 前左
        front[2][1] = right[1][2]; // 中下 → 前中
        front[2][2] = right[0][2]; // 左下 → 前右

        // right[*,2] ← back[0][*]
        right[2][2] = back[0][2]; // 后右 → 右下
        right[1][2] = back[0][1]; // 后中 → 中下
        right[0][2] = back[0][0]; // 后左 → 左下

        // back[0][*] ← left[*,0]
        back[0][2] = left[0][0]; // 左右 → 后右
        back[0][1] = left[1][0]; // 左中 → 后中
        back[0][0] = left[2][0]; // 左左 → 后左

        // left[*,0] ← front_old[2][*]
        left[2][0] = p3; // 前右 → 左右
        left[1][0] = p2; // 前中 → 左中
        left[0][0] = p1; // 前左 → 左左

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
        // 保存 front 中间行
        char p1 = front[1][0];
        char p2 = front[1][1];
        char p3 = front[1][2];

        // 1. right[*,1] → front[1][*]
        front[1][0] = right[2][1]; // 右下 → 前左
        front[1][1] = right[1][1]; // 中下 → 前中
        front[1][2] = right[0][1]; // 左下 → 前右

        // 2. back[1][*] → right[*,1]
        right[2][1] = back[1][2]; // 后左 → 右下
        right[1][1] = back[1][1]; // 后中 → 中下
        right[0][1] = back[1][0]; // 后右 → 左下

        // 3. left[*,1] → back[1][*]
        back[1][0] = left[2][1]; // 左下 → 后左
        back[1][1] = left[1][1]; // 左中 → 后中
        back[1][2] = left[0][1]; // 左上 → 后右

        // 4. front_old[1][*] → left[*,1]
        left[2][1] = p3; // 前右 → 左下
        left[1][1] = p2; // 前中 → 左中
        left[0][1] = p1; // 前左 → 左上
    }
    void n4()
    {
        p4();
        p4();
        p4();
    }
    void p5()
    {
        // 保存 front 顶行
        char p1 = front[0][0];
        char p2 = front[0][1];
        char p3 = front[0][2];

        // 1. right[*,0] → front[0][*]
        front[0][0] = right[2][0]; // 右下 → 前左
        front[0][1] = right[1][0]; // 中下 → 前中
        front[0][2] = right[0][0]; // 左下 → 前右

        // 2. back[2][*] → right[*,0]
        right[2][0] = back[2][2]; // 后左 → 右下
        right[1][0] = back[2][1]; // 后中 → 中下
        right[0][0] = back[2][0]; // 后右 → 左下

        // 3. left[*,2] → back[2][*]
        back[2][0] = left[2][2]; // 左下 → 后左
        back[2][1] = left[1][2]; // 左中 → 后中
        back[2][2] = left[0][2]; // 左上 → 后右

        // 4. front_old[0][*] → left[*,2]
        left[2][2] = p3; // 前右 → 左下
        left[1][2] = p2; // 前中 → 左中
        left[0][2] = p1; // 前左 → 左上

        // 5. 旋转 up 面（逆时针 90°）
        char temp[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp[i][j] = up[i][j];

        up[0][0] = temp[2][0]; // 左下 → 左上
        up[0][1] = temp[1][0]; // 中下 → 中上
        up[0][2] = temp[0][0]; // 右下 → 右上
        up[1][2] = temp[0][1]; // 右中 → 右中
        up[2][2] = temp[0][2]; // 右上 → 右下
        up[2][1] = temp[1][2]; // 中上 → 中下
        up[2][0] = temp[2][2]; // 左上 → 左下
        up[1][0] = temp[2][1]; // 左中 → 左中
    }
    void n5()
    {
        p5();
        p5();
        p5();
    }
    void p6()
    {
        // 保存 up 底行
        char p1 = up[2][0];
        char p2 = up[2][1];
        char p3 = up[2][2];

        // left[2][*] → up[2][*]
        up[2][0] = left[2][0];
        up[2][1] = left[2][1];
        up[2][2] = left[2][2];

        // down[0][*] → left[2][*]（逆序）
        left[2][0] = down[0][2]; // down右上 → left左下
        left[2][1] = down[0][1]; // down中上 → left中下
        left[2][2] = down[0][0]; // down左上 → left右下

        // right[2][*] → down[0][*]（逆序）
        down[0][0] = right[2][2]; // right右下 → down左上
        down[0][1] = right[2][1]; // right中下 → down中上
        down[0][2] = right[2][0]; // right左下 → down右上

        // up_old[2][*] → right[2][*]
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
        // 保存 up 中间行
        char p1 = up[1][0]; // up左
        char p2 = up[1][1]; // up中
        char p3 = up[1][2]; // up右

        // 1. right[*,1] → up[1][*]（逆序）
        up[1][0] = left[1][0];
        up[1][1] = left[1][1];
        up[1][2] = left[1][2];

        // 2. down[1][*] → right[*,1]
        left[1][0] = down[1][2];
        left[1][1] = down[1][1];
        left[1][2] = down[1][0];

        // 3. left[*,1] → down[1][*]
        down[1][0] = right[1][2]; // left下 → down左
        down[1][1] = right[1][1]; // left中 → down中
        down[1][2] = right[1][0]; // left上 → down右

        // 4. up_old[1][*] → left[*,1]
        right[1][0] = p1; // up左 → left上
        right[1][1] = p2; // up中 → left中
        right[1][2] = p3; // up右 → left下
    }
    void n7()
    {
        p7();
        p7();
        p7();
    }
    void p8()
    {
        // 保存 up 顶行
        char p1 = up[0][0];
        char p2 = up[0][1];
        char p3 = up[0][2];

        // 1. left[*,2] → up[0][*]（逆序）
        up[0][0] = left[0][0]; // left下右 → up左上
        up[0][1] = left[0][1]; // left中右 → up中上
        up[0][2] = left[0][2]; // left上右 → up右上

        // 2. down[2][*] → left[*,2]
        left[0][2] = down[2][0]; // down左下 → left上右
        left[0][1] = down[2][1]; // down中下 → left中右
        left[0][0] = down[2][2]; // down右下 → left下右

        // 3. right[*,0] → down[2][*]（逆序）
        down[2][0] = right[0][2]; // right下左 → down左下
        down[2][1] = right[0][1]; // right中左 → down中下
        down[2][2] = right[0][0]; // right上左 → down右下

        // 4. up_old[0][*] → right[*,0]
        right[0][0] = p1; // up右上 → right上左
        right[0][1] = p2; // up中上 → right中左
        right[0][2] = p3; // up左上 → right下左

        // 5. back 面顺时针旋转 90°（从前往后看）
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

// 您的原始 read 函数
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
    return begin;
}

int main()
{
    // 定义一个包含所有18个操作的列表，方便遍历
    vector<pair<string, void (Cube::*)()>> moves = {
        {"'0+'", &Cube::p0}, {"'0-'", &Cube::n0}, {"'1+'", &Cube::p1}, {"'1-'", &Cube::n1}, {"'2+'", &Cube::p2}, {"'2-'", &Cube::n2}, {"'3+'", &Cube::p3}, {"'3-'", &Cube::n3}, {"'4+'", &Cube::p4}, {"'4-'", &Cube::n4}, {"'5+'", &Cube::p5}, {"'5-'", &Cube::n5}, {"'6+'", &Cube::p6}, {"'6-'", &Cube::n6}, {"'7+'", &Cube::p7}, {"'7-'", &Cube::n7}, {"'8+'", &Cube::p8}, {"'8-'", &Cube::n8}};

    cout << "Please provide the initial state for the cube:" << endl;
    Cube start_cube = Cube(read());

    // 广度优先搜索 (BFS) 初始化
    queue<Cube> q;
    unordered_set<string> visited;

    // 检查初始状态是否已经还原
    if (start_cube.isSolved())
    {
        cout << "The initial state is already solved." << endl;
        cout << "Operation: (empty)" << endl;
        return 0;
    }

    q.push(start_cube);
    visited.insert(start_cube.hash);

    // 开始 BFS 循环
    while (!q.empty())
    {
        Cube current_cube = q.front();
        q.pop();

        // 检查深度限制
        int current_depth = current_cube.operation.length() / 4;
        if (current_depth >= 5)
        {
            continue;
        }

        // 遍历所有18种可能的操作
        for (const auto &move : moves)
        {
            Cube next_cube = current_cube;

            (next_cube.*move.second)();

            next_cube.operation += move.first;
            next_cube.getHash();

            if (next_cube.isSolved())
            {
                cout << "Solution found!" << endl;
                cout << "Operation: " << next_cube.operation << endl;
                return 0;
            }

            if (visited.count(next_cube.hash))
            {
                continue;
            }

            visited.insert(next_cube.hash);
            q.push(next_cube);
        }
    }

    cout << "No solution found within 5 moves." << endl;

    return 0;
}