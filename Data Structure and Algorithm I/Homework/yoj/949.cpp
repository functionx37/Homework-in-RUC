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
    void srt()
    {
        sort(_vec, _vec + siz);
    }
    int find(T k)
    {
        for (int i = 0; i < siz; i++)
        {
            if (_vec[i] == k)
                return i;
        }
        return -1;
    }
    void unq_1()
    {
        srt();
        int i = 0, j = 1;
        while (j < siz)
        {
            if (_vec[i] != _vec[j])
            {
                i++;
                _vec[i] = _vec[j];
            }
            j++;
        }
        rmv(i + 1, siz);
    }
    void unq_2()
    {
        int i = 0, j = 0;
        while (j < siz)
        {
            if (find(_vec[j]) == j)
            {
                _vec[i] = _vec[j];
                i++;
            }
            j++;
        }
        rmv(i, siz);
    }
    void bub_srt()
    {
        for (int i = 0; i < siz - 1; i++)
        {
            int last_swap = 1;
            for (int j = 1; j < siz - i; j++)
            {
                if (_vec[j] < _vec[j - 1])
                {
                    swap(_vec[j], _vec[j - 1]);
                    last_swap = j;
                }
            }
            prt();
            if (last_swap == 1)
                break;
        }
    }
};

int main()
{
    int n;
    cin >> n;
    vec<int> v(n);
    v.bub_srt();
    return 0;
}