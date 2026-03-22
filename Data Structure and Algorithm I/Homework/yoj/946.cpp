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
    void iter(T (*func)(T))
    {
        for (int i = 0; i < siz; i++)
        {
            _vec[i] = func(_vec[i]);
        }
    }
    T rdce(T (*func)(T, T))
    {
        T rst = _vec[0];
        for (int i = 1; i < siz; i++)
        {
            rst = func(rst, _vec[i]);
        }
        return rst;
    }
};

int main()
{
    int n, m;
    cin >> n >> m;
    vec<int> v(n);
    if (m == 1)
    {
        v.iter([](int i)
               { return i - 1; });
        v.prt();
    }
    if (m == 2)
    {
        v.iter([](int i)
               { return i * 2; });
        v.prt();
    }
    if (m == 3)
    {
        cout << v.rdce([](int x, int y)
                       { return x + y; });
    }
    return 0;
}