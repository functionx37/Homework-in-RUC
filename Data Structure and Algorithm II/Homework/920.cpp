#include <iostream>
#include <cstring>
#include <cstdio>
using std::cin;
using std::cout;
using std::max;
using std::min;
struct node
{
    int n;
    node *nxt;
    node(int n)
    {
        this->n = n;
        nxt = NULL;
    }
    node()
    {
        nxt = NULL;
    }
};
node head[550], *tail[550];
int dfn[550], low[550], cnt = 0; // tarjan核心数组
int n, m;
bool del[550];
void tarjan(int x, int from) // 求割点数
{
    int son = 0;
    dfn[x] = ++cnt;
    low[x] = dfn[x];
    node *p = &head[x];
    while (p->nxt != NULL)
    {
        p = p->nxt;
        if (dfn[p->n])
            low[x] = min(low[x], dfn[p->n]);
        else
        {
            son++;
            tarjan(p->n, x);
            low[x] = min(low[x], low[p->n]);
            if (from != 0 && low[p->n] >= dfn[x])
                del[x] = true; // del=true就是割点
            if (from == 0 && son > 1)
                del[x] = true;
        }
    }
}
bool app[550];              // 是否出现过，判断一共有多少个点
unsigned long long sum = 0; // sum<1<<64
int num = 0, w = 0;
bool used[550];
bool flag;
void dfs(int x) // 判断是否为叶子连通块
{
    w++; // 子节点个数
    node *p = &head[x];
    while (p->nxt != NULL)
    {
        p = p->nxt;
        if (used[p->n]) // 遍历过的点或出发割点
            continue;
        if (del[p->n]) // 找到另一个非出发割点的割点，说明不是叶子连通块
        {
            flag = true;
            continue;
        }
        used[p->n] = true;
        dfs(p->n);
    }
    return;
}

int main()
{
    int u, v, t = 0;
    scanf("%d", &m);
    while (m != 0)
    {
        t++;
        cnt = 0;
        n = 0;
        num = 0;
        sum = 1;
        memset(app, 0, sizeof(app));
        memset(del, 0, sizeof(del));
        memset(dfn, 0, sizeof(dfn));
        memset(used, 0, sizeof(used));
        for (int i = 1; i <= 544; i++) // 最多500个点
            tail[i] = &head[i];
        for (int i = 1; i <= m; i++)
        {
            scanf("%d%d", &u, &v);
            app[u] = true;
            app[v] = true;
            if (u > n)
                n = u;
            if (v > n)
                n = v;
            tail[u]->nxt = new node(v);
            tail[u] = tail[u]->nxt;
            tail[v]->nxt = new node(u);
            tail[v] = tail[v]->nxt;
        }

        for (int i = 1; i <= n; i++)
            if (!dfn[i])
                tarjan(i, 0);

        for (int i = 1; i <= n; i++)
            if (del[i] && app[i])
            {
                used[i] = true;
                node *p = &head[i];
                while (p->nxt != NULL)
                {
                    p = p->nxt;
                    if (!del[p->n] && !used[p->n])
                    {
                        w = 0;
                        used[p->n] = true;
                        flag = 0;
                        dfs(p->n);
                        if (!flag) // 乘法原理
                        {
                            num++;    // 联通块个数
                            sum *= w; // 方案个数
                        }
                    }
                }
                used[i] = false; //
            }
        if (num == 0) // 如果没有割点
        {
            num = 2;
            if (n - 1 == m)
                sum = 2;
            else
                sum = n * (n - 1) / 2; // 加法原理
        }
        printf("Case %d: %d ", t, num);
        cout << sum << std::endl;
        scanf("%d", &m);
    }
    return 0;
}