#include <bits/stdc++.h>
using namespace std;

struct lnode
{
    int val;
    int dist;
    lnode *left;
    lnode *right;
    lnode(int v) : val(v), dist(1), left(nullptr), right(nullptr) {}
};

lnode *merge(lnode *x, lnode *y)
{
    if (!x)
        return y;
    if (!y)
        return x;
    if (x->val < y->val)
    {
        x->right = merge(x->right, y);
        if (!x->right)
        {
            x->dist = 1;
            return x;
        }
        if (!x->left || x->right->dist > x->left->dist)
            swap(x->left, x->right);
        if (!x->right)
            x->dist = 1;
        else
            x->dist = x->right->dist + 1;
        return x;
    }
    else
    {
        y->right = merge(y->right, x);
        if (!y->right)
        {
            y->dist = 1;
            return y;
        }
        if (!y->left || y->right->dist > y->left->dist)
            swap(y->left, y->right);
        if (!y->right)
            y->dist = 1;
        else
            y->dist = y->right->dist + 1;
        return y;
    }
}

lnode *insert(lnode *rt, int v)
{
    lnode *n = new lnode(v);
    rt = merge(rt, n);
    return rt;
}

lnode *pop(lnode *rt)
{
    if (!rt)
        return nullptr;
    cout << rt->val << " ";
    lnode *new_root = merge(rt->left, rt->right);
    delete rt;
    return new_root;
}

int main()
{
    int n;
    cin >> n;
    lnode *A = nullptr;
    while (n--)
    {
        int x;
        cin >> x;
        A = insert(A, x);
    }
    int m;
    cin >> m;
    lnode *B = nullptr;
    while (m--)
    {
        int x;
        cin >> x;
        B = insert(B, x);
    }
    A = merge(A, B);
    cout << A->dist << endl;
    while (A)
    {
        A = pop(A);
    }
    return 0;
}