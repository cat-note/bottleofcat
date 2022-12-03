/*
本题要求实现一个函数，求链式表的表长：
int Length( List L );
    L是给定单链表，函数Length要返回链式表的长度。
输入样例：
    1 3 4 5 2 -1
输出样例：
    5
*/
#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
typedef struct LNode *PtrToLNode;
struct LNode
{
    ElementType Data;
    PtrToLNode Next;
};
typedef PtrToLNode List;

List Read();

int Length(List L);

int main()
{
    List L = Read();
    printf("%d\n", Length(L));
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
int Length(List L)
{
    if (L == NULL) // 空链表的情况
        return 0;
    int len = 0; // 记录长度
    while (L != NULL)
    {
        len++;
        L = L->Next;
    }
    return len;
}

/*
    观察输出样例可以发现输入中的“-1”是为了告诉程序输入结束的，不装入链表中。
    这题注意空链表的情况就行。
        - SomeBottle 2022.12.3
*/