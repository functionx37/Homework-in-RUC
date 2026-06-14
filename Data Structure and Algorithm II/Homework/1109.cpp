#include <bits/stdc++.h>
using namespace std;
#define RI int
inline int read()
{
    RI x = 0, w = 0;
    char ch = 0;
    while (!isdigit(ch))
        w |= ch == '-', ch = getchar();
    while (isdigit(ch))
        x = (x << 1) + (x << 3) + (ch ^ 48), ch = getchar();
    return w ? -x : x;
}
const int MAXN = 1e4 + 10;
int N, M, ans[MAXN];
char a[MAXN], b[MAXN];
signed main()
{
    // freopen("a.in","r",stdin);
    scanf("%s%s", a, b);
    N = strlen(a), M = strlen(b);
    reverse(a, a + N), reverse(b, b + M);
    while (b[M - 1] == '0')
        M--;
    while (a[N - 1] == '0')
        N--;
    for (RI i = 0; i < N; i++)
        for (RI j = 0; j < M; j++)
            ans[i + j] += (a[i] - '0') * (b[j] - '0');
    for (RI i = 0; i < N + M - 1; i++)
        if (ans[i] >= 10)
            ans[i + 1] += ans[i] / 10, ans[i] %= 10;
    int pos = 0;
    for (RI i = N + M - 1;; i++)
    {
        if (!ans[i])
        {
            pos = i - 1;
            break;
        }
        if (ans[i] >= 10)
            ans[i + 1] += ans[i] / 10, ans[i] %= 10;
    }
    for (RI i = pos; i >= 0; i--)
        printf("%d", ans[i]);
    puts("");
    return 0;
}