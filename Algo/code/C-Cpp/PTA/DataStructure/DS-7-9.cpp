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

// 优先队列(小根堆)
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
    // 返回堆顶元素
    QNode top()
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
    bool compare(int node1, int node2)
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
            // 如果父节点的距离大于子节点，就交换
            if (compare(i, parentInd))
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
            if (compare(leftInd, i) && compare(leftInd, rightInd))
            {
                // 左子比父节点小且左子比右子小，交换左子和父节点
                swap(vQueue[leftInd], vQueue[i]);
                i = leftInd;
            }
            else if (compare(rightInd, i) && compare(rightInd, leftInd))
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
        QNode minNode = queue.top();
        int minV = minNode.vertex;          // 当前总路径最短的顶点
        int minDistance = minNode.distance; // 其路径长度
        queue.pop();                        // 弹出队头(这一步释放了minNode的内存)
        if (visits[minV])                   // 如果顶点已被访问就不继续下去
            continue;
        visits[minV] = true; // 标记顶点被访问
        if (minV == endV)    // 如果已经确定到达目标顶点的最短路径，就不用继续了
            break;
        // 扫描distances数组
        for (int i = 0; i < cityNum; i++)
        {
            int iMinVLen = G[minV][i].len;     // minV到i顶点的距离
            int iMinVPrice = G[minV][i].price; // minV到i顶点的价格
            // 从起点到i顶点的距离 = 起点到minV顶点的距离 + minV到i顶点的距离
            int newDist = minDistance + iMinVLen;
            // 从起点到i顶点的路径所需的价格同理
            int newPrice = distances[minV].price + iMinVPrice;
            // 前提: i顶点尚未被访问
            // 情况1: 如果新算出来的距离小于i顶点原本的距离，就更新
            // 情况2: 如果新算出来的距离等于i顶点原本的距离，就是出现了【多条最短路径】，此时【比较两条路径的总价格】，如果新路径更便宜就更新
            if (
                !visits[i] &&
                ((newDist < distances[i].len) || (newDist == distances[i].len && newPrice < distances[i].price)))
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
    注意本题首先是求最短路径，在【最短路径】有多条相同的情况下才【找最便宜的】，因此进行最短路径算法时还是主要以路径长度为主。
*/