/*
本题要求实现给定二叉搜索树的5种常用操作，待实现方法如下：
    BinTree Insert( BinTree BST, ElementType X );
    BinTree Delete( BinTree BST, ElementType X );
    Position Find( BinTree BST, ElementType X );
    Position FindMin( BinTree BST );
    Position FindMax( BinTree BST );
函数Insert将X插入二叉搜索树BST并返回结果树的根结点指针；
函数Delete将X从二叉搜索树BST中删除，并返回结果树的根结点指针；如果X不在树中，则打印一行Not Found并返回原树的根结点指针；
函数Find在二叉搜索树BST中找到X，返回该结点的指针；如果找不到则返回空指针；
函数FindMin返回二叉搜索树BST中最小元结点的指针；
函数FindMax返回二叉搜索树BST中最大元结点的指针。

输入样例：
    10
    5 8 6 2 4 1 0 10 9 7
    5
    6 3 10 0 5
    5
    5 7 0 10 3
输出样例：
    Preorder: 5 2 1 0 4 8 6 7 10 9
    6 is found
    3 is not found
    10 is found
    10 is the largest key
    0 is found
    0 is the smallest key
    5 is found
    Not Found
    Inorder: 1 2 4 6 8 9
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/
#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
typedef struct TNode *Position;
typedef Position BinTree;
struct TNode
{
    ElementType Data;
    BinTree Left;
    BinTree Right;
};

void PreorderTraversal(BinTree BT); /* 先序遍历，由裁判实现，细节不表 */
void InorderTraversal(BinTree BT);  /* 中序遍历，由裁判实现，细节不表 */

BinTree Insert(BinTree BST, ElementType X);
BinTree Delete(BinTree BST, ElementType X);
Position Find(BinTree BST, ElementType X);
Position FindMin(BinTree BST);
Position FindMax(BinTree BST);

int main()
{
    BinTree BST, MinP, MaxP, Tmp;
    ElementType X;
    int N, i;

    BST = NULL;
    scanf("%d", &N);
    for (i = 0; i < N; i++)
    {
        scanf("%d", &X);
        BST = Insert(BST, X);
    }
    printf("Preorder:");
    PreorderTraversal(BST);
    printf("\n");
    MinP = FindMin(BST);
    MaxP = FindMax(BST);
    scanf("%d", &N);
    for (i = 0; i < N; i++)
    {
        scanf("%d", &X);
        Tmp = Find(BST, X);
        if (Tmp == NULL)
            printf("%d is not found\n", X);
        else
        {
            printf("%d is found\n", Tmp->Data);
            if (Tmp == MinP)
                printf("%d is the smallest key\n", Tmp->Data);
            if (Tmp == MaxP)
                printf("%d is the largest key\n", Tmp->Data);
        }
    }
    scanf("%d", &N);
    for (i = 0; i < N; i++)
    {
        scanf("%d", &X);
        BST = Delete(BST, X);
    }
    printf("Inorder:");
    InorderTraversal(BST);
    printf("\n");

    return 0;
}

// ---------以下是我的题解

