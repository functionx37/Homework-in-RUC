#include <iostream>
#include <vector>
#include <algorithm>

// 定义区间结构体
struct Interval
{
    int l, r, w;
};

// 用于排序的比较函数，根据区间的右端点r升序排序
bool compareIntervals(const Interval &a, const Interval &b)
{
    if (a.r != b.r)
    {
        return a.r < b.r;
    }
    return a.l < b.l; // 如果右端点相同，按左端点排序（可选，但能保证排序稳定性）
}

int main()
{
    // 提高I/O效率
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<Interval> intervals(n);
    for (int i = 0; i < n; ++i)
    {
        std::cin >> intervals[i].l >> intervals[i].r >> intervals[i].w;
    }

    // 1. 根据区间的右端点进行排序
    std::sort(intervals.begin(), intervals.end(), compareIntervals);

    // 2. 创建DP数组
    // dp[i] 表示考虑前 i+1 个区间（从0到i）所能得到的最大权重
    std::vector<long long> dp(n);

    // 3. 基本情况：只考虑第一个区间
    dp[0] = intervals[0].w;

    // 4. 填充DP数组
    for (int i = 1; i < n; ++i)
    {
        // ---情况1: 不选择当前区间 i ---
        // 最大权重和与前 i-1 个区间的结果相同
        long long weight_without_current = dp[i - 1];

        // ---情况2: 选择当前区间 i ---
        long long weight_with_current = intervals[i].w;

        // 使用二分搜索找到在当前区间开始前就结束的最后一个区间 p
        // 即，找到满足 intervals[p].r <= intervals[i].l 的最大索引 p
        int low = 0, high = i - 1;
        int p = -1; // 默认没有这样的区间
        while (low <= high)
        {
            int mid = low + (high - low) / 2;
            if (intervals[mid].r <= intervals[i].l)
            {
                // 这个区间不冲突，可能还有更靠后的，所以记录下来并向右搜索
                p = mid;
                low = mid + 1;
            }
            else
            {
                // 这个区间冲突了，需要向左搜索
                high = mid - 1;
            }
        }

        // 如果找到了不冲突的区间p，则加上dp[p]
        if (p != -1)
        {
            weight_with_current += dp[p];
        }

        // dp[i] 取两种情况的最大值
        dp[i] = std::max(weight_with_current, weight_without_current);
    }

    // 5. 最终结果是DP数组的最后一个值
    std::cout << dp[n - 1] << std::endl;

    return 0;
}