#include <bits/stdc++.h>
using namespace std;

int h[100005];

int n = 0;

void up(int x)
{
    while (x > 1 && h[x] < h[x / 2])
    {
        std::swap(h[x], h[x / 2]);
        x /= 2;
    }
}

void down(int x)
{
    while (x * 2 <= n)
    {
        int t = x * 2;
        if (t + 1 <= n && h[t + 1] < h[t])
            t++;
        if (h[t] >= h[x])
            break;
        std::swap(h[x], h[t]);
        x = t;
    }
}

void build()
{
    for (int i = n; i >= 1; i--)
        down(i);
}

void insert(int v)
{
    n++;
    h[n] = v;
    up(n);
}

void remove()
{
    if (n == 0)
    {
        cout << -1 << endl;
        return;
    }
    swap(h[1], h[n]);
    cout << h[n] << endl;
    n--;
    down(1);
}

int main()
{
    int m;
    cin >> m;
    while (m--)
    {
        int a;
        cin >> a;
        if (a == 0)
        {
            int b;
            cin >> b;
            insert(b);
        }
        else if (a == 1)
        {
            remove();
        }
        else
        {
            cout << n << endl;
        }
    }
    return 0;
}