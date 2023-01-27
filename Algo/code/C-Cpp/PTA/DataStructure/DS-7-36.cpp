#include <cstdio>
#include <vector>
#include <queue>
#define INF 100000

using namespace std;

typedef vector<vector<bool>> Graph;

// 优先队列结点
struct QNode
{
    int vertex; // 顶点下标
    int dist;   // 距离起点的路径总长度
    bool operator>(const QNode &node) const
    {
        return dist > node.dist;
    }
};

Graph G;                // 定义图
int vertexNum, edgeNum; // 顶点数和边数

double compactCentral(int centerV); // 计算某个顶点的紧密度中心性

int main()
{
    scanf("%d %d", &vertexNum, &edgeNum);
    G.resize(vertexNum, vector<bool>(vertexNum, false)); // 初始化图
    // 读入边
    for (int i = 0; i < edgeNum; i++)
    {
        int v1, v2;
        scanf("%d %d", &v1, &v2);
        v1--;
        v2--; // 题目顶点编号为1~N,这里转换为0~N-1
        G[v1][v2] = true;
        G[v2][v1] = true;
    }
    // 计算紧密度中心性
    int count;
    scanf("%d", &count);
    for (int i = 0; i < count; i++)
    {
        int centerV;
        scanf("%d", &centerV); // 待计算的顶点
        centerV--;             // 转换为0~N-1的编号
        // 输出时还得转换为1~N的编号
        printf("Cc(%d)=%.2lf\n", centerV + 1, compactCentral(centerV));
    }
    return 0;
}

// 计算紧密度中心性,依赖Dijkstra
// 因为图是无权的，每条边默认长为1
double compactCentral(int centerV)
{
    priority_queue<QNode, vector<QNode>, greater<QNode>> pq;
    vector<int> distance(vertexNum, INF);   // 初始距离全为无穷
    vector<bool> visited(vertexNum, false); // 访问数组
    pq.push(QNode{centerV, 0});
    while (!pq.empty())
    {
        int currV = pq.top().vertex;
        int currDist = pq.top().dist;
        pq.pop();
        // 如果这个顶点已被访问，就不再继续
        if (visited[currV])
            continue;
        visited[currV] = true;
        // 更新距离数组
        for (int i = 0; i < vertexNum; i++)
        {
            // 跳过不与currV相连，或者已被访问的顶点
            if (visited[i] || !G[currV][i])
                continue;
            // 本图无权，那我就默认每条边长度为1
            int newDist = currDist + 1;
            // 如果经过currV到i顶点，总路径能更短，就更新
            if (newDist < distance[i])
            {
                distance[i] = newDist;
                pq.push(QNode{i, newDist}); // 加入队列
            }
        }
    }
    // Dijkstra算法结束，计算平均值的倒数
    int denominator = 0; // 分母-所有最短路径和
    for (int i = 0; i < vertexNum; i++)
    {
        if (i == centerV) // 中心顶点不算在内
            continue;
        denominator += distance[i];
    }
    // 计算紧密度中心性
    return (double)(vertexNum - 1) / denominator;
}

/*
    本题用优先队列辅助的Dijkstra算法可以快速解决，算是比较常规的题了，就不多赘述了。

    值得一提的是本题给的是【无权】的无向图，因此我【默认所有边的权都是1】，以便计算最短路径。

        - SomeBottle 2023.1.27
*/

/*

*/