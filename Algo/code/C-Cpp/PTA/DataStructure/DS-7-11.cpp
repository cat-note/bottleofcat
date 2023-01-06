#include <iostream>
#include <vector>
#include <queue>
// AOE - Activity On Edge
// 注: 本题的图是一个AOE网(无环有向图)，每条边是带工程耗时的
// AOE网终，边表示活动，顶点表示事件
// 指向【同一个顶点V】的多条边就代表【并行进行的活动】，这些边代表的活动全进行完后才能让顶点V代表的事件开始
using namespace std;

typedef struct AdjNode *AdjList;
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
    // 运算符重载，用于堆排序进行升序排序
    bool operator>(const Edge &activity) const
    {
        return vFrom > activity.vFrom;
    }
};

// 因为是无环有向图，本题比较适合用邻接表
class Graph
{
private:
    short vertexNum;                                             // 顶点数
    vector<AdjList> lists;                                       // 邻接表
    vector<short> topoSeq;                                       // 拓扑排序列表
    priority_queue<Edge, vector<Edge>, greater<Edge>> criticalA; // 关键活动优先队列
    int criticalCost;                                            // 关键活动总开销
public:
    // 根据顶点数来进行初始化
    Graph(short vertexNum)
    {
        lists.resize(vertexNum);
        topoSeq.resize(vertexNum);
        for (int i = 0; i < vertexNum; i++)
        {
            // 初始化链表头节点
            lists[i] = new AdjNode();
            lists[i]->cost = -1;
            lists[i]->vertex = 0; // 头节点vertex储存这个顶点的【入度】
            lists[i]->next = NULL;
        }
        this->vertexNum = vertexNum;
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
    // 判断有向图终是否没有环路，采用拓扑排序Topological Order
    bool noCycle()
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
        while (!criticalA.empty())
        {
            Edge edge = criticalA.top();
            // 记得输出的时候顶点要还原成题目要求的编号
            cout << edge.vFrom + 1 << "->" << edge.vTo + 1 << "\n";
            criticalA.pop();
        }
    }
    // 找到关键路径点和关键活动
    void findCritical()
    {
        vector<int> earliest(vertexNum, 0); // 储存顶点最早发生时间的数组
        vector<int> latest(vertexNum, -1);  // 储存顶点最晚发生时间的数组，默认值用-1代表无穷
        // 依据拓扑排序来推算顶点
        short firstV = topoSeq[0];            // 拓扑排序首个顶点(源点)
        short lastV = topoSeq[vertexNum - 1]; // 拓扑排序最后顶点(汇点)
        earliest[firstV] = 0;                 // 源点的最早发生时间=0
        // 接下来从源点开始推算最早发生时间
        for (int i = 0; i < vertexNum; i++)
        {
            // 取出拓扑排序中第i个顶点的邻接表
            short currVertex = topoSeq[i];
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
        // 汇点的最晚发生时间等于其最早发生时间，从汇点开始倒推出所有顶点的最迟发生时间
        latest[lastV] = earliest[lastV];
        // 这里从拓扑序列倒数第二个顶点开始，因为倒数第一个顶点就是汇点，已知了
        for (int i = vertexNum - 2; i >= 0; i--)
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
        // 寻找所有关键顶点对应的关键活动，加入到优先队列中以便输出
        for (short i = 0; i < vertexNum; i++)
        {
            // 如果是关键顶点
            if (earliest[i] == latest[i])
            {
                AdjList curr = lists[i]->next; // 取出关键顶点的邻接表
                while (curr)
                {
                    // 如果这个邻接点是关键顶点，就把这两点形成的边加入到优先队列
                    if (earliest[curr->vertex] == latest[curr->vertex])
                    {
                        criticalA.push(Edge{i, curr->vertex});
                        break;
                    }
                    curr = curr->next;
                }
            }
        }
        // 拓扑排序最后一个顶点的最早发生时间(也是最晚发生时间)就是关键活动的总开销
        criticalCost = earliest[lastV];
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
    if (G.noCycle())
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

    本题难就难在要把情况考虑周全: 

        1. AOE图中有几个【起点(源点)】和【几个终点(汇点)】? 如果AOE图的源点/汇点不止一个，就需要【人工加入】顶点来【让图中的源点/汇点只有一个】。

            * 比如有多个起点，我添加一个顶点让其【指向所有的起点，边权为0】，然后把这个顶点作为源点即可。
            * 多个终点的话，就让【所有终点】指向人工加入的顶点，把这个顶点作为汇点即可。

        2. 关键路径有几条？这个题目中给出的样例的关键路径只有一条，但是实际上可能出现【多条关键路径】的情况

            * 这种情况往往是图中有几个 活动持续时间(边权) 是相同的。

            * 比如我把这题给的样例中的 1->3 这条边的权重改成6，就会出现【两条关键路径】:

                1->2->4->6->7 
                1->3->4->6->7

                (可以在草稿纸上推算一下，确实如此)

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