#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> e(n + 1);

    for (int i = 0; i < m; i++)
    {
        int a, b, w;
        cin >> a >> b >> w;
        e[a].push_back({b, w});
        e[b].push_back({a, w});
    }

    vector<int> dis(n + 1, 0x7fffffff);
    vector<bool> vis(n + 1, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dis[1] = 0;
    pq.push({0, 1});

    int cnt = 0;

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();
        if (vis[u])
            continue;
        vis[u] = true;
        cnt += dis[u];
        for (auto [v, w] : e[u])
        {
            if (w < dis[v])
            {
                dis[v] = w;
                pq.push({dis[v], v});
            }
        }
    }

    cout << cnt;
    return 0;
}