#include <bits/stdc++.h>
using namespace std;

class queap
{
public:
    queue<int> sto;
    int big[10001];
    void enqueue(int v)
    {
        sto.push(v);
        big[sto.size()] = v;
        for (int i = sto.size() - 1; i > 0 && big[i] <= v; i--)
        {
            big[i] = v;
        }
    }
    void dequeue()
    {
        if (sto.empty())
            return;
        for (int i = 0; i < sto.size(); i++)
        {
            big[i] = big[i + 1];
        }
        sto.pop();
    }
    int getMax()
    {
        if (sto.empty())
            return -1;
        return big[1];
    }
};

int main()
{
    int n;
    cin >> n;
    queap a;
    while (n--)
    {
        int k;
        cin >> k;
        if (k == 0)
        {
            a.dequeue();
        }
        else if (k == 1)
        {
            int v;
            cin >> v;
            a.enqueue(v);
        }
        else
        {
            cout << a.getMax() << endl;
        }
    }
    return 0;
}