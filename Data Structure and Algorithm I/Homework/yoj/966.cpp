#include <bits/stdc++.h>
using namespace std;

stack<int> a;
char x[1000000];
bool yubei = 0;
int ans;

int main()
{
    cin.getline(x, 1000000);
    for (int i = 0; x[i] != 0; i++)
    {
        if (x[i] == '+')
            continue;
        else if (x[i] == '*')
            yubei = 1;
        else
        {
            int t = 0;
            int j = i;
            for (; x[j] >= '0' && x[j] <= '9'; j++)
            {
                t *= 10;
                t += x[j] - '0';
                t %= 10000;
            }
            i = j - 1;
            if (yubei)
            {
                int p = a.top();
                a.pop();
                a.push(p * t % 10000);
                yubei = 0;
            }
            else{
                a.push(t);
            }
        }
    }
    while(!a.empty()){
        ans = (ans+a.top())%10000;
        a.pop();
    }
    cout<<ans;
    return 0;
}