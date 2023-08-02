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
    int k;
    printf("Input K:");
    scanf(" %d", &k); // 输入k
    if (k <= 0)
    {
        printf("Invalid k val.\n");
        return 1;
    }
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
    List front = list->next, rear = list->next; // 两个指针
    int counter = 0;                            // 记录rear指针访问到的结点数
    // rear指针先扫描
    while (rear)
    {
        // 主要思路，保持[front,rear]范围内有k个元素，当rear指到末尾时，front指的就是倒数第k个结点。
        if (++counter >= k && rear->next) // （边界情况）值得注意的是，最后一次rear->next=NULL的时候链表已经扫描完毕，无需移动front
            front = front->next;
        rear = rear->next;
    }
    // output:
    // 如果最终计数器<k，说明链表中连k个结点都没有。
    if (counter < k)
        printf("0\n");
    else
        printf("%d\n", front->data);
    return 0;
}

/*
    如果要扫描一次链表就找到倒数第k位，则需要用两个指针来固定一个范围[front,rear]。
    整体上看来，就像计算机网络的滑动窗口一样，窗口大小为k，当这个窗口的末端滑到链表的末端时，窗口头部即为倒数第k个数。

    注意处理边界情况。

        - SomeBottle 2023.8.2
*/