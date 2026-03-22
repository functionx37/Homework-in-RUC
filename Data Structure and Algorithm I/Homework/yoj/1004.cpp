#include <bits/stdc++.h>
using namespace std;

stack<int> s;
int h[80005];
int n;
int cnt;

int main()
{
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> h[i];
    }
    for (int i = 0; i < n; i++)
    {
        while (!s.empty() && h[i] >= h[s.top()])
        {
            int j = s.top();
            s.pop();
            cnt += i - j - 1;
        }
        s.push(i);
    }
    while (!s.empty())
    {
        int j = s.top();
        s.pop();
        cnt += n - j - 1;
    }
    cout << cnt;
    return 0;
}