#include <cstdio>
#include <vector>
#include <queue>
#define INF 100000

using namespace std;

typedef vector<int> VecInt;
typedef vector<VecInt> Graph;

// 优先队列中储存的节点，只存放顶点下标以及和排序相关的键值
struct QNode
{
    int vertex; // 顶点下标
    int cost;   // 路径总开销
    bool operator>(const QNode &node) const
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
    VecInt paths(vertexNum, -1);                             // 路径数组，储存的是每个顶点的【前驱顶点】
    VecInt pathNum(vertexNum, 0);                            // 记录【起点到顶点i】的最短路径条数pathNum[i]
    vector<DNode> distance(vertexNum, DNode{INF, 0});        // 距离数组，储存【起点到顶点i的最短距离】以及【起点到顶点i能号召到的人手】
    vector<bool> visited(vertexNum, false);                  // 储存各顶点是否被访问
    priority_queue<QNode, vector<QNode>, greater<QNode>> pq; // 辅助用的优先队列
    // 初始化
    pq.push(QNode{startV, 0});                 // 把起始顶点先加入
    distance[startV].cost = 0;                 // 起始顶点距离起点路径长为0 (开销为0)
    distance[startV].power = manpower[startV]; // 起始顶点的人手
    pathNum[startV] = 1;                       // 起始顶点默认最短路径条数只有一条
    while (!pq.empty())
    {
        int currV = pq.top().vertex; // 当前distance数组中【距离起点】路径长最短的点
        int currCost = pq.top().cost;
        pq.pop();
        // 如果此顶点已被访问就不再继续
        if (visited[currV])
            continue;
        visited[currV] = true; // 标记已被访问，此时确定了currV顶点的最短路径
        // 如果到终点的最短路径已被确定就无需继续
        if (currV == endV)
            break;
        // 对其他顶点的最短距离和路径进行更新
        for (int i = 0; i < vertexNum; i++)
        {
            // 顶点被访问过或者相应的一条边不存在就跳过本次迭代
            if (visited[i] || G[currV][i] == INF)
                continue;
            int newCost = currCost + G[currV][i];               // 从起点开始经过currV到i所需的路径长度
            int newPower = distance[currV].power + manpower[i]; // 从起点开始经过currV到i能号召到的人手
            // 如果新的路径长度能更小，就更新
            if (newCost < distance[i].cost)
            {
                // 此时从起点到i的最短路径【必须经过currV顶点】，currV的最短路径条数决定了这里的最短路径数
                pathNum[i] = pathNum[currV];
                distance[i].cost = newCost;
                distance[i].power = newPower;
                // 更新路径,paths数组储存的是各顶点的【前驱顶点】
                paths[i] = currV; // 经过currV到i能有最短路径，i的前驱就是currV
                // 加入队列
                pq.push(QNode{i, newCost});
            }
            else if (newCost == distance[i].cost)
            {
                // 此时从起点到i的最短路径【不止经过currV顶点】，需要把currV的最短路径条数累加到顶点i的最短路径数上
                pathNum[i] += pathNum[currV];
                if (newPower > distance[i].power) // 如果路径长度不更新，但能集结更多人手，就更新人手
                {
                    distance[i].power = newPower;
                    // 更新路径,paths数组储存的是各顶点的【前驱顶点】
                    paths[i] = currV; // 经过currV到i能有最短路径，i的前驱就是currV
                    // 这里没有更新路径长度，无需加入队列
                }
            }
        }
    }
    printf("%d %d\n", pathNum[endV], distance[endV].power);
    // 最后打印路径，借助一下递归，因为paths数组储存的是前驱，需要自底向上输出.
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
    通过样例进行分析，本题有两个基本要求:
        1. 尽快赶往事发地(找最短路径)
        2. 召集尽可能多的救援队(有多条最短路径时找救援队最多的)
    题目需要的结果是:
        1. 最短路径的条数
            - 需要额外一个pathNum对【到每个顶点】的最短路径进行统计
        2. 沿途能召集到的【总人手】
            - 需要额外对人手进行记录，这里我一并放在distance数组中。
        3. 最短路径【经过的顶点】
            - 通过paths数组记录每个顶点的【前驱】顶点，最后利用栈或者递归进行输出
    题目指定了起点和终点，这题还是比较适合Dijkstra的
    ----------------------------------
    💡!!!!!一大教训!!!!!!

