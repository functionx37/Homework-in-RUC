#include <bits/stdc++.h>
using namespace std;

string a;
int nxt[1000000];

int main()
{
    int n;
    cin >> n >> a;
    int j = 0;
    int t = -1;
    nxt[0] = -1;
    while (j < n - 1)
    {
        if (t < 0 || a[j] == a[t])
        {
            j++;
            t++;
            nxt[j] = t;
        }
        else
        {
            t = nxt[t];
        }
    }
    if (nxt[n - 1] == 0 && a[0] != a[n - 1])
    {
        nxt[n - 1] = -1;
    }
    cout << n - nxt[n - 1] - 1;
    return 0;
}