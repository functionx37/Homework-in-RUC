#include <bits/stdc++.h>
using namespace std;

class steap
{
public:
    stack<int> sto;
    stack<int> big;
    void pop()
    {
        sto.pop();
        big.pop();
    }
    void push(int v)
    {
        sto.push(v);
        big.push(big.empty() ? v : max(v, big.top()));
    }
    int top()
    {
        return sto.top();
    }
    int getmax()
    {
        return big.top();
    }
};

int main()
{
    int m;
    cin >> m;
    steap a;
    while (m--)
    {
        int k;
        cin >> k;
        if (k == 1)
        {
            int v;
            cin >> v;
            a.push(v);
        }
        else if (k == 2)
        {
            a.pop();
        }
        else if (k == 3)
        {
            cout << a.top() << endl;
        }
        else
        {
            cout << a.getmax() << endl;
        }
    }
    return 0;
}