/*
注: 这个是错误题解，我最开始理解错了题目意思，写成了Dijkstra!
但是没关系捏，稍微改改就能变成最小生成树算法Prim了
*/
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
// 初始化优先队列，用于辅助Dijkstra
// 优先队列元素是QNode, greater
priority_queue<QNode, vector<QNode>, greater<QNode>> pQueue;

// 城镇数量，道路数量
int townNum, wayNum;

// 函数声明
void AddEdge(int v1, int v2, int cost); // 添加v1至v2的边，边权为cost
int Dijkstra(int startV);               // Dijkstra算法

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
    // 对每个顶点进行Dijkstra算法，找到总成本最小的情况
    int minCost = -1;
    for (int i = 0; i < townNum; i++)
    {
        int cost = Dijkstra(i);
        if (cost == -1)
        {
            // 图不连通，不足以保证畅通
            minCost = -1;
            break;
        }
        if (minCost == -1 || cost < minCost)
            minCost = cost;
    }
    cout << minCost;
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

// 从顶点startV(下标从0开始)开始进行Dijkstra算法
// 本题只在乎成本，而不在乎路径，因此也不需要paths数组
// Dijkstra在队列辅助下能优化到复杂度O(nlogn)
int Dijkstra(int startV)
{
    // 初始化成本数组(Dijkstra的距离数组)
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
        // 如果已经访问过，就跳到下一迭代
        if (visits[minV])
            continue;
        // 标记已经访问到minV顶点
        visits[minV] = true;
        for (int i = 0; i < townNum; i++)
        {
            // 假设i是minV顶点的邻接点，计算i顶点和起始顶点间的成本
            // i顶点和起始顶点startV间的成本 = minV和startV间的成本 + minV和i间的成本
            int newCost = costs[minV] + G[minV][i];
            // 如果新算出来的成本比已有的i顶点成本更低就更新
            // 前提: i顶点未被访问
            if (newCost < costs[i])
            {
                costs[i] = newCost;
                // 将顶点下标和新的成本加入到队列中
                pQueue.push(QNode{i, newCost});
            }
        }
    }
    // 计算所有村落都连通的总成本
    int totalCost = 0;
    for (int i = 0; i < townNum; i++)
    {
        if (visits[i])
        {
            // 如果i顶点被访问过
            totalCost += costs[i];
        }
        else
        {
            // 但凡有一个顶点未被访问过，这个图就是不连通的
            totalCost = -1;
            break;
        }
    }
    return totalCost;
}