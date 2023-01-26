#include <iostream>
#include <vector>
// 这里定义无穷为1e6
#define INF 1e6

using namespace std;
// 这题仍然是求带权无向图的单源最短路径，我想试着手写优先队列进行辅助
// 优先队列结点
struct QNode
{
    int vertex;   // 顶点下标
    int distance; // 顶点距离【起始顶点】的距离
};

// 优先队列(小根堆实现)
class Queue
{
public:
    Queue(int nodeNum)
    {
        vQueue.resize(nodeNum);
    }
    // 插入元素
    void push(QNode node)
    {
        vQueue[currSize++] = node; // 加入元素到堆尾部
        siftUp(currSize - 1);      // 将新增元素上滤
    }
    // 返回堆顶元素(引用类型，免除拷贝带来的开销)
    QNode &top()
    {
        return vQueue[0];
    }
    // 弹出堆顶元素(最小元素)
    void pop()
    {
        vQueue[0] = vQueue[currSize - 1]; // 将最后一个元素放到堆顶
        currSize--;                       // 堆中元素减少
        siftDown(0);                      // 将堆顶元素下滤
    }
    // 堆是否为空
    bool empty()
    {
        return currSize <= 0;
    }

private:
    vector<QNode> vQueue; // 堆储存核心
    int currSize = 0;     // 当前数组大小为0

    // 比较vQueue中node1下标对应的距离是否小于下标node2的
    // 这里做了防下标溢出判断，且巧妙处理了【只有左子节点，没有右子节点】的情况
    bool lessThan(int node1, int node2)
    {
        if (node1 < currSize)
        {
            // 这里是<=，比如下滤时比较左孩子和右孩子，如果二者权重一样，那么就将左孩子和父节点交换
            if (node2 < currSize)
                return vQueue[node1].distance <= vQueue[node2].distance;
            else // 如果有node1但没有node2，默认为true
                return true;
        }
        else
        {
            return false;
        }
    }

    // 将下标为ind的元素进行上滤
    void siftUp(int ind)
    {
        int i = ind;
        while (i > 0)
        {
            int parentInd = (i - 1) / 2; // 父节点下标
            // 如果子节点的距离小于父节点，就交换，保证父节点小于子节点
            if (lessThan(i, parentInd))
                swap(vQueue[parentInd], vQueue[i]);
            else // 否则上滤结束
                break;
            // 转到父节点继续比较
            i = parentInd;
        }
    }
    // 将下标为ind的元素进行下滤
    void siftDown(int ind)
    {
        int i = ind;
        while (i < currSize)
        {
            int leftInd = 2 * i + 1;    // 左子
            int rightInd = leftInd + 1; // 右子
            if (lessThan(leftInd, i) && lessThan(leftInd, rightInd))
            {
                // 左子比父节点小且左子比右子小，交换左子和父节点
                swap(vQueue[leftInd], vQueue[i]);
                i = leftInd;
            }
            else if (lessThan(rightInd, i) && lessThan(rightInd, leftInd))
            {
                // 右子比父节点小且右子比左子小，交换右子和父节点
                swap(vQueue[rightInd], vQueue[i]);
                i = rightInd;
            }
            else
            {
                // 已经达到最优，无需继续下滤
                break;
            }
        }
    }
    // 交换结点
    void swap(QNode &v1, QNode &v2)
    {
        QNode temp = v1;
        v1 = v2;
        v2 = temp;
    }
};

// 记录一条路的长度与价格
class Way
{
public:
    Way(int len, int price)
    {
        set(len, price);
    }

    void set(int len, int price)
    {
        this->len = len;
        this->price = price;
    }

    int len;
    int price;
};

typedef vector<Way> VecWay;
typedef vector<VecWay> Graph;
// 无向图邻接矩阵
Graph G;

// 城市数，高速公路数
int cityNum, wayNum;

// 声明函数
void AddEdge(int v0, int v1, int len, int price); // 往图中添加一条边
Way *Dijkstra(int startV, int endV);              // 从startV顶点开始进行Dijkstra算法

