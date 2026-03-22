#include <bits/stdc++.h>
using namespace std;

// 给定两个字符串 s 和 t，以及一个整数 K。
// 定义一对匹配为从 s 中选择严格递增的下标 i1 < i2 < ... < iL
// 和从 t 中选择严格递增的下标 j1 < j2 < ... < jL，
// 使得形成长度为 L 的配对序列 (s[i1], t[j1]), (s[i2], t[j2]), ...。
// 在这些配对中，允许最多 K 对字符不相等（称为“不相等匹配”或“容错匹配”），其余配对必须相等。
// 请你求出在最多 K 次不相等匹配限制下，能得到的最长匹配长度 L。

int ans[1001][1001][11];

int main()
{
    string s, t;
    int k;
    cin >> s >> t >> k;
    for (int i = 1; i <= s.size(); i++)
    {
        for (int j = 1; j <= t.size(); j++)
        {
            for (int p = 0; p <= k; p++)
            {
                if (s[i - 1] == t[j - 1])
                {
                    ans[i][j][p] = max(max(ans[i - 1][j][p], ans[i][j - 1][p]), ans[i - 1][j - 1][p] + 1);
                }
                else if (p > 0)
                {
                    ans[i][j][p] = max(max(ans[i - 1][j][p], ans[i][j - 1][p]), ans[i - 1][j - 1][p - 1] + 1);
                }
                else
                {
                    ans[i][j][p] = max(ans[i - 1][j][p], ans[i][j - 1][p]);
                }
            }
        }
    }
    cout << ans[s.size()][t.size()][k];
    return 0;
}