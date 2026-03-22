#include <bits/stdc++.h>
using namespace std;

template <typename T>
class vec
{
public:
    T *_vec;
    int siz;
    vec(int n)
    {
        siz = n;
        _vec = new T[siz];
        for (int i = 0; i < siz; i++)
        {
            cin >> _vec[i];
        }
    }
    ~vec()
    {
        delete[] _vec;
    }
    void rmv(int lo, int hi)
    {
        for (int i = hi; i < siz; i++)
        {
            _vec[i - hi + lo] = _vec[i];
        }
        siz -= hi - lo;
    }
    void prt()
    {
        for (int i = 0; i < siz; i++)
        {
            cout << _vec[i] << " ";
        }
        cout << endl;
    }
};

int main()
{
    int n, k;
    cin >> n >> k;
    vec<int> v(n);
    for (int i = 0; i < k; i++)
    {
        int l, h;
        cin >> l >> h;
        v.rmv(l, h);
    }
    v.prt();
    return 0;
}