int main()
{
    // 出发地城市编号，终点城市编号
    int start, destination;
    cin >> cityNum >> wayNum >> start >> destination;
    // 初始化无向图
    G.resize(cityNum);
    for (int i = 0; i < cityNum; i++)
        G[i].resize(cityNum, Way(INF, INF)); // 初始情况下全为无穷
    // 注: C++11语法还可以写成resize(cityNum,{INF,INF})
    // 读入边
    for (int i = 0; i < wayNum; i++)
    {
        int v0, v1, len, price;
        cin >> v0 >> v1 >> len >> price;
        AddEdge(v0, v1, len, price);
    }
    // 通过Dijkstra找到出发点到终点的最便宜的最短路径
    Way *result = Dijkstra(start, destination);
    cout << result->len << " " << result->price << "\n";
    delete result;
    return 0;
}

// 往图中添加一条连接v0,v1顶点的边，包括边权len和价格price
void AddEdge(int v0, int v1, int len, int price)
{
    // 注意，无向图的邻接矩阵是对称的
    G[v0][v1].set(len, price);
    G[v1][v0].set(len, price);
}

// 找到从顶点startV到顶点endV的最便宜且最短路径
Way *Dijkstra(int startV, int endV)
{
    // 距离数组, 初始化每个顶点[路径长=无穷, 路径总价格=0]
    VecWay distances(cityNum, Way(INF, 0));
    // 访问数组，标记顶点是否被访问
    vector<bool> visits(cityNum, false);
    // 新建优先队列, 优先队列队头是distances距离数组里路径最短的一个顶点
    Queue queue(cityNum);
    // 初始顶点下标为startV，距离起点路径长为0
    queue.push(QNode{startV, 0});
    while (!queue.empty())
    {
        // 队列顶部就是distances数组内路径最短的顶点
        QNode &minNode = queue.top();
        int minV = minNode.vertex;          // 当前总路径最短的顶点
        int minDistance = minNode.distance; // 其路径长度
        queue.pop();                        // 弹出队头
        if (visits[minV])
            continue;        // 访问过的就不再继续
        visits[minV] = true; // 标记顶点被访问
        if (minV == endV)    // 如果已经确定到达目标顶点的最短路径，就不用继续了
            break;
        // 扫描distances数组
        for (int i = 0; i < cityNum; i++)
        {
            // 如果顶点已被访问，或者这条边不存在，就跳过
            if (visits[i] || G[minV][i].len == INF)
                continue;
            int iMinVLen = G[minV][i].len;     // minV到i顶点的距离
            int iMinVPrice = G[minV][i].price; // minV到i顶点的价格
            // 从起点到i顶点的距离 = 起点到minV顶点的距离 + minV到i顶点的距离
            int newDist = minDistance + iMinVLen;
            // 从起点到i顶点的路径所需的价格同理
            int newPrice = distances[minV].price + iMinVPrice;
            // 情况1: 如果新算出来的距离小于i顶点原本的距离，就更新
            // 情况2: 如果新算出来的距离等于i顶点原本的距离，就是出现了【多条最短路径】，此时【比较两条路径的总价格】，如果新路径更便宜就更新
            if ((newDist < distances[i].len) || (newDist == distances[i].len && newPrice < distances[i].price))
            {
                distances[i].len = newDist;
                distances[i].price = newPrice;
                // 将更新了距离的顶点及其路径长度加入队列
                queue.push(QNode{i, newDist});
            }
        }
    }
    // 返回从startV到endV的最短路径长和价格
    return new Way(distances[endV].len, distances[endV].price);
}

