#include <bits/stdc++.h>
using namespace std;

using HuffForest = priority_queue<long long, vector<long long>, greater<long long>>;

long long generateTree(HuffForest &forest)
{
    long long min_total_length = 0;

    if (forest.size() <= 1)
    {
        return 0;
    }

    while (forest.size() > 1)
    {

        long long T1_weight = forest.top();
        forest.pop();

        long long T2_weight = forest.top();
        forest.pop();

        long long S_weight = T1_weight + T2_weight;

        min_total_length += S_weight;

        forest.push(S_weight);
    }

    return min_total_length;
}

int main()
{
    int n;
    cin >> n;
    HuffForest forest;
    for (int i = 0; i < n; i++)
    {
        long long weight;
        cin >> weight;
        if (weight > 0)
        {
            forest.push(weight);
        }
    }

    cout << generateTree(forest) << endl;

    return 0;
}