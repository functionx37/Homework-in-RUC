#include <bits/stdc++.h>
#define ll long long

using namespace std;

const int maxn = 1e6 + 10;
ll n, m, a[maxn], b[maxn];

bool check(ll x)
{
    ll res = n * m;

    for (int i = 1; i <= n; i++)
    {
        if (b[i] >= a[i])
            res -= (x - 1) / b[i] + 1;
        else if (a[i] * m >= x)
            res -= (x - 1) / a[i] + 1;
        else
            res -= (x - a[i] * m - 1) / b[i] + 1 + m;

        if (res < 0)
            return 0;
    }

    return res >= 0;
}
int main()
{
    scanf("%d%d", &n, &m);

    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);

    for (int i = 1; i <= n; i++)
        scanf("%d", &b[i]);

    ll l = 1, r = 1e18, ans = 0;

    while (l <= r)
    {
        ll mid = (l + r) >> 1;

        if (check(mid))
            ans = mid, l = mid + 1;
        else
            r = mid - 1;
    }

    printf("%lld", ans);
    return 0;
}