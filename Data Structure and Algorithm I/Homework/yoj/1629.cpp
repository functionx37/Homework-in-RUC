#include <bits/stdc++.h>
using namespace std;

int nxt[1000001];

void get_Next(vector<int> s) // 这个函数对字符串s进行预处理得到next数组
{
    int j = 0;
    nxt[0] = 0; // 初始化
    for (int i = 1; i < s.size(); i++)
    { // i指针指向的是后缀末尾，j指针指向的是前缀末尾
        while (j > 0 && s[i] != s[j])
            j = nxt[j - 1]; // 前后缀不相同，去找j前一位的最长相等前后缀
        if (s[i] == s[j])
            j++;    // 前后缀相同，j指针后移
        nxt[i] = j; // 更新next数组
    }
}
int strSTR(vector<int> s, vector<int> t) // 从s中查找t，如果存在返回t出现的位置，如果不存在返回-1
{
    if (t.size() == 0)
        return 0;
    get_Next(t);
    int j = 0;
    for (int i = 0; i < s.size(); i++)
    {
        while (j > 0 && s[i] != t[j])
            j = nxt[j - 1];
        if (s[i] == t[j])
            j++;
        if (j == t.size())
            return i - t.size() + 1;
    }
    return -1;
}

int n;

vector<int> a, b, diff1, diff2;

int main()
{
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int temp;
        cin >> temp;
        a.push_back(temp);
    }
    for (int i = 0; i < n; i++)
    {
        int temp;
        cin >> temp;
        b.push_back(temp);
    }
    for (int i = 0; i < n; i++)
    {
        diff1.push_back(a[(i + 1) % n] - a[i]);
        diff2.push_back(b[(i + 1) % n] - b[i]);
    }
    for (int i = 0; i < n; i++)
    {
        diff1.push_back(a[(i + 1) % n] - a[i]);
    }
    int k = strSTR(diff1, diff2);
    if (k >= n)
        k = -1;
    cout << k;
    return 0;
}