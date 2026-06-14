#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int x, y;
} Point;

int compare_points(const void *a, const void *b)
{
    Point *p1 = (Point *)a;
    Point *p2 = (Point *)b;
    if (p1->x != p2->x)
        return p1->x - p2->x;
    return p1->y - p2->y;
}

typedef struct Node
{
    int *y;
    int len;
} Node;

Node tree[400000];
Point points[100000];
int n;

void build(int node, int l, int r)
{
    if (l == r)
    {
        tree[node].y = (int *)malloc(sizeof(int));
        tree[node].y[0] = points[l].y;
        tree[node].len = 1;
        return;
    }
    int mid = (l + r) >> 1;
    build(node << 1, l, mid);
    build((node << 1) + 1, mid + 1, r);
    int len1 = tree[node << 1].len, *y1 = tree[node << 1].y;
    int len2 = tree[(node << 1) + 1].len, *y2 = tree[(node << 1) + 1].y;
    tree[node].len = len1 + len2;
    tree[node].y = (int *)malloc(tree[node].len * sizeof(int));
    int i = 0, j = 0, k = 0;
    while (i < len1 && j < len2)
        tree[node].y[k++] = y1[i] < y2[j] ? y1[i++] : y2[j++];
    while (i < len1)
        tree[node].y[k++] = y1[i++];
    while (j < len2)
        tree[node].y[k++] = y2[j++];
}

int lower_bound(int *arr, int len, int y)
{
    int l = 0, r = len;
    while (l < r)
    {
        int m = (l + r) >> 1;
        if (arr[m] < y)
            l = m + 1;
        else
            r = m;
    }
    return l;
}

int upper_bound(int *arr, int len, int y)
{
    int l = 0, r = len;
    while (l < r)
    {
        int m = (l + r) >> 1;
        if (arr[m] <= y)
            l = m + 1;
        else
            r = m;
    }
    return l;
}

int query(int node, int l, int r, int L, int R, int y0, int y1)
{
    if (r < L || l > R)
        return 0;
    if (L <= l && r <= R)
    {
        int *arr = tree[node].y;
        int cnt = upper_bound(arr, tree[node].len, y1) - lower_bound(arr, tree[node].len, y0);
        return cnt;
    }
    int mid = (l + r) >> 1;
    return query(node << 1, l, mid, L, R, y0, y1) + query((node << 1) + 1, mid + 1, r, L, R, y0, y1);
}

int find_left(int x)
{
    int l = 0, r = n - 1, res = n;
    while (l <= r)
    {
        int m = (l + r) >> 1;
        if (points[m].x >= x)
            res = m, r = m - 1;
        else
            l = m + 1;
    }
    return res;
}

int find_right(int x)
{
    int l = 0, r = n - 1, res = -1;
    while (l <= r)
    {
        int m = (l + r) >> 1;
        if (points[m].x <= x)
            res = m, l = m + 1;
        else
            r = m - 1;
    }
    return res;
}

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
        scanf("%d %d", &points[i].x, &points[i].y);
    qsort(points, n, sizeof(Point), compare_points);
    build(1, 0, n - 1);

    int q;
    scanf("%d", &q);
    while (q--)
    {
        int x0, x1, y0, y1;
        scanf("%d %d %d %d", &x0, &x1, &y0, &y1);
        int l = find_left(x0), r = find_right(x1);
        printf("%d\n", l > r ? 0 : query(1, 0, n - 1, l, r, y0, y1));
    }

    for (int i = 0; i < 400000; ++i)
        if (tree[i].len > 0)
            free(tree[i].y);
    return 0;
}
