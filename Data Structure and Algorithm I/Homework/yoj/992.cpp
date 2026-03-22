#include <bits/stdc++.h>
using namespace std;

ostringstream os;
class treenode
{
public:
    int data, color = 0;
    treenode *prt = nullptr;
    treenode *lc = nullptr, *rc = nullptr;
    treenode(long long _data = 0, treenode *_prt = nullptr) : data(_data), prt(_prt) {}
};

class rbtree
{
public:
    treenode *root = nullptr, *hot = nullptr;
    treenode *search(int &i);
    treenode *insert(int &i);
    bool remove(int &i);
    void traverse(treenode *a);
    void traverse1(treenode *a);
    treenode *connect34(treenode *a, treenode *b, treenode *c, treenode *T0, treenode *T1, treenode *T2, treenode *T3);
    void rotate(treenode *v);
    int checkbalance(treenode*a)
    {
        int h1=a->lc?checkbalance(a->lc):-1,h2=a->rc?checkbalance(a->rc):-1;
        if(h1!=h2)
        {
            cout<<a->data<<"\n";
        }
        return h1+a->color;
    }
};

int main()
{
    int n;
    cin >> n;
    int a, b;
    rbtree tree;
    while (n--)
    {
        cin >> a >> b;
        if (a)
        {
            tree.insert(b);
            //tree.checkbalance(tree.root);
        }
        else
        {
            tree.remove(b);
            //tree.checkbalance(tree.root);
        }
        // cout<<"\n";
        // tree.traverse1(tree.root);
        // cout<<"\n";
    }
    queue<treenode*> q;
    q.push(tree.root);
    while(!q.empty())
    {
        auto tmp = q.front();
        q.pop();
        if(tmp==tree.root)
        {
            cout<<tmp->data;
        }
        else
        {
            cout<<" "<<tmp->data;
        }
        if(tmp->lc)
        {
            q.push(tmp->lc);
        }
        if(tmp->rc)
        {
            q.push(tmp->rc);
        }
    }
    return 0;
}

treenode *rbtree::connect34(treenode *a, treenode *b, treenode *c, treenode *T0, treenode *T1, treenode *T2, treenode *T3)
{
    a->lc = T0, a->rc = T1;
    b->lc = a, b->rc = c;
    c->lc = T2, c->rc = T3;
    a->prt = c->prt = b;
    if (T0)
        T0->prt = a;
    if (T1)
        T1->prt = a;
    if (T2)
        T2->prt = c;
    if (T3)
        T3->prt = c;
    return b;
}

void rbtree::rotate(treenode *v)
{
    treenode *p = v->prt, *g = p->prt, *gg = g->prt;
    if (g->lc == p && p->lc == v)
    {
        connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
        p->prt = gg;
        if (gg)
            g == gg->lc ? gg->lc = p : gg->rc = p;
        else
            root = p;
    }
    else if (g->rc == p && p->rc == v)
    {
        connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
        p->prt = gg;
        if (gg)
            g == gg->lc ? gg->lc = p : gg->rc = p;
        else
            root = p;
    }
    else if (g->lc == p && p->rc == v)
    {
        connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
        v->prt = gg;
        if (gg)
            g == gg->lc ? gg->lc = v : gg->rc = v;
        else
            root = v;
    }
    else
    {
        connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
        v->prt = gg;
        if (gg)
            g == gg->lc ? gg->lc = v : gg->rc = v;
        else
            root = v;
    }
}

treenode *rbtree::search(int &e)
{
    hot = nullptr;
    if (!root || root->data == e)
    {
        return root;
    }
    for (hot = root;;)
    {
        treenode *v = e < hot->data ? hot->lc : hot->rc;
        if (!v || v->data == e)
        {
            return v;
        }
        hot = v;
    }
}

treenode *rbtree::insert(int &e)
{
    if (!root)
    {
        treenode *v = root = new treenode(e, nullptr);
        v->color = 1;
        return v;
    }
    treenode *v = search(e);
    if (v)
        return v;
    v = new treenode(e, hot);
    e < hot->data ? hot->lc = v : hot->rc = v;
    for (treenode *now = v;;)
    {
        if (now == root)
        {
            now->color = 1;
            break;
        }
        if (now->prt->color)
        {
            break;
        }
        treenode *p = now->prt, *g = p->prt, *u = p == g->lc ? g->rc : g->lc;
        if (!u || u->color)
        {
            if ((p == g->lc) == (now == p->lc))
            {
                p->color = 1;
            }
            else
            {
                now->color = 1;
            }
            g->color = 0;
            rotate(now);
            break;
        }
        else
        {
            p->color = u->color = 1;
            g->color = 0;
            now = g;
        }
    }
    return v;
}

bool rbtree::remove(int &e)
{
    treenode *v = search(e), *w = v;
    if (!v)
        return false;

    if (v->lc && v->rc)
    {
        w = v->rc;
        while (w->lc)
        {
            w = w->lc;
        }
        swap(v->data, w->data);
        hot = w->prt;
    }
    treenode *x = w->lc ? w->lc : w->rc;
    if (w->prt)
        w == w->prt->lc ? w->prt->lc = x : w->prt->rc = x;
    else
        root = x;
    if (x)
        x->prt = w->prt;
    int color = w->color;
    delete w;

    if (!root || !color)
    {
        return true;
    }
    if (!hot || x && x->color == 0)
    {
        x->color = 1;
        return true;
    }

    for (treenode *i = hot, *j = x; i;)
    {
        treenode *p = i, *s = (j == i->lc) ? i->rc : i->lc;
        if (!s || s->color)
        {
            if (s && s->lc && s->lc->color == 0)
            {
                if (j == i->lc)
                {
                    s->lc->color = p->color;
                    p->color = 1;
                    rotate(s->lc);
                }
                else
                {
                    s->color = p->color;
                    s->lc->color = p->color = 1;
                    rotate(s->lc);
                }
                return true;
            }
            else if (s && s->rc && s->rc->color == 0)
            {
                if (j == i->rc)
                {
                    s->rc->color = p->color;
                    p->color = 1;
                    rotate(s->rc);
                }
                else
                {
                    s->color = p->color;
                    s->rc->color = p->color = 1;
                    rotate(s->rc);
                }
                return true;
            }
            else
            {
                if (p->color == 0)
                {
                    s->color = 0;
                    p->color = 1;
                    return true;
                }
                else
                {
                    s->color = 0;
                    j = i;
                    i = i->prt;
                }
            }
        }
        else
        {
            s->color = 1;
            p->color = 0;
            if (s == p->lc)
            {
                rotate(s->lc);
            }
            else
            {
                rotate(s->rc);
            }
        }
    }
    return true;
}

void rbtree::traverse(treenode *a)
{
    os << a->data << " ";
    if (a->lc)
        traverse(a->lc);
    if (a->rc)
        traverse(a->rc);
}

void rbtree::traverse1(treenode *a)
{
    if (a->lc)
        traverse1(a->lc);
    cout << a->data << " ";
    if (a->rc)
        traverse1(a->rc);
}