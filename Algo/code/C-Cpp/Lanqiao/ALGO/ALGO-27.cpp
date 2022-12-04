/*
蓝桥杯算法训练VIP-FBI树
题目描述
    我们可以把由“0”和“1”组成的字符串分为三类：全“0”串称为B串，全“1”串称为I串，既含“0”又含“1”的串则称为F串。
    FBI树是一种二叉树，它的结点类型也包括F结点，B结点和I结点三种。由一个长度为2^N的“01”串S可以构造出一棵FBI树T，递归的构造方法如下：
        1) T的根结点为R，其类型与串S的类型相同；
        2) 若串S的长度大于1，将串S从中间分开，分为等长的左右子串S1和S2；由左子串S1构造R的左子树T1，由右子串S2构造R的右子树T2。
        现在给定一个长度为2^N的“01”串，请用上述构造方法构造出一棵FBI树，并输出它的后序遍历序列。
输入格式
    第一行是一个整数N（0<=N<=10），第二行是一个长度为2^N的“01”串。
    数据规模和约定，对于全部的数据，N<=10。
输出格式
    包括一行，这一行只包含一个字符串，即FBI树的后序遍历序列。
样例输入
    3
    10001011
样例输出
    IBFBBBFIBFIIIFF
*/
#include <iostream>

using namespace std;

// F、B、I类型用枚举型表示
typedef enum
{
    F = 'F',
    B = 'B',
    I = 'I'
} fbiType;

typedef struct Node treeNode;
struct Node
{
    treeNode *left;  // 左子
    treeNode *right; // 右子
    fbiType type;    // 节点类型
};

fbiType typeOf(string zeroOne);

void treeGrow(treeNode *tree, string zeroOne);

void traverse(treeNode *tree);

int main()
{
    int N;          // 我怎么感觉这题的N，没有什么卵用？
    string zeroOne; // 01字符串
    cin >> N;
    cin >> zeroOne;
    // 建立一个节点作为根节点
    treeNode *root = new treeNode();
    root->left = NULL;
    root->right = NULL;
    root->type = typeOf(zeroOne); // 得到字串的类型
    treeGrow(root, zeroOne);      // 树生长
    traverse(root);               // 后序(Post Order)遍历树
    return 0;
}
/**
 * @brief 判断01字符串的类型
 *
 * @param zeroOne 01字符串
 * @return fbiType F/B/I
 */
fbiType typeOf(string zeroOne)
{
    char prevChar = zeroOne[0];
    for (int i = 1, len = zeroOne.length(); i < len; i++)
    {
        // 只要和上一个字符不相同就说明字符串中同时出现了0和1，则为F串
        if (zeroOne[i] != prevChar)
            return F;
    }
    return prevChar == '0' ? B : I; // 全为0为B串、全为1则为I串
}

/**
 * @brief 构造树的函数(递归)
 *
 * @param tree 指向现有树节点
 * @param zeroOne 当前节点的01字符串
 */
void treeGrow(treeNode *tree, string zeroOne)
{
    int strLen = zeroOne.length();
    if (strLen <= 1) // 字符串长度为1时说明达到叶节点，生长到此为止
        return;
    int halfLen = strLen / 2;
    // 将字符串切为左右两部分
    string leftStr = zeroOne.substr(0, halfLen);
    string rightStr = zeroOne.substr(halfLen);
    // 创建左右子树
    treeNode *leftTree = new treeNode();
    treeNode *rightTree = new treeNode();
    leftTree->left = NULL;
    leftTree->right = NULL;
    rightTree->left = NULL;
    rightTree->right = NULL;
    // 左右子节点的类型
    leftTree->type = typeOf(leftStr);
    rightTree->type = typeOf(rightStr);
    // 指定父节点
    tree->left = leftTree;
    tree->right = rightTree;
    // 递归生长
    treeGrow(leftTree, leftStr);
    treeGrow(rightTree, rightStr);
}
/**
 * @brief 后序遍历树(左、右、根)
 *
 * @param tree 树结点
 */
void traverse(treeNode *tree)
{
    if (tree->left) // 如果有左节点就深入到左子树
        traverse(tree->left);
    if (tree->right) // 如果有右子树就深入到右子树
        traverse(tree->right);
    // 最后再打印当前节点的信息，实现后序
    cout << (char)tree->type;
}

/*
    这题考察的其实就是【满二叉树】的生成与【后序遍历】。

    吐槽：
        最开始很令我不解的是题目给的输入中的第一个数N，这玩意就像来缓解气氛的气氛组一样，感觉没什么用...
        原题是这样解释输入的：
            第一行是一个整数N（0<=N<=10），第二行是一个长度为2N的“01”串
        样例输入是这样的：
            3
            10001011
        第一行是N，那么按题目说...第二行如果是2N长的字串应该是6吧，不对啊！

        程序写完，确实没用上这个3，但看到输出，我恍然大悟，题目的意思是2的N次方啊！
        一眼些瓶，鉴定为复制的题目，这下把上标格式给掉了吧。
        得亏程序不太用得上这个N就是了。

    为什么字符串长度是2^N呢？
        因为二叉树每【长出一棵子树】，字符串长度就会【减半】，填入当前子树的两个节点。
        直至最后叶节点中，字符串长度为1。至此生成了一棵【满二叉树】(整棵二叉树填满了节点)。

    后序遍历对于每颗子树来说：
        左->右->根

    树的生成和遍历在这里都依靠递归实现。

        - SomeBottle 2022.12.3

*/