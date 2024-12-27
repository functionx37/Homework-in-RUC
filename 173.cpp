#include <bits/stdc++.h>
using namespace std;

int main()
{
    int l, m;
    cin >> l >> m;
    int x = 0;
    int a[l + 1];
    for (int i = 0; i < l + 1; i++)
    {
        a[i] = 1;
    }
    for (int i = 0; i < m; i++)
    {
        int c, b;
        cin >> c >> b;
        for (int j = c; j <= b; j++)
        {
            a[j] = 0;
        }
    }
    for (int i = 0; i < l + 1; i++)
    {
        if (a[i] == 1)
            x++;
    }
    cout << x;
    return 0;
}