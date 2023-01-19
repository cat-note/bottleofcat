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
        }
        pq.pop(); // 弹出栈顶
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
    
*/