#include <iostream>
#include <vector>
#include <queue>
#define INF 1e6

using namespace std;

typedef vector<int> VecInt;
typedef vector<VecInt> Graph;

// 优先队列内的节点
struct QNode
{
    int vertex; // 顶点下标
    int cost;   // 顶点和起始点间的成本
    // 运算符重载, 常函数
    // 这里主要用于堆元素的比较(std::greater内会用>运算符比较QNode)，以维持堆序性
    // - node是父节点，本节点是新插入的节点
    // - 如果 本节点的路径成本 > 父节点的，满足小根堆，返回true, 不会进行交换
    // - 如果 本节点的路径成本 < 父节点的，不满足小根堆堆序性，返回false, 进行堆中节点交换
    // 这里参数用的是QNode引用类型，免除拷贝带来的时间消耗
    bool operator>(const QNode &node) const
    {
        return cost > node.cost;
    };
};

Graph G; // 初始化图，本题仍然是无向图
// 初始化优先队列，用于辅助Prim
// 优先队列元素是QNode, greater
priority_queue<QNode, vector<QNode>, greater<QNode>> pQueue;

// 城镇数量，道路数量
int townNum, wayNum;

// 函数声明
void AddEdge(int v1, int v2, int cost); // 添加v1至v2的边，边权为cost
int Prim(int startV);                   // Prim算法

int main()
{
    cin >> townNum >> wayNum;
    // 初始化图
    G.resize(townNum);
    for (int i = 0; i < townNum; i++)
        G[i].resize(townNum, INF); // 初始所有村落未连通，成本无穷
    // 读入边权
    for (int i = 0; i < wayNum; i++)
    {
        int v1, v2, cost;
        cin >> v1 >> v2 >> cost;
        AddEdge(v1, v2, cost);
    }
    // 因为最小生成树的权是唯一的，所以对随便一个顶点进行Prim算法即可
    cout << Prim(0);
    return 0;
}

// v1,v2是1开始的城镇编号，即顶点; cost是成本，即边权
void AddEdge(int v1, int v2, int cost)
{
    // 无向图邻接矩阵对称
    // 注意转换为0开始的编号
    G[v1 - 1][v2 - 1] = cost;
    G[v2 - 1][v1 - 1] = cost;
}

// 从顶点startV(下标从0开始)开始进行Prim算法
// 本题只在乎最小生成树中所有边的总成本，而不在乎路径，因此也不需要paths数组
// Prim在优先队列辅助下能优化到复杂度O(nlogn)
int Prim(int startV)
{
    // 总成本
    int totalCost = 0;
    // 初始化成本数组(Prim的最小边数组，储存最小边的长度)
    VecInt costs(townNum, INF); // 默认成本全为无穷
    // 初始化访问数组
    vector<bool> visits(townNum, false);
    // 将起始顶点及其距离起始顶点的距离(0)加入队列
    pQueue.push(QNode{startV, 0});
    // 设定起始点的成本为0
    costs[startV] = 0;
    while (!pQueue.empty())
    {
        int minV = pQueue.top().vertex; // 获得成本最小顶点的下标
        pQueue.pop();                   // 弹出栈顶
        // 如果已经访问过minV就不往下执行，进入下一迭代
        // 这个判断得写在这里，如果写在下面入队的地方(第100行)，队中会有大量被访问过的元素，因而导致不必要的时间开销
        if (visits[minV])
            continue;
        // 标记已经访问到minV顶点
        visits[minV] = true;
        // 一旦访问了minV，说明可以确定minV的最短邻接边了，将其加入总和
        totalCost += costs[minV];
        for (int i = 0; i < townNum; i++)
        {
            // 假设i是minV顶点的邻接点，判断(minV连接i的这条边)的成本是否小于(顶点i之前那条边)的成本
            // 如果 minV <---> i 这条边成本比 之前i顶点那条边的成本 更低就更新
            // * 可以发现，Prim算法在每个顶点处关注的是【当前点最小的邻接边】，而Dijkstra关注的是【从起始点到当前点的最短路径】
            if (G[minV][i] < costs[i])
            {
                costs[i] = G[minV][i];
                // 将顶点下标和新的成本加入到队列中
                pQueue.push(QNode{i, G[minV][i]});
            }
        }
    }

    for (int i = 0; i < townNum; i++)
    {
        if (!visits[i])
        {
            // 但凡有一个顶点未被访问过，这个图就是不连通的
            totalCost = -1;
            break;
        }
    }
    return totalCost;
}

/*
    注意本题重在连通最小成本，而不是找最短路径

    怎么判断题目意思是求最短路径还是最小生成树？
*/