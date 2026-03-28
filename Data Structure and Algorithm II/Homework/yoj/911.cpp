#include <iostream>
#include <cmath>

using namespace std;

int num[10001][100] = {0};

int power(int a, int i)
{
    int res = 1;
    while (i > 0)
    {
        res *= a;
        i--;
    }
    return res;
}

void dfs(int n)
{
    int i = 0;
    while (power(2, i) <= n)
    {
        i++;
    }
    i--;
    int remain = n - power(2, i);
    cout << "2";
    if (i == 0)
    {
        cout << "(0)";
    }
    else if (i > 1)
    {
        cout << '(';
        dfs(i);
        cout << ')';
    }
    if (remain)
    {
        cout << "+";
        dfs(remain);
    }
}

int main()
{
    int n;
    cin >> n;
    dfs(n);
    return 0;
}