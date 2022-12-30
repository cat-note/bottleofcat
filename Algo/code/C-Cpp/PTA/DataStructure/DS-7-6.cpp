/*
7-6 列出连通集
给定一个有N个顶点和E条边的无向图，请用DFS和BFS分别列出其所有的连通集。假设顶点从0到N−1编号。进行搜索时，假设我们总是从编号最小的顶点出发，按编号递增的顺序访问邻接点。

输入格式:
    输入第1行给出2个整数N(0<N≤10)和E，分别是图的顶点数和边数。随后E行，每行给出一条边的两个端点。每行中的数字之间用1空格分隔。
输出格式:
    按照"{ v1 v2 ... vk }"的格式，每行输出一个连通集。先输出DFS的结果，再输出BFS的结果。
输入样例:
    8 6
    0 7
    0 1
    2 0
    4 1
    2 4
    3 5
输出样例:
    { 0 1 4 2 7 }
    { 3 5 }
    { 6 }
    { 0 1 2 7 4 }
    { 3 5 }
    { 6 }
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/
#include <iostream>
#include <vector>
#include <queue>

using namespace std;
// 考虑了一下，还是采用邻接表来储存图吧
typedef struct Node *Linked;

struct Node
{
    int data;
    Linked next;
};

vector<Linked> vertexAdj; // 顶点数组

void AddEdge(int v1, int v2);              // 向图中加入一条边
void InsertNode(Linked list, Linked node); // 向某条链中插入一个结点
void DFS(int vertexInd);                   // 进行DFS遍历
void BFS(int vertexInd);                   // 进行BFS遍历
void ResetAccess();                        // 重置结点访问情况

int main()
{
    int vertexNum, edgeNum; // 顶点数和边数
    cin >> vertexNum >> edgeNum;
    vertexAdj.resize(vertexNum); // 初始化顶点数组
    for (int i = 0; i < vertexNum; i++)
    {
        Linked vNode = new Node();
        vNode->data = 0; // 0代表尚未被访问
        vNode->next = NULL;
        vertexAdj[i] = vNode;
    }
    // 将顶点用边连接起来
    for (int i = 0; i < edgeNum; i++)
    {
        int v1, v2;
        cin >> v1 >> v2;
        AddEdge(v1, v2);
    }
    // DFS
    for (int i = 0; i < vertexNum; i++)
    {
        // 保证每个结点都能扫描到，即使是没有度的
        if (!vertexAdj[i]->data)
        {
            cout << "{ ";
            DFS(i);
            cout << "}\n";
        }
    }
    ResetAccess(); // 重置结点访问标记
    // BFS
    for (int i = 0; i < vertexNum; i++)
    {
        // 保证每个结点都能扫描到，即使是没有度的
        if (!vertexAdj[i]->data)
        {
            cout << "{ ";
            BFS(i);
            cout << "}\n";
        }
    }
    return 0;
}

void AddEdge(int v1, int v2)
{
    Linked node1 = new Node();
    node1->data = v2;
    Linked node2 = new Node();
    node2->data = v1;
    InsertNode(vertexAdj[v1], node1); // 插入到顶点v1的邻接链
    InsertNode(vertexAdj[v2], node2); // 插入到顶点v2的邻接链
}

// 向链list中插入一个结点，保证链表按编号升序排列
void InsertNode(Linked list, Linked node)
{
    Linked curr = list;
    while (curr->next != NULL)
    {
        if (node->data > curr->next->data)
            curr = curr->next;
        else
            break;
    }
    node->next = curr->next;
    curr->next = node;
}
// 重置结点访问情况
void ResetAccess()
{
    for (int i = 0, len = vertexAdj.size(); i < len; i++)
        vertexAdj[i]->data = 0;
}

// 参数传入一个结点的下标，从这个结点开始遍历
void DFS(int vertexInd)
{
    Linked curr = vertexAdj[vertexInd]->next;
    vertexAdj[vertexInd]->data = 1; // 标记这个结点已访问
    cout << vertexInd << " ";       // 打印结点编号
    while (curr)
    {
        int currInd = curr->data; // 获得当前结点下标
        // 如果这个结点没有被访问过就访问一下
        if (!vertexAdj[currInd]->data)
        {
            vertexAdj[currInd]->data = 1; // 标记此结点已访问
            DFS(currInd);
        }
        curr = curr->next;
    }
}

// 参数传入一个结点的下标
void BFS(int vertexInd)
{
    queue<int> vQueue;              // 用于遍历的队列
    vQueue.push(vertexInd);         // 将当前结点下标加入队尾
    vertexAdj[vertexInd]->data = 1; // 标记结点已访问
    // 队列不为空就继续
    while (!vQueue.empty())
    {
        int currInd = vQueue.front(); // 获得队头元素
        vQueue.pop();                 // 弹出队头元素
        cout << currInd << " ";       // 输出顶点编号
        Linked curr = vertexAdj[currInd]->next;
        while (curr)
        {
            int i = curr->data; // 获得当前遍历到的结点编号
            if (!vertexAdj[i]->data)
            {
                vQueue.push(i);         // 加到队尾
                vertexAdj[i]->data = 1; // 标记结点已访问
            }
            curr = curr->next;
        }
    }
}

/*
    本题主要考察无向图的储存以及其DFS和BFS遍历。

    无向图储存:
        这里我采用了邻接表的方式储存无向图。
        邻接表由一个顶点数组和若干条链表组成（这个数组的每个元素是一个链表的头指针）
        本题中输入样例的邻接表如下:
            [0] -> 1 -> 2 -> 7
            [1] -> 0 -> 4
            [2] -> 0 -> 4
            [3] -> 5
            [4] -> 1 -> 2
            [5] -> 3
            [6]
        (本题是无向图) 数组的每个元素代表一条链表，链表每个结点代表一条边。
            * 可以看到顶点0和1由一条边相连，数组0号元素对应的链表中有1，而1号元素对应的链表中有0。
            * 本题中要求【总是从编号最小的顶点出发，按编号递增的顺序访问邻接点】，因此我在将结点插入链表时是【保证了链表结点值的升序】顺序的。

    DFS遍历:
        深度优先搜索遍历顾名思义，就是优先不停往下“深挖”寻找邻接点的邻接点，找不到未被访问的邻接点时就回溯到上一层继续寻找...其间会有多次回溯，因此需用到递归或者栈来实现。
            * 对图遍历的时候可能图中会出现环路，因此对于【每个结点都需要有一个标记位】来标记其是否已经被访问过。
            * 【并不是所有结点都是连通的】，因此实际上我需要【遍历所有结点】，对其中【没有被访问过的结点】进行DFS遍历。

    BFS遍历：
        广度优先搜索遍历是先将当前结点的【所有邻接点】都遍历到，再遍历这些邻接点的所有邻接点。树的层序遍历其实就是BFS。
        BFS需要利用队列来实现, 通常是非递归算法。
            * 在遍历方面的注意事项和DFS的一致，需要遍历所有顶点，并给每个顶点设置访问标记位。

        - SomeBottle 2022.12.30


*/