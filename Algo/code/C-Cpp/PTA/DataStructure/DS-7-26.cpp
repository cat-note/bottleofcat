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
        return priority <= obj.priority;
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

    优先队列
*/