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
        if (built) // 💡如果这条路已经修好就在并查集中对顶点进行合并，相当于把修好的路先加入
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
        // N个顶点的最小生成树有N-1条边，因此【最多】往图中加入了N-1条边时就可以结束循环
        if (edgeAddNum >= vertexNum - 1)
            break;
    }
    printf("%d", totalCost);
    return 0;
}

/*
    “整个地区任何两个城镇间都可以实现快速交通（但不一定有直接的快速道路相连，只要互相间接通过快速路可达即可）”

    很容易能看出这题考察的是最小生成树算法(常用的有Kruskal和Prim)，我采用了Kruskal算法。
    ----------------------------------------
    但这题其实还埋了一个坑——【有的路已经建好了】。

    我一开始把【判断路是否已经建成】的语句写到Kruskal算法的【是否构成环路】的判断语段中了，这是不符合题意的:

        - 因为在Kruskal算法往图中加入边时会【先判断边的两端顶点是否已经连通】，如果已经连通就会【忽略这条边】

            而如果我把【判断路是否已经建成】写在这里面的话，如果【已经存在的边】的【两端顶点事先已经被连通】，那么Kruskal会【忽略已经存在的边】，因此会导致运算错误。

        - 总而言之就是【已经建好的路】其实就相当于【已经加入图中的边】，而如果不事先把这些边加入图中，会影响到Kruskal算法的发挥。

    正确的写法应该是【在Kruskal算法前】，先把【已经存在的边都加入】，只把【没有放进图中的边】交给Kruskal。

    -------------------------------------
    💡本题我收获的教训:
        应该只把【没有加入图中的边】交给最小生成树算法来处理。

        - SomeBottle 2023.2.7
*/

/*
7-50 畅通工程之局部最小花费问题
    某地区经过对城镇交通状况的调查，得到现有城镇间快速道路的统计数据，并提出“畅通工程”的目标：使整个地区任何两个城镇间都可以实现快速交通（但不一定有直接的快速道路相连，只要互相间接通过快速路可达即可）。现得到城镇道路统计表，表中列出了任意两城镇间修建快速路的费用，以及该道路是否已经修通的状态。现请你编写程序，计算出全地区畅通需要的最低成本。

输入格式:
    输入的第一行给出村庄数目N (1≤N≤100)；随后的N(N−1)/2行对应村庄间道路的成本及修建状态：每行给出4个正整数，分别是两个村庄的编号（从1编号到N），此两村庄间道路的成本，以及修建状态 — 1表示已建，0表示未建。
输出格式:
    输出全省畅通需要的最低成本。
输入样例:
    4
    1 2 1 1
    1 3 4 0
    1 4 1 1
    2 3 3 0
    2 4 2 1
    3 4 5 0
输出样例:
    3
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/