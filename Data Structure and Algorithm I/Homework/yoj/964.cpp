#include <bits/stdc++.h>
using namespace std;

stack<int> x, y, z;
int a[100001], b[100001];

int main()
{
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    for (int i = 0; i < n; i++)
    {
        cin >> b[i];
    }
    for (int i = n - 1; i >= 0; i--)
    {
        x.push(a[i]);
        y.push(b[i]);
    }
    while (!x.empty())
    {
        int temp = x.top();
        x.pop();
        z.push(temp);
        while (!z.empty() && !y.empty() && z.top() == y.top())
        {
            z.pop();
            y.pop();
        }
    }
    cout << (z.empty() ? "Yes" : "No");
    return 0;
}