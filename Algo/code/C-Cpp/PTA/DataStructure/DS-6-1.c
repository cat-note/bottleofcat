/*
本题要求实现一个函数，将给定的单链表逆转。
List Reverse( List L );

输入样例
    5
    1 3 4 5 2
输出样例
    1
    2 5 4 3 1
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
typedef struct Node *PtrToNode;
struct Node
{
    ElementType Data;
    PtrToNode Next;
};
typedef PtrToNode List;

List Read();        /* 细节在此不表 */
void Print(List L); /* 细节在此不表 */

List Reverse(List L);

int main()
{
    List L1, L2;
    L1 = Read();
    L2 = Reverse(L1);
    Print(L1);
    Print(L2);
    return 0;
}

List Read()
{
    int listLen; // 链表长度
    scanf("%d", &listLen);
    // 空链表
    if (listLen == 0)
        return NULL;
    List head = (List)malloc(sizeof(struct Node));
    head->Next = NULL;
    List tail = head;
    for (int i = 0; i < listLen; i++)
    {
        scanf("%d", &tail->Data);
        if (i != listLen - 1)
        {
            List node = (List)malloc(sizeof(struct Node));
            node->Next = NULL;
            tail->Next = node;
            tail = node;
        }
    }
    return head;
}

void Print(List L)
{
    // 经过测试发现PTA那边空链表会返回Empty
    if (!L)
    {
        printf("Empty\n");
        return;
    }
    // 非空链表
    while (L)
    {
        printf("%d ", L->Data);
        L = L->Next;
    }
    printf("\n");
}

// 题解代码在这里哦

List Reverse(List L)
{
    // 如果是0节点或只有1个节点，直接返回（易错点！）
    if (L == NULL || L->Next == NULL)
        return L;
    List reversed = L;      // 逆转链表
    List nextTmp = L->Next; // 临时储存下一个节点的地址
    reversed->Next = NULL;  // 最先插入逆转链表的节点是最后一个节点，Next置为NULL
    do
    {
        L = nextTmp;
        nextTmp = L->Next;  // 更新下一个节点的地址
        L->Next = reversed; // 接到逆转链表头部
        reversed = L;       // 逆转链表头部地址改为此节点
    } while (nextTmp != NULL);
    return reversed;
}

/*
    这题需要注意的是，如果链表为空或只有一个节点，直接返回即可，无需处理。（这是一个边界情况）

        - SomeBottle 2022.12.1
*/