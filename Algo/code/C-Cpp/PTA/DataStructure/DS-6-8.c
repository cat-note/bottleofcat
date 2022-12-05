/*
本题要求给定二叉树的高度，需实现的方法如下：
    int GetHeight( BinTree BT );
要求函数返回给定二叉树BT的高度值。

输入样例(先序遍历序列)：
    ABD##FE###CG#H##I##
输出样例：
    4
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/
#include <stdio.h>
#include <stdlib.h>

typedef char ElementType;
typedef struct TNode *Position;
typedef Position BinTree;
struct TNode
{
    ElementType Data;
    BinTree Left;
    BinTree Right;
};

BinTree CreateBinTree();
BinTree TreeGrow(BinTree tree);
int GetHeight(BinTree BT);

int main()
{
    BinTree BT = CreateBinTree();
    printf("%d\n", GetHeight(BT));
    return 0;
}

// 以下为题解代码

int GetHeight(BinTree BT)
{
    if (BT == NULL) // 空树
        return 0;
    int leftHeight = GetHeight(BT->Left);   // 获得左树高度
    int rightHeight = GetHeight(BT->Right); // 获得右树高度
    // 找更高的一棵子树, 将其高度+1返回, 也算一种贪心吧
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// 我顺带写了这个树生成函数, 用于测试

BinTree CreateBinTree()
{
    return TreeGrow(NULL);
}

// 先序遍历序列生成树
BinTree TreeGrow(BinTree tree)
{
    char chr = getchar();
    if (chr == '#')
    {
        return NULL;
    }
    else
    {
        tree = (BinTree)malloc(sizeof(struct TNode));
        tree->Data = chr;
        tree->Left = TreeGrow(tree->Left);
        tree->Right = TreeGrow(tree->Right);
        return tree;
    }
}

/*
    吐槽：题目中创建树的函数他原本写成CreatBinTree了，经典Create掉了e

    本题很适合用递归来解决问题，每次递归都选取左右子树中高度更高的一棵子树，然后将其高度+1返回即可。
    递归深入到某个空节点的时候终止，返回0，然后回溯到上一层。
        - SomeBottle 2022.12.5
*/
