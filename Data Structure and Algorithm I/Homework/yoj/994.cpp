#include <bits/stdc++.h>
using namespace std;

struct TreeNode
{
    int key;
    TreeNode *left;
    TreeNode *right;
    int size;
    int count;
    int height;

    TreeNode(int value)
        : key(value), size(1), count(1), height(1), left(nullptr), right(nullptr) {}
};

TreeNode *findMin(TreeNode *root)
{
    while (root->left != nullptr)
        root = root->left;
    return root;
}

TreeNode *findMax(TreeNode *root)
{
    while (root->right != nullptr)
        root = root->right;
    return root;
}

int getHeight(TreeNode *node) // 获取树高
{
    return node ? node->height : 0;
}

void update(TreeNode *node) // 更新树高
{
    if (node == nullptr)
        return;
    node->size = node->count + (node->left ? node->left->size : 0) + (node->right ? node->right->size : 0);
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
}

TreeNode *connect34(TreeNode *a, TreeNode *b, TreeNode *c,
                    TreeNode *t0, TreeNode *t1, TreeNode *t2, TreeNode *t3) // 3+4 重构函数
{
    // 重构左子树：a 连接 t0, t1
    a->left = t0;
    a->right = t1;
    update(a);

    // 重构右子树：c 连接 t2, t3
    c->left = t2;
    c->right = t3;
    update(c);

    // 重构根节点：b 连接 a, c
    b->left = a;
    b->right = c;
    update(b);

    return b; // 返回新的子树根节点
}

TreeNode *maintain(TreeNode *root) // 平衡维护函数
{
    if (root == nullptr)
        return nullptr;

    update(root); // 先更新当前节点信息

    int lh = getHeight(root->left);
    int rh = getHeight(root->right);

    // 如果高度差不超过1，则平衡
    if (std::abs(lh - rh) <= 1)
        return root;

    if (lh > rh) // 左边高，需要右旋
    {
        TreeNode *L = root->left;
        // 判断是 LL 还是 LR
        if (getHeight(L->left) >= getHeight(L->right))
        {
            // LL型：右旋一次
            // 涉及节点：L->left (a), L (b), root (c)
            return connect34(L->left, L, root,
                             L->left->left, L->left->right, L->right, root->right);
        }
        else
        {
            // LR型：先左旋后右旋
            // 涉及节点：L (a), L->right (b), root (c)
            TreeNode *LR = L->right;
            return connect34(L, LR, root,
                             L->left, LR->left, LR->right, root->right);
        }
    }
    else // 右边高，需要左旋
    {
        TreeNode *R = root->right;
        // 判断是 RR 还是 RL
        if (getHeight(R->right) >= getHeight(R->left))
        {
            // RR型：左旋一次
            // 涉及节点：root (a), R (b), R->right (c)
            return connect34(root, R, R->right,
                             root->left, R->left, R->right->left, R->right->right);
        }
        else
        {
            // RL型：先右旋后左旋
            // 涉及节点：root (a), R->left (b), R (c)
            TreeNode *RL = R->left;
            return connect34(root, RL, R,
                             root->left, RL->left, RL->right, R->right);
        }
    }
}

TreeNode *insert(TreeNode *root, int value) // 插入节点
{
    if (root == nullptr)
        return new TreeNode(value);

    if (value < root->key)
        root->left = insert(root->left, value);
    else if (value > root->key)
        root->right = insert(root->right, value);
    else
        root->count++;
    return maintain(root);
}

TreeNode *remove(TreeNode *root, int value) // 删除元素
{
    if (root == nullptr)
        return root;

    if (value < root->key)
        root->left = remove(root->left, value);
    else if (value > root->key)
        root->right = remove(root->right, value);
    else
    {
        if (root->count > 1)
            root->count--;
        else
        {
            if (root->left == nullptr)
            {
                TreeNode *temp = root->right;
                delete root;
                return temp; // 叶子或单链情况，返回后上一层递归会进行maintain
            }
            else if (root->right == nullptr)
            {
                TreeNode *temp = root->left;
                delete root;
                return temp;
            }
            else
            {
                TreeNode *successor = findMin(root->right);
                root->key = successor->key;
                root->count = successor->count;
                successor->count = 1;
                root->right = remove(root->right, successor->key);
            }
        }
    }
    return maintain(root);
}

void inorderTraversal(TreeNode *root)
{
    if (root == nullptr)
        return;
    inorderTraversal(root->left);
    std::cout << root->key << " ";
    inorderTraversal(root->right);
}

TreeNode *search(TreeNode *root, int target)
{
    if (root == nullptr || root->key == target)
        return root;
    if (target < root->key)
        return search(root->left, target);
    return search(root->right, target);
}

int queryRank(TreeNode *root, int v)
{
    if (root == nullptr)
        return 0;
    if (root->key == v)
        return (root->left ? root->left->size : 0) + 1;
    if (root->key > v)
        return queryRank(root->left, v);
    return queryRank(root->right, v) + (root->left ? root->left->size : 0) + root->count;
}

int querykth(TreeNode *root, int k)
{
    if (root == nullptr)
        return -1; // 或者抛出异常，表示 k 超出了树的大小

    // 1. 获取左子树的大小（如果没有左子树，则为0）
    int leftSize = (root->left ? root->left->size : 0);

    // 2. 情况A: k 在左子树范围内
    if (k <= leftSize)
    {
        return querykth(root->left, k);
    }

    // 3. 情况B: k 在当前节点的范围内 (左子树大小 < k <= 左子树大小 + 当前节点重复次数)
    // 比如左边有5个，当前有2个。如果找第6个或第7个，那就是当前节点。
    if (k <= leftSize + root->count)
    {
        return root->key;
    }

    // 4. 情况C: k 在右子树范围内
    // 减去左边的数量和当前节点的数量
    return querykth(root->right, k - leftSize - root->count);
}

// 求 x 的前驱（小于 x，且最大的数）
int getPredecessor(TreeNode *root, int val)
{
    int ans = INT_MIN; // 初始化为最小值，表示未找到
    TreeNode *curr = root;
    while (curr != nullptr)
    {
        if (curr->key < val)
        {
            ans = curr->key;    // 当前值小于val，可能是前驱，记录下来
            curr = curr->right; // 尝试往右找更大的
        }
        else
        {
            // 当前值 >= val，太大了，往左找
            curr = curr->left;
        }
    }
    return ans;
}

// 求 x 的后继（大于 x，且最小的数）
int getSuccessor(TreeNode *root, int val)
{
    int ans = INT_MAX; // 初始化为最大值，表示未找到
    TreeNode *curr = root;
    while (curr != nullptr)
    {
        if (curr->key > val)
        {
            ans = curr->key;   // 当前值大于val，可能是后继，记录下来
            curr = curr->left; // 尝试往左找更小的
        }
        else
        {
            // 当前值 <= val，太小了，往右找
            curr = curr->right;
        }
    }
    return ans;
}

int main()
{
    // freopen("D:\\Download\\error.in", "r", stdin);
    int n;
    cin >> n;
    TreeNode *root = nullptr;
    while (n--)
    {
        int a, b;
        cin >> a >> b;
        if (a == 1)
        {
            root = insert(root, b);
        }
        else if (a == 2)
        {
            root = remove(root, b);
        }
        else if (a == 3)
        {
            cout << queryRank(root, b) << endl;
        }
        else if (a == 4)
        {
            cout << querykth(root, b) << endl;
        }
        else if (a == 5)
        {
            cout << getPredecessor(root, b) << endl;
        }
        else if (a == 6)
        {
            cout << getSuccessor(root, b) << endl;
        }
    }
    return 0;
}