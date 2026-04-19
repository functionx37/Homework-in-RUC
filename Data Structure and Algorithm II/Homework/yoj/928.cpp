#include <stdio.h>
int line[1000000][2];
int answer_line[1000000][2];
int n, tmp;
int max_n = 1;
int partition(int arr[1000000][2], int low, int high)
{
    int pivot = arr[low][1]; // 选择第一个元素作为基准
    int i = low;
    int j = high;

    while (i < j)
    {
        // 从右向左找第一个小于等于pivot的数
        while (i < j && arr[j][1] > pivot)
        {
            j--;
        }
        // 从左向右找第一个大于等于pivot的数
        while (i < j && arr[i][1] <= pivot)
        {
            i++;
        }
        // 交换这两个元素
        if (i < j)
        {
            tmp = arr[i][1], arr[i][1] = arr[j][1], arr[j][1] = tmp;
            tmp = arr[i][0], arr[i][0] = arr[j][0], arr[j][0] = tmp;
        }
    }
    // 将基准值放到正确的位置（i和j相遇的位置）
    tmp = arr[i][1], arr[i][1] = arr[low][1], arr[low][1] = tmp;
    tmp = arr[i][0], arr[i][0] = arr[low][0], arr[low][0] = tmp;
    return i; // 返回基准值的索引
}
void quickSort(int arr[1000000][2], int low, int high)
{
    if (low < high)
    {
        // pi 是分区操作后基准值的索引
        int pi = partition(arr, low, high);

        // 递归排序基准值左边的子数组
        quickSort(arr, low, pi - 1);
        // 递归排序基准值右边的子数组
        quickSort(arr, pi + 1, high);
    }
}

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d %d", &line[i][0], &line[i][1]);

    quickSort(line, 0, n - 1);

    answer_line[0][0] = line[0][0], answer_line[0][1] = line[0][1];
    for (int i = 1; i < n; i++)
    {
        if (answer_line[max_n - 1][1] <= line[i][0])
        {
            answer_line[max_n][0] = line[i][0], answer_line[max_n][1] = line[i][1];
            max_n++;
        }
    }
    printf("%d\n", max_n);
    return 0;
}