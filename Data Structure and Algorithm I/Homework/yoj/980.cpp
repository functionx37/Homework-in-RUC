#include <bits/stdc++.h>
using namespace std;

vector<int> edge[1005];
int n, m;
bool v[1005];
vector<int> S;

void dfs(int k)
{
    for (auto it : edge[k])
    {
        if (!v[it])
        {
            v[it] = 1;
            dfs(it);
        }
    }
    S.push_back(k);
}

int main()
{
    cin >> n >> m;
    for (int i = 0; i < m; i++)
    {
        int x, y;
        cin >> x >> y;
        edge[x].push_back(y);
    }
    for (int i = 1; i <= n; i++)
    {
        sort(edge[i].begin(), edge[i].end());
    }
    for (int i = 1; i <= n; i++)
    {
        if (!v[i])
        {
            v[i] = 1;
            dfs(i);
        }
    }
    for (int i = S.size() - 1; i >= 0; --i)
    {
        cout << S[i] << " ";
    }
    return 0;
}