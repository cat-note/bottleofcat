#include <cstdio>

using namespace std;

int main()
{
    int seq[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int seqLen = sizeof(seq) / sizeof(seq[0]);
    int p = 12; // 循环左移位数
    // 以下是暴力算法。
    p = p % seqLen; // 可能会左移超过一个序列的长度，先模一下，得到实际要移动的位数。
    // 开一个一样大的临时数组
    int tempArr[seqLen];
    int i = 0; // i指向临时数组
    for (; i < seqLen - p; i++)
    {
        // 先把原序列的[p,seqLen-1]这部分读入临时数组的前半部分
        tempArr[i] = seq[i + p];
    }
    for (int j = 0; j < p; j++)
    {
        // 再把原序列的[0,p-1]这部分读入临时数组的后半部分
        tempArr[i++] = seq[j];
    }
    // 打印结果
    printf("Result: ");
    for (int i = 0; i < seqLen; i++)
        printf("%d ", tempArr[i]);
    printf("\n");
    return 0;
}

/*
    这一题主要是一个线性表循环移位问题，把线性表循环左移p位。

    这个源代码是我第一时间能想到的暴力解(Brute-Force)。
    主要思路就是把序列分为两半来看待，将两半对调即可，借用一个辅助数组。
    这题可能被坑的地方就在于，需要考虑到循环移位次数超过序列长度的情况。 

    空间复杂度O(n)级别（需辅助数组），时间复杂度O(n)级别 

        - SomeBottle 2023.7.30
*/