#include <bits/stdc++.h>
using namespace std;

void radixSort(vector<int> &arr)
{
    int n = arr.size();
    vector<int> output(n);
    vector<int> count(256);

    for (int byte = 0; byte < 4; ++byte)
    {
        // 清空计数数组
        fill(count.begin(), count.end(), 0);
        // 统计当前字节的频次
        for (int i = 0; i < n; ++i)
        {
            int digit = (arr[i] >> (byte * 8)) & 0xFF;
            count[digit]++;
        }
        // 转换为前缀和（计数排序的标准步骤）
        for (int i = 1; i < 256; ++i)
        {
            count[i] += count[i - 1];
        }
        // 从后往前遍历，保证稳定性
        for (int i = n - 1; i >= 0; --i)
        {
            int digit = (arr[i] >> (byte * 8)) & 0xFF;
            output[count[digit] - 1] = arr[i];
            count[digit]--;
        }
        // 将 output 复制回 arr
        arr.swap(output);
    }
}

int main()
{
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> nums[i];
    }
    radixSort(nums);
    for (int x : nums)
    {
        cout << x << '\n';
    }
    return 0;
}