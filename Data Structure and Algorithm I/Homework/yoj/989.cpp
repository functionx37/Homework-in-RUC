#include <bits/stdc++.h>
using namespace std;

struct TreeNode // 二叉搜索树节点
{
    int key;
    TreeNode *left;
    TreeNode *right;
    // 维护其他信息，如高度，节点数量等
    int size;  // 当前节点为根的子树大小
    int count; // 当前节点的重复数量

    TreeNode(int value)
        : key(value), size(1), count(1), left(nullptr), right(nullptr) {}
};

void inorderTraversal(TreeNode *root) // 中序遍历
{
    if (root == nullptr)
        return;
    std::cout << root->key << " "; // 二叉搜索树的中序遍历权值的序列为非降的序列
    inorderTraversal(root->left);
    inorderTraversal(root->right);
}

TreeNode *findMin(TreeNode *root) // 查找值最小节点
{
    while (root->left != nullptr)
        root = root->left;
    return root; // 最小值为二叉搜索树左链的顶点
}

TreeNode *findMax(TreeNode *root) // 查找值最大节点
{
    while (root->right != nullptr)
        root = root->right;
    return root; // 最大值为二叉搜索树右链的顶点
}

TreeNode *search(TreeNode *root, int target) // 按值搜索
{
    if (root == nullptr)
        return nullptr;
    if (root->key == target)
        return root;
    else if (target < root->key)
        return search(root->left, target);
    else
        return search(root->right, target);
}

TreeNode *insert(TreeNode *root, int value) // 插入节点
{
    if (root == nullptr) // 若 root 为空，直接返回一个值为 value 的新节点。
        return new TreeNode(value);
    if (value < root->key) // 在左子树中插入
        root->left = insert(root->left, value);
    else if (value > root->key) // 在右子树插入
        root->right = insert(root->right, value);
    else
        root->count++;                                                                                      // 节点值相等，增加重复数量
    root->size = root->count + (root->left ? root->left->size : 0) + (root->right ? root->right->size : 0); // 更新节点的子树大小
    return root;
}

TreeNode *remove(TreeNode *root, int value) // 删除元素
{
    // 返回值为删除 value 后的新 root，使用时用 root = remove(root, value);
    if (root == nullptr)
        return root;
    if (value < root->key)
        root->left = remove(root->left, value);
    else if (value > root->key)
        root->right = remove(root->right, value);
    else
    {
        if (root->count > 1)
            root->count--; // 节点重复数量大于1，减少重复数量
        else
        {
            // 链节点返回子节点，叶节点直接删除
            if (root->left == nullptr)
            {
                TreeNode *temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr)
            {
                TreeNode *temp = root->left;
                delete root;
                return temp;
            }
            // 左子树最右的节点或右子树最左的节点代替
            else
            {
                TreeNode *successor = findMin(root->right); // 此处以右子树的最小值为例
                root->key = successor->key;
                root->count = successor->count; // 更新重复数量
                successor->count = 1;           // 便于后续成功删除节点
                root->right = remove(root->right, successor->key);
            }
        }
    }
    // 维护size，不写成 --root->size 是因为value可能不在树中，导致未发生删除
    root->size = root->count + (root->left ? root->left->size : 0) + (root->right ? root->right->size : 0);
    return root;
}

int main()
{
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
        else
        {
            root = remove(root, b);
        }
    }
    inorderTraversal(root);
    return 0;
}