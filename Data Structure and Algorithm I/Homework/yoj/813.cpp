#include <bits/stdc++.h>
using namespace std;

class treenode
{
public:
    char data;
    treenode *prt, *lc, *rc;
    treenode(char _data = 0, treenode *_prt = nullptr, treenode *_lc = nullptr, treenode *_rc = nullptr) : data(_data), prt(_prt), lc(_lc), rc(_rc) {}
};

class tree
{
public:
    treenode *root;
    int weight;
    tree(treenode *_root, int _weight) : root(_root), weight(_weight) {}
};

tree *getmin(list<tree *> &forest)
{
    tree *m = forest.front();
    auto mi = forest.begin();
    for (auto i = forest.begin(); i != forest.end(); i++)
    {
        if ((*i)->weight < m->weight)
        {
            mi = i;
            m = *i;
        }
    }
    forest.erase(mi);
    return m;
}

unordered_map<char, string> code;
void findcode(treenode *t, vector<int> &v)
{
    if (t->lc)
    {
        v.push_back(0);
        findcode(t->lc, v);
        v.pop_back();
    }
    if (t->rc)
    {
        v.push_back(1);
        findcode(t->rc, v);
        v.pop_back();
    }
    if (t->data)
    {
        string s;
        for (auto i : v)
        {
            s.push_back(i + '0');
        }
        code[t->data] = s;
    }
}

int main()
{
    string s;
    cin >> s;
    unordered_map<char, int> cnt;
    for (auto i : s)
    {
        cnt[i] += 1;
    }
    list<tree *> forest;
    for (auto i : cnt)
    {
        treenode *tmp = new treenode(i.first);
        forest.push_back(new tree(tmp, i.second));
    }
    while (forest.size() > 1)
    {
        tree *m1 = getmin(forest), *m2 = getmin(forest);
        treenode *tmp = new treenode(0, nullptr, m1->root, m2->root);
        m1->root->prt = m2->root->prt = tmp;
        forest.push_back(new tree(tmp, m1->weight + m2->weight));
    }

    vector<int> v;
    findcode(forest.front()->root, v);
    int cost = 0;
    for (auto i : s)
    {
        cost += code[i].size();
    }
    cout << cost << endl;
    for (auto i : code)
    {
        cout << i.first << " " << i.second << endl;
    }
    return 0;
}