BinTree Insert(BinTree BST, ElementType X)
{
    if (!BST)
    { // 是个空树，说明找到了插入位置
        // 创建节点
        BinTree treeNode = (BinTree)malloc(sizeof(struct TNode));
        treeNode->Data = X; // 节点内容
        treeNode->Left = NULL;
        treeNode->Right = NULL;
        BST = treeNode;
    }
    else if (X < BST->Data)
    {                                     // X值小于当前节点值，往左子树放
        BST->Left = Insert(BST->Left, X); // 往左子树深入，寻找插入位置
    }
    else if (X > BST->Data)
    {                                       // X值大于当前节点值，往右子树放
        BST->Right = Insert(BST->Right, X); // 往右子树深入
    }
    // 如果有X==BST->Data, 说明二叉搜索树中已经有X这个元素了，不做任何操作
    return BST;
}
/*
 这一题最难写的就是删除了，二叉搜索树的删除有几种情况：
 1. 删除节点没有子树，直接删掉
 2. 删除节点没有左子树，用右子树接替
 3. 删除节点没有右子树，用左子树接替
 4. 删除节点有左右子树:
    a. 用左子树中序遍历的最后一个节点接替(左子树中的最大值)
    b. 用右子树中序遍历的首个节点接替(右子树的最小值)
 分析本题输入输出样例，可以发现采用的是a方案
*/
BinTree Delete(BinTree BST, ElementType X)
{
    Position prevNode = NULL; // 记录前驱节点
    int prevDirection = 0;    // 记录当前节点是前驱的左子(0)还是右子(1)
    Position currNode = BST;  // 当前节点
    while (currNode != NULL)
    {
        if (currNode->Data == X)
        { // 找到了待删除节点
            if (currNode->Left && currNode->Right)
            {
                // 待删除节点有左右子树
                // 用左子树的最大值(中序遍历最后一个值)接替
                Position leftPrevNode = NULL; // 记录左子树当前节点的前驱
                Position leftMaxNode = currNode->Left;
                while (leftMaxNode->Right != NULL)
                {
                    leftPrevNode = leftMaxNode;       // 更新前驱节点的记录
                    leftMaxNode = leftMaxNode->Right; // 不断往右子树深入, 找到子树最大值
                }
                // 修改当前节点的值为左子树最大值
                currNode->Data = leftMaxNode->Data;
                if (!leftPrevNode)
                    // leftPrevNode=NULL说明左子树中没有右子树
                    // 此时leftMaxNode是左子树的根节点
                    // 当前节点的左子树要改成左子的左子树
                    currNode->Left = leftMaxNode->Left;
                else
                    // 有右子树，此时要移除leftMaxNode这个节点
                    // 将前驱节点中的Right指针置空
                    leftPrevNode->Right = NULL;
                free(leftMaxNode); // 释放内存
            }
            else if (currNode->Left)
            {
                // 待删除节点只有左子树
                // 用左子树接替
                // 这里并不真的删掉这个节点，而是修改节点值为左子树根节点值
                // 实际删掉的是左子树的根节点
                Position leftNode = currNode->Left;
                currNode->Data = leftNode->Data; // 改变当前节点值为左子树根节点
                currNode->Left = leftNode->Left; // 左子树其他节点接上当前节点
                currNode->Right = leftNode->Right;
                free(leftNode); // 释放原根节点的内存
            }
            else if (currNode->Right)
            {
                // 待删除节点只有右子树
                // 用右子树接替
                Position rightNode = currNode->Right;
                currNode->Data = rightNode->Data;
                currNode->Left = rightNode->Left;
                currNode->Right = rightNode->Right;
                free(rightNode);
            }
            else
            {
                // 待删除节点没有子树
                // 直接删掉即可
                free(currNode);
                // 改变当前节点前驱节点的Left或Right指向为空
                if (!prevNode) // 没有前驱节点，说明这是整棵树中最后一个节点
                    BST = NULL;
                else if (prevDirection) // prevDirection=1代表当前节点是前驱节点的右子
                    prevNode->Right = NULL;
                else
                    prevNode->Left = NULL;
            }
            return BST;
        }
        prevNode = currNode; // 更新记录的前驱节点
        if (X < currNode->Data)
        { // X小于当前节点值，往左子树找
            currNode = currNode->Left;
            prevDirection = 0;
        }
        else if (X > currNode->Data)
        { // X大于当前节点值，往右子树找
            currNode = currNode->Right;
            prevDirection = 1;
        }
    }
    printf("Not Found\n");
    return BST;
}

Position Find(BinTree BST, ElementType X)
{
    while (BST != NULL)
    {
        if (BST->Data == X)
            return BST;
        else if (X < BST->Data)
            BST = BST->Left; // 往左子树找
        else if (X > BST->Data)
            BST = BST->Right; // 往右子树找
    }
    return NULL;
}

Position FindMin(BinTree BST)
{
    if (!BST) // 空树
        return NULL;
    // 左侧最深处就是最小节点
    while (BST->Left != NULL)
    {
        BST = BST->Left;
    }
    return BST;
}

Position FindMax(BinTree BST)
{
    if (!BST) // 空树
        return NULL;
    // 右侧最深处就是最大节点
    while (BST->Right != NULL)
    {
        BST = BST->Right;
    }
    return BST;
}

// --------------之前写的先序遍历与中序遍历

void PreorderTraversal(BinTree BT)
{                   // 先序遍历 根-左-右
    if (BT == NULL) // 空树
        return;
    printf(" %d", BT->Data);      // 先序先输出
    PreorderTraversal(BT->Left);  // 遍历左子树
    PreorderTraversal(BT->Right); // 遍历右子树
}

void InorderTraversal(BinTree BT)
{                   // 中序遍历 左-根-右
    if (BT == NULL) // 空树
        return;
    InorderTraversal(BT->Left);  // 遍历左子树
    printf(" %d", BT->Data);     // 中序在中间输出
    InorderTraversal(BT->Right); // 遍历右子树
}

/*
    这题最难写的就是删除二叉搜索树中的【节点删除】操作了，值得反复回味。
    具体注意点我在上面的注释给写了
    这里的二叉排序树中的每一颗树的左子树都存放着【比根节点小】的值，而右子树都存放着【比根节点大】的值。
        - SomeBottle 2022.12.6
*/