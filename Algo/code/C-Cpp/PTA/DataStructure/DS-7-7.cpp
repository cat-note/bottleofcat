#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

// 本题需要用到的数据结构实际上是一个无向图
// 而结点数N和边数M都有可能非常多，因此这里采用邻接矩阵进行储存
// 二维数组每个元素[i][j]代表顶点i和顶点j是否相连，bool型即可
typedef vector<vector<bool>> Graph;

Graph G;              // 定义图(邻接矩阵)
vector<int> sixNum;   // 记录每个顶点的六度结点数
int nodeNum, edgeNum; // 结点数和边数

void AddEdge(int v1, int v2); // 添加连接v1和v2编号的顶点的边
void BFS(int v0);             // 从v0顶点开始进行BFS遍历(v0范围是0~N-1)
void ResetAccess();           // 重置顶点访问情况
void PrintPercentage();       // 打印百分数

int main()
{
    scanf(" %d %d", &nodeNum, &edgeNum);
    G.resize(nodeNum); // 初始化N*N邻接矩阵
    // 对角线上的元素是G[i][i]，值肯定是0，这里我让其代表顶点i是否被DFS访问过
    for (int i = 0; i < nodeNum; i++)
        G[i].resize(nodeNum, false); // 初始化值全为0，0代表没有边
    sixNum.resize(nodeNum);
    // 读入边
    for (int i = 0; i < edgeNum; i++)
    {
        int v1, v2;
        scanf(" %d %d", &v1, &v2);
        AddEdge(v1, v2); // 添加新边
    }
    // BFS扫描
    // 这里用循环是因为要考虑非连通图
    for (int i = 0; i < nodeNum; i++)
    {
        BFS(i);
        ResetAccess(); // 将对角线的访问标记归零
    }
    PrintPercentage();
    return 0;
}

// 添加一条边，连接编号为v1和v2的结点
// 注意，邻接矩阵下标起始为0，但题目中编号是从1开始
void AddEdge(int v1, int v2)
{
    // 这里默认v1和v2不相等了
    G[v1 - 1][v2 - 1] = true; // 有边的值为1，代表从v1到v2结点距离为1
    G[v2 - 1][v1 - 1] = true; // 无向图的邻接矩阵是对称的
}

// 从v0顶点开始进行BFS遍历(v0从0开始，是下标)
// 找到v0顶点的六度范围内的相邻点
void BFS(int v0)
{
    queue<int *> vQueue;
    // 队列中每个元素是一个二元数组，储存的是[顶点下标, 顶点所在层数]
    int *vertex0 = new int[2]{v0, 0};
    vQueue.push(vertex0);
    G[v0][v0] = true; // 标记为已访问
    while (!vQueue.empty())
    {
        int *currVertex = vQueue.front();
        int currV = currVertex[0];      // 当前遍历到的顶点下标
        int currDegree = currVertex[1]; // 当前遍历到的顶点所在层数
        delete[] currVertex;            // 释放内存
        vQueue.pop();
        // 层数<=6才继续
        if (currDegree > 6)
            break;
        sixNum[v0]++; // 增加v0顶点的六度结点数(包括自己)
        // 将currV顶点的邻接点加入队列
        for (int i = 0; i < nodeNum; i++)
        {
            // 忽略邻接矩阵对角线元素
            // G[currV][i]!=0时说明currV顶点和i顶点相连
            // 且顶点i未被访问
            if (i != currV && G[currV][i] && !G[i][i])
            {
                G[i][i] = true;                              // 标记顶点i已被访问
                int *vertex = new int[2]{i, currDegree + 1}; // 记录[当前顶点下标,所在层数]
                vQueue.push(vertex);
            }
        }
    }
}

void PrintPercentage()
{
    for (int i = 0; i < nodeNum; i++)
    {
        float percentage = (float)sixNum[i] / nodeNum;
        printf("%d: %.2f%%\n", i + 1, percentage * 100);
    }
}

// 将G[i][i]全置为false（顶点未被访问）
void ResetAccess()
{
    for (int i = 0; i < nodeNum; i++)
        G[i][i] = false;
}