/*
克鲁斯卡尔算法实现寻找最小生成树
其中判断无环的部分采用了时间复杂度较大的DFS，因此在测试最多顶点数(1000)的图时会TLE（超时）
并查集咱还没玩清楚，采用并查集说不定能满足题目要求。
*/
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// 优先队列中的节点
struct QNode
{
    short v0; // 顶点1
    short v1; // 顶点2
    int cost; // 连接这两个顶点的边权
    bool operator>(const QNode &node) const
    {
        return cost > node.cost;
    };
};

class Graph
{
public:
    // 根据顶点数初始化图
    Graph(int vertexNum)
    {
        G.resize(vertexNum);
        visited.resize(vertexNum);
        for (int i = 0; i < vertexNum; i++)
            G[i].resize(i, false);
    }

    // 标记两点间是否有边
    void set(short v0, short v1, bool hasEdge)
    {
        if (v0 == v1) // 防止非法操作
            return;
        if (v1 > v0)
            G[v1][v0] = hasEdge;
        else
            G[v0][v1] = hasEdge;
    }

    // 从startV顶点开始进行DFS
    bool noCycle(short startV)
    {
        // 重置访问数组
        for (int i = 0, len = visited.size(); i < len; i++)
            visited[i] = false;
        return DFS(startV);
    }

private:
    vector<vector<bool>> G; // 储存无向图的邻接矩阵
    vector<bool> visited;   // 用于DFS的访问数组

    // DFS判断是否无环 (当前遍历到的顶点)
    bool DFS(short v0, short previous = 0)
    {
        // 如果访问了被访问过的顶点，就是有环路了
        if (visited[v0])
            return true;
        visited[v0] = true; // 标记已访问
        // DFS进入v0的邻接点
        // 下三角的遍历要分为两道
        for (int i = 0; i < v0; i++)
            if (G[v0][i])
            { // 有v0到邻接的i顶点的边
                // v0的邻接点中有已经访问过的点，且这个点不是上一次访问的，这就出现了环
                if (visited[i] && i != previous)
                    return false;
                if (!DFS(i, v0)) // 只要出现了环就返回false
                    return false;
            }
        for (int i = v0 + 1, len = G.size(); i < len; i++)
            if (G[i][v0])
            { // 有v0到邻接的i顶点的边
                if (visited[i] && i != previous)
                    return false;
                if (!DFS(i, v0)) // 只要出现了环就返回false
                    return false;
            }
        return true;
    }
};

int main()
{
    // 优先队列(小根堆实现)，用来实现堆排序
    priority_queue<QNode, vector<QNode>, greater<QNode>> pQueue;
    int townNum, wayNum; // 城镇数量，道路数量
    cin >> townNum >> wayNum;
    // 访问数组，标记每个顶点是否被访问
    vector<bool> visits(townNum, false);
    // 接下来读入边
    for (int i = 0; i < wayNum; i++)
    {
        short v1, v2; // 顶点<=1000，short类型足够
        int cost;
        cin >> v1 >> v2 >> cost;
        // 加入到优先队列中，这其实就是堆排序过程
        // 这里的顶点编号从0开始，因此要减1
        v1--;
        v2--;
        pQueue.push(QNode{v1, v2, cost});
    }
    // 初始化图
    Graph G(townNum);
    // 总成本
    int totalCost = 0;
    // 记录最后加入的边数
    int totalEdge = 0;
    // 不停弹出优先队列队首(堆顶)元素，这些元素是按边权升序弹出的
    while (!pQueue.empty())
    {
        short v0 = pQueue.top().v0;
        short v1 = pQueue.top().v1;
        int cost = pQueue.top().cost;
        pQueue.pop(); // 弹出队首
        // 先把v0到v1的边加入到图中
        G.set(v0, v1, true);
        // 如果加入v0到v1的边后出现了环路(从v0开始进行DFS以判断)
        if (!G.noCycle(v0))
        {
            // 撤销v0到v1的边，我们就当无事发生
            G.set(v0, v1, false);
        }
        else
        {
            totalCost += cost; // 没有出现环路，就把v0到v1顶点的成本加入总成本
            totalEdge++;       // 记录边数
        }
    }
    // n个顶点的最小生成树必有n-1条边，没有的话就是非连通
    if (totalEdge != townNum - 1)
        totalCost = -1;
    cout << totalCost;
    return 0;
}