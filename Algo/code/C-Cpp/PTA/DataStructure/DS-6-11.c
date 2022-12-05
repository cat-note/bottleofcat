/*
本题要求按照先序遍历的顺序输出给定二叉树的叶结点，实现以下方法：
    void PreorderPrintLeaves( BinTree BT );
函数PreorderPrintLeaves应按照先序遍历的顺序输出给定二叉树BT的叶结点，格式为一个空格跟着一个字符。
输入样例(先序遍历序列)：
    ABD##FE###CG#H##I##
输出样例：
    Leaf nodes are: D E H I
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

BinTree CreateBinTree(); /* 实现细节忽略 */
BinTree TreeGrow(BinTree tree);
void PreorderPrintLeaves(BinTree BT);

int main()
{
    BinTree BT = CreateBinTree();
    printf("Leaf nodes are:");
    PreorderPrintLeaves(BT);
    printf("\n");

    return 0;
}

// 以下是题解代码

void PreorderPrintLeaves(BinTree BT)
{
    if (!BT) // 空树
        return;
    // 没有左右子树的节点就是叶节点
    if (!BT->Left && !BT->Right)
        printf(" %c", BT->Data);
    PreorderPrintLeaves(BT->Left);
    PreorderPrintLeaves(BT->Right);
}

// ---------------------------------------------我顺带写了这个树生成函数, 用于测试

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
    这题仍然是考察二叉树的先序遍历，只不过只输出叶节点。
    没有左右子树的节点就是叶节点了。
        - SomeBottle 2022.12.5
*/