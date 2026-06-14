#include <iostream>
using namespace std;
int a[205] = {};
int dp1[205][205] = {}, dp2[205][205] = {};
int sum[205] = {};
int main(void)
{
    int n = 0, cnt = 0;
    int max_ans = -1e8, min_ans = 1e8;
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }
    while (cnt < n)
    {
        cnt++;
        for (int i = 1; i <= n; i++)
        {
            sum[i] = sum[i - 1] + a[i];
        }
        for (int i = 1; i <= n; i++)
        {
            for (int j = i + 1; j <= n; j++)
            {
                dp1[i][j] = 1e8;
                dp2[i][j] = -1e8;
            }
        }
        for (int i = 2; i <= n; i++)
        {
            for (int j = 1; j + i - 1 <= n; j++)
            {
                int x = j, y = j + i - 1;
                int add = sum[y] - sum[x] + a[x];
                for (int t = x; t + 1 <= y; t++)
                {
                    dp1[x][y] = min(dp1[x][y], dp1[x][t] + dp1[t + 1][y] + add);
                    dp2[x][y] = max(dp2[x][y], dp2[x][t] + dp2[t + 1][y] + add);
                }
            }
        }
        for (int i = 0; i < n; i++)
        {
            a[i] = a[i + 1];
        }
        a[n] = a[0];
        min_ans = min(min_ans, dp1[1][n]);
        max_ans = max(max_ans, dp2[1][n]);
    }
    cout << min_ans << endl
         << max_ans;
    return 0;
}