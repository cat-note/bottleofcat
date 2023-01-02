/*
7-7 六度空间
“六度空间”理论又称作“六度分隔（Six Degrees of Separation）”理论。这个理论可以通俗地阐述为：“你和任何一个陌生人之间所间隔的人不会超过六个，也就是说，最多通过五个人你就能够认识任何一个陌生人。”
假如给你一个社交网络图，请你对每个节点计算符合“六度空间”理论的结点占结点总数的百分比。

输入格式:
    输入第1行给出两个正整数，分别表示社交网络图的结点数N（1<N≤10^3，表示人数）、边数M（≤33×N，表示社交关系数）。随后的M行对应M条边，每行给出一对正整数，分别是该条边直接连通的两个结点的编号（节点从1到N编号）。
输出格式:
    对每个结点输出与该结点距离不超过6的结点数占结点总数的百分比，精确到小数点后2位。每个结节点输出一行，格式为“结点编号:（空格）百分比%”。
输入样例:
    10 9
    1 2
    2 3
    3 4
    4 5
    5 6
    6 7
    7 8
    8 9
    9 10
输出样例:
    1: 70.00%
    2: 80.00%
    3: 90.00%
    4: 100.00%
    5: 100.00%
    6: 100.00%
    7: 100.00%
    8: 90.00%
    9: 80.00%
    10: 70.00%
我想的输入样例:
    7 7
    1 2
    5 2
    2 6
    6 5
    6 7
    7 4
    3 7
输出:
    1: 100.00%
    2: 100.00%
    3: 100.00%
    4: 100.00%
    5: 100.00%
    6: 100.00%
    7: 100.00%
代码长度限制
    16 KB
时间限制
    2500 ms
内存限制
    64 MB
*/
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

/*
    这题最开始让我想起来了计算机网络里的基于向量的路由更新算法，然而并不用写的这么复杂...

    最开始我用DFS写了一下，不过喜提WA了，换成BFS摆弄了几下就写出来了。

    本题的意思其实就是对于任何一个图中的顶点，找出和这个顶点相邻距离<=6的所有节点。
        * 在使用BFS的情况下，本题的核心考察点是: 怎么判断某个顶点所在的层数？

    实际上我们只需要对BFS使用的队列动一下手脚就行，比如我这里就让队列的每个元素是一个二元数组:
        [ 顶点下标, 顶点所在层数 ]

    要得到某个顶点v0的所有“六度空间”结点，只需要找到【从v0开始的六层BFS】所遍历到的所有顶点即可。

        - 对于首个顶点v0，我将其层数设为0，其【所有邻接点】的层数就是0+1=1

        - 往后对于每个BFS遍历到的顶点V，它的层数为n，那么其所有邻接点的层数就是【n+1】

        - 当从BFS队列中取出的顶点【所在层数>6】时，循环终止，BFS停止，至此找到了【v0顶点的6层以内的所有“六度”顶点】。

        - SomeBottle 2023.1.2
*/