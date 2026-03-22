#include <bits/stdc++.h>
using namespace std;

int n, m;
int mp[505][505];
int val[505][505];

int main()
{
    cin >> m >> n;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin >> mp[i][j];
        }
    }
    val[0][0] = mp[0][0];
    for (int i = 1; i < m; i++)
    {
        if (mp[0][i] == -1)
        {
            val[0][i] = -100000;
        }
        else if (val[0][i - 1] == -100000)
        {
            val[0][i] = -100000;
        }
        else
        {
            val[0][i] = val[0][i - 1] + mp[0][i];
        }
    }

    for (int i = 1; i < n; i++)
    {
        if (mp[i][0] == -1)
        {
            val[i][0] = -100000;
        }
        else if (val[i - 1][0] == -100000)
        {
            val[i][0] = -100000;
        }
        else
        {
            val[i][0] = val[i - 1][0] + mp[i][0];
        }
    }
    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < m; j++)
        {
            if (mp[i][j] == -1)
            {
                val[i][j] = -100000;
            }
            else if (val[i - 1][j] == -100000 && val[i][j - 1] == -100000 && val[i - 1][j - 1] == -100000)
            {
                val[i][j] = -100000;
            }
            else
            {
                val[i][j] = max((max(val[i - 1][j], val[i][j - 1])), val[i - 1][j - 1]) + mp[i][j];
            }
        }
    }
    if (val[n - 1][m - 1] == -100000)
    {
        cout << "No";
    }
    else
    {
        cout << "Yes\n"
             << val[n - 1][m - 1];
    }
    return 0;
}