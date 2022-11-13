/*
蓝桥杯基础练习VIP-Huffuman树
时间限制: 1s 内存限制: 128MB

题目描述
    Huffman树在编码中有着广泛的应用。在这里，我们只关心Huffman树的构造过程。
    给出一列数{pi}={p0,  p1,  …,  pn-1}，用这列数构造Huffman树的过程如下：
        1. 找到{pi}中最小的两个数，设为pa和pb，将pa和pb从{pi}中删除掉，然后将它们的和加入到{pi}中。这个过程的费用记为pa  +  pb。
        2. 重复步骤1，直到{pi}中只剩下一个数。
        在上面的操作过程中，把所有的费用相加，就得到了构造Huffman树的总费用。

    本题任务
        对于给定的一个数列，现在请你求出用该数列构造Huffman树的总费用。

    例如，对于数列{pi}={5,  3,  8,  2,  9}，Huffman树的构造过程如下：
        1. 找到{5,  3,  8,  2,  9}中最小的两个数，分别是2和3，从{pi}中删除它们并将和5加入，得到{5,  8,  9,  5}，费用为5。
        2. 找到{5,  8,  9,  5}中最小的两个数，分别是5和5，从{pi}中删除它们并将和10加入，得到{8,  9,  10}，费用为10。
        3. 找到{8,  9,  10}中最小的两个数，分别是8和9，从{pi}中删除它们并将和17加入，得到{10,  17}，费用为17。
        4. 找到{10,  17}中最小的两个数，分别是10和17，从{pi}中删除它们并将和27加入，得到{27}，费用为27。
        5. 现在，数列中只剩下一个数27，构造过程结束，总费用为5+10+17+27=59。
输入格式
    输入的第一行包含一个正整数n（n< =100）。
    接下来是n个正整数，表示p0,  p1,  …,  pn-1，每个数不超过1000。
输出格式
    输出用这些数构造Huffman树的总费用。
样例输入
    5
    5 3 8 2 9
样例输出
    59
*/
#include <iostream>
#include <vector>

using namespace std;

/*
    这题值得玩味的地方在【寻找最小的两个数字】这里，我采用了两个变量来储存最小数和次小数在数列中的下标
        1. 如果要 用一个循环 找到两个最小数：
            两个变量smallest和secondSmallest【在循环前】的初始值【不能在数组下标范围内】，必须要在循环体中进行初始化
                最初smallest=secondSmallest=-1，在循环中它们才会被赋上相应的下标数值，保证结果中smallest不会和secondSmallest撞车
        2. 一次循环中的查找方法：
            先比对当前的数是否小于最小数
                - 如果小于的话，在更新最小数之前，要把当前最小数先推后到次小数
                - 如果不小于的话，就比对次小数，看当前数是否小于次小数，如果小于就更新次小数

        本题的另外一个思路应该是在每次合并数字前【对整个数列进行排序】，保证最小数在数列尾部
        - SomeBottle20221113
*/
int main()
{
    int seqLen; // 数列长度
    cin >> seqLen;
    vector<int> numSeq(seqLen); // 初始化数列
    for (int i = 0; i < seqLen; i++)
        cin >> numSeq[i];
    int totalCost = 0; // 总费用
    // 循环“构建”霍夫曼树直至seqLen=1(只剩一个元素)
    while (seqLen > 1)
    {
        int smallest = -1;       // 最小数的下标
        int secondSmallest = -1; // 次小数的下标
        for (int i = 0; i < seqLen; i++)
        {
            if (smallest == -1 || numSeq[i] < numSeq[smallest])
            {
                secondSmallest = smallest; // 把最小值推到次小值
                smallest = i;              // 更新最小值
            }
            else if (secondSmallest == -1 || numSeq[i] < numSeq[secondSmallest])
            {
                secondSmallest = i; // 更新次小值
            }
        }
        totalCost += (numSeq[smallest] += numSeq[secondSmallest]); // 合并两数，并加入总费用
        numSeq.erase(numSeq.begin() + secondSmallest);             // 移除次小数（最小数已经被替换为两数的合并值）
        seqLen--;                                                  // 两个数合并为了一个数，数列长度-1
    }
    cout << totalCost;
    return 0;
}