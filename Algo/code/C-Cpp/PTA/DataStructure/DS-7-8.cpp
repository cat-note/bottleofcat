#include <iostream>
#include <vector>
#include <queue>
// 100个顶点的完全图也只有4950条边，设置INF为1e+5
#define INF 1e+5
#define UNK -1
// 因为本题中路径权重全为非负，这里定义未知UNK为-1
using namespace std;

typedef vector<vector<int>> Graph;
// 储存顶点及到这个顶点的最短路径的结构体
struct CostOfV
{
    int index;    // 顶点下标(从0开始)
    int cost;     // 路径长度
    bool success; // 是否能变成所有动物(图是否连通)
};
typedef struct CostOfV *VCost;
// 初始化图，本题用到的是无向图，因为魔咒正反念可以让动物互相转换
// 对角线G[i][i]用于标记顶点i是否被访问过
Graph G;
int animalNum, magicNum; // 动物总数, 魔咒总数

// 声明函数
void AddEdge(int v1, int v2, int magicLen); // 往图中加入一条边
VCost Dijkstra(int v0);                     // 从v0顶点开始进行Dijkstra, 寻找所有最短路径中最长的那一条

// 第一眼看到这题，我脑内浮现出了最小生成树和最短路径
int main()
{
    cin >> animalNum >> magicNum;
    // 初始化图
    G.resize(animalNum);
    for (int i = 0; i < animalNum; i++)
        G[i].resize(animalNum, 0); // 0代表无边连接
    // 往图中加入边
    for (int i = 0; i < magicNum; i++)
    {
        int v1, v2, magicLen;
        cin >> v1 >> v2 >> magicLen;
        AddEdge(v1, v2, magicLen);
    }
    /* 哈利波特带的动物对应图中的某个顶点V
       我的目标是:
          - 寻找顶点V距离【所有其他顶点的最短路径】
          - 找到一个顶点Vd，它距离顶点V的路径长度是【所有最短路径】中最长的
          - 设从这个【顶点V】出发到【顶点Vd】的
                * 也就是单次遍历中的最长路径
          - 从图中找到这个顶点V，使得V和Vd的路径最短
       只带1只动物不可能完成所有变形要求的情况是：
          - 图是【非连通图】
    */
    int optimalV = UNK;    // 结果顶点
    int optimalCost = UNK; // 结果路径长
    // 对每个顶点进行Dijkstra算法
    for (int i = 0; i < animalNum; i++)
    {
        VCost result = Dijkstra(i);
        if (!result->success)
        {
            // 图不连通，是不可能完成所有变形需求的！
            // 没必要继续进行下去了
            cout << "0"; // 输出0
            return 0;
        }
        // printf("READ VERTEX:%d MIN-COST:%d\n", result->index + 1, result->cost);
        //  如果【图中最长魔咒】可以更短，就更新
        if (optimalCost == UNK || result->cost < optimalCost)
        {
            optimalV = result->index;
            optimalCost = result->cost;
        }
        delete result; // 释放内存
    }
    // 别忘了我这的顶点是下标从0开始的，还要+1
    cout << (optimalV + 1) << " " << optimalCost;
    return 0;
}

// v1,v2是顶点编号(从1开始), magicLen是魔咒长度
// 实际上magicLen就是连接v1和v2这条边的权重
void AddEdge(int v1, int v2, int magicLen)
{
    // 我储存顶点的下标还是从0开始的
    // 别忘了无向图的邻接矩阵是对称的
    G[v1 - 1][v2 - 1] = magicLen;
    G[v2 - 1][v1 - 1] = magicLen;
}

// 从v0顶点(下标从0开始)执行迪杰斯特拉算法
// 返回所有距v0最短路径中最长的一条路径及其对应的顶点
VCost Dijkstra(int v0)
{
    // printf("-------------------\n");
    VCost result = new CostOfV(); // 返回值
    result->index = UNK;          // 尚无顶点
    result->success = true;       // 默认图连通
    // Dijkstra是基于BFS的，一样需要队列
    queue<int> vQueue;
    // 这里我用邻接矩阵对角线G[i][i]来标记i顶点是否被访问
    // 根据顶点数建立距离数组，初始到达每个顶点的路径长度皆未知
    vector<int> distances(animalNum, UNK);
    vQueue.push(v0);   // 将起始顶点加入队列
    G[v0][v0] = 1;     // 标记顶点v0已被访问
    distances[v0] = 0; // v0顶点自身到自身距离为0
    while (!vQueue.empty())
    {
        int currV = vQueue.front(); // 队头是当前顶点
        vQueue.pop();               // 弹出队头
        int currMinCost = UNK;      // 直到currV顶点为止的最小路径
        // 找到当前顶点currV的所有邻接点
        for (int i = 0; i < animalNum; i++)
        {
            // 1.非对角线元素G[i][i] 2.有这条边G[currV][i]
            if (i != currV && G[currV][i])
            {
                // 如果距离数组里有这个顶点，说明【到这个顶点的距离已知】
                if (distances[i] != UNK)
                {
                    // printf("JUDGE: %d->%d\n", i + 1, currV + 1);
                    //  算出【经过】顶点i到达顶点currV的最短路径
                    int distance = distances[i] + G[currV][i];
                    // 更新到达当前顶点currV的最短路径currMinCost
                    // 1.还没有currMinCost值时直接更新 2.新的距离小于currMinCost时更新
                    if (currMinCost == UNK || distance < currMinCost)
                        currMinCost = distance;
                }
                if (!G[i][i]) // 顶点i未被访问就加入队列
                {
                    G[i][i] = 1;                                   // 标记顶点i被访问
                    distances[i] = distances[currV] + G[currV][i]; // 初始化到顶点i的距离
                    vQueue.push(i);                                // 加入队列
                }
            }
        }
        // 只有首个顶点会有这种情况：没有已知最短距离
        // 首个顶点距离首个顶点的最短距离置为0
        if (currMinCost == UNK)
            currMinCost = 0;
        // 更新从v0到currV的最短距离到距离数组
        distances[currV] = currMinCost;
        // printf("OUTPUT VERTEX:%d COST:%d\n", currV + 1, currMinCost);
        //  同时比对，循环结束时找出所有最短路径中路径最长的顶点
        //  如果有更长的“最短路径”出现，就更新
        if (result->index == UNK || currMinCost > result->cost)
        {
            result->index = v0;         // 顶点下标
            result->cost = currMinCost; // 直到currV的最短路径开销
        }
    }
    // BFS进行结束，标志位归零
    for (int i = 0; i < animalNum; i++)
    {
        if (G[i][i])
            G[i][i] = 0;
        else
            result->success = false; // 有顶点未被访问到，这说明图不连通！
    }
    return result;
}