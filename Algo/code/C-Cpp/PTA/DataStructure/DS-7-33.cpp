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

*/