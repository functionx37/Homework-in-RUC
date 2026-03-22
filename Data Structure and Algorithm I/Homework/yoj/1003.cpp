#include <bits/stdc++.h>
using namespace std;

string x, y;

void op(string a, string b, stack<char> s, string temp, string ope)
{
    if (temp.size() == b.size())
    {
        if (temp == b)
            cout << ope << endl;
        return;
    }
    int len = s.size() + temp.size();
    if (len < a.size())
    {
        s.push(a[len]);
        op(a, b, s, temp, ope + "i ");
        s.pop();
    }
    if (!s.empty())
    {
        temp += s.top();
        s.pop();
        ope += "o ";
        op(a, b, s, temp, ope);
    }
}

int main()
{
    while (cin >> x)
    {
        cin >> y;
        cout << "[\n";
        op(x, y, stack<char>(), "", "");
        cout << "]\n";
    }
    return 0;
}