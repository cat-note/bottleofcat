#include <cstdio>
#include <cstring>

using namespace std;

// 定义单链表数据结构
typedef char ElementType;

typedef struct Node
{
    ElementType data;
    struct Node *next;
    Node(ElementType data)
    {
        this->data = data;
        next = NULL;
    }
} *List, LNode;

int main()
{
    // input:
    ElementType in1[] = {'l', 'o', 'a', 'd'}; // 单链表1中的值
    ElementType in2[] = {'b', 'e'};           // 单链表2中的值
    ElementType shared[] = {'i', 'n', 'g'};
    int inSize1 = sizeof(in1) / sizeof(in1[0]);
    int inSize2 = sizeof(in2) / sizeof(in2[0]);
    int inSize3 = sizeof(shared) / sizeof(shared[0]);
    // initialization:
    List list1 = new LNode(0); // head node
    List list2 = new LNode(0); // head node
    List sharedList = new LNode(0);
    // init shared part
    List tail = sharedList;
    for (int i = 0; i < inSize3; i++)
    {
        List node = new LNode(shared[i]); // 插入结点
        tail->next = node;
        tail = node;
    }
    // init list1
    tail = list1;
    for (int i = 0; i < inSize1; i++)
    {
        List node = new LNode(in1[i]); // 插入结点
        tail->next = node;
        tail = node;
    }
    tail->next = sharedList->next;
    // init list2
    tail = list2;
    for (int i = 0; i < inSize2; i++)
    {
        List node = new LNode(in2[i]); // 插入结点
        tail->next = node;
        tail = node;
    }
    tail->next = sharedList->next;
    // algorithm:
    List ptr1 = list1, ptr2;
    List res = NULL; // 存储结果
    // 直接上暴力写法！两层循环，扫就完事了
    while (ptr1 && !res)
    {
        ptr2 = list2;
        while (ptr2 && !res)
        {
            if (ptr1 == ptr2)
                res = ptr1;
            ptr2 = ptr2->next;
        }
        ptr1 = ptr1->next;
    }
    // output:
    if (!res)
        printf("No matched results.\n");
    else
    {
        printf("Addr:%p, Shared part:\n\t", res);
        while (res)
        {
            printf("-> %c ", res->data);
            res = res->next;
        }
        printf("\n");
    }
}

/*
    这题第一眼没想到很好的优解，考试的时候可没时间慢慢想，直接上两层循环暴力！
    时间复杂度O(n^2)，空间复杂度O(1)
        - SomeBottle 2023.8.4
*/