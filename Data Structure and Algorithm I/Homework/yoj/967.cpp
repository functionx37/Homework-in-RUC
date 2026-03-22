#include <bits/stdc++.h>
using namespace std;

stack<char> s;
char c;
stack<int> ans;
string str;

int main()
{
    cin >> str;
    for (int i = 0; i < str.size(); i++)
    {
        c = str[i];
        if (c == '@')
            break;
        else if (c >= '0' && c <= '9')
        {
            int x = c - '0';
            while (str[i + 1] >= '0' && str[i + 1] <= '9')
            {
                i++;
                x *= 10;
                x += str[i] - '0';
            }
            ans.push(x);
            cout << x << " ";
        }
        else if (c == '(')
            s.push(c);
        else if (c == ')')
        {
            while (s.top() != '(')
            {
                cout << s.top() << " ";
                int x = ans.top();
                ans.pop();
                int y = ans.top();
                ans.pop();
                if (s.top() == '+')
                    ans.push(x + y);
                else
                    ans.push(x * y);
                s.pop();
            }
            s.pop();
        }
        else if (c == '+')
        {
            while (!s.empty() && (s.top() == '*' || s.top() == '+'))
            {
                cout << s.top() << " ";
                int x = ans.top();
                ans.pop();
                int y = ans.top();
                ans.pop();
                if (s.top() == '+')
                    ans.push(x + y);
                else
                    ans.push(x * y);
                s.pop();
            }
            s.push(c);
        }
        else
        {
            while (!s.empty() && (s.top() == '*'))
            {
                cout << s.top() << " ";
                int x = ans.top();
                ans.pop();
                int y = ans.top();
                ans.pop();
                if (s.top() == '+')
                    ans.push(x + y);
                else
                    ans.push(x * y);
                s.pop();
            }
            s.push(c);
        }
    }
    while (!s.empty())
    {
        cout << s.top() << " ";
        int x = ans.top();
        ans.pop();
        int y = ans.top();
        ans.pop();
        if (s.top() == '+')
            ans.push(x + y);
        else
            ans.push(x * y);
        s.pop();
    }
    cout << "\n"
         << ans.top();
    return 0;
}