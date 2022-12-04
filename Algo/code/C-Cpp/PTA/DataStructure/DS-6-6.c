/*
本题要求实现带头结点的链式表操作集的函数：
    List MakeEmpty();
    Position Find( List L, ElementType X );
    bool Insert( List L, ElementType X, Position P );
    bool Delete( List L, Position P );
各个操作函数的定义为：
    List MakeEmpty()：创建并返回一个空的线性表；

    Position Find( List L, ElementType X )：返回线性表中X的位置。若找不到则返回ERROR；

    bool Insert( List L, ElementType X, Position P )：将X插入在位置P指向的结点之前，返回true。如果参数P指向非法位置，则打印“Wrong Position for Insertion”，返回false；

    bool Delete( List L, Position P )：将位置P的元素删除并返回true。若参数P指向非法位置，则打印“Wrong Position for Deletion”并返回false。
输入样例：
    6
    12 2 4 87 10 2
    4
    2 12 87 5
输出样例：
    2 is found and deleted.
    12 is found and deleted.
    87 is found and deleted.
    Finding Error: 5 is not in.
    5 is inserted as the last element.
    Wrong Position for Insertion
    Wrong Position for Deletion
    10 4 2 5
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/
#include <stdio.h>
#include <stdlib.h>

#define ERROR NULL
typedef enum
{
    false,
    true
} bool;
typedef int ElementType;
typedef struct LNode *PtrToLNode;
struct LNode
{
    ElementType Data;
    PtrToLNode Next;
};
typedef PtrToLNode Position;
typedef PtrToLNode List;

List MakeEmpty();
Position Find(List L, ElementType X);
bool Insert(List L, ElementType X, Position P);
bool Delete(List L, Position P);

int main()
{
    List L;
    ElementType X;
    Position P;
    int N;
    bool flag;

    L = MakeEmpty();
    scanf("%d", &N);
    while (N--)
    {
        scanf("%d", &X);
        flag = Insert(L, X, L->Next);
        if (flag == false)
            printf("Wrong Answer\n");
    }
    scanf("%d", &N);
    while (N--)
    {
        scanf("%d", &X);
        P = Find(L, X);
        if (P == ERROR)
            printf("Finding Error: %d is not in.\n", X);
        else
        {
            flag = Delete(L, P);
            printf("%d is found and deleted.\n", X);
            if (flag == false)
                printf("Wrong Answer.\n");
        }
    }
    flag = Insert(L, X, NULL);
    if (flag == false)
        printf("Wrong Answer\n");
    else
        printf("%d is inserted as the last element.\n", X);
    P = (Position)malloc(sizeof(struct LNode));
    flag = Insert(L, X, P);
    if (flag == true)
        printf("Wrong Answer\n");
    flag = Delete(L, P);
    if (flag == true)
        printf("Wrong Answer\n");
    for (P = L->Next; P; P = P->Next)
        printf("%d ", P->Data);
    return 0;
}

// 接下来是题解代码

List MakeEmpty()
{ // 生成空链表（只有个头）
    List head = (List)malloc(sizeof(struct LNode));
    head->Data = 0; // 这个可以用来存链表长度, 不过本题没用到
    head->Next = NULL;
    return head;
}

Position Find(List L, ElementType X)
{
    if (L->Next != NULL)
    { // 链表不为空
        L = L->Next;
        while (L != NULL)
        {
            if (L->Data == X)
                return L;
            L = L->Next;
        }
    }
    return ERROR;
}

// 可以发现有头节点的链表，其优点是涉及到第一个节点的增删时要便利一些
bool Insert(List L, ElementType X, Position P)
{
    Position prevNode = L; // 记录上一个节点
    L = L->Next;
    while (L != NULL || P == NULL)
    {
        if (L == P)
        {
            List node = (List)malloc(sizeof(struct LNode));
            node->Data = X;        // 赋予数据
            node->Next = P;        // 接在这个节点的前面
            prevNode->Next = node; // 接在前一个节点的后面
            return true;
        }
        prevNode = L;
        L = L->Next;
    }
    printf("Wrong Position for Insertion\n");
    return false;
}

bool Delete(List L, Position P)
{
    Position prevNode = L; // 记录前一个节点
    L = L->Next;
    while (L != NULL)
    {
        if (P == L)
        {
            prevNode->Next = P->Next; // 把下一个节点接到上一个节点
            free(P);                  // 释放内存
            return true;
        }
        prevNode = L;
        L = L->Next;
    }
    printf("Wrong Position for Deletion\n");
    return false;
}

/*
    比较类似上一题(6-5)，但不同的是这个链表采用了头节点。
    可以发现，在采用头节点后，进行增删操作不需要忧虑头指针的问题了。
    平常进行编程的时候采用带头节点的链表，说不定真能省不少事。

        - SomeBottle 2022.12.4
*/