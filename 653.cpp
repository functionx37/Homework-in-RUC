#include <bits/stdc++.h>

int a[105], n;

int main()
{
    std::cin >> n;
    a[0]=0;
    for (int i = 1; i <= n; i++)
    {
        std::cin >> a[i];
    }
    int ans = a[1];
    int sum = 0;
    for (int i = 1; i <= n; i++)
    {
        if (sum >= 0)
            sum += a[i];
        else
            sum = a[i];
        ans = std::max(ans, sum);
    }
    std::cout << ans;
    return 0;
}