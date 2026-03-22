#include <bits/stdc++.h>
using namespace std;

vector<int> edge[1005];
queue<int> q;
int n, m;
int v[1005];

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
    for (int i = 2; i <= n; i++)
        v[i] = -1;
    q.push(1);
    v[1] = 0;
    while (!q.empty())
    {
        int tp = q.front();
        q.pop();
        for (auto it : edge[tp])
        {
            if (v[it] == -1)
            {
                v[it] = v[tp] + 1;
                q.push(it);
            }
        }
    }
    for (int i = 1; i <= n; i++)
        cout << v[i]<<" ";
    return 0;
}