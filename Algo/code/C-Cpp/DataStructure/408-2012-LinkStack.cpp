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

// 定义链栈结点

typedef struct StackNode
{
    List node;
    struct StackNode *next;
    StackNode()
    {
        next = NULL;
    }
} *Stack, StackNode;

// 链栈出栈操作

int Pop(Stack stack, List &node)
{
    if (!stack->next)
        return 0; // 空栈，出栈失败
    Stack tmp = stack->next;
    node = tmp->node;
    stack->next = tmp->next;
    delete tmp;
    return 1;
}

// 链栈入栈操作

void Push(Stack stack, List node)
{
    Stack elem = new StackNode();
    elem->node = node;
    elem->next = stack->next;
    stack->next = elem;
}

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
    // Init linkStack
    Stack stack1 = new StackNode(); // 链栈头结点
    Stack stack2 = new StackNode(); // 链栈头结点
    // ---------------------------------------------------algorithm below:
    List res = NULL; // 存储结果
    // 遍历第一个表
    List ptr = list1->next;
    while (ptr)
    {
        Push(stack1, ptr); // 遍历到的结点地址入栈1
        ptr = ptr->next;
    }
    // 遍历第二个表
    ptr = list2->next;
    while (ptr)
    {
        Push(stack2, ptr); // 遍历到的结点地址入栈2
        ptr = ptr->next;
    }
    // 两栈同时出栈
    List tmp1, tmp2; // 临时结点，用于接受出栈结点
    while (Pop(stack1, tmp1) && Pop(stack2, tmp2))
    {
        if (tmp1->data == tmp2->data) // 只要出栈的两个结点的数据是一致的，就是相同的后缀部分
            res = tmp1;
        else // 直到出栈两个结点数据不同为止
            break;
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
    再看了一眼，发现题目的要求是“请设计一个时间上尽可能高效的算法”，那我空间上岂不是可以嗯造了!
    于是这回我决定用链栈来优化算法，主要思想如下：
        1. 分别用两个栈记录两个链表的遍历结点地址
        2. 两个链表遍历结束后，两个栈中存放的是各表从最后一个结点到首个结点的地址
            * 也就是说，利用链栈将两个链表给倒过来了，然后找尾部相同的部分，即为共同后缀。
        3. 此时同时将两个栈进行弹栈，直到弹出的两个元素不同，算法终止。

    时间复杂度：O(n)级别（两链表遍历+两栈弹栈）
    空间复杂度：O(n)级别（需要用栈额外记录地址）

        - SomeBottle 2023.11.12
*/