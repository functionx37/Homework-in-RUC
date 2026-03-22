#include <bits/stdc++.h>
using namespace std;

vector<int> fib = {1, 1};
int getfib(int p)
{
    for (int i = 0; i < 40; i++)
    {
        if (fib[i] <= p && fib[i + 1] > p)
        {
            return fib[i] - 1;
        }
    }
    return -1;
}

int main()
{
    cin.tie(0), cout.tie(0), ios::sync_with_stdio(false);
    for (int i = 2; i < 40; i++)
    {
        fib.push_back(fib[i - 1] + fib[i - 2]);
    }
    int n, x;
    cin >> n >> x;
    vector<int> nums(n);
    for (int i = 0; i < n; i++)
    {
        cin >> nums[i];
    }
    int lo = 0, hi = n;
    vector<int> ans;
    while (hi > lo)
    {
        int mi = lo + getfib(hi - lo);
        if (nums[mi] < x)
        {
            lo = mi + 1;
            ans.push_back(nums[mi]);
        }
        else
        {
            hi = mi;
        }
    }
    if (nums[lo] == x)
    {
        for (int i = 0; i < ans.size(); i++)
        {
            cout << ans[i] << " ";
        }
        cout << x << endl;
    }
    else
    {
        cout << -1 << endl;
    }
}