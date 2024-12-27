#include <bits/stdc++.h>

int main()
{
    int a = 0, b = 0;
    while (1)
    {
        int x;
        std::cin >> x;
        switch (x)
        {
        case 0:
            a++;
            break;
        case 1:
            b++;
            break;
        }
        if (a >= 21 && a - b > 1)
        {
            printf("0");
            break;
        }
        if (b >= 21 && b - a > 1)
        {
            printf("1");
            break;
        }
    }
    return 0;
}