#include <bits/stdc++.h>
using namespace std;

int main()
{
    int rst = -0x7fffffff, ans = 0;
    int n;
    cin >> n;
    while (n--)
    {
        int temp;
        cin >> temp;
        ans += temp;
        if (ans > rst)
            rst = ans;
        if (ans < 0)
            ans = 0;
    }
    cout << rst;
    return 0;
}