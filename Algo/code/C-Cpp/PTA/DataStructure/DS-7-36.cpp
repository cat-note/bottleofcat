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
    本题用【优先队列辅助的Dijkstra算法】可以高效解决，算是比较常规的题了，就不多赘述了。

    值得一提的是本题给的是【无权】的无向图，因此我【默认所有边的权都是1】，以便计算最短路径。

        - SomeBottle 2023.1.27
*/

/*
7-36 社交网络图中结点的“重要性”计算
    在社交网络中，个人或单位（结点）之间通过某些关系（边）联系起来。他们受到这些关系的影响，这种影响可以理解为网络中相互连接的结点之间蔓延的一种相互作用，可以增强也可以减弱。而结点根据其所处的位置不同，其在网络中体现的重要性也不尽相同。

    “紧密度中心性”是用来衡量一个结点到达其它结点的“快慢”的指标，即一个有较高中心性的结点比有较低中心性的结点能够更快地（平均意义下）到达网络中的其它结点，因而在该网络的传播过程中有更重要的价值。在有N个结点的网络中，结点vi的“紧密度中心性”Cc(vi)数学上定义为vi到其余所有结点vj(j≠i) 的最短距离d(vi,vj)的平均值的倒数：

        Cc(vi)=(N-1) / (所有最短路径d的求和)

    对于非连通图，所有结点的紧密度中心性都是0。

    给定一个无权的无向图以及其中的一组结点，计算这组结点中每个结点的紧密度中心性。

输入格式:
    输入第一行给出两个正整数N和M，其中N（≤10^4）是图中结点个数，顺便假设结点从1到N编号；M（≤10^5）是边的条数。随后的M行中，每行给出一条边的信息，即该边连接的两个结点编号，中间用空格分隔。最后一行给出需要计算紧密度中心性的这组结点的个数K（≤100）以及K个结点编号，用空格分隔。
输出格式:
    按照Cc(i)=x.xx的格式输出K个给定结点的紧密度中心性，每个输出占一行，结果保留到小数点后2位。
输入样例:
    9 14
    1 2
    1 3
    1 4
    2 3
    3 4
    4 5
    4 6
    5 6
    5 7
    5 8
    6 7
    6 8
    7 8
    7 9
    3 3 4 9
输出样例:
    Cc(3)=0.47
    Cc(4)=0.62
    Cc(9)=0.35
代码长度限制
    16 KB
时间限制
    20000 ms
内存限制
    64 MB
*/