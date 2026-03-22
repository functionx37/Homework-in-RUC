#include <bits/stdc++.h>
using namespace std;

int n;
int temp;
vector<int> qian, zhong;

void work(vector<int> q, vector<int> z)
{
    if (q.empty() || z.empty())
    {
        return;
    }
    int root = q.front();
    auto rank = find(z.begin(), z.end(), root);
    int k = rank - z.begin();
    vector<int> ql(q.begin() + 1, q.begin() + 1 + k);
    vector<int> qr(q.begin() + 1 + k, q.end());
    vector<int> zl(z.begin(), rank);
    vector<int> zr(rank + 1, z.end());
    work(ql, zl);
    work(qr, zr);
    cout << root << " ";
}

int main()
{
    cin >> n;
    for (int i = 1; i <= n; ++i)
    {
        cin >> temp;
        qian.push_back(temp);
    }
    for (int i = 1; i <= n; ++i)
    {
        cin >> temp;
        zhong.push_back(temp);
    }
    work(qian, zhong);
    return 0;
}