#include <bits/stdc++.h>
using namespace std;

int n;
int cun[5001];

stack<int> temp;
bool went[5001];

int main()
{
    cin >> n;
    for (int i = 1; i <= n; ++i)
        cin >> cun[i];
    temp.push(1);
    while (!temp.empty())
    {
        int tp = temp.top();
        temp.pop();
        cout << cun[tp] << " ";
        if (tp * 2 < n)
            temp.push(tp * 2 + 1);
        if (tp * 2 <= n)
            temp.push(tp * 2);
    }
    cout << endl;
    int cur = 1; 
    while (true)
    {
        while (cur <= n)
        {
            temp.push(cur);
            cur *= 2;
        }
        if (temp.empty())
            break;
        cur = temp.top();
        temp.pop();
        cout << cun[cur] << " ";
        cur = cur * 2 + 1;
    }
    cout << endl;
    temp.push(1);
    while (!temp.empty())
    {
        int tp = temp.top();
        if (went[tp])
        {
            temp.pop();
            cout << cun[tp] << " ";
        }
        else
        {
            if (tp * 2 < n)
                temp.push(tp * 2 + 1);
            if (tp * 2 <= n)
                temp.push(tp * 2);
            went[tp] = 1;
        }
    }
    return 0;
}