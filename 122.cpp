#include <bits/stdc++.h>

int n, m;
bool a[100][100], flag = 0;
void map(int i, int j)
{
    if (i == n && j == m)
    {
        flag = 1;
        return;
    }
    if (a[i][j + 1] == 1)
    {
        a[i][j + 1] = 0;
        map(i, j + 1);
    }
    if (a[i + 1][j] == 1)
    {
        a[i + 1][j] = 0;
        map(i + 1, j);
    }
    if (a[i - 1][j] == 1)
    {
        a[i - 1][j] = 0;
        map(i - 1, j);
    }
    if (a[i][j - 1] == 1)
    {
        a[i][j - 1] = 0;
        map(i, j - 1);
    }
}

int main()
{
    std::cin >> n >> m;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            std::cin >> a[i][j];
        }
    }
    map(1, 1);
    if (flag == 1)
        printf("YES");
    else
        printf("NO");
    return 0;
}