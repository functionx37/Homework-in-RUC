#include <bits/stdc++.h>
using namespace std;

int height[10000001];
stack<int> S;
int ans;

int main()
{
    ios::sync_with_stdio(false);
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
        cin >> height[i];
    height[n] = 0;
    for (int i = 0; i <= n; i++)
    {
        while (!S.empty() && height[i] <= height[S.top()])
        {
            int h = S.top();
            S.pop();
            int width = S.empty() ? i : i - S.top() - 1;
            ans = max(ans, width * height[h]);
        }
        S.push(i);
    }
    cout << ans;
    return 0;
}