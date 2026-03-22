#include <bits/stdc++.h>
using namespace std;

int n;
int par[50001];
int rel[50001];
int cnt = 0;

int find(int x)
{
    if (par[x] != x)
    {
        int root = find(par[x]);             // 先递归找到根
        rel[x] = (rel[x] + rel[par[x]]) % 3; // 再更新关系
        par[x] = root;                       // 再压缩路径
    }
    return par[x];
}

void join(int x, int y, int state)
{
    int fx = find(x), fy = find(y);
    par[fy] = fx;
    rel[fy] = (state + rel[x] - rel[y] + 3) % 3;
}

int main()
{
    int m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
    {
        rel[i] = 0;
        par[i] = i;
    }
    while (m--)
    {
        int k, a, b;
        cin >> k >> a >> b;
        if (a > n || b > n)
        {
            cnt++;
            continue;
        }
        if (k == 2 && a == b)
        {
            cnt++;
            continue;
        }
        int ra = find(a), rb = find(b);
        int rl = (rel[a] - rel[b] + 3) % 3;
        if (ra == rb)
        {

            if (k == 1 && rl != 0)
                cnt++;
            else if (k == 2 && rl != 1)
                cnt++;
        }
        else
        {
            join(b, a, k - 1); // 非常重要！！！ a、b顺序可不能倒！！！
        }
    }
    cout << cnt;
    return 0;
}