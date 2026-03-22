#include <bits/stdc++.h>
using namespace std;

int n, k;
int a[1000001];

int partition(int arr[], int left, int right)
{
    if (left == right)
        return left;
    int pivot = arr[left];
    int lo = left + 1;
    int hi = right;
    while (lo <= hi)
    {
        if (arr[lo] < pivot)
        {
            lo++;
        }
        else if (arr[lo] > pivot)
        {
            int tmp = arr[lo];
            arr[lo] = arr[hi];
            arr[hi] = tmp;
            hi--;
        }
        else
        {
            lo++;
        }
    }
    arr[left] = arr[hi];
    arr[hi] = pivot;
    return hi;
}

int quickselect(int arr[], int left, int right, int k)
{
    if (left == right)
    {
        return arr[left];
    }
    int p = partition(arr, left, right);
    if (p == k)
    {
        return arr[p];
    }
    else if (k < p)
    {
        return quickselect(arr, left, p - 1, k); // 在左边找
    }
    else
    {
        return quickselect(arr, p + 1, right, k); // 在右边找
    }
}

int main()
{
    cin >> n >> k;
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    cout << quickselect(a, 0, n - 1, k - 1);
    return 0;
}