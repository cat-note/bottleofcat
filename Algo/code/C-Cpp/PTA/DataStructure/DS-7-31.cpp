#include <cstdio>
#include <vector>
#include <stack>

using namespace std;

typedef vector<int> Seq;

// 树结点
struct Node
{
    int bstVal;  // 二叉搜索树键值
    int heapVal; // 小根堆键值
};

vector<Node> nodes; // 储存所有节点
Seq leftChilds;     // 储存左孩子
Seq rightChilds;    // 储存右孩子

bool meetHeap(int index); // 检查结点是否满足小根堆

int main()
{
    int nodeNum; // 节点个数
    scanf("%d", &nodeNum);
    nodes.resize(nodeNum);
    leftChilds.resize(nodeNum);
    rightChilds.resize(nodeNum);
    vector<bool> hasParent(nodeNum, false); // 标记是否有父节点
    // 读入树
    for (int i = 0; i < nodeNum; i++)
    {
        int k1, k2, left, right;
        scanf("%d %d", &k1, &k2);
        nodes[i].bstVal = k1;
        nodes[i].heapVal = k2;
        scanf("%d %d", &left, &right);
        leftChilds[i] = left;
        rightChilds[i] = right;
        if (left >= 0)
            hasParent[left] = true; // 标记有前驱
        if (right >= 0)
            hasParent[right] = true;
    }
    // 首先要找到树的根结点(没有前驱的节点)
    int rootInd = -1;
    for (int i = 0; i < nodeNum; i++)
        if (!hasParent[i])
            rootInd = i;
    if (rootInd == -1)
    {
        // 根本找不着根结点的情况
        printf("NO");
    }
    else
    {
        bool isCartesian = true; // 标记是否是笛卡尔树
        // DFS遍历树(堆栈实现)
        // 二叉搜索树有个性质: 中序遍历序列是一个递增的序列
        // 因此这里也采用中序遍历进行判断
        stack<int> traversal;
        vector<bool> visited(nodeNum); // 标记各节点是否被访问
        int prevBSTVal = -1;           // 前一个中序遍历二叉搜索树的值
        traversal.push(rootInd);       // 初始化:把根结点下标压入栈
        while (!traversal.empty())
        {
            int curr = traversal.top(); // 出栈结点下标
            // 直到最左下角的节点
            while (leftChilds[curr] != -1)
            {
                // 先把左分支的元素全压入栈
                int left = leftChilds[curr];
                if (visited[left]) // 不继续遍历访问过的分支
                    break;
                else
                    visited[left] = true; // 标记已访问
                curr = left;              // 转移指针到左孩子
                traversal.push(curr);     // 左孩子入栈
            }
            traversal.pop(); // 处理节点
            if (nodes[curr].bstVal < prevBSTVal || !meetHeap(curr))
            {
                // 如果中序遍历序列并不是递增的，就不是二叉搜索树
                // 如果不满足小根堆的堆序性，则不满足小根堆
                isCartesian = false;
                break;
            }
            else
            {
                prevBSTVal = nodes[curr].bstVal; // 记录为中序遍历的前一项
            }
            // 如果这个节点有右子树
            if (rightChilds[curr] != -1)
            {
                traversal.push(rightChilds[curr]); // 将右孩子压入栈
            }
        }
        printf(isCartesian ? "YES" : "NO");
        return 0;
    }
}

// 检查结点是否满足小根堆
bool meetHeap(int index)
{
    int left = leftChilds[index];
    int right = rightChilds[index];
    if (left != -1 && nodes[index].heapVal > nodes[left].heapVal)
    {
        // 在左节点不为空的情况下，如果节点值大于其左孩子就不满足
        return false;
    }
    if (right != -1 && nodes[index].heapVal > nodes[right].heapVal)
    {
        // 在右节点不为空的情况下，如果节点值大于其右孩子就不满足
        return false;
    }
    return true;
}

/*
    本题虽然题目写着“笛卡尔树”，但实际上考的不是笛卡尔树的建树。
    笛卡尔树的每个节点有一个二元组(k1,k2)，整棵树的节点中就k1而言满足【二叉搜索树】的规律，而就k2而言满足【小根堆】的堆序性。

    因此本题考察的是对二叉搜索树和小根堆【是否成立】的判断。
    ---------------------
    判断小根堆是比较简单的:
        - 只需要遍历树中每个节点，如果【每个节点的k2值都比其左右孩子的k2值小】，那么就满足小根堆。

    --------------------
    判断二叉搜索树的话，不要上来就用先序遍历。
    题目特意卡了一种情况:
        - 如果只比较了【每个节点的k1值是否比左孩子k1值大、比右孩子k1值小】的话，是不能涵盖到所有情况的。比如下面这个:

                    5
                   / \
                  2   7
                     / \
                   [3]  8

        这棵树中【每个节点】的k1值都比其左孩子的大，且比右孩子的小。

        但是可以发现，“3”这个节点值在【根结点(值为5)的右子树】中，右子树中每个元素都应该比根结点小，这种情况明显是错误的。
        所以这种方法没法正确判断二叉搜索树。

    💡二叉搜索树的【中序遍历】有个特性:
        中序遍历序列是保证【递增】的

    借此，只需要中序遍历树，检查【中序遍历序列是否保持递增】即可。
    ---------------------
    本题中采用了堆栈(非递归)的方式实现了【中序遍历】，主要思想是:

        1. 先【找到根结点】，将根结点压入栈

        2. 从栈顶节点出发，把【左分支】上所有节点压入栈(直至没有左节点的节点为止)。
            - 同时标记这些节点【被访问】
            - 如果遇到【被访问过的节点】，就不再继续。

        3. 弹出栈顶元素(输出)

        4. 如果弹出的栈顶元素有【右孩子】，就把右孩子压入栈中。

    其中2，3，4是迭代过程，栈空时迭代结束。
    ----------------------
        - SomeBottle 2023.1.23

*/