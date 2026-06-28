#include <cstdio>
#include <queue>
#include <cstring>
#define MAX 50001
using namespace std;
int n, m, cnt = 1, x, y, vis[MAX], g[MAX], vote, ans = 0, sign[MAX];
struct edge
{
    int to, next;
} e[MAX];
queue<int> Q;
void add(int from, int to)
{
    e[cnt] = (edge){to, g[from]};
    g[from] = cnt;
    cnt++;
}
void bfs(int a)
{
    vis[a] = a;
    vote++;
    Q.push(a);
    while (Q.size())
    {
        int x = Q.front();
        if (sign[x])
        {
            while (Q.size())
            {
                Q.pop();
            }
            ans++;
            return;
        }
        Q.pop();
        for (int i = g[x]; i; i = e[i].next)
        {
            int to = e[i].to;
            if (vis[to] != a)
            {
                Q.push(to);
                vis[to] = a;
                vote++;
            }
        }
    }
    if (vote == n)
    {
        sign[a] = 1;
        ans++;
    }
}
int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; i++)
    {
        scanf("%d%d", &x, &y);
        add(y, x);
    }
    for (int i = 1; i <= n; i++)
    {
        vote = 0;
        bfs(i);
    }
    printf("%d", ans);
}