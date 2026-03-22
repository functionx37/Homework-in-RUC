#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct node
{
    T val;
    node *pre = nullptr;
    node *nxt = nullptr;
    node() {};
    node(T v) : val(v) {};
};

template <typename T>
class lst
{
public:
    int len = 0;
    node<T> *head = new node<T>;
    node<T> *tail = new node<T>;
    lst(int n)
    {
        head->nxt = tail;
        tail->pre = head;
        for (int i = 0; i < n; i++)
        {
            T v;
            cin >> v;
            ist(tail->pre, v);
        }
    }
    ~lst()
    {
        auto it = head;
        while (it != tail)
        {
            it = it->nxt;
            delete it->pre;
        }
        delete tail;
    }
    void ist(node<T> *nd, T v)
    {
        auto nt = nd->nxt;
        auto nw = new node<T>(v);
        nw->pre = nd;
        nw->nxt = nt;
        nd->nxt = nw;
        nt->pre = nw;
        ++len;
    }
    void rmv(node<T> *nd)
    {
        auto aft = nd->nxt;
        auto bef = nd->pre;
        bef->nxt = aft;
        aft->pre = bef;
        delete nd;
        len--;
    }
    void prt()
    {
        auto it = head;
        while (true)
        {
            it = it->nxt;
            if (it == tail)
                break;
            cout << it->val << " ";
        }
        cout << endl;
        return;
    }
    node<T> *at(int k)
    {
        int i = 0;
        auto it = head;
        while (it != tail)
        {
            it = it->nxt;
            ++i;
            if (i == k)
                return it;
        }
        return nullptr;
    }
    node<T> *find(int v)
    {
        auto it = head;
        while (true)
        {
            it = it->nxt;
            if (it == tail)
                return nullptr;
            if (it->val == v)
                return it;
        }
    }
    void unq()
    {
        auto it = head;
        while (true)
        {
            it = it->nxt;
            if (it == tail)
                return;
            auto fst = find(it->val);
            if (fst != it)
                rmv(fst);
        }
    }
    void srt()
    {
        if (len <= 1)
            return;
        auto it = head->nxt->nxt;
        while (it != tail)
        {
            auto jt = head;
            while (jt != it)
            {
                if ((jt == head || jt->val <= it->val) && jt->nxt->val >= it->val)
                {
                    ist(jt, it->val);
                    break;
                }
                jt = jt->nxt;
            }
            jt = it;
            it = it->nxt;
            rmv(jt);
        }
    }
};

int main()
{
    int n;
    cin >> n;
    lst<int> a(n);
    a.srt();
    a.prt();
    return 0;
}