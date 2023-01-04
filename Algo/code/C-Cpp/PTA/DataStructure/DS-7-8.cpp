#include <iostream>
#include <vector>

// 100个顶点的完全图也只有4950条边，设置INF为1e+5
#define INF 1e+5
// 因为本题中路径权重全为非负，这里定义未知UNK为-1
using namespace std;

typedef vector<vector<int>> Graph;
// 储存顶点及到这个顶点的最短路径的结构体
struct CostOfV
{
    int cost;     // 路径长度
    bool success; // 是否能变成所有动物(图是否连通)
};
typedef struct CostOfV *VCost;
// 初始化图，本题用到的是无向图，因为魔咒正反念可以让动物互相转换
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
        G[i].resize(animalNum, INF); // INF代表无边连接
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
          - 从图中找到这个顶点V，使得V和Vd的路径最短 -> Dijkstra算法
       只带1只动物不可能完成所有变形要求的情况是：
          - 图是【非连通图】
    */
    int optimalV = -1;     // 结果顶点
    int optimalCost = INF; // 结果路径长
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
        //  如果【图中最长魔咒】可以更短，就更新
        if (result->cost < optimalCost)
        {
            optimalV = i; // 记录顶点下标
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

// 从v0顶点(下标从0开始)执行迪杰斯特拉算法(简化暴力版，没有路径path, 只求距离distance)
// 返回所有距v0最短路径中最长的一条路径长度
VCost Dijkstra(int v0)
{
    VCost result = new CostOfV(); // 返回值
    result->cost = -1;            // 尚无最长距离
    result->success = true;       // 默认图连通
    // 根据顶点数建立距离数组，初始到达每个顶点的路径长度皆为“无穷”
    vector<int> distances(animalNum, INF);
    // 顶点是否被访问
    vector<bool> visits(animalNum, false);
    // 初始化工作
    // 初始化距离数组，将v0的邻接点的权重加入
    for (int i = 0; i < animalNum; i++)
        distances[i] = G[v0][i];
    visits[v0] = true; // 标记顶点v0已被访问

    // 遍历所有顶点，更新所有顶点距离v0的距离
    for (int V = 0; V < animalNum; V++)
    {
        // 每次迭代的【第1步】:
        // 找距离数组distances中距离最小的顶点
        int minV = -1;         // 最小顶点的下标(这里写v0是为了防止找不到minV的情况)
        int minDistance = INF; // 最小顶点距v0顶点的距离
        for (int i = 0; i < animalNum; i++)
        {
            // 顶点未被访问且该顶点距离v0顶点的距离能更小
            if (!visits[i] && distances[i] < minDistance)
            {
                minV = i;
                minDistance = distances[i];
            }
        }
        if (minV == -1) // 找不到未被访问的最小顶点了，循环终止
            break;
        visits[minV] = true; // 标记顶点minV已经被访问
        // 每次迭代的【第2步】:
        // 找这个距离最小顶点minV的邻接点j，对其每个邻接点j进行更新
        for (int j = 0; j < animalNum; j++)
        {
            // 算出j顶点距离v0顶点的距离: minV离v0的距离 + minV距离j的距离
            int distance = distances[minV] + G[minV][j];
            // 如果顶点j未被访问，且顶点j距离v0的距离可以更新地更小
            if (!visits[j] && distance < distances[j])
            {
                distances[j] = distance;
            }
        }
    }
    // 扫描一遍visits数组，看看有没有顶点未被访问
    for (int i = 0; i < animalNum; i++)
        if (!visits[i])
            result->success = false; // 如果有顶点未被访问就是非连通图

    // distances距离数组计算完毕，找出其中的最长距离
    for (int i = 0; i < animalNum; i++)
    {
        if (distances[i] != INF && distances[i] > result->cost)
        {
            result->cost = distances[i];
        }
    }
    return result;
}

/*
    写这题最开始我完全图测试点过不去，于是我用JavaScript写了个简单的随机完全图测试点生成工具，和此代码放在同一个目录下: completeGraph.js
    使用方法：
        - 在命令行中输入 node completeGraph.js [vertexNum]
        - vertexNum为图的顶点数

    这个题目还是有点绕的。
    题目中给出的提示:
        1. 魔咒可以正向读也可以反向读，借此【来回】转变动物种类
            - 可以知道这题用的是【无向图】

        2. 哈利波特带去一只动物，要把这只动物变成【最难变的】那一种动物，实际上就是要找【所需咒语最长的】(咒语尽量短)。
            - 把带去的动物看作【初始顶点】，实际上要求的是【初始顶点到其他所有顶点的最短路径】中的【最长的一条路径的距离】。

        3. 要找到【变成最难变的动物时所需魔咒最短】的情况，就需要【枚举带每一种动物的情况】
            - 遍历所有顶点，在【所有顶点对应的最长路径】中找出【最短的一条】

        4. 无论用什么咒语都没法变成的动物，在图中其实就是一个【不连通的顶点】。
            - 只要图是【非连通的】，那么带一只动物无法转变成所有其他动物。

    综上，本题的核心要点其实是求【无向图中单源的最短路径】，比较容易想到的有Floyd和Dijkstra算法，我这里就采用Dijkstra算法了。

    -----------------------
    Dijkstra算法有暴力实现法和优先队列实现法，这里我采用了暴力实现，且作了一定的简化。
        * Dijkstra算法一般会用到 distances(距离) paths(路径) visits(是否访问) 三个数组，这里因为【我只需要知道最短的距离】，所以没有用到paths。

    在我看来，Dijkstra算法的步骤主要是三步，其中后两步是迭代过程:

        前提:
            - distances数组每个元素默认值是INF(数值无穷大)
            - 图G中每两个不相连的顶点，边的权值默认为INF(无穷大)
            - visits数组每个元素默认为false(未被访问)

        1. 初始化
            - 设从【顶点v0】开始Dijkstra算法
            - 扫描v0的【邻接点边的权值】，将v0邻接点边权更新到distances距离数组
                * 注: 这里的边权其实就是顶点【距离v0顶点的距离】。
            - 标记visits[v0]=true, 也就是顶点v0已被访问

        2. 寻找distances数组中目前【未被访问】的【最小距离】
            - 扫描distances数组，找到距离最小顶点的下标minV和其最小距离minDistance
            - 标记visits[minV]=true (这个顶点已被访问)

        3. 再次扫描distances数组
            - 扫描到的顶点为j，其距离为distances[j]

            - 假设这个顶点j是【顶点minV的邻接点】，计算这个【顶点j距离起始顶点v0的距离】:
                * 距离distance = 顶点j和顶点minV间的距离G[minV][j] + 顶点minV距离v0顶点的距离distances[minV]

            - 如果这个顶点j【未被访问过】且【其距离起始点v0的距离可以更新】
                * 更新distances[j]=distance
                (还记得distances元素默认为无穷大INF吗？distances[j]为INF时，distance是肯定大于distances[j]的，不会更新)
                (也就是说，这里的更新是针对【已知距离的邻接点而言】的)

        其中2, 3是不断的迭代过程，直到【找不到最小距离顶点minV】或者【扫描完所有顶点】为止，Dijkstra单次算法结束。

    最后，从Dijkstra求出的distances数组中找出【最长的一条最短路径】即可。

    对每个顶点都进行一次Dijkstra算法，将求出的路径长进行对比，【找到最短的一条】对应的顶点，即为题目所求的动物。

    这题是我第一次写Dijkstra，非常值得反复回味。

        - SomeBottle 2023.1.4

*/

/*
7-8 哈利·波特的考试
哈利·波特要考试了，他需要你的帮助。这门课学的是用魔咒将一种动物变成另一种动物的本事。例如将猫变成老鼠的魔咒是haha，将老鼠变成鱼的魔咒是hehe等等。反方向变化的魔咒就是简单地将原来的魔咒倒过来念，例如ahah可以将老鼠变成猫。另外，如果想把猫变成鱼，可以通过念一个直接魔咒lalala，也可以将猫变老鼠、老鼠变鱼的魔咒连起来念：hahahehe。

现在哈利·波特的手里有一本教材，里面列出了所有的变形魔咒和能变的动物。老师允许他自己带一只动物去考场，要考察他把这只动物变成任意一只指定动物的本事。于是他来问你：带什么动物去可以让最难变的那种动物（即该动物变为哈利·波特自己带去的动物所需要的魔咒最长）需要的魔咒最短？例如：如果只有猫、鼠、鱼，则显然哈利·波特应该带鼠去，因为鼠变成另外两种动物都只需要念4个字符；而如果带猫去，则至少需要念6个字符才能把猫变成鱼；同理，带鱼去也不是最好的选择。

输入格式:
    输入说明：输入第1行给出两个正整数N (≤100)和M，其中N是考试涉及的动物总数，M是用于直接变形的魔咒条数。为简单起见，我们将动物按1~N编号。随后M行，每行给出了3个正整数，分别是两种动物的编号、以及它们之间变形需要的魔咒的长度(≤100)，数字之间用空格分隔。
输出格式:
    输出哈利·波特应该带去考场的动物的编号、以及最长的变形魔咒的长度，中间以空格分隔。如果只带1只动物是不可能完成所有变形要求的，则输出0。如果有若干只动物都可以备选，则输出编号最小的那只。
输入样例:
    6 11
    3 4 70
    1 2 1
    5 4 50
    2 6 50
    5 6 60
    1 3 70
    4 6 60
    3 6 80
    5 1 100
    2 4 60
    5 2 80
输出样例:
    4 70
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/