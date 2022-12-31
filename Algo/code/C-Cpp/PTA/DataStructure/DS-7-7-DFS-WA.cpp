/*
错误题解
本题依赖的数据结构是无向图，我为了节省空间，利用无向图邻接矩阵的对称性，只储存了下三角
问题就出在这里。只储存下三角的话，DFS只能按【下标降序】进行遍历，但凡图中结点不是按下标顺序连接的，DFS都没法扫全
下三角这种储存方法中我只能知道【两个结点之间是否相联】，是不适合DFS的。
    - SomeBottle 2022.12.31
*/
#include <cstdio>
#include <vector>

using namespace std;

// 本题需要用到的数据结构实际上是一个无向图
// 而结点数N和边数M都有可能非常多，因此这里采用邻接矩阵进行储存
// 因为这里没有边权，只需要用1字节的bool类型即可
typedef vector<vector<bool>> Graph;

Graph G;              // 定义图(邻接矩阵)
vector<int> nearNum;  // 记录每个顶点的距离不超过6的邻接点的数量
int nodeNum, edgeNum; // 结点数和边数

void AddEdge(int v1, int v2);               // 添加连接v1和v2编号的顶点的边
void DFS(int v0, int curr, short distance); // DFS遍历节点
void ResetAccess();                         // 重置顶点访问情况
void PrintPercentage();                     // 打印百分数

int main()
{
    scanf(" %d %d", &nodeNum, &edgeNum);
    G.resize(nodeNum); // 初始化N*N邻接矩阵
    // 初始化临近点记录数组, 初始值为1是因为要把结点自己算进去
    nearNum.resize(nodeNum, 1);
    // 注意这里只初始化了矩阵下三角，因为无向图的邻接矩阵是对称的，这样能节省空间
    // 对角线上的元素肯定都是false，因此i下标对应的行只用初始化i个元素
    // 但是这里我仍然初始化了i+1个元素，每行的最后一个元素是G[i][i]，代表顶点i是否被DFS访问过
    for (int i = 0; i < nodeNum; i++)
        G[i].resize(i + 1, false); // 初始化值全为false
    // 读入边
    for (int i = 0; i < edgeNum; i++)
    {
        int v1, v2;
        scanf(" %d %d", &v1, &v2);
        AddEdge(v1, v2); // 添加新边
    }
    // DFS扫描
    // 这里从顶点数-1遍历到下标为1的顶点，因为这里储存的是下三角，G[0][0]没有意义
    for (int i = nodeNum - 1; i >= 1; i--)
    {
        DFS(i, i, 1); // 距离从1开始
        ResetAccess();
    }
    PrintPercentage();
    return 0;
}

// 添加一条边，连接编号为v1和v2的结点
// 注意，邻接矩阵下标起始为0，但题目中编号是从1开始
void AddEdge(int v1, int v2)
{
    // 这里默认v1和v2不相等了
    // 注意，因为G只储存下三角，这里需要找到较大的下标
    if (v1 > v2)
        G[v1 - 1][v2 - 1] = true;
    else
        G[v2 - 1][v1 - 1] = true;
}

// v0是从0开始的顶点编号，curr是当前遍历到的结点，distance是距离v0的距离
void DFS(int v0, int curr, short distance)
{
    if (distance > 6) // 深度最多为6层
        return;
    G[curr][curr] = true; // 标记该curr顶点已被访问
    for (int j = 0; j < v0; j++)
    { // 对这一行的j下标结点进行DFS
        // G[curr][j]代表是否有连接i与j的边，G[j][j]代表j下标结点是否被访问过
        if (G[curr][j] && !G[j][j])
        {
            // 要注意，这里遍历的是邻接矩阵下三角，
            // 每遍历到v0六度范围内的j结点，v0和j的六度结点数都要+1
            printf("%d->%d distance:%d\n", v0 + 1, j + 1, distance);
            nearNum[v0]++;
            nearNum[j]++;
            // DFS(v0,j,distance+1);
        }
    }
}

void PrintPercentage()
{
    for (int i = 0; i < nodeNum; i++)
    {
        float percentage = (float)nearNum[i] / nodeNum;
        printf("%d: %.2f%%\n", i + 1, percentage * 100);
    }
}

// 将G[i][i]全置为false（顶点未被访问）
void ResetAccess()
{
    for (int i = 0; i < nodeNum; i++)
        G[i][i] = false;
}