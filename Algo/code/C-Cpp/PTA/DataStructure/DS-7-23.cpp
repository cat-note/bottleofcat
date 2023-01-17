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

/*
    本题求的是二叉树的高度，因此程序主体就按【二叉树的后序遍历】写。
        * 由下至上逐层求得高度直至根结点。

    高度怎么求知道了，接下来要解决的是——树呢？你根本就不在沈阳大街，你躲哪儿去了！

    题目很贴心地给出了二叉树的【先序遍历序列】和【中序遍历序列】，借此我可以还原这棵树。
    -------------------------------
    先序遍历: 根->左->右
    中序遍历: 左->根->右
    后序遍历: 左->右->根

    💡 要【还原一棵二叉树】，【中序】遍历序列【必不可少】!!

    在先序序列和后序序列中，都可以很直观地看到【哪个是根结点】:

        - 先序序列中【首个】节点就是根结点
        - 后序序列中【最后一个】节点就是根结点

    如果仅有先序序列和后序序列，是【不一定能确定一棵树】的，这里就举两个极端的例子:

           1                          1
            \                        /
             2                      2
            /                        \
           3                          3
          /                          /
         4                          4

      先序: 1 2 3 4              先序: 1 2 3 4
      后序: 4 3 2 1              后序: 4 3 2 1
      中序: 1 4 3 2              中序: 2 4 3 1

    可以发现，一些情况下先序和后序【无法确认某棵子树到底是左子树还是右子树】。
    像上面这个极端例子中的两棵【单枝树】，它们的先序、后序序列甚至是【完全一致】的。
    ---------------------------------
    有了【中序序列】，我就可以根据【已找到的根结点】，从中序序列中划分出【左子树】和【右子树】的元素。

    比如样例中的:
        先序: [A]BDFGHIEC
        中序: FDHGIBE[A]C
    可以看到树根结点是[A]结点，在【中序序列】中以[A]结点为界限，将中序序列划分为两部分:

        左子树的元素: FDHGIBE (7个元素)
        右子树的元素: C (1个元素)

    因为先序遍历也是【先扫完左子树再扫右子树】，因此在【先序序列】中，【左子树的元素是相邻的】，且【右子树的元素也是相邻的】，根据上面划分出来的两部分可以【对先序序列进行划分】:

        A       [BDFGHIE]     [C]
        ↑       ↑             ↑
        根结点  左子树7个元素  右子树1个元素

    这便是【每层递归体中】要完成的操作，之后将划分好的先序序列和中序序列递交给下层递归即可。
    ----------------------------------
    分析一遍样例:
                                    先 [A]BDFGHIEC
                                    中 FDHGIBE[A]C
                                    /             \
                        先 [B]DFGHIE               先 [C]
                        中 FDHGI[B]E               中 [C]
                       /            \
             先 [D]FGHI             先 [E]
             中 F[D]HGI             中 [E]
            /          \
        先 [F]          先 [G]HI
        中 [F]          中 H[G]I
                       /        \
                    先 [H]       先 [I]
                    中 [H]       中 [I]

    具体程序实现中为图方便，我采用了C++ string来储存遍历序列，它提供了find_first_of, substr这些方便的方法。
    可以看到，这个题解的代码量并不大。

        - SomeBottle 2023.1.17
*/
/*
7-23 还原二叉树
    给定一棵二叉树的先序遍历序列和中序遍历序列，要求计算该二叉树的高度。

输入格式:
    输入首先给出正整数N（≤50），为树中结点总数。下面两行先后给出先序和中序遍历序列，均是长度为N的不包含重复英文字母（区别大小写）的字符串。
输出格式:
    输出为一个整数，即该二叉树的高度。
输入样例:
    9
    ABDFGHIEC
    FDHGIBEAC
输出样例:
    5
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/