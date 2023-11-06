#include <cstdio>

using namespace std;

void Reverse(int *seq, int start, int end);

int main()
{
    int seq[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int seqLen = sizeof(seq) / sizeof(seq[0]);
    int p = 12; // 循环左移位数
    // 以下是优解
    p = p % seqLen;              // （很重要！）可能会左移超过一个序列的长度，先模一下，得到实际要移动的位数。
    Reverse(seq, 0, p - 1);      // 逆置[0...p-1]子序列
    Reverse(seq, p, seqLen - 1); // 逆置[p...n-1]子序列
    Reverse(seq, 0, seqLen - 1); // 最后逆置整个序列
    printf("Result: ");
    for (int i = 0; i < seqLen; i++)
        printf("%d ", seq[i]);
    printf("\n");
    return 0;
}

// 逆置子序列
void Reverse(int *seq, int start, int end)
{
    int temp;
    while (start < end)
    {
        temp = seq[start];
        seq[start++] = seq[end];
        seq[end--] = temp;
    }
}

/*
    这一题主要是一个线性表循环移位问题，把线性表循环左移p位。

    这题可能被坑的地方就在于，需要考虑到循环移位次数超过序列长度的情况，需要把移动位数p模一下序列长度。

    优解的方法非常巧妙，观察发现循环左移p位，其实通过在原地对子序列进行逆置（倒转）就能实现：

        1. 逆置下标为[0...p-1]的子序列（设为序列A）
        2. 逆置下标为[p...n-1]的子序列（设为序列B）
        3. 逆置下标为[0...n-1]的序列

        可以用分块矩阵来理解：
         AB -> 分别逆置 (A^-1 B^-1) -> 整体逆置(A^-1 B^-1)^-1 = BA，正好就是循环移位的结果。


    空间复杂度O(1)级别（原地操作），时间复杂度O(n)级别（Reverse算法为线性时间复杂度）

        - SomeBottle 2023.11.6
*/