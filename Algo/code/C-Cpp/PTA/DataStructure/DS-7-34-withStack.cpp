#include <cstdio>
#include <vector>
#include <stack>

using namespace std;

/*
    本题考察对【有向图是否有环】的判断，适合采用拓扑排序
*/

// 邻接表节点
typedef struct ListNode
{
    int data; // 记录入度数或邻接点下标
    ListNode *next;
} *List;

// 邻接表
class AdjList
{
private:
    int vertexNum;
    vector<List> listArr;

public:
    AdjList(int vertexNum)
    {
        this->vertexNum = vertexNum;
        // 初始化邻接表
        listArr.resize(vertexNum);
        for (int i = 0; i < vertexNum; i++)
        {
            List node = new ListNode();
            node->data = 0; // 初始入度数为0
            node->next = NULL;
            listArr[i] = node;
        }
    }
    ~AdjList()
    {
        // 析构时释放内存
        for (int i = 0; i < vertexNum; i++)
        {
            List curr = listArr[i]->next;
            while (curr)
            {
                List next = curr->next;
                delete curr;
                curr = next;
            }
            delete listArr[i];
        }
    }
    // 往有向图中添加一条v1指向v2的边
    void addEdge(int v1, int v2)
    {
        List node = new ListNode();
        node->data = v2; // 指向v2
        node->next = listArr[v1]->next;
        listArr[v1]->next = node; // 头插法
        listArr[v2]->data++;      // v2顶点的入度增加
    }
    // 拓扑排序判断是否无环
    bool noCycle()
    {
        // 复制各顶点的入度，用于拓扑排序，💡【不影响原邻接表】
        // i顶点被访问后，indegree[i]会被标为-1，代表顶点被取出
        vector<int> indegree(vertexNum);
        stack<int> zeroStack; // 储存0入度顶点的栈
        for (int i = 0; i < vertexNum; i++)
        {
            indegree[i] = listArr[i]->data; // 备份入度，因为后面要拆边
            if (listArr[i]->data == 0)      // 顺带把0入度的顶点压入栈
                zeroStack.push(i);
        }
        while (!zeroStack.empty())
        {
            int currV = zeroStack.top(); // 取出一个0度顶点
            zeroStack.pop();
            indegree[currV] = -1; // 标记已访问
            List curr = listArr[currV]->next;
            while (curr) // 扫描邻接点
            {
                // “拆掉”入边，更新邻接点入度
                if (--indegree[curr->data] == 0)
                    zeroStack.push(curr->data); // 如果更新后变成0入度了，就压入栈
                curr = curr->next;
            }
        }
        // 检查是否有顶点未被取出
        for (int i = 0; i < vertexNum; i++)
        {
            if (indegree[i] != -1) // 有顶点未被取出就说明有环
                return false;
        }
        return true;
    }
};

int main()
{
    int vertexNum; // 顶点数
    scanf("%d", &vertexNum);
    // 初始化邻接表
    AdjList graph(vertexNum);
    // 读入各点的依赖
    for (int i = 0; i < vertexNum; i++)
    {
        int depend; // 依赖数
        scanf("%d", &depend);
        // 某个顶点的依赖顶点就是其前驱顶点。
        for (int j = 0; j < depend; j++)
        {
            int vertex;
            scanf("%d", &vertex);
            vertex--; // 题目顶点编号是1~N，这里转换为0~N-1
            graph.addEdge(vertex, i);
        }
    }
    // 拓扑排序判断是否有环
    printf(graph.noCycle() ? "1" : "0");
    return 0;
}
/*
    之所以能用栈进行辅助，是基于一点的:

        - 去除一个【0入度】的顶点【不会影响图中现存的0入度顶点】。
    -----------------------
    因为题目给的数据规模并不大，因此栈辅助的效果并不明显。

        - SomeBottle 2023.1.25
*/