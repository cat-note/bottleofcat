/*
7-3 树的同构
给定两棵树T1和T2。如果T1可以通过若干次左右孩子互换就变成T2，则我们称两棵树是“同构”的。例如图1给出的两棵树就是同构的，因为我们把其中一棵树的结点A、B、G的左右孩子互换后，就得到另外一棵树。而图2就不是同构的。
(题图:https://raw.githubusercontent.com/SomeBottle/bottleofcat/main/Algo/code/C-Cpp/PTA/Pics/question-7-3.png)

现给定两棵树，请你判断它们是否是同构的。

输入格式:
    输入给出2棵二叉树树的信息。对于每棵树，首先在一行中给出一个非负整数N (≤10)，即该树的结点数（此时假设结点从0到N−1编号）；随后N行，第i行对应编号第i个结点，给出该结点中存储的1个英文大写字母、其左孩子结点的编号、右孩子结点的编号。如果孩子结点为空，则在相应位置上给出“-”。给出的数据间用一个空格分隔。注意：题目保证每个结点中存储的字母是不同的。
输出格式:
    如果两棵树是同构的，输出“Yes”，否则输出“No”。
输入样例1（对应图1）：
    8
    A 1 2
    B 3 4
    C 5 -
    D - -
    E 6 -
    G 7 -
    F - -
    H - -
    8
    G - 4
    B 7 6
    F - -
    A 5 1
    H - -
    C 0 -
    D - -
    E 2 -
输出样例1:
    Yes
输入样例2（对应图2）：
    8
    B 5 7
    F - -
    A 0 3
    C 6 -
    H - -
    D - -
    G 4 -
    E 1 -
    8
    D 6 -
    B 5 -
    E - -
    H - -
    C 0 2
    G - 3
    F - -
    A 1 4
输出样例2:
    No
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/
#include <iostream>

using namespace std;

typedef struct
{
    char name;    // 这个节点的名
    int leftPos;  // 左子位置
    int rightPos; // 右子位置
} TreeNode, *Tree;

Tree ReadTree(int &treeLen);                                   // 读入树
int FindTreeRoot(Tree tree, int treeLen);                      // 寻找树的根节点
bool IsIsomorph(Tree tree1, int root1, Tree tree2, int root2); // 判断是否同构

int main()
{
    int treeLen1;
    int treeLen2;
    Tree tree1 = ReadTree(treeLen1);
    Tree tree2 = ReadTree(treeLen2);
    if (!treeLen1 && !treeLen2)
    { // 特殊情况：两树皆空，同构
        cout << "Yes";
    }
    else if (!treeLen1 || !treeLen2)
    { // 特殊情况：只有一棵树为空，非同构
        cout << "No";
    }
    else
    {
        // 很重要的一点是，题目输入结点并不是根节点一定放在最前面的，要先寻找根结点
        int treeRoot1 = FindTreeRoot(tree1, treeLen1);
        int treeRoot2 = FindTreeRoot(tree2, treeLen2);
        if (tree1[treeRoot1].name != tree2[treeRoot2].name) // 根节点都不相等就不谈同构了
            cout << "No";
        else
            cout << (IsIsomorph(tree1, treeRoot1, tree2, treeRoot2) ? "Yes" : "No");
    }
    return 0;
}

Tree ReadTree(int &treeLen)
{                // 读取并创建树，这里我采用顺序储存
    int nodeNum; // 树节点数
    cin >> nodeNum;
    treeLen = nodeNum;
    Tree tree = new TreeNode[nodeNum]; // 初始化树结点
    // 注意，题目给出的输入并不一定是自二叉树根开始的
    for (int i = 0; i < nodeNum; i++)
    {
        cin >> tree[i].name;
        // 注意，这里题目会用字符-代表孩结点为空
        // 题目特意让N<=10了，结点编号只会是0-9
        char leftPos, rightPos;
        cin >> leftPos >> rightPos;
        if (leftPos == '-')
            tree[i].leftPos = -1; // 用-1代表空孩子
        else
            tree[i].leftPos = (int)(leftPos - '0'); // 转换为数值
        if (rightPos == '-')
            tree[i].rightPos = -1; // 用-1代表空孩子
        else
            tree[i].rightPos = (int)(rightPos - '0'); // 转换为数值
    }
    return tree;
}

int FindTreeRoot(Tree tree, int treeLen)
{
    bool hasPre[treeLen]{false}; // 临时用的数组, 标记对应编号的结点是否有前驱
    for (int i = 0; i < treeLen; i++)
    {
        if (tree[i].leftPos != -1)
            hasPre[tree[i].leftPos] = true; // 标记有前驱
        if (tree[i].rightPos != -1)
            hasPre[tree[i].rightPos] = true; // 标记有前驱
    }
    for (int i = 0; i < treeLen; i++)
        if (!hasPre[i])
            return i; // 找到唯一一个没有前驱的结点并返回
    return -1;        // 按理来说不应该找不到,不过这里还是加个返回-1吧
}

// 判断是否同构
bool IsIsomorph(Tree tree1, int pos1, Tree tree2, int pos2)
{
    Tree node1 = &tree1[pos1]; // 第一棵树的结点
    Tree node2 = &tree2[pos2]; // 第二棵树的结点
    // cout<<"COMPARE TREE1 NODE:"<<node1->name<<" TREE2 NODE:"<<node2->name<<"\n";
    //  结点1的左孩和右孩名, 如果没有那就是'-'
    char node1Left = node1->leftPos != -1 ? tree1[node1->leftPos].name : '-';
    char node1Right = node1->rightPos != -1 ? tree1[node1->rightPos].name : '-';
    // 结点2的左孩和右孩名, 如果没有那就是'-'
    char node2Left = node2->leftPos != -1 ? tree2[node2->leftPos].name : '-';
    char node2Right = node2->rightPos != -1 ? tree2[node2->rightPos].name : '-';
    bool result = false;
    // cout<<"\t node1Left:"<<node1Left<<" node2Left:"<<node2Left<<" node1Right:"<<node1Right<<" node2Right:"<<node2Right<<"\n";
    if (node1Left == '-' && node2Left == '-' && node1Right == '-' && node2Right == '-')
    {
        // 抵达叶节点
        result = true;
    }
    else if (node1Left == node2Left && node1Right == node2Right)
    {
        // 如果二者孩子名都是对应的，说明直到这一层是同构的
        // 左孩子不为空
        if (node1Left != '-')
            result = IsIsomorph(tree1, node1->leftPos, tree2, node2->leftPos);
        // 右孩子不为空
        if (node1Right != '-')
            result &= IsIsomorph(tree1, node1->rightPos, tree2, node2->rightPos);
    }
    else if (node1Left == node2Right && node1Right == node2Left)
    {
        // 也可以左右孩子交换一下再看行不行
        // 左孩子不为空
        if (node1Left != '-')
            result = IsIsomorph(tree1, node1->leftPos, tree2, node2->rightPos);
        // 右孩子不为空
        if (node1Right != '-')
            result &= IsIsomorph(tree1, node1->rightPos, tree2, node2->leftPos);
    }
    return result;
}

/*
    这题很搞的地方是输入:
        从题目给的样例可以发现，输入【并不一定是从树的根节点】开始给的，这就需要我额外花费一些开销去【寻找根节点】。

    (因此这题我对于二叉树结点采用了顺序储存而不是链式，没必要再大费周章去链式构建二叉树)

    至于找到根节点后的解题过程，就主要是DFS遍历思想了，只不过这回的DFS是“双排”模式，需要同时遍历两棵树的同一层。

    大概步骤如下:
        1. 判断树是否为空，特殊处理空树
            (注意，两棵树皆为空树也是同构的)

        2. 判断两树【根节点是否是同一类】结点，如果这里就不同，那就直接不同构了。

        3. 开始DFS同时遍历两棵树，对于每两棵树/子树:

            对比两棵树【根节点下的左孩子和右孩子】:

                - 如果两棵树根节点的左孩子和右孩子都能对应上就可以进入更深层递归
                - 如果没有对应上，【交换其中一个根节点的左右孩子】，如果交换后能对应上，就能进入更深层递归
                - 其他情况，说明两棵树【不同构】了。

    - SomeBottle 2022.12.12
*/