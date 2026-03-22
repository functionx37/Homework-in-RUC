#include <bits/stdc++.h>
using namespace std;

int n, W;
int wei[1005], val[1005], ans[1005][10005];

int main()
{
    cin >> n >> W;
    for (int i = 1; i <= n; i++)
    {
        cin >> wei[i] >> val[i];
    }
    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j <= W; j++)
        {
            if (j >= wei[i])
            {
                ans[i][j] = max(ans[i - 1][j], ans[i - 1][j - wei[i]] + val[i]);
            }
            else
            {
                ans[i][j] = ans[i - 1][j];
            }
        }
    }
    cout<<ans[n][W];
    return 0;
}