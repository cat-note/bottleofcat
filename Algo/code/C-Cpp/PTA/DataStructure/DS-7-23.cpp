#include <iostream>
#include <string>

using namespace std;

int rebuildTree(const string &preorder, const string &inorder); // 重建树以求得高度

int main()
{
    int nodeNum; // 结点数
    cin >> nodeNum;
    string preorder; // 前序序列
    string inorder;  // 后序序列
    // 读入序列
    cin >> preorder;
    cin >> inorder;
    // 开始递归
    cout << rebuildTree(preorder, inorder);
    return 0;
}

int rebuildTree(const string &preorder, const string &inorder)
{
    if (preorder.length() == 0 || inorder.length() == 0)
        return 0; // 叶节点没有左右孩子
    // 在中序序列中找到当前【根节点】所在的下标
    int rootPos = inorder.find_first_of(preorder[0]);
    // 进入左孩子, 根据中序序列划分出【左子树】的先序序列和中序序列
    int leftH = rebuildTree(preorder.substr(1, rootPos), inorder.substr(0, rootPos));
    // 进入右孩子，根据中序序列划分出【右子树】的先序序列和中序序列
    int rightH = rebuildTree(preorder.substr(1 + rootPos), inorder.substr(rootPos + 1));
    return leftH > rightH ? leftH + 1 : rightH + 1; // 找比较高的那一棵计算高度
}