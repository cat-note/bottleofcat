#include <cstdio>

using namespace std;

int main()
{
    int maxSum = 0; // 最大子列和
    int seqLen;     // 子列长度
    scanf(" %d", &seqLen);
    int currSeqSum = 0; // 当前的累加和
    for (int i = 0; i < seqLen; i++)
    {
        int num;
        scanf(" %d", &num);
        currSeqSum += num;       // 累加求和(因为连续子序列是序列中连续的一段)
        if (currSeqSum > maxSum) // 有新的最大和出现就更新
            maxSum = currSeqSum;
        else if (currSeqSum < 0) // 如果累加结果成了负数，不能让负数影响到后面的求和，回到0再继续
            currSeqSum = 0;
    }
    printf("%d", maxSum);
    return 0;
}

/*
    这种解法应该是最优解了，时间复杂度为O(n)，甚至不用数组辅助。

    这题主要关注点在于【求出连续子序列的最大和】，得益于是【连续】的子序列，只要连续累加能得到最大值，就找到了子序列的最大和。

    问题就在于序列中会出现负数，可以发现，当【目前的 累加和 变成负数】时必会影响后面的求和。因此得先把累加和【归0】，再继续累加。

        - SomeBottle 2022.12.7
*/
