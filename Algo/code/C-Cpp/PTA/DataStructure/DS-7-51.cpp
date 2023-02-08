#include <cstdio>

using namespace std;

// 链表节点
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
    // 链表带头节点
    List seq[2] = {new Node(-1), new Node(-1)}; // 初始的两个链表
    // 读入链表
    int numRead;
    int currSeq = 0;    // 当前读取的链表
    List tail = seq[0]; // 尾指针
    while (true)
    {
        scanf("%d", &numRead);
        if (numRead == -1)
            if (currSeq != 1)
            {
                tail = seq[++currSeq]; // 开始读取下一个链表
                continue;
            }
            else
            {
                break; // 两个都读完了
            };
        List node = new Node(numRead);
        tail->next = node; // 尾插法
        tail = node;
    }
    // 合并链表
    List merged = new Node(-1); // 合并后的链表
    tail = merged;              // 指定尾指针
    List curr1 = seq[0]->next;
    List curr2 = seq[1]->next;
    while (curr1 && curr2)
    {
        if (curr1->data <= curr2->data)
        {
            // 如果当前扫描到的链表1元素<=链表2元素，就先把链表1元素加入
            tail->next = curr1;
            tail = curr1;
            curr1 = curr1->next;
        }
        else
        {
            // 反之就将链表2的元素加入
            tail->next = curr2;
            tail = curr2;
            curr2 = curr2->next;
        }
    }
    if (curr1) // 💡如果curr1还有元素，就直接把剩下的接到tail后面
    {
        tail->next = curr1;
    }
    else if (curr2) // 💡如果curr2还有元素，就直接把剩下的接到tail后面
    {
        tail->next = curr2;
    }
    // 输出
    List curr = merged->next;
    bool putSpace = false; // 防止输出多余空格
    while (curr)
    {
        if (putSpace)
            printf(" ");
        else
            putSpace = true;
        List next = curr->next;
        printf("%d", curr->data);
        delete curr; // 顺带释放内存
        curr = next;
    }
    if (!putSpace) // 最终的链表为空
        printf("NULL");
    delete seq[0];
    delete seq[1];
    delete merged;
    return 0;
}

/*
    这题比较类似于归并排序的【合并】过程。

    因为输入的两个序列已经有序，因此：

        - 维护两个指针变量，同时对两个链表进行扫描

        - 每次选择其中一个链表的元素，加入到结果链表中，保证【结果链表是非递减的】

            * 每次只需要对比两个指针变量指向的两个元素即可，优先选择【值更小的元素】加入结果中。

    💡 与数组不同，链表是【可以整段移动】的。

        - 扫描过程中，当【其中一条链表为空】时，可以直接【把另一条链表的剩余节点接到结果中】。

    无论是像归并排序那样扫描到底，还是像这样直接把剩余元素转接，在本题测试点的表现是相差无几的。
    但毫无疑问直接转接链表才【充分发挥了链表的结构】。

    - SomeBottle 2023.2.8
*/

/*
7-51 两个有序链表序列的合并
    已知两个非降序链表序列S1与S2，设计函数构造出S1与S2合并后的新的非降序链表S3。

输入格式:
    输入分两行，分别在每行给出由若干个正整数构成的非降序序列，用−1表示序列的结尾（−1不属于这个序列）。数字用空格间隔。
输出格式:
    在一行中输出合并后新的非降序链表，数字间用空格分开，结尾不能有多余空格；若新链表为空，输出NULL。
输入样例:
    1 3 5 -1
    2 4 6 8 10 -1
输出样例:
    1 2 3 4 5 6 8 10
代码长度限制
    16 KB
时间限制
    1500 ms
内存限制
    128 MB
*/