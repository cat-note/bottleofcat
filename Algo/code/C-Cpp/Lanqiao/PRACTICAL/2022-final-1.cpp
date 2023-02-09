#include <cstdio>

using namespace std;

// 链表节点
typedef struct Node
{
    int data;
    Node *prev; // 指向前一个节点
    Node *next; // 指向后一个节点
    Node(int data)
    {
        this->data = data;
        prev = NULL;
        next = NULL;
    }
} *List;

int main()
{
    List seq = new Node(-1); // 头节点
    List tail = seq;         // 尾指针
    int N, M;                // N个数，M个操作
    scanf("%d %d%*c", &N, &M);
    List nodeMap[N + 1]; // 通过数值作为下标直接访问到节点
    for (int i = 1; i <= N; i++)
    {
        List node = new Node(i);
        node->prev = tail;
        nodeMap[i] = node; // 记录数值到节点的映射关系
        tail->next = node; // 构造序列
        tail = node;
    }
    // 开始处理操作
    for (int i = 0; i < M; i++)
    {
        char operation;
        int num;
        scanf("%c %d%*c", &operation, &num);          // 记得接走换行符
        List optNode = nodeMap[num];                  // 取出对应节点
        if (operation == 'L' && seq->next != optNode) // 左移节点到最左端，当节点已经在最左端就不作操作
        {
            // 先把optNode移走
            optNode->prev->next = optNode->next;
            if (optNode != tail) // 💡 防止访问NULL指针，比如说把尾部的元素移动到头部
                optNode->next->prev = optNode->prev;
            else // 💡 如果移动的是表尾的节点，tail指针也要回退
                tail = optNode->prev;
            // 把optNode插入头部(最左端)
            seq->next->prev = optNode;
            optNode->next = seq->next;
            optNode->prev = seq;
            seq->next = optNode;
        }
        else if (operation == 'R' && tail != optNode) // 右移节点到最右端，当节点已经在最右端就不做操作
        {
            // 一样先把optNode移走
            optNode->prev->next = optNode->next;
            optNode->next->prev = optNode->prev;
            // 把optNode插入尾部(最右端)
            optNode->next = NULL;
            optNode->prev = tail;
            tail->next = optNode;
            tail = optNode; // 💡 移动尾指针
        }
    }
    // 输出序列
    List curr = seq->next;
    while (curr)
    {
        if (curr != seq->next) // 不输出多余空格
            printf(" ");
        printf("%d", curr->data);
        /*
        List next=curr->next; // 释放内存，算法题中非必须
        delete curr;
        curr=next;
        */
        curr = curr->next;
    }
    // delete seq; // 释放头节点内存
    return 0;
}

/*
    可以看到，这题要做的是按照指令将序列中的【某一个元素】提到【序列的最左边】或者【序列的最右边】，因此主要需求有如下两点:

        1. 随机访问元素

        2. 快速移动元素到头部和尾部

    “随机访问元素”这点很快能让人想到数组结构，而“快速移动元素到头部和尾部”又令人想起链表。

    因此这题实际上可以结合这两种数据结构进行解决:

        1. 以数值x作为下标，用数组存放链表节点以实现快速访问

        2. 用双向链表储存整个序列的顺序，可以在头部/尾部快速地进行插入/删除操作。

    ----------------------------

    既然玩到了链表，指针变量的管理一定是非常容易错的点:

        - 当要【左移】的是当前序列中【首个元素】，就不需要进行操作
            * 这种情况下有临界情况，比如链表中只有一个节点

        - 当要【右移】的是当前序列中【最后一个元素】，也不需要进行操作

        - 一定要注意链表【尾指针】的管理！

            + 当左移了最后一个元素后，尾指针需要移动。

            + 当把前面的元素移动到最后，尾指针也需要移动。
    -----------------------------

        - SomeBottle 2023.2.9

*/

/*
这题是2022年第十三届蓝桥杯大赛软件类决赛Java大学B组真题

左移右移
    时间限制: 3s 内存限制: 512MB 提交: 478 解决: 78
题目描述
    小蓝有一个长度为 N 的数组，初始时从左到右依次是 1, 2, 3, . . . N。
    之后小蓝对这个数组进行了 M 次操作，每次操作可能是以下 2 种之一：
        1. 左移 x，即把 x 移动到最左边。
        2. 右移 x，即把 x 移动到最右边。
    请你回答经过 M 次操作之后，数组从左到右每个数是多少？
输入格式
    第一行包含 2 个整数，N 和 M。 以下 M 行每行一个操作，其中 “L x”表示左移 x，“R x”表示右移 x。
输出格式
    输出 N 个数，代表操作后的数组。
样例输入
    5 3
    L 3
    L 2
    R 1
样例输出
    2 3 4 5 1
提示
    样例中的数组变化如下：
        [1, 2, 3, 4, 5] → [3, 1, 2, 4, 5] → [2, 3, 1, 4, 5] → [2, 3, 4, 5, 1]
    对于 50% 的评测用例，1 ≤ N, M ≤ 10000；
    对于 100% 的评测用例，1 ≤ N, M ≤ 200000, 1 ≤ x ≤ N。
*/