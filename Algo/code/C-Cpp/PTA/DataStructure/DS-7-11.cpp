#include <iostream>
#include <vector>
#include <queue>
// AOE - Activity On Edge
// 注: 本题的图是一个AOE网(无环有向图)，每条边是带工程耗时的
// AOE网终，边表示活动，顶点表示事件
// 指向【同一个顶点V】的多条边就代表【并行进行的活动】，这些边代表的活动全进行完后才能让顶点V代表的事件开始
using namespace std;

typedef struct AdjNode *AdjList; // 邻接表
typedef struct Edge *EdgeList;   // 边表
// 邻接表节点
struct AdjNode
{
    short vertex; // 顶点编号(从0开始)
    int cost;     // 这条边(活动)的开销
    AdjList next; // 下一个节点
};

// AOE网中从vFrom到vTo的一条边(一个活动)
struct Edge
{
    short vFrom; // 起始顶点
    short vTo;   // 指向顶点
    EdgeList next;
    // 运算符重载，用于堆排序进行升序排序
    bool operator>=(const Edge &activity) const
    {
        return vFrom >= activity.vFrom;
    }
};

// 因为是无环有向图，本题比较适合用邻接表
class Graph
{
private:
    short vertexNum;       // 顶点数
    vector<AdjList> lists; // 邻接表
    vector<short> topoSeq; // 拓扑排序列表
    EdgeList criticalA;    // 按序存放关键活动
    int criticalCost;      // 关键活动总开销
public:
    // 根据顶点数来进行初始化
    Graph(short vertexNum)
    {
        vertexNum++; // 给人工汇点预留一个空间
        lists.resize(vertexNum);
        topoSeq.resize(vertexNum);
        criticalA = new Edge();
        criticalA->next = NULL;
        for (int i = 0; i < vertexNum; i++)
        {
            // 初始化链表头节点
            lists[i] = new AdjNode();
            lists[i]->cost = 0;   // 头节点cost用于标记这个点是不是人工汇点
            lists[i]->vertex = 0; // 头节点vertex储存这个顶点的【入度】
            lists[i]->next = NULL;
        }
        lists[vertexNum - 1]->cost = 1; // 头节点cost用于标记这个点是不是人工汇点
        this->vertexNum = vertexNum;
        checkEnding(); // 检查是否多终点
    }
    // 析构工作，释放图占用的内存
    ~Graph()
    {
        for (int i = 0; i < vertexNum; i++)
        {
            AdjList curr = lists[i]->next;
            delete lists[i];
            while (curr != NULL)
            {
                AdjList next = curr->next;
                delete curr;
                curr = next;
            }
        }
        EdgeList curr = criticalA->next;
        while (curr)
        {
            EdgeList next = curr->next;
            delete curr;
            curr = next;
        }
        delete criticalA;
    }
    // 加入一条from顶点指向to顶点的有向边, 活动损耗为cost
    void add(short from, short to, int cost)
    {
        AdjList node = new AdjNode();
        // 把新节点加到头节点后面
        node->vertex = to;
        node->cost = cost;
        node->next = lists[from]->next;
        lists[from]->next = node;
        lists[to]->vertex++; // to顶点的入度增加
    }
    // 判断是否有多终点，如果有就人工加入一个汇点
    void checkEnding()
    {
        // vertexNum-1下标是预留给人工汇点的位置
        queue<short> endings; // 临时存放终点
        short endingNum = 0;  // 终点数
        for (int i = 0; i < vertexNum - 1; i++)
        {
            if (!lists[i]->next)
            {
                endings.push(i);
                endingNum++;
            }
        }
        // 有多于一个终点，就让【所有终点指向人工汇点】
        if (endingNum > 1)
        {
            while (!endings.empty())
            {
                // 人工加入一个耗时为0的活动(边)，从终点指向人工汇点(下标为vertexNum-1)
                add(endings.front(), vertexNum - 1, 0);
                endings.pop();
            }
        }
        else
        {
            // 否则，无视人工汇点这一位
            delete lists[vertexNum - 1];
            vertexNum--;
        }
    }
    // 进行拓扑排序Topological Order，顺便判断有向图终是否没有环路
    bool topoOrder()
    {
        vector<short> degrees(vertexNum); // 初始化一个临时储存顶点入度的数组
        for (int i = 0; i < vertexNum; i++)
            degrees[i] = lists[i]->vertex; // 复制顶点入度
        // 接下来进行拓扑排序
        bool hasZero = true; // 是否有零入度的顶点
        short topoVNum = 0;  // 拓扑排序结束后有多少顶点被取走
        while (hasZero)
        {
            hasZero = false;
            for (int i = 0; i < vertexNum; i++)
                if (degrees[i] == 0)
                {
                    // 找到了0入度的顶点
                    hasZero = true;
                    degrees[i] = -1; // 标记为-1，代表已经删掉该顶点
                    // 取出的顶点加入拓扑序列
                    topoSeq[topoVNum] = i;
                    topoVNum++; // 被取走的顶点数+1
                    // 如果顶点是0出度(终点)，终点数量++
                    AdjList curr = lists[i]->next;
                    while (curr != NULL)
                    {
                        degrees[curr->vertex]--; // 更新其他顶点的入度
                        curr = curr->next;
                    }
                }
        }
        // 拓扑排序结束, 如果取出的节点数=总数，说明无环
        return topoVNum == vertexNum;
    }
    // 打印关键活动的开销以及关键路径
    void printCritical()
    {
        cout << criticalCost << "\n";
        EdgeList curr = criticalA->next;
        while (curr)
        {
            // 别忘了题目中编号是从1开始
            cout << curr->vFrom + 1 << "->" << curr->vTo + 1 << "\n";
            curr = curr->next;
        }
    }
    // 将关键活动加入criticalA数组
    void pushActivity(short from, short to)
    {
        EdgeList node = new Edge();
        node->vFrom = from;
        node->vTo = to;
        EdgeList curr = criticalA;
        while (curr->next)
        {
            if (*node >= *(curr->next))
                curr = curr->next;
            else
                break;
        }
        node->next = curr->next;
        curr->next = node;
    }
    // 找到关键路径点和关键活动
    void findCritical()
    {
        vector<int> earliest(vertexNum, 0); // 储存顶点最早发生时间的数组
        vector<int> latest(vertexNum, -1);  // 储存顶点最晚发生时间的数组，默认值用-1代表无穷
        // 依据拓扑排序来推算顶点
        // 接下来从源点开始推算最早发生时间
        for (int i = 0; i < vertexNum; i++)
        {
            // 取出拓扑排序中第i个顶点的邻接表
            short currVertex = topoSeq[i];
            // 如果拓扑排序的这个顶点的入度为0，说明是起点，标记最早发生时间为0
            if (lists[currVertex]->vertex == 0)
                earliest[currVertex] = 0;
            AdjList curr = lists[currVertex]->next;
            // 找到当前顶点的所有出边
            while (curr != NULL)
            {
                // 如果(当前顶点的最早发生时间) + (邻接边的时间) > (邻接点的最早发生时间)，就更新邻接点的最早发生时间
                int eTime = earliest[currVertex] + curr->cost;
                if (eTime > earliest[curr->vertex])
                {
                    earliest[curr->vertex] = eTime;
                }
                curr = curr->next;
            }
        }
        // 因为能保证一定只有一个终点(汇点)，这里拓扑排序最后一位就是汇点
        short endingV = topoSeq[vertexNum - 1];
        latest[endingV] = earliest[endingV]; // 汇点的最迟开始时间=最早开始时间
        // 这里从拓扑序列倒数第一个顶点开始，倒推
        for (int i = vertexNum - 1; i >= 0; i--)
        {
            // 取出这个顶点的邻接表
            short currVertex = topoSeq[i]; // 当前取出的拓扑排序顶点
            AdjList curr = lists[currVertex]->next;
            // 因为是倒推，这个顶点的后继的最迟发生时间肯定已经知道了
            // curr是当前顶点的一个后继顶点
            while (curr != NULL)
            {
                // 如果(当前顶点的后继顶点的最迟发生时间) - (邻接边的时间) < (当前顶点的最迟发生时间)，就更新当前顶点的最迟发生时间
                int lTime = latest[curr->vertex] - curr->cost;
                // -1是默认值，代表无穷
                if (latest[currVertex] == -1 || lTime < latest[currVertex])
                {
                    latest[currVertex] = lTime;
                }
                curr = curr->next;
            }
        }
        // 寻找所有关键顶点对应的关键活动，加入到有序链表中以便输出
        for (short i = 0; i < vertexNum; i++)
        {
            // 如果是关键顶点
            if (earliest[i] == latest[i])
            {
                AdjList curr = lists[i]->next; // 取出关键顶点的邻接表
                while (curr)
                {
                    bool isManualVertex = lists[curr->vertex]->cost; // 头节点的cost用于标记这个顶点是不是人工汇点
                    // 如果这个邻接点【不是人工汇点】，但是关键顶点，就把这两点形成的边加入到有序链表
                    if (!isManualVertex && earliest[curr->vertex] == latest[curr->vertex])
                    {
                        pushActivity(i, curr->vertex);
                    }
                    curr = curr->next;
                }
            }
        }
        // AOE网的汇点的最早发生时间(也是最晚发生时间)就是关键活动的总开销
        // 汇点的最早发生时间就是工程耗时
        criticalCost = earliest[endingV];
    }
};

