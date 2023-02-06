#include <cstdio>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

// 并查集，用来判断两人是否在同一个朋友圈内
class UnionFind
{
private:
    vector<int> counts;  // 集合中元素计数
    vector<int> parents; // 父节点联系
public:
    UnionFind(int size)
    {
        counts.resize(size, 1); // 初始每个集合都只有一个元素
        parents.resize(size);
        for (int i = 0; i < size; i++) // 初始化: 各节点的父节点就是自己
            parents[i] = i;
    }
    // 获得某个集合的元素数
    int count(int root)
    {
        return counts[root];
    }
    // 寻找各节点所在树的根
    // 顺便做路径压缩
    int find(int index)
    {
        return parents[index] == index ? index : parents[index] = find(parents[index]);
    }
    // 并操作
    void merge(int a, int b)
    {
        int aInd = find(a); // 先找到二者根节点下标
        int bInd = find(b);
        if (aInd == bInd) // 二者已经在同一个集合就不予处理
            return;
        // 往往把节点数少的并入节点数多的树中
        if (counts[bInd] < counts[aInd])
        {
            // b并入a
            parents[bInd] = aInd;
            counts[aInd] += counts[bInd];
        }
        else
        {
            // a并入b
            parents[aInd] = bInd;
            counts[bInd] += counts[aInd];
        }
    }
};

// 用id表示客户
struct Client
{
    int id;
    Client()
    {
        id = -1; // -1代表未指定id
    }
    void operator=(const int &id)
    {
        this->id = id;
    }
};

// 队列中的元素
typedef struct QNode
{
    int id;      // 顾客编号
    int arrival; // 到达时间
    int cost;    // 处理事务花费的时间
    QNode *next;
    QNode()
    {
        this->id = -1;
        this->arrival = -1;
        this->cost = -1;
        next = NULL;
    }
} *List;

int main()
{
    int clientNum, circleNum; // 顾客数，朋友圈个数
    scanf("%d %d", &clientNum, &circleNum);
    // --------------------------------------------------------------------------------------------读入朋友圈信息
    int clientCnt = 0;                       // 顾客编号
    unordered_map<string, Client> clientMap; // 映射顾客名到一个编号
    string clientNames[clientNum];           // 映射编号到顾客名
    UnionFind uf(clientNum);                 // 初始化并查集
    char strBuf[4];                          // 临时字符数组
    string strTemp;                          // 临时字符串
    for (int i = 0; i < circleNum; i++)
    {
        int num; // 朋友圈内人数
        scanf("%d", &num);
        int root = clientCnt; // 这个朋友圈的根，用于并查集合并集合
        for (int j = 0; j < num; j++)
        {
            scanf("%s", strBuf);
            strTemp.assign(strBuf);
            uf.merge(root, clientCnt);        // 同一个朋友圈的顾客并到同一个集合
            clientMap[strTemp] = clientCnt;   // 给顾客一个编号
            clientNames[clientCnt] = strTemp; // 储存编号对应的顾客名
            clientCnt++;
        }
    }
    // --------------------------------------------------------------------------------------------按来访顺序读取顾客
    List q = new QNode(); // 头节点
    List tail = q;        // 尾指针
    for (int i = 0; i < clientNum; i++)
    {
        List node = new QNode();
        scanf("%s", strBuf);
        strTemp.assign(strBuf);
        Client &customer = clientMap[strTemp]; // 取得顾客编号
        if (customer.id == -1)
        {
            // 顾客编号-1代表未被指定
            // 💡因为有的顾客并不在“朋友圈”中
            customer = clientCnt;             // 给顾客一个编号
            clientNames[clientCnt] = strTemp; // 储存编号对应的顾客名
            clientCnt++;
        }
        node->id = customer.id;
        scanf("%d %d", &node->arrival, &node->cost); // 读入到达时间和业务办理时间
        if (node->cost > 60)                         // 【每个事务】最多占用60分钟。
            node->cost = 60;
        tail->next = node; // 接到队尾
        tail = node;
    }
    // --------------------------------------------------------------------------------------------开始处理加塞情况
    List curr = q->next; // 当前正在处理的客户
    int waitFor = 0;     // 累积等待时间
    int time = 0;        // 当前时间
    vector<int> output;  // 输出顾客的顺序
    while (curr)
    {
        if (curr->arrival > time) // 💡如果有客户在【前一个人事务处理完后才来】
            time = curr->arrival;
        // 💡等待时间 = 被服务时刻 - 到达时间
        waitFor += (time - curr->arrival); // 累积等待时间
        time += curr->cost;                // 时间后移
        output.push_back(curr->id);
        // 往后找和当前顾客在【同一朋友圈】中的节点
        int root = uf.find(curr->id); // 获得当前客户所在集合的根
        List prev = curr;             // 记录前一个节点，用于删除节点
        List rear = curr->next;
        int maxNode = uf.count(root) - 1; // 最多能找到maxNode个节点
        int nodeFound = 0;                // 找到了几个在相同朋友圈的节点
        while (rear)
        {
            if (rear->arrival > time) // 💡这个顾客的到达时间在【前面朋友处理完事务】之后，不用帮了
                break;
            if (uf.find(rear->id) == root) // 如果找到了朋友圈的一个顾客，就帮他把事务也办了
            {
                waitFor += (time - rear->arrival); // 累积等待时间
                time += rear->cost;                // 时间后移
                output.push_back(rear->id);
                // 因为这个顾客的事务已经被朋友办了，因此可以从链表中移除
                prev->next = rear->next;
                delete rear;
                rear = prev; // rear指针回退到prev的位置，因为后面还有rear=rear->next
                nodeFound++;
            }
            if (nodeFound >= maxNode) // 💡找到了朋友圈的所有朋友就没必要继续了
                break;
            prev = rear;
            rear = rear->next;
        }
        curr = curr->next;
    }
    // --------------------------------------------------------------------------------------------输出结果
    for (int i = 0, len = output.size(); i < len; i++)
    {
        printf("%s\n", clientNames[output[i]].c_str()); // 输出客户名字
    }
    printf("%.1lf", (double)waitFor / clientNum); // 输出平均等待时间
    // 释放链表内存
    while (q)
    {
        List next = q->next;
        delete q;
        q = next;
    }
    return 0;
}

/*
    这题让我有点想起了操作系统的进程调度算法，思路上来说其实并不难，主要难在【要注意的细节点很多】

    以下是我觉得需要注意的地方:

        1. 不是所有人都在朋友圈中，如果要编号的话需要注意这点。

        2. (等待时间 = 被服务时刻 - 到达时刻) (和操作系统进程调度的等待被服务时间很类似)

        3. 没有说到达时间一定是从0开始。

        4. 后一个人可能在【前一个人事务处理完】后都还没来，也有可能在【前一个人刚办完事时】来。

        5. 题目说的是【每个事务】最多占用60分钟而不是每个人。
*/