/*
    * 注意本题首先是求最短路径，在【最短路径有多条相同】的情况下才【找最便宜的】，因此进行最短路径算法时还是主要以路径长度为主。

    本题我仍然采用的是Dijkstra算法，不过利用了【优先队列】进行优化。
        * 所谓优先队列其实就是一个堆，这里我采用的是小根堆，保持【堆顶元素永远是堆中最小的】
        * 这里我手写实现了优先队列 (一方面是想尝试写一下，另一方面是我还没玩清楚C++的运算符重载和STL的优先队列)

    -------------------------------------
    有了优先队列，本题的Dijkstra算法的步骤如下:

        设Dijkstra算法从startV这个顶点开始，直到访问过endV后结束。

        1. 初始化
            - 初始化距离数组distances
            - 初始化访问数组visits
                * 注: 本题只需要知道路径长度，因此并不需要路径数组paths
            - 初始化优先队列queue
                * 注: 这里的优先队列每个元素储存[顶点下标, 顶点距离startV的总路径长]，堆中元素排序以【顶点路径长】为标准，保证【路径最短】的顶点在堆顶。
            - 将初始节点startV加入优先队列queue

        接下来的2,3步就是在一个循环体里的迭代过程了。

        2. 找到距离数组中最短路径的顶点
            - 弹出queue堆顶元素，这个元素就是【目前距离startV路径最短的顶点】，我将其称为minV，minV的路径长为minDistance

                注: 这里其实就是优化了暴力Dijkstra中的【扫描distances距离数组找到最小路径长顶点】的步骤。

            !!!! 如果这个顶点minV已被访问且minV==endV，说明已经确定了startV到endV的最短距离，迭代结束。!!!!

            - 如果这个顶点minV未被访问，就标记visits[minV]=true，代表其已被访问

        3. 扫描distances距离数组
            - 扫描到的顶点i的总路径长是distances[i].len, 其目前的总价格是distances[i].price

            - 假设i顶点是【minV顶点的邻接点】，计算这个顶点i【距离起点startV的路径长】和【总价格】
                * 路径长newDist = minV距离起点startV的路径长minDistance + minV距离顶点i的路径长G[minV][i].len
                * 总价格newPrice = minV和startV间的路径总价格distances[minV].price + minV和顶点i间的路径总价格G[minV][i].price

            - 如果【i顶点未被访问】:
                - 如果 (新算出来的路径长newDist) < (i顶点目前的总路径长distances[i].len)，那么就说明【i顶点的路径长可以更新】
                    * distances[i].len=newDist
                    * 同时更新价格: distances[i].price=newPrice
                    * 将更新后的 [顶点i, 顶点i距离startV的路径长] 【加入到优先队列】中

                !!接下来是本题的精华!!
                - 如果 (新算出来的路径长newDist) == (i顶点目前的总路径长distances[i].len)，那么就说明【startV到i顶点的最短路径有多条相同的】
                    * 比较顶点i之前的价格和新算出来的价格，如果 (新算出来的价格newPrice) < (i顶点之前的价格distances[i].price), 就更新:
                        * distances[i].len=newDist
                        * distances[i].price=newPrice
                        * 将更新后的 [顶点i, 顶点i距离startV的路径长] 【加入到优先队列】中

        当优先队列为空或者visits[endV]==true时，迭代结束，Dijkstra算法运行完成。
    ----------------------------------------
    老实说，这题用暴力Dijkstra和优先队列辅助Dijkstra的区别不算很大，优先队列Dijkstra在【邻接矩阵较为稀疏(边比较少)】的时候有很好的效果。

    这个优先队列我最开始尝试把元素全部动态初始化丢在堆内存里，但是指针玩着玩着就出了莫名其妙的内存错误，菜是原罪啊！
    最后我还是用静态初始化队列元素解决了。

    这回也是我第一次接触C++的面向对象编程。

        - SomeBottle 2023.1.5

*/
/*
7-9 旅游规划
有了一张自驾旅游路线图，你会知道城市间的高速公路长度、以及该公路要收取的过路费。现在需要你写一个程序，帮助前来咨询的游客找一条出发地和目的地之间的最短路径。如果有若干条路径都是最短的，那么需要输出最便宜的一条路径。

输入格式:
    输入说明：输入数据的第1行给出4个正整数N、M、S、D，其中N（2≤N≤500）是城市的个数，顺便假设城市的编号为0~(N−1)；M是高速公路的条数；S是出发地的城市编号；D是目的地的城市编号。随后的M行中，每行给出一条高速公路的信息，分别是：城市1、城市2、高速公路长度、收费额，中间用空格分开，数字均为整数且不超过500。输入保证解的存在。
输出格式:
    在一行里输出路径的长度和收费总额，数字间以空格分隔，输出结尾不能有多余空格。
输入样例:
    4 5 0 3
    0 1 1 20
    1 3 2 30
    0 3 4 10
    0 2 2 20
    2 3 1 20
输出样例:
    3 40
代码长度限制
    16 KB
Java (javac)
    时间限制
        800 ms
    内存限制
        64 MB
其他编译器
    时间限制
        400 ms
    内存限制
        64 MB
*/