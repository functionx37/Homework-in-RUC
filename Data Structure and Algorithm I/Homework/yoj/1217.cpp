#include <bits/stdc++.h>
using namespace std;

int mp[305][305];

int m, n;

void dfs(int x, int y)
{
    mp[x][y] = 0;
    if (x - 1 > 0 && mp[x - 1][y] == 1)
        dfs(x - 1, y);
    if (x + 1 <= m && mp[x + 1][y] == 1)
        dfs(x + 1, y);
    if (y - 1 > 0 && mp[x][y - 1] == 1)
        dfs(x, y - 1);
    if (y + 1 <= n && mp[x][y + 1] == 1)
        dfs(x, y + 1);
}
int cnt;

int main()
{
    cin >> m >> n;
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            cin >> mp[i][j];
        }
    }
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (mp[i][j] == 1)
            {
                cnt++;
                dfs(i, j);
            }
        }
    }
    cout << cnt;
    return 0;
}