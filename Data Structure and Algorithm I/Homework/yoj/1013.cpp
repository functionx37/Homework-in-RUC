#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll a[1000001];
ll c[1000001];
int n;

int lowbit(ll x)
{
    return x & -x;
}

void init() // Θ(n) 建树方法1
{
    for (int i = 1; i <= n; ++i)
    {
        c[i] += a[i];
        int j = i + lowbit(i);
        if (j <= n)
            c[j] += c[i];
    }
}

ll getsum(int x) // a[1]..a[x]的和
{
    ll ans = 0;
    while (x > 0)
    {
        ans = ans + c[x];
        x = x - lowbit(x);
    }
    return ans;
}

ll getsum(int x, int y)
{
    return getsum(y) - getsum(x - 1);
}

void add(int x, int k) // 单点修改
{
    while (x <= n)
    { // 不能越界
        c[x] = c[x] + k;
        x = x + lowbit(x);
    }
}

int main()
{
    int q;
    scanf("%d %d", &n, &q);
    for (int i = 1; i <= n; i++)
        scanf("%lld", &a[i]);
    init();
    while (q--)
    {
        ll k, x, y;
        scanf("%lld %lld %lld", &k, &x, &y);
        if (k == 1)
        {
            add(x, y);
        }
        else
        {
            printf("%lld\n", getsum(x, y));
        }
    }
    return 0;
}