#include <cstdio>
#include <string>
#include <vector>

using namespace std;

// 因为题目中整数越小，优先级越高
// 本题实际上依赖的就是基于小根堆的优先队列

struct Message
{
    int priority; // 消息优先级
    string text;  // 消息内容
    bool operator<(const Message &obj) const
    {
        // 观察样例可以发现，当孩子节点小于或等于父亲节点时要交换
        return priority < obj.priority;
    }
};

template <typename T>
class PriorityQueue
{
private:
    int capacity;   // 队列容量
    int currSize;   // 当前数组有多少元素
    vector<T> qArr; // 底层数组

    // 比较node1下标的节点是否小于node2下标的节点
    // 用于子节点和父节点的比较和子节点与另一个子节点的比较
    // 如果node1不存在会返回false，如果node2不存在会返回true
    bool lessThan(int node1, int node2)
    {
        if (node1 >= currSize)
            return false;
        if (node2 >= currSize)
            return true;
        return qArr[node1] < qArr[node2];
    }

    // 交换节点
    void swap(T &node1, T &node2)
    {
        T temp = node1;
        node1 = node2;
        node2 = temp;
    }

    // 对节点进行上滤操作
    void siftUp(int index)
    {
        // 没有父节点就无需上滤
        while (index > 0)
        {
            int parentInd = (index - 1) / 2; // 父节点下标
            if (lessThan(index, parentInd))
                swap(qArr[index], qArr[parentInd]);
            else
                break;         // 不用交换就循环终止
            index = parentInd; // 否则继续向上滤
        }
    }

    // 对节点进行下滤操作
    void siftDown(int index)
    {
        // 有孩子的情况下进行下滤
        while (index < currSize)
        {
            int leftInd = index * 2 + 1; // 左孩子下标
            int rightInd = leftInd + 1;  // 右孩子下标
            if (lessThan(leftInd, index) && lessThan(leftInd, rightInd))
            { // 左孩子小于父节点且左孩子小于右孩子，交换左孩子与父节点
                swap(qArr[leftInd], qArr[index]);
                index = leftInd;
            }
            else if (lessThan(rightInd, index) && lessThan(rightInd, leftInd))
            { // 右孩子小于父节点且右孩子小于左孩子，交换右孩子与父节点
                swap(qArr[rightInd], qArr[index]);
                index = rightInd;
            }
            else
            {
                break; // 否则无需继续下滤
            }
        }
    }

public:
    PriorityQueue(int size)
    {
        // 初始化优先队列
        capacity = size;
        currSize = 0;
        qArr.resize(size);
    }
    // 加入新元素
    void push(const T &node)
    {
        if (currSize >= capacity) // 队满就不予操作了
            return;
        qArr[currSize++] = node;
        // 注意，在currSize变动后再调用上滤方法，不然会因为数组元素数未更新而判断出错
        siftUp(currSize - 1); // 上滤最后一个元素
    }
    // 是否堆空
    bool empty()
    {
        return currSize == 0;
    }
    // 取得堆顶元素(无堆空检查)
    T &top()
    {
        return qArr[0];
    }
    // 弹出堆顶
    void pop()
    {
        if (currSize <= 0)
            return;
        qArr[0] = qArr[currSize - 1]; // 把最后一个节点移动到堆顶
        siftDown(0);                  // 对这个节点进行下滤操作
        currSize--;
    }
};

int main()
{
    int instNum; // 指令数
    scanf("%d", &instNum);
    PriorityQueue<Message> pq(instNum); // 初始化优先队列
    char instTemp[4];                   // 指令字符数组
    char msgTemp[11];                   // 消息临时字符数组
    for (int i = 0; i < instNum; i++)
    {
        scanf("%s", instTemp);
        if (instTemp[0] == 'P')
        {
            Message msg;
            scanf("%s", msgTemp);
            msg.text.assign(msgTemp); // 读入消息内容
            scanf("%d", &msg.priority);
            pq.push(msg); // 加入优先队列
        }
        else if (instTemp[0] == 'G')
        {
            // 堆空的情况
            if (pq.empty())
            {
                printf("EMPTY QUEUE!\n");
                continue;
            }
            printf("%s\n", pq.top().text.c_str());
            pq.pop(); // 弹出堆顶
        }
    }
    return 0;
}

/*
    本题对于程序运行时间的要求比较苛刻，故采用cstdio标准库的输入输出函数。
        * 从iostream库改用cstdio后运行时间缩短了3倍左右
    ----------------------------------------------
    优先队列本质上是数据结构里的【堆】。
    本题中第三个字段(整数)的值越低，优先级越高，因此本题的优先队列是基于【小根堆】(保证堆顶元素最小)的。

        - 小根堆的堆序性的体现: 【所有父节点】都要【小于其子节点】。

    优先队列的维护基于以下两个方法:

        - SiftUp 上滤
            将某个节点在堆中向上(朝着根节点的方向)移动，以维持小根堆的堆序性。

        - SiftDown 下滤
            将某个节点在堆中向下(朝着叶节点的方向)移动，以维持小根堆的堆序性。

    因为堆是一个完全二叉树，因此优先队列底层十分适合用数组对树结点进行储存。
    优先队列的基本操作:

        - 插入元素
            将新元素插入到堆尾首个空位（数组的尾部），然后对这个元素进行【上滤】操作。

        - 弹出堆顶元素
            弹出堆顶元素，将堆中最后一个节点（数组最后一个元素）移动至【堆顶】，然后对这个堆顶元素进行【下滤】操作。
    -------------------------------------------
    题目保证了优先级无重复，因此【新插入节点和其父节点大小相同】时的处理可以随意写。

        - SomeBottle 2023.1.19
*/
/*
7-26 Windows消息队列
    消息队列是Windows系统的基础。对于每个进程，系统维护一个消息队列。如果在进程中有特定事件发生，如点击鼠标、文字改变等，系统将把这个消息加到队列当中。同时，如果队列不是空的，这一进程循环地从队列中按照优先级获取消息。请注意优先级值低意味着优先级高。请编辑程序模拟消息队列，将消息加到队列中以及从队列中获取消息。

输入格式:
    输入首先给出正整数N（≤10^5），随后N行，每行给出一个指令——GET或PUT，分别表示从队列中取出消息或将消息添加到队列中。如果指令是PUT，后面就有一个消息名称、以及一个正整数表示消息的优先级，此数越小表示优先级越高。消息名称是长度不超过10个字符且不含空格的字符串；题目保证队列中消息的优先级无重复，且输入至少有一个GET。
输出格式:
    对于每个GET指令，在一行中输出消息队列中优先级最高的消息的名称和参数。如果消息队列中没有消息，输出EMPTY QUEUE!。对于PUT指令则没有输出。
输入样例:
    9
    PUT msg1 5
    PUT msg2 4
    GET
    PUT msg3 2
    PUT msg4 4
    GET
    GET
    GET
    GET
输出样例:
    msg2
    msg3
    msg4
    msg1
    EMPTY QUEUE!
代码长度限制
    16 KB
时间限制
    150 ms
内存限制
    64 MB
*/