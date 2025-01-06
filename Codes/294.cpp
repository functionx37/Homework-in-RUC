#include <bits/stdc++.h>

double a[55], b[55], c[55], d[55];

int main()
{
    int n;
    int x = 0;
    std::cin >> n;
    for (int j = 1; j <= n; j++)
    {
        std::cin >> a[j];
    }
    for (int j = 1; j <= n; j++)
    {
        std::cin >> b[j];
    }
    for (int j = 1; j <= n; j++)
    {
        std::cin >> c[j];
    }
    for (int j = 1; j <= n; j++)
    {
        std::cin >> d[j];
        if (b[j] + c[j] * d[j] / 10 <= 0)
        {
            a[j] = 10000;
        }
    }
    for (int i = 1; i <= n; i++)
    {
        bool f = 0;
        for (int j = 1; j <= n; j++)
        {
            if (a[j] < a[i]||a[i]==10000)
            {
                f = 1;
                break;
            }
        }
        if (f == 0)
        {
            x = i;
        }
    }
    std::cout << x;
    return 0;
}