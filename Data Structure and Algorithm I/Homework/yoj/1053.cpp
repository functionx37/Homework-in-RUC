#include <bits/stdc++.h>
using namespace std;

void shellSort(vector<int> &vec)
{
    int n = vec.size();
    int gap = n/2;
    for (int i = gap; i < n; i++)
    {
        int temp = vec[i];
        int j;
        for (j = i; j >= gap && vec[j - gap] > temp; j -= gap)
        {
            vec[j] = vec[j - gap];
        }
        vec[j] = temp;
    }
}

int main()
{
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; i++)
    {
        cin >> nums[i];
    }
    shellSort(nums);
    for (int i = 0; i < n; i++)
    {
        cout << nums[i] << " ";
    }
    return 0;
}