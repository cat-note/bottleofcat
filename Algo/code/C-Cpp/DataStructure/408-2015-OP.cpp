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
    int n = 500;                                             // 假如题目给出的数值最大不超过500
    int in[] = {21, -15, -15, -7, 15, 500, 17, 28, -17, 90}; // 单链表中的值
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
    bool ht[n + 1];            // 开一个n这么大的数组，可以把数值作为下标来对数组进行访问
    memset(ht, 0, sizeof(ht)); // ht置空
    // 我这个思路实际上就是哈希表的直接定址法
    List ptr = list;
    while (ptr->next)
    {
        // 取个绝对值先
        ElementType absData = ptr->next->data < 0 ? -ptr->next->data : ptr->next->data;
        if (ht[absData]) // 如果哈希表中已标记此值，说明出现过
        {
            // 移除该节点
            List tmp = ptr->next;
            ptr->next = ptr->next->next;
            delete tmp;
        }
        else
        {
            ht[absData] = true;
        }
        ptr = ptr->next;
    }
    // output:
    ptr = list->next;
    while (ptr)
    {
        printf("%d -> ", ptr->data);
        ptr = ptr->next;
    }
    return 0;
}

/*
    这题很有意思的一点是特意在题干强调了 |data|<=n ，出题人显然不会给出没有用的信息。
    实际上，题目并没有限制空间复杂度，而在知道了数据规模n后，我可以直接开一张哈希表来标记每个数值的绝对值是否出现过。

    该算法实现的时间复杂度为O(n)，空间复杂度也和数据规模同等级，O(n)

        - SomeBottle 2023.8.1
*/