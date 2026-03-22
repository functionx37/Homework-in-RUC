#include <bits/stdc++.h>
using namespace std;

class bitmap
{
public:
    int siz;
    char *mem;
    bitmap(int n)
    {
        siz = (n + 7) / 8;
        mem = new char[siz];
        memset(mem, 0, siz);
    }
    ~bitmap()
    {
        delete[] mem;
    }
    void set(int k)
    {
        mem[k >> 8] |= (0x80 >> (k & 0x7));
    }
    void clear(int k)
    {
        mem[k >> 8] &= ~(0x80 >> (k & 0x7));
    }
    bool test(int k)
    {
        return mem[k >> 8] & (0x80 >> (k & 0x7));
    }
};

int main()
{
    bitmap x(100);
    int a, b, p, q;
    cin >> a >> b >> p >> q;
    x.set(a);
    x.set(b);
    x.clear(p);
    cout << x.test(q);
    return 0;
}