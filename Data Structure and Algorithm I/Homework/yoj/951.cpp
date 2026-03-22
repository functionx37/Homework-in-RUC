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
            insert(tail->pre, v);
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
    void insert(node<T> *nd, T v)
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
        auto del = nd->pre;
        if (del == head)
            return;
        auto bef = del->pre;
        bef->nxt = nd;
        nd->pre = bef;
        delete del;
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
    node<T> *find(int k)
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
};

int main()
{
    int n;
    cin >> n;
    lst<int> a(n);
    int r, m;
    cin >> r >> m;
    a.insert(a.find(r), m);
    a.rmv(a.find(r));
    a.prt();
    return 0;
}