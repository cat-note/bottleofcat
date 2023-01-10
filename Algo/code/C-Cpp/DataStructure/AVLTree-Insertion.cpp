#include <iostream>

using namespace std;

typedef struct TreeNode *Tree;

struct TreeNode
{
    Tree left;  // 左子树
    Tree right; // 右子树
    short bf; // 平衡因子
    int val; // 节点值
};

class AVLTree
{
};

int main()
{

    return 0;
}