#include <bits/stdc++.h>

int n;
bool lie[30] = {0};
bool zhu[30] = {0};
bool fu[30] = {0};
int total = 0;

void dfs(int x)
{
    if (x > n)
    {
        total++;
        return;
    };
    for (int y = 1; y <= n; y++)
    {
        if ((!lie[y]) && (!zhu[y - x + n]) && (!fu[x + y]))
        {
            lie[y] = 1;
            zhu[y - x + n] = 1;
            fu[x + y] = 1;
            dfs(x + 1);
            lie[y] = 0;
            zhu[y - x + n] = 0;
            fu[x + y] = 0;
        }
    }
}

int main()
{
    std::cin >> n;
    dfs(1);
    std::cout << total;
    return 0;
}