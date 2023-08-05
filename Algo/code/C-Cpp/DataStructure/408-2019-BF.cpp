#include <cstdio>
#include <cstring>

using namespace std;

// 定义单链表数据结构
typedef int ElementType;

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
    int in[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // 单链表中的值
    int inSize = sizeof(in) / sizeof(in[0]);
    // initialization:
    List list = new LNode(0); // head node
    List tail = list;
    for (int i = 0; i < inSize; i++)
    {
        List node = new LNode(in[i]); // 插入结点
        tail->next = node;
        tail = node;
    }
    // algorithm:
    List ptr1, ptr2, pre;
    ptr1 = list->next;
    pre = list; // pre指向ptr2访问的上一个结点
    while (ptr1)
    {
        ptr2 = ptr1;
        while (ptr2->next)
        {
            pre = ptr2;        // pre指向ptr2访问的上一个结点
            ptr2 = ptr2->next; // 寻找倒数结点
        }
        if (ptr2 != ptr1) // 还没有排完，ptr2指向的末尾结点要插入在ptr1所指结点后面
        {
            pre->next = ptr2->next; // 把ptr2指向的结点从表尾拆出来
            ptr2->next = ptr1->next;
            ptr1->next = ptr2; // 接到ptr1所指结点后面
            ptr1 = ptr1->next; // 因为ptr1后面新插了一个结点，需要将ptr1后移一位，以继续算法
        }
        ptr1 = ptr1->next;
    }
    // output:
    ptr1 = list->next;
    while (ptr1)
    {
        printf("-> %d ", ptr1->data);
        ptr1 = ptr1->next;
    }
    printf("\n");
}

/*
    奶奶滴，已经没什么能阻挡我写暴力了！

    > 算法很简单粗暴，每扫描到一个结点N，就把顺序找到的最后一个结点插入到这个结点N的后面即可。
    > 注意，最后一个结点插入到N后面后，指向N的指针需要移动到指向这个新插入的节点。

    时间复杂度：O(n^2)级别
    空间复杂度：O(1)级别

        - SomeBottle 2023.8.5

*/