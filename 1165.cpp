#include <bits/stdc++.h>

int main()
{
    int n, sum = 0;
    std::cin >> n;
    for (int i = 0; i < n; i++)
    {
        int b;
        std::cin >> b;
        if (b > 0)
            sum += b;
    }
    std::cout << sum;
    return 0;
}