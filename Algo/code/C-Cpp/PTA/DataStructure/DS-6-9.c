/*
本题要求给定二叉树的4种遍历，函数如下：
    void InorderTraversal( BinTree BT );
    void PreorderTraversal( BinTree BT );
    void PostorderTraversal( BinTree BT );
    void LevelorderTraversal( BinTree BT );
要求4个函数分别按照访问顺序打印出结点的内容，格式为一个空格跟着一个字符。

输入样例(先序遍历序列)：
    ABD##FE###CG#H##I##
输出样例：
    Inorder: D B E F A G H C I
    Preorder: A B D F E C G H I
    Postorder: D E F B H G I C A
    Levelorder: A B C D F G I E H
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
void InorderTraversal(BinTree BT);
void PreorderTraversal(BinTree BT);
void PostorderTraversal(BinTree BT);
void LevelorderTraversal(BinTree BT);

int main()
{
    BinTree BT = CreateBinTree();
    printf("Inorder:");
    InorderTraversal(BT);
    printf("\n");
    printf("Preorder:");
    PreorderTraversal(BT);
    printf("\n");
    printf("Postorder:");
    PostorderTraversal(BT);
    printf("\n");
    printf("Levelorder:");
    LevelorderTraversal(BT);
    printf("\n");
    return 0;
}

// ----------------------------------------接下来是题解代码

void PreorderTraversal(BinTree BT)
{                   // 先序遍历 根-左-右
    if (BT == NULL) // 空树
        return;
    printf(" %c", BT->Data);      // 先序先输出
    PreorderTraversal(BT->Left);  // 遍历左子树
    PreorderTraversal(BT->Right); // 遍历右子树
}

void InorderTraversal(BinTree BT)
{                   // 中序遍历 左-根-右
    if (BT == NULL) // 空树
        return;
    InorderTraversal(BT->Left);  // 遍历左子树
    printf(" %c", BT->Data);     // 中序在中间输出
    InorderTraversal(BT->Right); // 遍历右子树
}

void PostorderTraversal(BinTree BT)
{                   // 后序遍历 左-右-根
    if (BT == NULL) // 空树
        return;
    PostorderTraversal(BT->Left);  // 遍历左子树
    PostorderTraversal(BT->Right); // 遍历右子树
    printf(" %c", BT->Data);       // 后序在最后输出
}

typedef struct QueueNode MyQueue;
struct QueueNode
{
    BinTree Data;
    MyQueue *Next;
};

// 给新入队元素分配内存的方法
MyQueue *MakeQueueNode(BinTree tree)
{
    MyQueue *node = (MyQueue *)malloc(sizeof(MyQueue)); // 队头指针
    node->Data = tree;                                  // 记录当前树节点
    node->Next = NULL;
    return node;
}

void LevelorderTraversal(BinTree BT)
{                   // 层序遍历，是BFS
    if (BT == NULL) // 空树
        return;
    // 层序遍历需要借助于一个队列
    // 因为根本不知道题目会给多大规模的二叉树，所以这里采用链队列
    MyQueue *qHead = MakeQueueNode(BT); // 队头指针
    MyQueue *qTail = qHead;             // 队尾指针
    while (qHead != NULL)
    {
        BinTree currTree = qHead->Data; // 获得某棵树
        printf(" %c", currTree->Data);  // 输出序列
        if (currTree->Left)
        { // 如果树有左子树
            MyQueue *node = MakeQueueNode(currTree->Left);
            qTail->Next = node; // 接到队尾后面
            qTail = node;       // 更新队尾
        }
        if (currTree->Right)
        {
            MyQueue *node = MakeQueueNode(currTree->Right);
            qTail->Next = node; // 接到队尾后面
            qTail = node;       // 更新队尾
        }
        MyQueue *temp = qHead;
        qHead = qHead->Next; // 移动队头指针，代表队头节点出队
        free(temp);          // 释放出队节点的内存
    }
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
    二叉树的前中后序都是深度优先搜索遍历(DFS), 这三种遍历的区别主要在于【什么时候输出】，主要改变递归体中相应语句的顺序即可。

    而二叉树的层序遍历就是广度优先搜索遍历了(BFS)，因此需要用【队列】来进行辅助。本题中没有说到数据规模，因此采用【链队列】。
        - 先将树根节点加入队中
        - 循环：
            - 让队头节点出队，打印出这个节点的名字
            - 将这个节点的左子树和右子树加到【队尾】
            - 移动队头到下一个节点
            (循环终止条件: 队头指向NULL，即队列为空)

        - SomeBottle 2022.12.5
*/