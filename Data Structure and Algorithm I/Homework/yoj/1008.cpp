#include <bits/stdc++.h>
using namespace std;

int max_val = -0x7fffffff;
int min_val = 0x7fffffff;

int main()
{
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> a[i];
        max_val = max(max_val, a[i]);
        min_val = min(min_val, a[i]);
    }

    double bucket_size = (max_val - min_val) / (n - 1);

    vector<int> bucket_min(n, 0x7fffffff);
    vector<int> bucket_max(n, -0x7fffffff);

    for (int x : a)
    {
        int idx = (x - min_val) / bucket_size;
        if (idx >= n)
            idx = n - 1;
        bucket_min[idx] = min(bucket_min[idx], x);
        bucket_max[idx] = max(bucket_max[idx], x);
    }

    int max_gap = 0;
    int prev_max = bucket_max[0];

    for (int i = 1; i < n; ++i)
    {
        if (bucket_min[i] == 0x7fffffff)
        {
            continue;
        }
        int gap = bucket_min[i] - prev_max;
        if (gap > max_gap)
        {
            max_gap = gap;
        }
        prev_max = bucket_max[i];
    }

    cout << max_gap;
    return 0;
}