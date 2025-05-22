#include <bits/stdc++.h>

int n;
int a[105][105];
int f[105][105];

int main()
{
    std::cin >> n;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            std::cin >> a[i][j];
            f[i][j] = 0;
        }
    }
    for (int i = n - 1; i >= 0; i--)
    {
        for (int j = 0; j <= i; j++)
        {
            if (i == n - 1)
            {
                f[i][j] = a[i][j];
            }
            else
            {
                f[i][j] = std::max(f[i + 1][j], f[i + 1][j + 1]) + a[i][j];
            }
        }
    }
    std::cout << f[0][0];
    return 0;
}