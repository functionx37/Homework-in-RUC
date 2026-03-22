#include <bits/stdc++.h>
using namespace std;

int n, m, s;

int main()
{
    cin >> n >> m >> s;
    vector<vector<pair<int, int>>> e(n + 1);

    for (int i = 0; i < m; i++)
    {
        int a, b, w;
        cin >> a >> b >> w;
        e[a].push_back({b, w});
    }

    vector<int> dis(n + 1, 0x7fffffff);
    vector<bool> vis(n + 1, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dis[s] = 0;
    pq.push({0, s});

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();
        if (vis[u])
            continue;
        vis[u] = true;
        for (auto [v, w] : e[u])
        {
            if (dis[u] + w < dis[v])
            {
                dis[v] = dis[u] + w;
                pq.push({dis[v], v});
            }
        }
    }

    for (int i = 1; i <= n; i++)
    {
        cout << dis[i] << " ";
    }
    return 0;
}