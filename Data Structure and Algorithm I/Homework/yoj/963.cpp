#include <bits/stdc++.h>
using namespace std;

int n, m;
string num;
stack<char> new_;

int get_c(char x)
{
    if (x >= '0' && x <= '9')
        return x - '0';
    return x - 'A' + 10;
}

char c_get(int x)
{
    if (x < 10)
        return x + '0';
    else
        return x - 10 + 'A';
}

int get_s()
{
    int x = 0;
    for (auto it : num)
    {
        x *= n;
        x += get_c(it);
    }
    return x;
}

void s_get()
{
    int x = get_s();
    while (x)
    {
        new_.push(c_get(x % m));
        x /= m;
    }
}

int main()
{
    cin >> n >> num >> m;
    s_get();
    while(!new_.empty()){
        cout<<new_.top();
        new_.pop();
    }
    return 0;
}