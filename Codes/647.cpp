#include <bits/stdc++.h>

int main()
{
    double minx = 3;
    double maxx = 0;
    double sum = 0;
    int n;
    std::cin >> n;
    for (int i = 0; i < n * n; i++)
    {
        double a;
        std::cin >> a;
        sum += a;
        minx = std::min(a, minx);
        maxx = std::max(maxx, a);
    }
    sum /= (n * n * 1.0);
    printf("%.3lf %.3lf %.2lf", maxx, minx, sum);
    return 0;
}