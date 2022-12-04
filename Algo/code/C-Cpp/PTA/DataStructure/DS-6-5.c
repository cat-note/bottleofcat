/*
本题要求实现链式表的操作集的几个函数:
    Position Find( List L, ElementType X );
    List Insert( List L, ElementType X, Position P );
    List Delete( List L, Position P );
各个操作函数的定义为：
    Position Find( List L, ElementType X )：返回线性表中首次出现X的位置。若找不到则返回ERROR；

    List Insert( List L, ElementType X, Position P )：将X插入在位置P指向的结点之前，返回链表的表头。如果参数P指向非法位置，则打印“Wrong Position for Insertion”，返回ERROR；

    List Delete( List L, Position P )：将位置P的元素删除并返回链表的表头。若参数P指向非法位置，则打印“Wrong Position for Deletion”并返回ERROR。
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
typedef int ElementType;
typedef struct LNode *PtrToLNode;
struct LNode
{
    ElementType Data;
    PtrToLNode Next;
};
typedef PtrToLNode Position;
typedef PtrToLNode List;

Position Find(List L, ElementType X);
List Insert(List L, ElementType X, Position P);
List Delete(List L, Position P);

int main()
{
    List L;
    ElementType X;
    Position P, tmp;
    int N;

    L = NULL;
    scanf("%d", &N);
    while (N--)
    {
        scanf("%d", &X);
        L = Insert(L, X, L);
        if (L == ERROR)
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
            L = Delete(L, P);
            printf("%d is found and deleted.\n", X);
            if (L == ERROR)
                printf("Wrong Answer or Empty List.\n");
        }
    }
    L = Insert(L, X, NULL);
    if (L == ERROR)
        printf("Wrong Answer\n");
    else
        printf("%d is inserted as the last element.\n", X);
    P = (Position)malloc(sizeof(struct LNode));
    tmp = Insert(L, X, P);
    if (tmp != ERROR)
        printf("Wrong Answer\n");
    tmp = Delete(L, P);
    if (tmp != ERROR)
        printf("Wrong Answer\n");
    for (P = L; P; P = P->Next)
        printf("%d ", P->Data);
    return 0;
}

// 以下是题解代码

List Insert(List L, ElementType X, Position P)
{
    Position prevNode = NULL; // 记录位置P的上一个节点
    List head = L;            // 储存一下表头
    while (L != NULL || P == NULL)
    { // 空链表情况下P要=L=NULL
        if (L == P)
        { // 找到位置
            // 新建一个节点
            List node = (List)malloc(sizeof(struct LNode));
            node->Data = X; // 存入数据
            node->Next = P;
            if (prevNode)
                prevNode->Next = node; // 接上上一个节点（如果有的话）
            if (head == NULL || head == P)
                head = node; // 如果插入在头部，返回的表头要改变
            return head;
        }
        prevNode = L;
        L = L->Next;
    }
    printf("Wrong Position for Insertion\n");
    return ERROR;
}

Position Find(List L, ElementType X)
{
    if (L != NULL)
    {
        while (L != NULL)
        {
            if (L->Data == X)
                return L;
            L = L->Next;
        }
    }
    return ERROR;
}

List Delete(List L, Position P)
{
    Position prevNode = NULL; // 储存前一个节点
    Position head = L;        // 记录表头
    if (L != NULL)
    {
        while (L != NULL)
        {
            if (L == P)
            {
                if (prevNode)
                {                             // 不是在表头
                    prevNode->Next = L->Next; // 下一个节点接到上一个节点后面
                }
                else
                {                   // 在表头
                    head = L->Next; // 表头指向下一个节点
                }
                free(L);     // 释放节点
                return head; // 返回表头
            }
            prevNode = L;
            L = L->Next;
        }
    }
    printf("Wrong Position for Deletion\n");
    return ERROR;
}

/*
    对链表进行插入/删除操作的时候一定要【考虑表头指针】的情况，可能操作是在表头进行的，这个时候表头指针变量要改变。
    还有就是在链表中间进行插入/删除操作时，不要忘记了【前一个节点】。
        - SomeBottle 2022.12.4
*/