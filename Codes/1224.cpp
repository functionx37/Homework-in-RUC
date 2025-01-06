#include <bits/stdc++.h>

double resttime;

int main()
{
    double X, t0, t1;
    std::cin >> X >> t0 >> t1;
    resttime = X - (t0 + t1);
    int Y, Z;
    std::cin >> Y >> Z;
    int n, m, n0, m0;
    int d2c = std::abs(n - n0) + std::abs(m - m0);
    int d2b = 100000000;
    std::cin >> n >> m >> n0 >> m0;
    int x, y;
    for (int i = 0; i < Y; i++)
    {
        int x0, y0;
        std::cin >> x0 >> y0;
        if (std::abs(n0 - x0) + std::abs(m0 - y0) < d2b)
        {
            d2b = std::abs(n0 - x0) + std::abs(m0 - y0);
            x = x0;
            y = y0;
        }
    }
    bool ifwork = 1;
    int b2c = std::abs(n - x) + std::abs(m - y);
    for (int p = 0; p < Z; p++)
    {
        int i, j;
        std::cin >> i >> j;
        if (i == x && j == y)
        {
            ifwork = 0;
        }
    }
    bool ifbike = 1;
    if (3 * d2c < d2b + b2c)
        ifbike = 0;
    double expected = 0, real = 0;
    if (ifbike = 0)
    {
        expected = 3 * d2c;
        real = expected;
    }
    else
    {
        expected += 3 * d2b;
        expected += b2c;
        expected += 0.5;
        real = expected;
       
        if (ifwork == 0)
        {
            real += 2 * b2c;
        }
    }
    if (real <=  resttime)
        printf("%.2lf", resttime - real);
    else if (expected >= resttime)
        printf("老师我%.2lf分钟之内到不了教室，可能会晚一点[苦涩][合十][合十]", X - t0);
    else
        printf("我爱小红");
    return 0;
}