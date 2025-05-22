#include <bits/stdc++.h>

int main()
{
    char a[100];
    memset(a, 0, sizeof(a));
    int k;
    std::cin.getline(a, 99, ' ');
    std::cin >> k;
    int n = 0;
    int sum = 0;
    int num = 0;
    for (int i = 0; i < 100; i++)
    {
        if (a[i] == 0)
            break;
        n++;
    }
    for (int i = 0; i < n; i++)
    {
        if (a[i] != 'X')
        {
            num += (a[i] - 48) * std::pow(10, n - i - 1);
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (a[i] == 88)
        {
            int p;
            if (i == 0)
                p = 1;
            else
                p = 0;
            for (; p < 10; p++)
            {
                int x;
                x = num + p * std::pow(10, n - i - 1);
                if (x % k == 0)
                    sum++;
            }
            break;
        }
    }
    std::cout << sum;
    return 0;
}