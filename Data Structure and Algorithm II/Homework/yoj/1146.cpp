#include <iostream>
#include <vector>
using namespace std;

// 检查是否可以通过删除不超过 m 个点，使得相邻点的距离至少为 mid
bool check(const vector<int>& points, int m, int mid) {
    int count = 0;
    int last = 0; // 左边界点 x=0
    // 遍历每个点
    for (int i = 0; i < points.size(); i++) {
        if (points[i] - last < mid) {
            count++; // 距离不够，需要删除当前点
        } else {
            last = points[i]; // 更新上一个点的位置
        }
    }
    // 检查右边界点 x=L
    if (count > m) {
        return false; // 需要删除的点超过 m 个
    }
    return true;
}

// 二分查找求解最大可能的最小距离
int maxMinDistance(int L, int N, int M, const vector<int>& points) {
    int left = 0;      // 最小可能距离
    int right = L;     // 最大可能距离
    int result = 0;    // 最终结果
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (check(points, M, mid)) {
            result = mid;  // 满足条件，尝试更大的距离
            left = mid + 1;
        } else {
            right = mid - 1; // 不满足条件，减小距离
        }
    }
    return result;
}

int main() {
    int L, N, M;
    cin >> L >> N >> M;
    vector<int> points(N);
    for (int i = 0; i < N; i++) {
        cin >> points[i];
    }

    // 调用函数求解
    int result = maxMinDistance(L, N, M, points);
    cout << result << endl;

    return 0;
}
