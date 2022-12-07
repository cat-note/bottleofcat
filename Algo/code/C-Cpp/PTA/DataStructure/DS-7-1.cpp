/*

*/
#include <cstdio>
#include <vector>

using namespace std;

int main()
{
    int seqLen; // 序列长度
    scanf(" %d", &seqLen);
    vector<int> sequence(seqLen);  // 初始化序列
    vector<int> subSeqSum(seqLen); // 初始化子序列求和序列
    int maxSum = 0;                // 最大和
    for (int i = 0; i < seqLen; i++)
    { // 读入序列
        int num;
        scanf(" %d", &num);
        sequence[i] = num;
        subSeqSum[i] = num;
        if (num > maxSum) // 先找到长为1的子序列的最大和
            maxSum = num;
    }
    // maxSum=0说明输入的数值没有一个是大于0的，就无需进行比较
    if (maxSum)
    {
        int subSeqAdd = 1; // 子序列长度从2开始, 在1的基础上+1
        while (subSeqAdd < seqLen)
        {
            // 比如子序列长度为2，subSeqAdd=1, 因此要从0遍历到seqLen-subSeqAdd-1
            for (int i = 0, end = seqLen - subSeqAdd; i < end; i++)
            {
                // subSeqSum储存每个连续子序列的求和
                // 比如i=0, 子序列长度为2, subSeqSum[0]储存的是序列中0号和1号下标的求和
                // 而i=0, 子序列长度为3时，subSeqSum[0]储存的是序列中0、1、2下标的求和(可以发现subSeqSum[0]+2就能得到)
                subSeqSum[i] += sequence[i + subSeqAdd];
                // 查看这个子序列的求和是否大于最大和
                if (subSeqSum[i] > maxSum)
                    maxSum = subSeqSum[i];
            }
            subSeqAdd++;
        }
    }
    printf("%d", maxSum);
    return 0;
}