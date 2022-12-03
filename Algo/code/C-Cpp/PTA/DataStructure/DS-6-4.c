/*
本题要求实现一个函数，找到并返回链式表的第K个元素:
    ElementType FindKth( List L, int K );
L是给定单链表，函数FindKth要返回链式表的第K个元素。如果该元素不存在，则返回ERROR。

输入样例：
    1 3 4 5 2 -1
    6
    3 6 1 5 4 2
输出样例：
    4 NA 1 2 5 3
*/
#include <stdio.h>
#include <stdlib.h>

#define ERROR -1
typedef int ElementType;
typedef struct LNode *PtrToLNode;
struct LNode
{
    ElementType Data;
    PtrToLNode Next;
};
typedef PtrToLNode List;

List Read(); /* 细节在此不表 */

ElementType FindKth(List L, int K);

int main()
{
    int N, K;
    ElementType X;
    List L = Read();
    scanf("%d", &N);
    while (N--)
    {
        scanf("%d", &K);
        X = FindKth(L, K);
        if (X != ERROR)
            printf("%d ", X);
        else
            printf("NA ");
    }
    return 0;
}

List Read() // 这里我把题目声明的Read函数顺便给写了
{
    int data;
    scanf(" %d", &data); // 先读入第一个数据，如果是-1代表这是0个元素的链表
    if (data == -1)
        return NULL;
    List head = (List)malloc(sizeof(struct LNode));
    List tail = head;
    head->Data = data;
    head->Next = NULL;
    while (1)
    {
        scanf(" %d", &data);
        if (data == -1)
            break;
        List node = (List)malloc(sizeof(struct LNode));
        node->Data = data;
        node->Next = NULL;
        tail->Next = node;
        tail = node;
    }
    return head;
}

// 下面是题解代码

ElementType FindKth(List L, int K)
{
    int index = 1;
    while (L != NULL)
    {
        if (index++ == K)
            return L->Data;
        L = L->Next;
    }
    return ERROR;
}

/*
    说是查找，其实就是访问特定位置上的元素。只不过链表只能顺序访问，需要循环来帮忙。
        - SomeBottle 2022.12.3
*/