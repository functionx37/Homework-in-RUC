#include <bits/stdc++.h>
using namespace std;
#define re register
#define int long long
const int inf = 2e18;
const int N = 4e5 + 10;

int ls[N], rs[N], dim[N], x_min[N], x_max[N], y_min[N], y_max[N], ans;
struct node
{
    int x, y;
} s[N];
int dis(int x, int y, int b) { return (x - s[b].x) * (x - s[b].x) + (y - s[b].y) * (y - s[b].y); }
bool cmpx(node a, node b) { return a.x < b.x; }
bool cmpy(node a, node b) { return a.y < b.y; }

void maintain(int x)
{
    x_min[x] = x_max[x] = s[x].x;
    y_min[x] = y_max[x] = s[x].y;
    if (ls[x])
    {
        x_min[x] = min(x_min[x], x_min[ls[x]]);
        x_max[x] = max(x_max[x], x_max[ls[x]]);
        y_min[x] = min(y_min[x], y_min[ls[x]]);
        y_max[x] = max(y_max[x], y_max[ls[x]]);
    }
    if (rs[x])
    {
        x_min[x] = min(x_min[x], x_min[rs[x]]);
        x_max[x] = max(x_max[x], x_max[rs[x]]);
        y_min[x] = min(y_min[x], y_min[rs[x]]);
        y_max[x] = max(y_max[x], y_max[rs[x]]);
    }
}

int build(int l, int r)
{
    if (l > r)
        return 0;
    int mid = (l + r) >> 1;
    double avx = 0, avy = 0, vax = 0, vay = 0;
    for (re int i = l; i <= r; i++)
        avx += s[i].x, avy += s[i].y;
    avx /= (double)(r - l + 1), avy /= (double)(r - l + 1);
    for (re int i = l; i <= r; i++)
        vax += (s[i].x - avx) * (s[i].x - avx), vay += (s[i].y - avy) * (s[i].y - avy);
    if (vax >= vay)
        dim[mid] = 1, nth_element(s + l, s + mid, s + r + 1, cmpx);
    else
        dim[mid] = 2, nth_element(s + l, s + mid, s + r + 1, cmpy);
    ls[mid] = build(l, mid - 1), rs[mid] = build(mid + 1, r);
    maintain(mid);
    return mid;
}

int f(int x, int y, int b)
{ // 估值函数
    int res = 0;
    if (x_min[b] > x)
        res += (x_min[b] - x) * (x_min[b] - x);
    if (x_max[b] < x)
        res += (x - x_max[b]) * (x - x_max[b]);
    if (y_min[b] > y)
        res += (y_min[b] - y) * (y_min[b] - y);
    if (y_max[b] < y)
        res += (y - y_max[b]) * (y - y_max[b]);
    return res;
}

void query(int x, int y, int l, int r)
{
    if (l > r)
        return;
    int mid = (l + r) >> 1;
    ans = min(ans, dis(x, y, mid));
    if (l == r)
        return;
    int disl = f(x, y, ls[mid]), disr = f(x, y, rs[mid]);
    if (disl < ans && disr < ans)
    {
        if (disl < disr)
        {
            query(x, y, l, mid - 1);
            if (disr < ans)
                query(x, y, mid + 1, r);
        }
        else
        {
            query(x, y, mid + 1, r);
            if (disl < ans)
                query(x, y, l, mid - 1);
        }
    }
    else
    {
        if (disl < ans)
            query(x, y, l, mid - 1);
        if (disr < ans)
            query(x, y, mid + 1, r);
    }
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

signed main()
{
    int n = read();
    for (re int i = 1; i <= n; ++i)
        s[i].x = read(), s[i].y = read();
    int rt = build(1, n);
    int m = read();
    while (m--)
    {
        ans = inf;
        int x = read(), y = read();
        query(x, y, 1, n);
        printf("%lld\n", ans);
    }
}