/*
给定一个插入序列就可以唯一确定一棵二叉搜索树。然而，一棵给定的二叉搜索树却可以由多种不同的插入序列得到。例如分别按照序列{2, 1, 3}和{2, 3, 1}插入初始为空的二叉搜索树，都得到一样的结果。于是对于输入的各种插入序列，你需要判断它们是否能生成一样的二叉搜索树。

输入格式:
    输入包含若干组测试数据。每组数据的第1行给出两个正整数N (≤10)和L，分别是每个序列插入元素的个数和需要检查的序列个数。第2行给出N个以空格分隔的正整数，作为初始插入序列。随后L行，每行给出N个插入的元素，属于L个需要检查的序列。

* 简单起见，我们保证每个插入序列都是1到N的一个排列。当读到N为0时，标志输入结束，这组数据不要处理。

输出格式:
    对每一组需要检查的序列，如果其生成的二叉搜索树跟对应的初始序列生成的一样，输出“Yes”，否则输出“No”。

输入样例:
    4 2
    3 1 4 2
    3 4 1 2
    3 2 4 1
    2 1
    2 1
    1 2
    0
输出样例:
    Yes
    No
    No
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/
#include <iostream>

using namespace std;

typedef struct TreeNode *Tree;
// 树结点
struct TreeNode
{
    int num;
    bool cmp; // 对比位，为1代表对比时有结点插入到这个位置
    Tree left;
    Tree right;
};

Tree ReadTree(int insertNum);             // 读取并创建树
bool CmpTree(int insertNum, Tree target); // 比较是否是同一颗二叉树
void ResetCmp(Tree tree);                 // 把树结点的cmp标记全归0
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
    bool result = true;
    for (int i = 0; i < insertNum; i++)
    {
        int newNum;
        cin >> newNum; // 读入待插入数
        // 一旦标记两棵树不相同，就不用继续下面的运算了
        // 但是外层for循环要把STDIN中残余的内容读走
        if (!result)
            continue;
        Tree curr = target;
        Tree prevNode = NULL;
        while (curr != NULL)
        { // 在搜索树种查找这个待插入数
            if (prevNode && !prevNode->cmp)
            {
                result = false; // 标记两棵树并不相同
                break;
            }
            prevNode = curr;
            if (newNum == curr->num)
            {
                curr->cmp = true; // 顺利在此处插入结点
                break;
            }
            if (newNum > curr->num)
            {
                curr = curr->right;
            }
            else if (newNum < curr->num)
            {
                curr = curr->left;
            }
        }
    }
    ResetCmp(target); // 重置标记位cmp
    return result;
}

void ResetCmp(Tree tree)
{
    if (tree->left)
        ResetCmp(tree->left);
    if (tree->right)
        ResetCmp(tree->right);
    tree->cmp = false;
}

void FreeTree(Tree tree)
{
    if (tree->left)
        FreeTree(tree->left);
    if (tree->right)
        FreeTree(tree->right);
    free(tree);
}

/*
    * BST=二叉搜索树

    看到这题时我最开始想到的是，每次对比时都 一边插入结点创建一棵新的BST，一边比较【插入的结点的值】是否能对应上【原BST的对应位置上的结点值】。

    然鹅...如你所见，当我总算把这玩意写出来时，被内存超限(MLE)狠狠打了脸...
    (即使每次对比后释放临时BST的内存也不行)

    --------

    想了一会儿，我发现根本没必要再创建别的BST，在【原BST上做标记】就行了！

        比如原BST1树插入的结点是这些: 3 1 4 2，大致形状如下:
                        3
                    1       4
                        2
        现在我要创建一颗新的BST2树，其插入的结点是: 3 4 1 2，对比BST2和BST1的步骤如下：

            a. 给BST1每个结点附上标记:
                        3[0]
                    1[0]    4[0]
                        2[0]
            b. 开始读取插入BST2的结点，先读到3，于是【在BST1中搜索3】
                因为BST1根节点就是3，因此3可以直接插入在BST2的根结点这里
                * 标记BST1的根节点标记位为1:
                        3[1]
                    1[0]    4[0]
                        2[0]
            c. 接下来读入4，在【BST1中搜索4】
                BST1->3-右侧->4
                找到4后检查【父结点标记位】，发现父节点(3)标记位为1
                于是将4可以插入在BST2的这个位置。
                * 标记BST1中的4的标记位为1:
                        3[1]
                    1[0]    4[1]
                        2[0]

            !!!!!!!!!!!!!!!!!!!!!!!!!!!!

            接下来也是不停搜索待插入的结点，只要:
                💡
                1. 【能在原BST1中找到这个结点】
                    且
                2. 【这个结点在原BST1中的父结点】的标记位为【1】
                    (说明父节点已经在BST2中插入)
            就能保证直到这个结点插入，两棵树还没有不相同的迹象。

            !!!!!!!!!!!!!!!!!!!!!!!!!!!!

            最终BST1的标记位会如下:
                        3[1]
                    1[1]    4[1]
                        2[1]
                此时能将3 4 1 2这四个结点都插入BST2且保证BST2和BST1完全一致。

    --------

    这种解题方法是依赖于BST的创建特性的:
        每插入一个结点，都是【从BST根节点开始】比较，从而找到插入位置的。

    - SomeBottle 2022.12.13
*/