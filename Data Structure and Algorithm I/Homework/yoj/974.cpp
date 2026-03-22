#include <bits/stdc++.h>
using namespace std;

int n;
int cun[5001];
vector<int> zon;

void zhong(int k)
{
    if (k * 2 <= n)
        zhong(2 * k);
    zon.push_back(cun[k]);
    if (k * 2 < n)
        zhong(2 * k + 1);
}

int main()
{
    cin >> n;
    for (int i = 1; i <= n; ++i)
        cin >> cun[i];
    zhong(1);
    int sk;
    cin >> sk;
    for (int i = 0; i < zon.size(); ++i)
    {
        if (zon[i] == sk)
        {
            cout << zon[i - 1] << " " << zon[i + 1];
            break;
        }
    }
    return 0;
}