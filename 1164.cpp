#include <bits/stdc++.h>

inline int gcd(int a, int b)
{
    int r;
    while (b > 0)
    {
        r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int main()
{
    int a, b;
    std::cin >> a >> b;
    printf("%d", a * b / gcd(a, b));
    return 0;
}