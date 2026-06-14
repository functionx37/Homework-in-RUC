#include <bits/stdc++.h>
using std::cin;
using std::cout;

int dp[205][205];

int main()
{
    int a = 0, b = 0;
    cin >> a >> b;
    for (int i = 1; i <= 205; i++)
    {
        dp[i][1] = 1;
        dp[i][i] = 1;
    }
    for (int i = 1; i <= a; i++)
    {
        for (int j = 2; j <= b; j++)
        {
            dp[i][j] = dp[i - 1][j - 1] + dp[i - j][j];
        }
    }
    cout << dp[a][b];
    return 0;
}