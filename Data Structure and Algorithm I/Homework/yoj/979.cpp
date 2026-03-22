#include <bits/stdc++.h>
using namespace std;

vector<int> edge[1005];
int n, m;
int in[1005];

void toposort()
{
    queue<int> S;
    for (int i = 1; i <= n; i++)
    {
        if (in[i] == 0)
        {
            S.push(i);
        }
    }
    while (!S.empty())
    {
        int top = S.front();
        S.pop();
        cout << top << " ";
        for (int it : edge[top])
        {
            in[it]--;
            if (in[it] == 0)
            {
                S.push(it);
            }
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
        in[y]++;
    }
    for (int i = 1; i <= n; i++)
    {
        sort(edge[i].begin(), edge[i].end());
    }
    toposort();
    return 0;
}