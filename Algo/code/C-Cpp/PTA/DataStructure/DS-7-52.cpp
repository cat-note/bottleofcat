#include <cstdio>

using namespace std;

typedef struct Node
{
    int data;
    Node *next;
    Node(int data)
    {
        this->data = data;
        next = NULL;
    }
} *List;

int main()
{
    List list_1 = new Node(-1); // 链表1
    List list_2 = new Node(-1); // 链表2
    List tail = list_1;         // 一个尾指针
    // 读入元素
    int numRead;
    bool readingSecond = false; // 是否在读第二个链表
    while (true)
    {
        scanf("%d", &numRead);
        if (numRead == -1)
            if (!readingSecond)
            {
                readingSecond = true; // 开始读取第二个链表
                tail = list_2;
                continue;
            }
            else
            {
                break; // 两个链表都读取完毕
            };
        List node = new Node(numRead);
        tail->next = node; // 尾插法
        tail = node;
    }
    // 开始处理元素
    List curr_1 = list_1->next;
    List curr_2 = list_2->next;
    List intersected = new Node(-1); // 交集序列
    tail = intersected;
    while (curr_1 && curr_2)
    {
        if (curr_1->data == curr_2->data)
        {
            // 元素值相同，是相交的部分
            tail->next = curr_1;
            tail = curr_1;
            curr_1 = curr_1->next;
            curr_2 = curr_2->next;
        }
        else if (curr_1->data < curr_2->data)
        {
            // 前一个链表的元素小于后一个的
            // 移动前一个链表的指针
            curr_1 = curr_1->next;
        }
        else
        {
            // 前一个链表的元素大于后一个的
            // 移动后一个链表的指针
            curr_2 = curr_2->next;
        }
    }
    tail->next=NULL; // 链表封口
    // 输出结果
    List curr = intersected->next;
    bool putSpace = false; // 防止输出多余空格
    while (curr)
    {
        if (putSpace)
            printf(" ");
        else
            putSpace = true;
        printf("%d", curr->data);
        List next = curr->next;
        curr = next;
    }
    if (!putSpace) // 没有输出就输出NULL
        printf("NULL");

    // 这里我就不释放内存了，没必要。
    return 0;
}

/*
    这题和7-51一样，因为题目给出的是【有序序列】，维护两个指针变量分别扫描两个链表即可:

        - 两个指针对应的元素一致，就找到了一个交集元素，同时后移两个指针
        - 两个指针对应的元素不一致:
            1. 如果前一个指针对应元素的值小于后一个 -> 前一个指针后移
            2. 反之 -> 后一个指针后移

    💡 本题的“交集”允许有重复元素出现。

    - SomeBottle 2023.2.8
*/

/*
7-52 两个有序链表序列的交集
已知两个非降序链表序列S1与S2，设计函数构造出S1与S2的交集新链表S3。

输入格式:
    输入分两行，分别在每行给出由若干个正整数构成的非降序序列，用−1表示序列的结尾（−1不属于这个序列）。数字用空格间隔。
输出格式:
    在一行中输出两个输入序列的交集序列，数字间用空格分开，结尾不能有多余空格；若新链表为空，输出NULL。
输入样例:
    1 2 5 -1
    2 4 5 8 10 -1
输出样例:
    2 5
代码长度限制
    16 KB
时间限制
    1000 ms
内存限制
    64 MB
*/