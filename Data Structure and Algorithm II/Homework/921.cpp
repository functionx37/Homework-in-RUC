#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
int n, m, tot, head[N], val[N], sum[N], ans, x[N], y[N], vis[N], dis[N];
int dfn[N], low[N], cnt, in_stack[N], sc, color[N];
stack<int> s;
struct Edge
{
    int next, to, dis;
} edge[N];

void add(int from, int to, int w = 0)
{
    edge[++tot].next = head[from];
    edge[tot].to = to;
    edge[tot].dis = w;
    head[from] = tot;
}

inline int read()
{
    int x = 0, f = 1;
    char ch = getchar();
    while (!isdigit(ch))
    {
        if (ch == '-')
            f = -1;
        ch = getchar();
    }
    while (isdigit(ch))
    {
        x = x * 10 + ch - 48;
        ch = getchar();
    }
    return x * f;
}

void tarjan(int u)
{
    dfn[u] = low[u] = ++cnt;
    in_stack[u] = 1;
    s.push(u);
    for (int i = head[u]; i; i = edge[i].next)
    {
        int v = edge[i].to;
        if (!dfn[v])
            tarjan(v), low[u] = min(low[u], low[v]);
        else if (in_stack[v])
            low[u] = min(dfn[v], low[u]);
    }
    int k;
    if (dfn[u] == low[u])
    {
        ++sc;
        do
        {
            k = s.top();
            s.pop();
            sum[sc] += val[k], in_stack[k] = 0, color[k] = sc;
        } while (u != k);
    }
}

void spfa(int s)
{
    queue<int> q;
    q.push(s);
    vis[s] = 1, dis[s] = sum[s];
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        vis[u] = 0;
        for (int i = head[u]; i; i = edge[i].next)
        {
            int v = edge[i].to;
            if (dis[v] < dis[u] + edge[i].dis)
            {
                dis[v] = dis[u] + edge[i].dis;
                if (!vis[v])
                    vis[v] = 1, q.push(v);
            }
        }
    }
}

int main()
{
    n = read(), m = read();
    for (int i = 1; i <= m; i++)
    {
        x[i] = read(), y[i] = read();
        add(x[i], y[i]);
    }
    for (int i = 1; i <= n; i++)
        val[i] = read();
    for (int i = 1; i <= n; i++)
        if (!dfn[i])
            tarjan(i);
    tot = 0;
    memset(head, 0, sizeof(head));
    for (int i = 1; i <= m; i++)
        if (color[x[i]] != color[y[i]])
            add(color[x[i]], color[y[i]], sum[color[y[i]]]);

    int s = read(), p = read();
    spfa(color[s]);
    while (p--)
    {
        int t = read();
        ans = max(ans, dis[color[t]]);
    }
    cout << ans;
    return 0;
}