#include <cstdio>
#include <vector>

using namespace std;

/*
    注: 本题考察的不是哈密顿回路！
*/
typedef vector<bool> BoolSeq;
typedef vector<BoolSeq> Graph;

int vertexNum, edgeNum, startV; // 顶点数和边数,以及开始顶点
Graph G;                        // 定义图

void DFS(int currV, BoolSeq &visited, int &visitNum); // DFS遍历

int main()
{
    scanf("%d %d %d", &vertexNum, &edgeNum, &startV);
    startV--;
    // 初始化图，图中只需要记录有没有边即可
    G.resize(vertexNum, vector<bool>(vertexNum, false));
    // 读入所有边
    for (int i = 0; i < edgeNum; i++)
    {
        int v1, v2;
        scanf("%d %d", &v1, &v2);
        v1--;
        v2--; // 题目采用编号1~N，但我这是1~N-1
        G[v1][v2] = true;
        G[v2][v1] = true;
    }
    // 进行DFS遍历
    int visitNum = 0;                  // 已经访问的顶点数
    BoolSeq visited(vertexNum, false); // 标记每个节点是否被访问
    DFS(startV, visited, visitNum);
    // 不是所有顶点都访问了
    if (visitNum < vertexNum)
    {
        printf(" 0"); // 附加输出一个0
    }
    return 0;
}

// DFS遍历，返回访问的节点数
void DFS(int currV, BoolSeq &visited, int &visitNum)
{
    if (visitNum != 0)
        printf(" ");         // 不输出多余空格
    printf("%d", currV + 1); // 输出时还原编号
    visited[currV] = true;
    visitNum++;
    // 已经访问完了所有节点就不需要继续
    if (visitNum >= vertexNum)
        return;
    for (int i = 0; i < vertexNum; i++)
    {
        if (!visited[i] && G[currV][i])
        {
            DFS(i, visited, visitNum);
            // 原路返回(输出回溯经过的顶点)
            printf(" %d", currV + 1); // 输出时还原编号
        }
    }
}

/*
    刚开始以为这题是哈密顿图，仔细看了一下题发现其实就是考察深度优先搜索遍历(DFS)。

    值得注意的是，本题输出格式并没说不能输出多余空格，但判定的时候【要求不能输出多余空格】。
    --------------------------

    本题的深度优先搜索写法比较常规，【输出语句】的位置是本题考察的重点。

    比如对于下面这张图:

        1 - 2 - 4
         \
          3
    要求输出是: 1 2 4 2 1 3 1

    也就是说，当到达某一分支最深处的时候就要“原路返回”，然后还要【完成剩余分支】的遍历。
    很明显，“原路返回”的过程就是回溯的过程，因此本题的DFS在回溯的时候也要【把回溯经过的顶点】打印出来。

    比如上图中我【位于1号顶点】，深挖2号顶点后面的分支DFS(2)，那么【回溯】的时候必然要经过【1号顶点】，
    因此可以快速推导出:
        输出回溯路径的语句【紧跟在函数自身调用语句的后面】。
    -----------------------------
        - SomeBottle 2023.1.25
*/
/*
7-33 地下迷宫探索
    本实验案例以探索地下通道迷宫作为内容。

    假设有一个地下通道迷宫，它的通道都是直的，而通道所有交叉点（包括通道的端点）上都有一盏灯和一个开关。请问你如何从某个起点开始在迷宫中点亮所有的灯并回到起点？

输入格式:
    输入第一行给出三个正整数，分别表示地下迷宫的节点数N（1<N≤1000，表示通道所有交叉点和端点）、边数M（≤3000，表示通道数）和探索起始节点编号S（节点从1到N编号）。随后的M行对应M条边（通道），每行给出一对正整数，分别是该条边直接连通的两个节点的编号。
输出格式:
    若可以点亮所有节点的灯，则输出从S开始并以S结束的包含所有节点的序列，序列中相邻的节点一定有边（通道）；否则虽然不能点亮所有节点的灯，但还是输出点亮部分灯的节点序列，最后输出0，此时表示迷宫不是连通图。

    由于深度优先遍历的节点序列是不唯一的，为了使得输出具有唯一的结果，我们约定以节点小编号优先的次序访问（点灯）。在点亮所有可以点亮的灯后，以原路返回的方式回到起点。
输入样例1:
    6 8 1
    1 2
    2 3
    3 4
    4 5
    5 6
    6 4
    3 6
    1 5
输出样例1:
    1 2 3 4 5 6 5 4 3 2 1
输入样例2:
    6 6 6
    1 2
    1 3
    2 3
    5 4
    6 5
    6 4
输出样例2:
    6 4 5 4 6 0
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/