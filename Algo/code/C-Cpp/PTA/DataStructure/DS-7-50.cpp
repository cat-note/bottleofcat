#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

// 实现并查集
class UnionFind
{
private:
    vector<int> counts;  // 储存每个集合的元素数
    vector<int> parents; // 储存每个节点的父节点下标
public:
    UnionFind(int size)
    {
        counts.resize(size, 1); // 初始每个集合一个元素
        parents.resize(size);
        for (int i = 0; i < size; i++) // 初始每个元素自身构成一个集合
            parents[i] = i;
    }
    // 寻找节点所在树的根节点下标
    // 顺带做路径压缩
    int find(int i)
    {
        return parents[i] == i ? i : parents[i] = find(parents[i]);
    }
    // 并操作
    void merge(int a, int b)
    {
        int aInd = find(a);
        int bInd = find(b);
        if (aInd == bInd) // 如果两个元素已经在同一个集合中就不予操作
            return;
        // 倾向于把小集合并入大集合中
        if (counts[aInd] < counts[bInd])
        {
            parents[aInd] = bInd;
            counts[bInd] += counts[aInd];
        }
        else
        {
            parents[bInd] = aInd;
            counts[aInd] += counts[bInd];
        }
    }
};

// 代表一条路
struct Road
{
    int v0;   // 顶点1
    int v1;   // 顶点2
    int cost; // 成本
    Road(int v1, int v2, int cost)
    {
        this->v0 = v1 - 1; // 转换为0~N-1编号
        this->v1 = v2 - 1;
        this->cost = cost;
    }
    bool operator>(const Road &rd) const
    {
        return cost > rd.cost;
    }
};

int main()
{
    int vertexNum; // 顶点数（村庄数）
    scanf("%d", &vertexNum);
    // N(N-1)/2条边，很明显是完全图
    int edgeNum = vertexNum * (vertexNum - 1) / 2; // 边数
    // 开始读入边
    priority_queue<Road, vector<Road>, greater<Road>> pq; // 优先队列来存边
    UnionFind uf(vertexNum);                              // 初始化并查集
    for (int i = 0; i < edgeNum; i++)
    {
        int v1, v2, cost, built;
        scanf("%d %d %d %d", &v1, &v2, &cost, &built);
        if (built) // 如果这条路已经修好就在并查集中对顶点进行合并
            uf.merge(v1 - 1, v2 - 1);
        else // 否则就交由Kruskal算法实现
            pq.push(Road(v1, v2, cost));
    }
    // Kruskal算法寻找最小生成树
    int totalCost = 0;  // 总开销
    int edgeAddNum = 0; // 加入图中的边数
    while (!pq.empty())
    {
        const Road &rd = pq.top();
        // 如果添加了这条边会导致图中出现环路，就不添加这条边
        if (uf.find(rd.v0) != uf.find(rd.v1))
        {
            uf.merge(rd.v0, rd.v1); // 把两个顶点加入一个集合中
            edgeAddNum++;           // 一条新边加入图中
            totalCost += rd.cost;
        }
        pq.pop();
        // N个顶点的最小生成树有N-1条边，因此达到N-1条边时就可以结束循环
        if (edgeAddNum >= vertexNum - 1)
            break;
    }
    printf("%d", totalCost);
    return 0;
}

/*
    最小生成树，但这题有坑。
    我一开始把【判断路是否已经建成】的语句写到Kruskal算法中了，这是不符合题意的。

*/