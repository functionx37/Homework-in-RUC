#include <stdio.h>

// 调整大顶堆：确保以root为根的子树满足堆特性
void sort(int arr[], int n, int root)
{
    int largest = root;       // 初始化最大值为根节点
    int left = 2 * root + 1;  // 左子节点索引
    int right = 2 * root + 2; // 右子节点索引

    // 若左子节点大于根节点，更新最大值索引
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // 若右子节点大于当前最大值，更新最大值索引
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // 若最大值不是根节点，交换并递归调整受影响的子树
    if (largest != root)
    {
        int temp = arr[root];
        arr[root] = arr[largest];
        arr[largest] = temp;

        sort(arr, n, largest); // 递归调整交换后的子树
    }
}

// 堆排序主函数
void mainsort(int arr[], int n)
{
    // 1. 构建大顶堆（从最后一个非叶子节点开始向前调整）
    for (int i = n / 2 - 1; i >= 0; i--)
        sort(arr, n, i);

    // 2. 逐步提取堆顶（最大值）并调整堆
    for (int i = n - 1; i > 0; i--)
    {
        // 堆顶（最大值）与当前未排序部分末尾交换
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // 对剩余未排序部分重新调整为大顶堆
        sort(arr, i, 0);
    }
}

// 测试
int main()
{
    int n;
    scanf("%d", &n);
    int arr[n];
    for (int i = 0; i < n; i += 1)
    {
        scanf("%d", &arr[i]);
    }
    mainsort(arr, n);
    for (int i = 0; i < n; i++)
    {
        printf("%d\n", arr[i]);
    }
    return 0;
}