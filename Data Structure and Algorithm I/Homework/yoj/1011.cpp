#include <bits/stdc++.h>
using namespace std;

struct node
{
    int v = 0;
    node *l = nullptr;
    node *r = nullptr;
    int l_max = 0;
    int r_max = 0;
    node(int val = 0) : v(val) {}
};

node *rt = nullptr;

node *build()
{
    string temp;
    if (!getline(cin, temp, ','))
    {
        return nullptr;
    }

    if (temp == "#" || temp.empty())
    {
        return nullptr;
    }

    node *root = new node(stoi(temp));
    root->l = build();
    root->r = build();

    return root;
}

int rst = 0;

int cal(node *ptr)
{
    if (ptr == nullptr)
        return 0;
    ptr->l_max = max(cal(ptr->l), 0);
    ptr->r_max = max(cal(ptr->r), 0);
    rst = max(rst, ptr->l_max + ptr->r_max + ptr->v);
    return ptr->v + max(ptr->l_max, ptr->r_max);
}

int main()
{
    rt = build();
    cal(rt);
    cout << rst;
    return 0;
}