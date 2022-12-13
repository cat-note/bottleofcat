// 这种写法可以正确判断，但内存超限(MLE)
#include <iostream>

using namespace std;

typedef struct TreeNode *Tree;
// 树结点
struct TreeNode
{
    int num;
    Tree left;
    Tree right;
};

Tree ReadTree(int insertNum);             // 读取并创建树
bool CmpTree(int insertNum, Tree target); // 比较是否是同一颗二叉树
void FreeTree(Tree tree);                 // 释放树的内存

int main()
{
    while (true)
    {
        int insertNum, checkNum;
        // 得到插入节点数和待检查序列数
        cin >> insertNum;
        if (insertNum == 0) // 停止输入
            break;
        cin >> checkNum;
        // 创建最初的二叉搜索树
        Tree primalTree = ReadTree(insertNum);
        // 对比二叉搜索树
        for (int i = 0; i < checkNum; i++)
            cout << (CmpTree(insertNum, primalTree) ? "Yes" : "No") << "\n";
        FreeTree(primalTree); // 释放树内存
    }
    return 0;
}

Tree ReadTree(int insertNum)
{
    // 到这里，插入节点数insertNum至少是1
    // 先创建树根
    Tree root = new TreeNode();
    cin >> root->num;
    for (int i = 1; i < insertNum; i++)
    {
        int newNum;
        cin >> newNum; // 待插入结点
        Tree node = new TreeNode();
        node->num = newNum;
        Tree curr = root;
        while (curr != NULL)
        {
            if (newNum > curr->num)
            {
                // 比当前结点大的插在右边
                // 找到了要插入的位置就插入
                if (!curr->right)
                {
                    curr->right = node;
                }
                else
                {
                    curr = curr->right;
                }
            }
            else if (newNum < curr->num)
            {
                // 比当前结点小的插在左边
                if (!curr->left)
                {
                    curr->left = node;
                    break;
                }
                else
                {
                    curr = curr->left;
                }
            }
            else
            {
                // 还有一种情况是树中已经有同值的结点，就什么都不用做
                break;
            }
        }
    }
    return root;
}

bool CmpTree(int insertNum, Tree target)
{
    Tree root = new TreeNode(); // 临时用于比较插入的树
    cin >> root->num;
    for (int i = 1; i < insertNum; i++)
    {
        int newNum;
        cin >> newNum;       // 待插入树的数值
        Tree curr = target;  // 搜索target树的指针
        Tree insCurr = root; // 创建树的指针
        while (curr != NULL)
        {
            // 如果有结点不相等
            if (curr->num != insCurr->num)
                return false;
            Tree node = new TreeNode();
            node->num = newNum;
            if (newNum > curr->num)
            {
                // 这个数比这个结点大，插在右边
                curr = curr->right;
                if (!insCurr->right)
                    insCurr->right = node;
                insCurr = insCurr->right;
            }
            else if (newNum < curr->num)
            {
                // 比这个结点小，插在左边
                curr = curr->left;
                if (!insCurr->left)
                    insCurr->left = node;
                insCurr = insCurr->left;
            }
            else
            {
                break;
            }
        }
    }
    FreeTree(root); // 释放临时树的内存
    return true;
}

void FreeTree(Tree tree)
{
    if (tree->left)
        FreeTree(tree->left);
    if (tree->right)
        FreeTree(tree->right);
    free(tree);
}