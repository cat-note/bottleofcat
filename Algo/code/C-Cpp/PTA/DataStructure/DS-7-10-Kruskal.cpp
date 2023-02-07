#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

// 并查集
class UnionFind
{
private:
    int size;
    vector<int> parents; // 储存各节点的父节点
    vector<int> counts;  // 记录集合中的元素数
public:
    UnionFind(int size)
    {
        this->size = size;
        parents.resize(size);
        counts.resize(size, 1); // 初始每个元素自己就是集合，集合中都只有1个元素
        for (int i = 0; i < size; i++)
            parents[i] = i; // 初始各节点的父节点就是自己，每个节点是一个独立的集合
    }
    // 查找操作，找到树根
    int find(int index)
    {
        // 如果父节点就是自己，说明这个树根就是自己
        // 否则进入递归，顺便做路径压缩
        return parents[index] == index ? index : parents[index] = find(parents[index]);
    }
    // 合并操作
    void merge(int a, int b)
    {
        int aRoot = find(a);
        int bRoot = find(b);
        if (aRoot == bRoot) // 属于同一个集合就不予操作
            return;
        // 为了一定程度上维持复杂度，往往将元素少的集合并入元素多的集合
        if (counts[aRoot] > counts[bRoot])
        {
            parents[bRoot] = aRoot;
            counts[aRoot] += counts[bRoot];
        }
        else
        {
            parents[aRoot] = bRoot;
            counts[bRoot] += counts[aRoot];
        }
    }
};

// 一条道路
struct Road
{
    int start; // 开始地点
    int end;   // 结束地点
    int cost;  // 成本
    bool operator>(const Road &obj) const
    {
        return cost > obj.cost;
    }
};

int main()
{
    priority_queue<Road, vector<Road>, greater<Road>> pq; // 优先队列
    int townNum, roadNum;                                 // 城镇数和道路数目
    scanf("%d %d", &townNum, &roadNum);
    for (int i = 0; i < roadNum; i++)
    {
        Road road;
        // 读入边和边权
        scanf("%d %d %d", &road.start, &road.end, &road.cost);
        road.start--; // 题目中编号是1~N，但我这里处理是0~N-1
        road.end--;
        pq.push(road); // 加入优先队列
    }
    // 接下来就是Kruskal算法
    UnionFind uf(townNum); // 并查集
    int roadAddNum = 0;    // 加入图中的边数
    int totalCost = 0;     // 总成本
    while (!pq.empty())
    {
        const Road &curr = pq.top(); // 取出栈顶，这是当前成本最低的边
        // 如果在没加入这条边时，两个城镇已经在同一个集合中了
        // 说明加入了这条边就会形成环
        if (uf.find(curr.start) != uf.find(curr.end))
        {
            // 不会形成环就加入这条边，把两个点放到同一个集合中(合并)
            uf.merge(curr.start, curr.end);
            totalCost += curr.cost; // 计入总成本
            roadAddNum++;
        }
        pq.pop();                  // 弹出栈顶
        if (roadAddNum >= townNum) // N个顶点的图的最小生成树有N-1条边，达到N-1条边就可以跳出循环了。
            break;
    }
    // 如果图是连通的，最后所有顶点应该都和0号顶点在同一个集合中
    for (int i = 1; i < townNum; i++)
        if (uf.find(0) != uf.find(i)) // 但凡有顶点不在集合中就说明未连通
        {
            printf("-1"); // 有未被访问的就说明无法连通
            return 0;
        }
    printf("%d", totalCost);
    return 0;
}

/*
    Kruskal算法很好理解，只不过在【判断环路】这一块需要借助【并查集】这种数据结构。

    Kruskal算法的核心操作就是:

        1. 读入图中【所有边】，按照【边权】进行排序。
            * 本题我采用堆排序(优先队列)来实现，因为这种方式下可以边读入边排序。

        2. 从优先队列中不断取出【最优先】的一条边，【尝试】加入到图中，直至【队空】为止。
            * 本题中的最优先就是【成本最低】。

                a. 如果加入了这条边后【图中会出现环路】，就不加入这条边。

                b. 如果加入这条边后不会出现环路，就正常加入。
            ------------------------------
            环路判断往往采用的是【并查集】:

                - 最开始图中所有顶点没有边相连，每个顶点都是一个独立的集合，它们【互不相交】。

                - 每有一条边将两个顶点连接起来，就相当于把两个互不相交的集合【合并成一个集合】。

                - 如果在【合并前】发现【两个顶点已经处于同一集合】中，说明加入边会出现【环路】。

        3. 通过【并查集】检查【所有顶点是否在同一个集合】中，如果不是的，说明图未连通。
            * 随便取一个顶点，用并查集的find方法查询【其他顶点是否全都与这个顶点在同一个集合中】即可。

    【图】描述的是【顶点】的关系，而并查集的【集合】描述的是【元素】的关系，在判断无向图的连通性的这点上，二者颇有种不谋而合的感觉。

        - SomeBottle 2023.1.19
*/