#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
using namespace std;

// 定义点结构
struct Point {
    int x, y;
};

// 比较函数，按x坐标排序
bool compareX(const Point &a, const Point &b) {
    return a.x < b.x;
}

// 比较函数，按y坐标排序
bool compareY(const Point &a, const Point &b) {
    return a.y < b.y;
}

// 计算两点之间距离的平方
long long distSquare(Point p1, Point p2) {
    return (long long)(p1.x - p2.x) * (p1.x - p2.x) + 
           (long long)(p1.y - p2.y) * (p1.y - p2.y);
}

// 暴力求解最近点对（当点数较少时使用）
long long bruteForce(vector<Point> &points, int start, int end) {
    long long minDist = LLONG_MAX;
    for (int i = start; i < end; i++) {
        for (int j = i + 1; j < end; j++) {
            long long dist = distSquare(points[i], points[j]);
            minDist = min(minDist, dist);
        }
    }
    return minDist;
}

// 求解分割带中的最近点对
long long stripClosest(vector<Point> &strip, long long d) {
    long long minDist = d;
    
    // 对带内的点按y坐标排序
    sort(strip.begin(), strip.end(), compareY);
    
    // 对每个点，只需检查其后方y坐标差值在sqrt(d)范围内的点
    for (size_t i = 0; i < strip.size(); i++) {
        for (size_t j = i + 1; j < strip.size() && 
             (long long)(strip[j].y - strip[i].y) * (strip[j].y - strip[i].y) < minDist; j++) {
            long long dist = distSquare(strip[i], strip[j]);
            minDist = min(minDist, dist);
        }
    }
    
    return minDist;
}

// 递归求解最近点对
long long closestPair(vector<Point> &points, int start, int end) {
    // 如果只有少量点，使用暴力算法
    if (end - start <= 3) {
        return bruteForce(points, start, end);
    }
    
    // 计算中点
    int mid = start + (end - start) / 2;
    Point midPoint = points[mid];
    
    // 递归处理左右两部分
    long long dl = closestPair(points, start, mid);
    long long dr = closestPair(points, mid, end);
    
    // 获取当前最小距离
    long long d = min(dl, dr);
    
    // 收集在中轴线x坐标差值sqrt(d)范围内的点
    vector<Point> strip;
    for (int i = start; i < end; i++) {
        if ((long long)(points[i].x - midPoint.x) * (points[i].x - midPoint.x) < d) {
            strip.push_back(points[i]);
        }
    }
    
    // 处理跨中轴线的情况
    return min(d, stripClosest(strip, d));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        cin >> points[i].x >> points[i].y;
    }
    
    // 特殊情况：只有两个点
    if (n == 2) {
        cout << distSquare(points[0], points[1]) << endl;
        return 0;
    }
    
    // 按x坐标排序
    sort(points.begin(), points.end(), compareX);
    
    // 求解最近点对
    long long result = closestPair(points, 0, n);
    cout << result << endl;
    
    return 0;
}