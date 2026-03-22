#include <bits/stdc++.h>
using namespace std;

vector<int> edge[1005];
int n, m;
bool v[1005];

void dfs(int k)
{
    cout << k << " ";
    for (auto it : edge[k])
    {
        if (!v[it])
        {
            v[it] = 1;
            dfs(it);
        }
    }
}

int main()
{
    cin >> n >> m;
    for (int i = 0; i < m; i++)
    {
        int x, y;
        cin >> x >> y;
        edge[x].push_back(y);
        edge[y].push_back(x);
    }
    for (int i = 1; i <= n; i++)
    {
        sort(edge[i].begin(), edge[i].end());
    }
    v[1] = 1;
    dfs(1);
    return 0;
}