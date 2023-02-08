#include <cstdio>
#include <vector>

using namespace std;

int main()
{
    int seqSize; // 序列大小
    scanf("%d", &seqSize);
    vector<int> seq1(seqSize); // 序列1
    vector<int> seq2(seqSize); // 序列2
    // 读入序列
    for (int i = 0; i < seqSize; i++)
        scanf("%d", &seq1[i]);
    for (int i = 0; i < seqSize; i++)
        scanf("%d", &seq2[i]);
    // 合并两序列。这里的并集应该是允许有重复元素的
    vector<int> merged(seqSize * 2);
    int ptr1 = 0;
    int ptr2 = 0;
    int mergedLen = merged.size();
    for (int i = 0; i < mergedLen; i++)
    {
        if (ptr1 >= seqSize)               // 第一个序列已经扫描结束
            merged[i] = seq2[ptr2++];      // 继续把第二个序列的元素读完
        else if (ptr2 >= seqSize)          // 第二个序列已经扫描结束
            merged[i] = seq1[ptr1++];      // 继续把第一个序列的元素读完
        else if (seq1[ptr1] <= seq2[ptr2]) // 第一个序列元素较小，优先加入合并序列
            merged[i] = seq1[ptr1++];
        else // 第二个序列元素较小，优先加入序列
            merged[i] = seq2[ptr2++];
    }
    // 直接访问中位数
    printf("%d", merged[(mergedLen - 1) / 2]);
    return 0;
}

/*
    这题要访问中位数，因此用数组这种随机存取结构实现是很合适的。

    另外，题目给出的两个序列都是有序序列，因此和【归并排序】一样，合并两个序列为一个序列，
    然后访问下标为(N-1)/2的元素即可。

    ------------------------------
    这题给的两个样例都非常狡猾，根本不知道到底能不能有重复元素:

        1 2 3 3 4 5 5 6 7 9
        1 2 3 4 5 6 7 9
        上面这两个合并后的序列的“中位数”都是4

        -100 -50 -10 0 1 1 1 1 2 3 4 5
        -100 -50 -10 0 1 2 3 4 5
        上面这两个合并后的序列的“中位数”也都是1

    你逗我呢？鉴于上一题(7-52)是不排除重复元素的，这里我也赌不排除重复元素。
    💡 事实证明，这题的并集确实【不排除重复元素】
    ------------------------------

        - SomeBottle 2023.2.8
*/

/*
7-53 两个有序序列的中位数
已知有两个等长的非降序序列S1, S2, 设计函数求S1与S2并集的中位数。有序序列A0,A1,⋯,A(N−1)的中位数指A[(N−1)/2]的值,即第⌊(N+1)/2⌋个数（A0为第1个数）。

输入格式:
    输入分三行。第一行给出序列的公共长度N（0<N≤100000），随后每行输入一个序列的信息，即N个非降序排列的整数。数字用空格间隔。
输出格式:
    在一行中输出两个输入序列的并集序列的中位数。
输入样例1:
    5
    1 3 5 7 9
    2 3 4 5 6
输出样例1:
    4
输入样例2:
    6
    -100 -10 1 1 1 1
    -50 0 2 3 4 5
输出样例2:
    1
代码长度限制
    16 KB
时间限制
    200 ms
内存限制
    64 MB
*/