/*
7-5 堆中的路径
将一系列给定数字依次插入一个初始为空的小顶堆H[]。随后对任意给定的下标i，打印从H[i]到根结点的路径。

输入格式:
    每组测试第1行包含2个正整数N和M(≤1000)，分别是插入元素的个数、以及需要打印的路径条数。下一行给出区间[-10000, 10000]内的N个要被插入一个初始为空的小顶堆的整数。最后一行给出M个下标。
输出格式:
    对输入中给出的每个下标i，在一行中输出从H[i]到根结点的路径上的数据。数字间以1个空格分隔，行末不得有多余空格。
输入样例:
    5 3
    46 23 26 24 10
    5 4 3
输出样例:
    24 23 10
    46 23 10
    26 10
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/
#include <iostream>
#include <vector>

using namespace std;

vector<int> H; // 初始化存放堆的数组

void Swap(int &src, int &dest);      // 交换结点值
void SiftUpInsert(int val, int pos); // 插入后上滤结点进行建堆(自顶向下)
void PrintPath(int start);           // 打印从start下标到根节点的路径

int main()
{
    int insertNum, pathNum;
    cin >> insertNum >> pathNum; // 输入元素个数和路径个数
    H.resize(insertNum);         // 初始化数组结点数
    for (int i = 0; i < insertNum; i++)
    {
        int insertVal;
        cin >> insertVal;
        SiftUpInsert(insertVal, i);
    }
    for (int i = 0; i < pathNum; i++)
    {
        int startInd;
        cin >> startInd;
        PrintPath(startInd);
        if (i != pathNum - 1)
            cout << "\n";
    }
    return 0;
}

// 交换结点值
void Swap(int &src, int &dest)
{
    int temp = src;
    src = dest;
    dest = temp;
}

// 自顶向下(上滤结点)建堆，pos为结点插入位置, val为结点值
void SiftUpInsert(int val, int pos)
{
    H[pos] = val;   // 新增结点
    while (pos > 0) // 不断和父节点比较直至根节点
    {
        int parentInd = (pos - 1) / 2; // 父节点下标
        // 如果当前结点比其父节点小，就将其和父节点交换
        if (H[pos] < H[parentInd])
            Swap(H[pos], H[parentInd]);
        else
            break;
        pos = parentInd; // 向上一层，继续上滤结点
    }
}

// 注意末尾不能有多余空格
void PrintPath(int start)
{
    int i = start - 1; // 注意题目默认下标从1开始，但我是从0开始
    for (; i > 0; i = (i - 1) / 2)
    {
        cout << H[i] << " ";
    }
    cout << H[i];
}

/*
    本题主要考验建堆操作，在纸上画一画可以得知本题采用的是自顶向下建堆法。
    (我另外还写了个自下而上建堆的实现，详见DS-7-5-SiftDown)

    💡 数据结构里的堆常用【完全二叉树】进行实现，而得益于完全二叉树的性质，其可以被完美存放在数组这种【顺序储存结构】中（因为能保证数组的容量不被浪费, 且能通过下标对节点和其父节点进行随机访问，十分便捷。）

    所谓自顶向下建堆法，就是：
        在向完全二叉树【插入结点的过程中】将结点【不断和父节点进行比较交换】，本题中就是使得父节点始终比所有子节点小。
            * 这个和父节点进行比较交换的操作可以称为【上滤】(Sift-up)
        “自顶向下”是指交换过程中，节点都是从最底层向树根部的方向进行交换，从顶部开始保证堆序性。

    自顶向下建堆法在代码实现上比自下而上建堆法要简单一些，但就最糟糕情况的时间复杂度而言要略逊一筹：
        * 自顶向下:O(nlogn)
        * 自下而上:O(n)
    这里有一个比较详细的图解视频: https://www.bilibili.com/video/BV1AF411G7cA

        - SomeBottle 2022.12.30

*/