#include <bits/stdc++.h>
using namespace std;

int a[2200001];

int bi(int x, int l, int r)
{
    if (l == r)
    {
        if (a[l] == x)
            return l;
        else
            return -1;
    }
    int m = (l + r) / 2;
    if (a[m] >= x)
        return bi(x, l, m);
    return bi(x, m + 1, r);
}

int main()
{
    std::ios::sync_with_stdio(false);
    int n, x;
    cin >> n >> x;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }
    cout << bi(x, 1, n);
    return 0;
}