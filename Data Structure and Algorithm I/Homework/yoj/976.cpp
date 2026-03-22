#include <bits/stdc++.h>
using namespace std;

vector<int> edge[1005];
queue<int> q;
int n, m;
bool v[1005];

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
    q.push(1);
    v[1] = 1;
    while (!q.empty())
    {
        int tp = q.front();
        q.pop();

        cout << tp << " ";
        for (auto it : edge[tp])
        {
            if (!v[it])
            {
                v[it] = 1;
                q.push(it);
            }
        }
    }
    return 0;
}