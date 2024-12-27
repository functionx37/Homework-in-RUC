#include <bits/stdc++.h>

int main()
{
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
    int sum1 = a * c + b * d;
    int sum2 = c + d;
    double x, y;
    if (sum2 == 2)
        x = sum1 * 0.9;
    else if (sum2 > 2 && sum2 < 5)
        x = sum1 * 0.8;
    else if (sum2 > 4)
        x = sum1 * 0.7;
    y = sum1 - (sum1 / 500) * 100;
    if (y < x)
    {
        printf("2 %.1lf",y);
    }
    else
        printf("1 %.1lf",x);
    return 0;
}