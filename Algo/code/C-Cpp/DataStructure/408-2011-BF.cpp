#include <cstdio>

using namespace std;

int main()
{
    // input:
    int seq1[] = {11, 13, 15, 17, 19};
    int seq2[] = {2, 4, 6, 8, 20};
    // int seq1[] = {5, 6, 7, 8, 9};
    // int seq2[] = {0, 1, 2, 3, 4};
    int L = sizeof(seq1) / sizeof(seq1[0]); // 序列长度，两序列等长
    // algorithm:
    int midIndex = (2 * L - 1) / 2; // 中位数在合并后序列中的下标
    int tempVal = -1;               // 记录放入“临时序列”的值
    int tempInd = 0;                // 临时序列下标
    int ind1 = 0, ind2 = 0;         // 分别指向seq1和seq2的下标
    while (tempInd <= midIndex)     // 指到中间位置的时候，就找到了合并后序列的中间值。
    {
        // 序列1当前元素比序列2当前元素小，就加入到临时序列中
        // 这个“临时序列”并不存在，这里的实现就是用一个变量存储放入序列中的最后一个值
        // 因为我只需要取这最后一个值。
        tempVal = seq1[ind1] <= seq2[ind2] ? seq1[ind1++] : seq2[ind2++];
        tempInd++;
    }
    // 因为两序列等长，且我最多只需要装入一个序列长度的元素数即可找出结果，因此这里可以省去很多边界情况的判断。
    // output:
    printf("%d\n", tempVal);
    return 0;
}

/*
    这题如果要想出高效的写法，是有点得不偿失的。
    好在本题的两个序列均有序，且等长，如果写暴力算法的话是非常方便的。

    主要思路其实就是有序序列的合并，只不过合并到一半就可以停止了，因为这个时候已经能知道序列中间值了。

    时间复杂度：O(N)级别
    空间复杂度：O(1)级别

        - SomeBottle 2023.8.7
*/