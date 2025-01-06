#include <bits/stdc++.h>

int main()
{
    int sum = 0;
    int add = 2;
    bool flag = 0;
    while (true)
    {
        int cur = 0;
        std::cin >> cur;
        if (cur == 0)
        {
            break;
        }
        if (flag == 1)
        {
            add += 2;
        }
        if (cur == 1)
        {
            flag = 0;
            add = 2;
            sum += 1;
        }
        if (cur == 2)
        {
            flag = 1;
            sum += add;
        }
    }
    std::cout << sum;
    return 0;
}