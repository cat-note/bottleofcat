#include <cstdio>

using namespace std;

typedef struct BiTNode
{
    struct BiTNode *left;
    int weight;
    struct BiTNode *right;
} *BiTree;

// 通过二叉树结点序列，递归构建二叉树
BiTree ConstructTree(int *treeSeq, int seqLen, int currentNode)
{
    if (currentNode >= seqLen) // 到叶结点了
        return NULL;
    if(treeSeq[currentNode]==-1) // -1也代表空结点
        return NULL;
    BiTree tNode = new BiTNode();
    tNode->weight = treeSeq[currentNode];                               // 给结点附上权重
    tNode->left = ConstructTree(treeSeq, seqLen, currentNode * 2);      // 构造左子树
    tNode->right = ConstructTree(treeSeq, seqLen, currentNode * 2 + 1); // 构造右子树
    return tNode;
}

// 计算WPL(二叉树，当前路径长度（边数）)
int GetWPL(BiTree bt, int pathLen)
{
    if (!bt)
        return 0;
    int res = 0;                 // 存储统计的叶结点WPL结果
    if (!bt->left && !bt->right) // 遇到叶结点了
        res += bt->weight * pathLen,printf("Node:%d,path:%d\n",bt->weight,pathLen);
    else
    {
        res += GetWPL(bt->left, pathLen + 1);
        res += GetWPL(bt->right, pathLen + 1);
    }
    return res;
}

int main()
{
    // input:
    int treeWSeq[] = {-1, 1, 2, 3, 4, 5, 6, 7, -1, 8, -1, -1, 9, 10, -1, -1};       // 二叉树的结点权重序列（从下标1开始），为-1代表此结点不存在
    BiTree bt = ConstructTree(treeWSeq, sizeof(treeWSeq) / sizeof(treeWSeq[0]), 1); // 构造二叉树
    // algorithm:
    int treeWPL=GetWPL(bt,0);
    // output:
    printf("WPL of the BiTree: %d\n",treeWPL);
    return 0;
}

/*
    我顺便写了个按照完全二叉树结点层序序列构造二叉树的函数，构造二叉树如下：
                       1
                  /       \
                 2         3
                / \       / \
               4   5     6  7
                \       / \
                 8     9  10
    标出的数字即为权重，树的WPL从定义上来说其实就是指的叶结点WPL之和。  

    计算WPL主要就是要找到从根节点到每个叶结点的路径，因此采用DFS（先序遍历）较为简单！

    时间复杂度：O(n)，和结点数目一个数量级，因为遍历了一遍结点
    空间复杂度：O(n)，最优（树较为平衡）时为O(log2n)

        - SomeBottle 2023.8.13

*/