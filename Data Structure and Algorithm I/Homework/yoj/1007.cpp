#include <bits/stdc++.h>
using namespace std;

int ans[100005];
int n, k;
int minl = 0x7fffffff;
deque<int> dq;

int main()
{
    cin >> n >> k;
    for (int i = 1; i <= n; i++)
    {
        int temp;
        cin >> temp;
        ans[i] = ans[i - 1] + temp;
    }
    dq.push_front(0);
    for (int i = 1; i <= n; i++)
    {
        while (!dq.empty() && ans[i] - ans[dq.front()] >= k)
        {
            minl = min(minl, i - dq.front());
            dq.pop_front();
        }
        while (!dq.empty() && ans[dq.back()] >= ans[i])
            dq.pop_back();
        dq.push_back(i);
    }
    cout << (minl == 0x7fffffff ? -1 : minl);
    return 0;
}