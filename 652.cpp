#include <bits/stdc++.h>

long long int sum;
int n;

int main()
{
    std::cin >> n;
    if (n > 1)
    {
        for (int i = 1; i <= n; i++)
        {
            sum += i;
        }
    }
    else
    {
        for (int i = n; i <= 1; i++)
        {
            sum += i;
        }
    }
    std::cout << sum;
    return 0;
}