#include <bits/stdc++.h>

int main()
{
    long long int a, b, c;
    char d;
    std::cin >> a >> b >> d;
    switch (d)
    {
    case '+':
        c = a + b;
        printf("%lld", c);
        break;
    case '-':
        c = a - b;
        printf("%lld", c);
        break;
    case '*':
        c = a * b;
        printf("%lld", c);
        break;
    case '/':
    {
        if (b != 0)
        {
            c = a / b;
            printf("%lld", c);
        }
        else
        {
            printf("NO");
        }
        break;
    }
    case '%':
        if (b != 0)
        {
            c = a % b;
            printf("%lld", c);
        }
        else
        {
            printf("NO");
        }
        break;
    }

    return 0;
}