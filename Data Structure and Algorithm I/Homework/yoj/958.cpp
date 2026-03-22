#include <bits/stdc++.h>
using namespace std;

template <typename T>
class lst
{
public:
    int siz;
    T sto[200];
    int pre[200];
    int nxt[200];
    bool is_del[200];
    lst(int n)
    {
        for (int i = 1; i <= n; i++)
        {
            cin >> sto[i];
            pre[i] = i - 1;
            nxt[i] = i + 1;
        }
        nxt[0] = 1;
        pre[0] = n;
        nxt[n] = 0;
        siz = n;
    }
    int at(int k)
    {
        int p = 0;
        for (int i = 1; i <= k; i++)
            p = nxt[p];
        return p;
    }
    void ist(int p, int v)
    {
        ++siz;
        sto[siz] = v;
        p = at(p);
        nxt[pre[p]] = siz;
        pre[siz] = pre[p];
        pre[p] = siz;
        nxt[siz] = p;
    }
    int find(int v)
    {
        int p = nxt[0];
        while (p != 0)
        {
            if (sto[p] == v)
                return p;
            p = nxt[p];
        }
        return 0;
    }
    void rmv(int p)
    {
        nxt[pre[p]] = nxt[p];
        pre[nxt[p]] = pre[p];
        is_del[p] = 1;
    }
    void prt()
    {
        int p = nxt[0];
        while (p != 0)
        {
            cout << sto[p] << " ";
            p = nxt[p];
        }
        cout << endl;
    }
};

int main()
{
    int n;
    cin >> n;
    lst<int> a(n);
    int m;
    cin >> m;
    while (m--)
    {
        int k;
        cin >> k;
        if (k == 0)
        {
            int q;
            cin >> q;
            a.rmv(a.find(q));
        }
        else
        {
            int p, q;
            cin >> p >> q;
            a.ist(q, p);
        }
    }
    a.prt();
    return 0;
}