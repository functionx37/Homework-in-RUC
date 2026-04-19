#include <iostream>
#include <vector>
#include <algorithm>

int t;

int main()
{
    std::cin >> t;
    for (int i = 0; i < t; i++)
    {
        long long n, m, k;
        std::cin >> n >> m >> k;
        std::vector<int> f, g;
        f.push_back(n), g.push_back(m);
        while (f[f.size() - 1] > 1)
            f.push_back((f[f.size() - 1] + 1) / 2);
        while (g[g.size() - 1] > 1)
            g.push_back((g[g.size() - 1] + 1) / 2);
        long long ans = 0;
        long long size1 = f.size(), size2 = g.size();
        for (int i = 0; i <= std::min(k, size1 - 1); i++)
        {
            int j = std::min(size2 - 1, std::max(0ll, k - i));
            ans = std::max(ans, n * m - 1ll * f[i] * g[j]);
        }
        std::cout << ans << std::endl;
    }
    return 0;
}