        在采用【优先队列】进行辅助的时候，优先队列的节点中存放的值【只需要和排序有关！】，不要把一些参与计算的值都放进去。

        我最开始把【总人手数】也加到优先队列中了，然而总人手数【根本就不影响优先队列的堆序性】。
        结果就是因为我用优先队列储存了参与运算的总人手数，写程序的时候绕来绕去，【怎么算也算不对】，白白浪费了我很大一部分时间排查！！！！

        再强调一遍，用优先队列对Djikstra进行辅助时只需要【把与排序有关的键值】加入队列即可！！！不要给自己带来不必要的麻烦
    ----------------------------------
    本题可以说是题DS-7-9的升级版，既有【多因素的最短路径取舍】，也有【路径记录】，甚至还需要计算【相同长度的最短路径条数】。
    这里谈谈最后一项: 记录最短路径条数

        其实理解起来也不难，专门腾出一个数组 pathNum[i] 来储存 【从起点到i号顶点之间的最短路径条数】 即可。

        多条最短路径指的就是多条总长度相同，且在两顶点之间最短的路径。

        也就是说：

        1. 在Djikstra算法中每当【对一个顶点进行路径长度更新】时，发现新算出来的路径长度和其原本的路径长一致，就是【出现了多条最短路径】。

            这个时候用【前驱顶点的路径条数】加上【当前顶点的路径条数】，就是当前顶点新的路径条数。

                * 💡因为此时当前顶点【不止有来自前驱顶点的最短路径】，需要累加路径条数。

        2. 如果在Djikstra更新的时候，新算出来的路径长度更短，就是目前【只有一条更短的路径】。

            这个时候把【前驱顶点的路径条数】赋值给【当前顶点的路径条数】即可。

                * 💡此时当前顶点【只有来自前驱顶点的最短路径】，因此前驱顶点的最短路径条数就是其最短路径条数。
    ---------------------------------------
        - SomeBottle 2023.1.26
*/

/*
💡下面是我自己想的一个测试例，用来测试【最短路径的条数】和【救援队的数量】的输出是否正确:
(对应【测试点1】:5条不同的最短路的情况)
    * 如果你优先队列玩脱了，这个测试例绝对能试出来，亲测有效。
----------------------------
输入:
5 7 0 4
10 30 50 40 20
0 1 1
1 3 1
1 2 2
0 2 3
2 3 1
2 4 3
0 4 6

正确输出:
4 150
0 1 3 2 4

*/

/*
7-35 城市间紧急救援
    作为一个城市的应急救援队伍的负责人，你有一张特殊的全国地图。在地图上显示有多个分散的城市和一些连接城市的快速道路。每个城市的救援队数量和每一条连接两个城市的快速道路长度都标在地图上。当其他城市有紧急求助电话给你的时候，你的任务是带领你的救援队尽快赶往事发地，同时，一路上召集尽可能多的救援队。

输入格式:
    输入第一行给出4个正整数N、M、S、D，其中N（2≤N≤500）是城市的个数，顺便假设城市的编号为0 ~ (N−1)；M是快速道路的条数；S是出发地的城市编号；D是目的地的城市编号。

    第二行给出N个正整数，其中第i个数是第i个城市的救援队的数目，数字间以空格分隔。随后的M行中，每行给出一条快速道路的信息，分别是：城市1、城市2、快速道路的长度，中间用空格分开，数字均为整数且不超过500。输入保证救援可行且最优解唯一。
输出格式:
    第一行输出最短路径的条数和能够召集的最多的救援队数量。第二行输出从S到D的路径中经过的城市编号。数字间以空格分隔，输出结尾不能有多余空格。
输入样例:
    4 5 0 3
    20 30 40 10
    0 1 1
    1 3 2
    0 3 3
    0 2 2
    2 3 2
输出样例:
    2 60
    0 1 3
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/