#include <cstdio>
#include <vector>
#include <queue>
#define INF 100000

using namespace std;

/*
    通过样例进行分析，本题应该有两个要求:
        1. 尽快赶往事发地(找最短路径)
        2. 召集尽可能多的救援队(有多条最短路径时找救援队最多的)
    注意，这里需要把多条最短路径记录下来，同时还要记录路径，需要paths数组
    题目指定了起点和终点，这题还是比较适合Dijkstra的
*/

typedef vector<int> VecInt;
typedef vector<VecInt> Graph;

// 优先队列中储存的节点
struct VNode
{
    int vertex;
    int cost; // 路径总开销
    bool operator>(const VNode &node) const
    {
        return cost > node.cost;
    }
};

// distance数组储存的节点
struct DNode
{
    int cost;  // 路径总开销
    int power; // 总人力
};

void printPath(VecInt &paths, int startV, int endV); // 打印路径

int main()
{
    Graph G;                              // 定义图
    VecInt manpower;                      // 各城市的人手
    int vertexNum, edgeNum, startV, endV; // 顶点数,边数,开始顶点,目标顶点
    scanf("%d %d %d %d", &vertexNum, &edgeNum, &startV, &endV);
    // 初始化图
    G.resize(vertexNum, VecInt(vertexNum, INF));
    // 初始化人手
    manpower.resize(vertexNum);
    // 读入人手数
    for (int i = 0; i < vertexNum; i++)
        scanf("%d", &manpower[i]);
    // 读入图
    for (int i = 0; i < edgeNum; i++)
    {
        int v1, v2, cost;
        scanf("%d %d %d", &v1, &v2, &cost);
        G[v1][v2] = cost;
        G[v2][v1] = cost;
    }
    // Dijkstra
    // 实际上路径数组储存的是每个顶点的【前驱顶点】
    VecInt paths(vertexNum, -1);                             // 路径数组
    VecInt pathNum(vertexNum, 0);                            // 记录各顶点为止的路径条数
    vector<DNode> distance(vertexNum, DNode{INF, 0});        // 距离数组
    vector<bool> visited(vertexNum, false);                  // 是否被访问
    priority_queue<VNode, vector<VNode>, greater<VNode>> pq; // 辅助用的优先队列
    pq.push(VNode{startV, 0});                               // 把起始顶点先加入
    distance[startV].cost = 0;                               // 起始顶点总路径开销为0
    distance[startV].power = manpower[startV];               // 起始顶点的人力
    pathNum[startV] = 1;                                     // 起始顶点的路径条数有1条
    while (!pq.empty())
    {
        int currV = pq.top().vertex; // 当前distance数组中总路径最短的点
        int currCost = pq.top().cost;
        pq.pop();
        // 如果此顶点已被访问就不再继续
        if (visited[currV])
            continue;
        visited[currV] = true; // 标记已被访问，此时确定了currV顶点的最短路径
        // 如果到终点的最短路径已被确定就无需继续
        if (currV == endV)
            break;
        // 对其他顶点的最短路径进行更新
        for (int i = 0; i < vertexNum; i++)
        {
            // 顶点被访问过或者一条边不存在就跳过本次迭代
            if (visited[i] || G[currV][i] == INF)
                continue;
            int newCost = currCost + G[currV][i];               // 经过currV到i所需的开销
            int newPower = distance[currV].power + manpower[i]; // 所需人力
            // 如果新的开销能更小，就更新
            // 如果开销没变，但能集结更多人力，也更新
            if (newCost < distance[i].cost)
            {
                // 到currV为止有pathNum[currV]条路径，i顶点新旧总开销不相同时，currV到i也只有唯一路径
                // 但是当新旧开销相同，也就是newCost=distance[i].cost时，说明有多条路径出现
                pathNum[i] = pathNum[currV];
                distance[i].cost = newCost;
                distance[i].power = newPower;
                // 更新路径,paths数组储存的是各顶点的【前驱顶点】
                paths[i] = currV; // 经过currV到i能有最短路径，i的前驱就是currV
                // 加入队列
                pq.push(VNode{i, newCost});
            }
            else if (newCost == distance[i].cost)
            {
                pathNum[i] += pathNum[currV];
                if (newPower > distance[i].power)
                {
                    distance[i].power = newPower;
                    // 更新路径,paths数组储存的是各顶点的【前驱顶点】
                    paths[i] = currV; // 经过currV到i能有最短路径，i的前驱就是currV
                }
            }
        }
    }
    printf("%d %d\n", pathNum[endV], distance[endV].power);
    // 最后打印路径，借助一下递归，因为paths数组储存的是前驱
    printPath(paths, startV, endV);
    return 0;
}

// 打印从startV到endV的最短路径
void printPath(VecInt &paths, int startV, int endV)
{
    if (endV == startV)
    {
        printf("%d", endV);
        return;
    }
    else
    {
        printPath(paths, startV, paths[endV]);
    }
    printf(" %d", endV);
}

/*
自己想的一个测试例，用来测试【最短路径的条数】和【救援队的数量】的输出是否正确:
(对应【测试点1】:5条不同的最短路的情况)
5 7 0 4
10 30 50 40 20
0 1 1
1 3 1
1 2 2
0 2 3
2 3 1
2 4 3
0 4 6

*/