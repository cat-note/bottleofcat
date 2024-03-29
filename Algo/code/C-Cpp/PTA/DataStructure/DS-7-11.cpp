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
        clearCache();
        delete criticalA;
    }
    // 清除关键活动占用的内存空间
    void clearCache()
    {
        EdgeList curr = criticalA->next;
        while (curr)
        {
            EdgeList next = curr->next;
            delete curr;
            curr = next;
        }
        criticalCost = 0;
        criticalA->next = NULL;
    }
    // 加入一条from顶点指向to顶点的有向边
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
    // 进行拓扑排序Topological Order，顺便判断有向图中是否没有环路
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
            // 寻找活动的插入地点
            if (node->vFrom >= curr->next->vFrom)
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
        clearCache();
        vector<int> earliest(vertexNum, 0); // 储存顶点最早发生时间的数组
        vector<int> latest(vertexNum, -1);  // 储存顶点最晚发生时间的数组，默认值用-1代表无穷
        // 依据拓扑排序来推算顶点
        // 接下来从源点开始推算【最早发生时间】
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
        // 这里从拓扑序列倒数第一个顶点开始，倒推顶点【最迟发生时间】
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
        // 寻找所有关键顶点对应的【关键活动】，加入到有序链表中以便输出
        for (short i = 0; i < vertexNum; i++)
        {
            // 如果是关键顶点
            if (earliest[i] == latest[i])
            {
                AdjList curr = lists[i]->next; // 取出关键顶点的邻接表
                while (curr)
                {
                    int activityEarliest = earliest[i];                     // 这个活动的最早开始时间
                    int activityLatest = latest[curr->vertex] - curr->cost; // 这个活动的最迟开始时间
                    bool isManualVertex = lists[curr->vertex]->cost;        // 头节点的cost用于标记这个顶点是不是人工汇点
                    // 1. 这个邻接点【不是人工汇点】
                    // 2. 这个邻接点是关键顶点
                    // 3. 连接这两点的活动是【关键活动】，关键活动的最早开始时间=最迟开始时间
                    if (!isManualVertex && earliest[curr->vertex] == latest[curr->vertex] && activityEarliest == activityLatest)
                    {
                        pushActivity(i, curr->vertex); // 把这两点形成的边加入到有序链表
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

        3. 任务调度不可行: 图中有环，拓扑排序过程就可以判断是否有环

        4. 输出中，起点编号相同时，与输入时任务的顺序相反:

            * 这种情况出现在【有多条关键路径】的时候，比如:

                - 输入时我的输入顺序是:
                    1->2
                    1->3

                - 输出时若1-2,1-3都是关键活动，且起点编号相同，那么输出就是:
                    1->3
                    1->2


    💡 对于第1点，写程序时要注意以下几点:

        - 计算最早发生时间前，要扫描拓扑序列，找到【所有入度为0】的顶点，这些顶点【都是起点】，把它们的最早发生时间全标为0

            * 对于起点，程序中我们并不需要加入人工起点，只需要找到所有起点即可（得益于拓扑排序）

        - 计算最迟发生时间时，也要扫描拓扑序列，找到【所有出度为0】的顶点，对于有多个终点的情况，需要人工添加一个汇点，让【多个终点指向同一个汇点，边权为0】，最终输出关键活动时不输出这个人工添加的点即可。

        - 关键活动的耗时决定了【整个工程的工期】，【汇点】的最早/最迟发生时间就是整个工程完成的时间

    💡 对于第2点，本题只要求输出【关键活动】，因此并不算很难:

        1. 先计算每个顶点(事件)的【最早发生时间】和【最迟发生时间】。

        2. (最早发生时间=最迟发生时间)的顶点就是关键路径中的点，这里我就暂且叫它们【关键顶点】

        3. 找出关键活动:

            - 扫描每个关键顶点 Vc 的【所有邻接点】，对于每个邻接点 Va:

                a. 计算(Vc和Va间活动的最早发生时间) = (Vc的最早发生时间)
                b. 计算(Vc和Va间活动的最迟发生时间) = (Va的最迟发生时间) - (Va和Vc之间活动的耗时)

            - 只要 (Vc和Va间活动的最早发生时间) = (Vc和Va间活动的最迟发生时间)，那么Vc和Va间的活动就是【关键活动】，加入到输出队列中。

    💡 关于第4点，我在这个题解中采用了【邻接表】储存图，且在插入新的边时采用【头插法】，这样就自然而然有了(起点相同时倒序输出)的功效:

        - 比如我的输入序列是
            1->2
            1->3

        - 按头插法插入链表是这样的:
            [1] -> 3 -> 2

        - 最后在输出的时候，自然而然就是先扫描到邻接点3，再扫描到邻接点2了，因而能输出:
            1->3
            1->2

    ----------------------------------------
    以下是AOE图的一些基本知识。

        * 关于AOE网有一篇很好的入门文章: https://zhuanlan.zhihu.com/p/170603727

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

    ----------------------------------------------

        - SomeBottle 2023.1.7
*/
/*
7-11 关键活动
假定一个工程项目由一组子任务构成，子任务之间有的可以并行执行，有的必须在完成了其它一些子任务后才能执行。“任务调度”包括一组子任务、以及每个子任务可以执行所依赖的子任务集。

比如完成一个专业的所有课程学习和毕业设计可以看成一个本科生要完成的一项工程，各门课程可以看成是子任务。有些课程可以同时开设，比如英语和C程序设计，它们没有必须先修哪门的约束；有些课程则不可以同时开设，因为它们有先后的依赖关系，比如C程序设计和数据结构两门课，必须先学习前者。

但是需要注意的是，对一组子任务，并不是任意的任务调度都是一个可行的方案。比如方案中存在“子任务A依赖于子任务B，子任务B依赖于子任务C，子任务C又依赖于子任务A”，那么这三个任务哪个都不能先执行，这就是一个不可行的方案。

任务调度问题中，如果还给出了完成每个子任务需要的时间，则我们可以算出完成整个工程需要的最短时间。在这些子任务中，有些任务即使推迟几天完成，也不会影响全局的工期；但是有些任务必须准时完成，否则整个项目的工期就要因此延误，这种任务就叫“关键活动”。

请编写程序判定一个给定的工程项目的任务调度是否可行；如果该调度方案可行，则计算完成整个工程项目需要的最短时间，并输出所有的关键活动。
-----------------------
输入格式:
    输入第1行给出两个正整数N(≤100)和M，其中N是任务交接点（即衔接相互依赖的两个子任务的节点，例如：若任务2要在任务1完成后才开始，则两任务之间必有一个交接点）的数量。交接点按1~N编号，M是子任务的数量，依次编号为1~M。随后M行，每行给出了3个正整数，分别是该任务开始和完成涉及的交接点编号以及该任务所需的时间，整数间用空格分隔。
输出格式:
    如果任务调度不可行，则输出0；否则第1行输出完成整个工程项目需要的时间，第2行开始输出所有关键活动，每个关键活动占一行，按格式“V->W”输出，其中V和W为该任务开始和完成涉及的交接点编号。关键活动输出的顺序规则是：任务开始的交接点编号小者优先，起点编号相同时，与输入时任务的顺序相反。
输入样例:
    7 8
    1 2 4
    1 3 3
    2 4 5
    3 4 3
    4 5 1
    4 6 6
    5 7 5
    6 7 2
输出样例:
    17
    1->2
    2->4
    4->6
    6->7
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/