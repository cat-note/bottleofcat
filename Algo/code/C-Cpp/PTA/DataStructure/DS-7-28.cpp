#include <iostream>
#include <vector>
#include <stack>

using namespace std;

typedef vector<int> Seq;

typedef struct TreeNode
{
    int data;
    TreeNode *left;
    TreeNode *right;
} *Tree;

void insertNode(Tree &bst, int data);                             // 插入节点
bool samePreorder(Tree bst, const Seq &preorder, bool &mirrored); // 判断是否满足先序序列
void printPostorder(Tree bst, bool mirrored);                     // 打印后序序列

int main()
{
    Tree bst = NULL; // 二叉搜索树
    int nodeNum;     // 二叉搜索树节点数
    cin >> nodeNum;
    Seq preorder(nodeNum); // 先序序列
    for (int i = 0; i < nodeNum; i++)
    {
        cin >> preorder[i];           // 读入先序序列
        insertNode(bst, preorder[i]); // 插入树中
    }
    bool mirrored = false; // 是否是镜像
    if (samePreorder(bst, preorder, mirrored))
    {
        cout << "YES\n";
        printPostorder(bst, mirrored);
    }
    else
    {
        cout << "NO";
    }
    return 0;
}

// 插入节点
void insertNode(Tree &bst, int data)
{
    Tree node = new TreeNode();
    node->left = NULL;
    node->right = NULL;
    node->data = data;
    if (!bst)
    { // 空树的情况
        bst = node;
        return;
    }
    Tree curr = bst;
    while (curr)
    {
        if (data >= curr->data)
        {
            if (!curr->right)
            {
                curr->right = node;
                break;
            }
            curr = curr->right;
        }
        else
        {
            if (!curr->left)
            {
                curr->left = node;
                break;
            }
            curr = curr->left;
        }
    }
}

// 判断是否满足先序序列
bool samePreorder(Tree bst, const Seq &preorder, bool &mirrored)
{
    int index = 0;         // 用于比对先序序列的下标
    stack<Tree> traversal; // 借助栈来先序遍历树
    traversal.push(bst);
    while (!traversal.empty())
    {
        Tree curr = traversal.top();
        traversal.pop();
        if (!curr) // 不处理空结点
            continue;
        // 逐一比对当前遍历的先序序列和题目给出的先序序列
        if (preorder[index++] != curr->data)
        {
            // 出现了序列不匹配，就不再继续
            if (!mirrored) // 尝试对比镜像
                return samePreorder(bst, preorder, mirrored = true);
            else // 镜像了耶不匹配，说明先序序列有误
                return false;
        }
        if (!mirrored)
        {
            // 当前不是镜像，根->左->右
            traversal.push(curr->right);
            traversal.push(curr->left);
        }
        else
        {
            // 如果是镜像就根->右->左
            traversal.push(curr->left);
            traversal.push(curr->right);
        }
    }
    return true;
}

// 打印后序遍历序列
void printPostorder(Tree bst, bool mirrored)
{
    if (!bst)
        return;
    stack<Tree> traversal;
    stack<int> output; // 输出栈
    traversal.push(bst);
    while (!traversal.empty())
    {
        Tree curr = traversal.top();
        traversal.pop();
        if (!curr)
            continue;
        output.push(curr->data); // 压入到输出栈
        if (!mirrored)
        {
            // 非镜像: 左-右-根, 这里因为要压入输出栈，还要反向一次，所以是这样写
            traversal.push(curr->left);
            traversal.push(curr->right);
        }
        else
        {
            // 镜像: 右-左-根
            traversal.push(curr->right);
            traversal.push(curr->left);
        }
    }
    // 输出栈中序列
    bool putSpace = false; // 是否在前面填充空格，防止多余空格输出
    while (!output.empty())
    {
        if (putSpace)
            cout << " ";
        else
            putSpace = true;
        cout << output.top();
        output.pop();
    }
}

/*
    这题的思路比较简单，不过有一个要注意的点:

        - 💡 题目中提到，这里的BST可以有【重复键值】。
            “而其右子树包含大于或【等于】该结点的键值”

    主要思路如下:

        1. 首先，直接将题目给出的先序序列依次输入就可以【构造出二叉搜索树】。

        2. 利用堆栈对二叉搜索树进行【先序遍历】:

            a. 先将根节点压入栈
            b. 从栈中弹出节点，比对该节点【在先序序列中的位置】是否【和题目提供的先序序列中的一致】。
            c. 将其右、左节点依次压入栈

            其中b,c是迭代过程，直到【b中发现先序序列不匹配】或者【栈空】为止。

                - 如果是发现和先序序列不匹配，就进入第【3】步

                - 如果是栈空，说明先序序列匹配，进入第【5】步

        3. 如果第2步先序序列不匹配，就对搜索二叉树的【镜像】进行【先序遍历】:

            不同的地方就是第【c】步中是将其【左、右】节点依次压入栈 (实现镜像BST)

            然后如果这个时候还出现了先序序列不匹配，说明【题目给的先序序列无法构造符合要求的BST】，进入第【4】步

        4. 输出NO

        5. 输出YES，接着利用堆栈对搜索二叉树进行【后序遍历】

            主体上和先序遍历很像，不同的地方在于:

                1. 除了遍历栈外新增了一个输出栈，从遍历栈【弹出】的元素会被【压入输出栈】，而不是输出。

                2. 如果是【非镜像】BST的节点，就将其【左、右】节点依次压入遍历栈 (因为输出栈还要把顺序倒过来一次)

                3. 如果是【镜像】BST的节点，就将其【右、左】节点依次压入遍历栈

            最后弹出【输出栈元素】并输出即可。

    ------------------------------------
        - SomeBottle 2023.1.21
*/
/*
7-28 搜索树判断

*/