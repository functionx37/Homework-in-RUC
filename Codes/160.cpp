#include <bits/stdc++.h>
using namespace std;

char a[30], b[30];

int main()
{
    cin >> a >> b;
    int n1 = strlen(a);
    int n2 = strlen(b);
    int i = 0;
    int j = 0;
    while (i < n1 && j < n2)
    {
        if (a[i] == b[j])
        {
            i = (i + 1) % n1;
            j++;
        }
        else
        {
            i = ((i - j) % n1 + n1) % n1 + 1;
            j = 0;
        }
    }
    if (j == n2)
        cout << "true";
    else
        cout << "false";
    return 0;
}