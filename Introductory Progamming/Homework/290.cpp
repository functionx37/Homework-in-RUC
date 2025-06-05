#include <bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin >> n;
    int num[21][101]={};
    int one[21]={};

    for (int i = 1; i <= n; i++)
    {
        cin >> one[i];
        for (int i1 = 1; i1 <= one[i]; i1++)
        {
            cin >> num[i][i1];
        }
    }
    sort(num[1]+1, num[1] + one[1]+1);
    bool check = 0;
    for (int i2 = 1; i2 <= one[1]; i2++)
    {
        int sum = 0;
        for (int i3 = 2; i3 <= n; i3++)
        {
            for (int i4 = 1; i4 <= one[i3]; i4++)
            {
                if (num[1][i2] == num[i3][i4])
                {
                    sum++;
                    break;
                }
            }
        }
        if (sum == n - 1)
        {
            cout << num[1][i2] << " ";
            check = 1;
        }
    }
    if (check == 0)
    {
        cout << "NO";
    }
    return 0;
}