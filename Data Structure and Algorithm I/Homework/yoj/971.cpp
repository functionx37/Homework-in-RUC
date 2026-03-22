#include <bits/stdc++.h>
using namespace std;

int n;
int cun[5001];

void qian(int k)
{
    cout << cun[k] << " ";
    if (k * 2 <= n)
        qian(2 * k);
    if (k * 2 < n)
        qian(2 * k + 1);
}
void zhong(int k)
{
    if (k * 2 <= n)
        zhong(2 * k);
    cout << cun[k] << " ";
    if (k * 2 < n)
        zhong(2 * k + 1);
}
void hou(int k)
{
    if (k * 2 <= n)
        hou(2 * k);
    if (k * 2 < n)
        hou(2 * k + 1);
    cout << cun[k] << " ";
}

int main()
{
    cin >> n;
    for (int i = 1; i <= n; ++i)
        cin >> cun[i];
    qian(1);
    cout << endl;
    zhong(1);
    cout << endl;
    hou(1);
    return 0;
}