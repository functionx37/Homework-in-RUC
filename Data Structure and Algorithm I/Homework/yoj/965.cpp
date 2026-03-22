#include <bits/stdc++.h>
using namespace std;

int lft;
char x;

int main()
{
    while (1)
    {
        cin >> x;
        if (x == '(')
            lft++;
        if (x == ')')
        {
            lft--;
            if (lft < 0)
            {
                cout << "NO";
                return 0;
            }
        }
        if (x == '@')
            break;
    }
    if (lft == 0)
        cout << "YES";
    else
        cout << "NO";
    return 0;
}