int main()
{
    // 顶点数, 边数
    int vertexNum, edgeNum;
    cin >> vertexNum >> edgeNum;
    Graph G(vertexNum); // 初始化图
    for (int i = 0; i < edgeNum; i++)
    {
        short v1, v2;
        int cost;
        cin >> v1 >> v2 >> cost;
        // 注意，我程序中的顶点编号全都是从0开始的，要减1
        G.add(v1 - 1, v2 - 1, cost);
    }
    // 检查图是否有环，如果图中有环，任务调度是不可行的
    if (G.topoOrder())
    {
        G.findCritical();  // 找到关键活动
        G.printCritical(); // 打印开销和关键活动
    }
    else
    {
        // 有环路就直接输出0
        cout << 0;
    }
    return 0;
}

/*

    本题要把情况考虑周全:

        1. AOE图中有几个【起点(源点)】和【几个终点(汇点)】? 如果AOE图的源点/汇点不止一个，就需要【人工加入】顶点来【让图中的源点/汇点只有一个】。

            * 比如有多个起点，我添加一个顶点让其【指向所有的起点，边权为0】，然后把这个顶点作为源点即可。
            * 多个终点的话，就让【所有终点】指向人工加入的顶点，把这个顶点作为汇点即可。

        2. 关键路径有几条？这个题目中给出的样例的关键路径只有一条，但是实际上可能出现【多条关键路径】的情况

            * 这种情况往往是图中有几个 活动持续时间(边权) 是相同的。

            * 比如我把这题给的样例中的 1->3 这条边的权重改成6，就会出现【两条关键路径】:

                1->2->4->6->7
                1->3->4->6->7

                (可以在草稿纸上推算一下，确实如此)

    对于第1点，写程序时要注意以下几点:

        - 计算最早发生时间前，要扫描拓扑序列，找到【所有入度为0】的顶点，这些顶点【都是起点】，把它们的最早发生时间全标为0

            * 对于起点，程序中我们并不需要加入人工起点，只需要找到所有起点即可（得益于拓扑排序）

        - 计算最迟发生时间时，也要扫描拓扑序列，找到【所有出度为0】的顶点，这些顶点【都是终点】，它们的最迟发生时间【就是最早发生时间】

        - 关键活动的耗时决定了【整个工程的工期】，【汇点】的最早/最迟发生时间就是整个工程完成的时间

            * 对于有多个终点的情况，需要人工添加一个汇点，让【多个终点指向同一个汇点，边权为0】，最终输出关键活动时不输出这个人工添加的点即可。

    ----------------------------------------
    AOE图中，指向【同一个顶点】的两条边代表【某个事件发生前要完成的两项活动】

    也就是说，一个顶点要等其【所有入边】代表的活动都完成【才能到达】

        - 要找某个顶点的【最早发生时间】，就是找这个顶点的【入边的发生时间 + 前一个顶点的最早发生时间】的【最大值】

            * 最早发生时间找的是【最大值】，因为事件(顶点)要等【之前的所有活动(入边)都执行完】才能发生，而这些活动(入边)的进行是并行的，只要最长的那个活动完成了，其他的活动肯定都完成了。

            * 找最早发生时间是从起点到终点顺着推

        - 要找某个顶点的【最迟发生时间】，就需要从终点倒推了。

            * 设任意多个【已知最迟发生时间】的顶点为V, 这些顶点的【前驱顶点】中都有【F顶点】，那么F顶点的【最迟发生时间】就是:

                [V顶点最迟发生时间 - 连接F和V的边的时间] 的最小值

            - 所谓某个顶点的最迟发生时间，就是说:

                如果这个顶点(事件)的发生时间迟于最短发生时间，后面必有顶点(事件)不能及时发生，也就是事情做不完了。

        - AOE图主要按只有 一个源点(起点)与一个汇点(终点) 的情况进行处理

            * 源点的最早发生时间=0
            * 汇点的最迟发生时间=其最早发生时间

    整个推算过程可借【拓扑排序】实现。

    关于AOE网有一篇很好的入门文章: https://zhuanlan.zhihu.com/p/170603727
    ----------------------------------------------


*/
/*
咱自己想了个用于测试多起点多终点，以及对多终点输出的样例:
14 15
1 2 3
1 3 3
2 4 5
3 4 3
4 5 1
4 6 6
5 7 5
6 7 2
8 10 3
8 9 4
10 11 2
9 11 5
11 12 6
11 13 3
13 14 2
*/