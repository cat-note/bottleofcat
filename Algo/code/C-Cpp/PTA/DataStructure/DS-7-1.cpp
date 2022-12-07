/*
7-1 最大子列和问题
题目描述见图片存档:
    https://raw.githubusercontent.com/SomeBottle/bottleofcat/main/Algo/code/C-Cpp/PTA/Pics/question-7-1.png
输入格式:
    输入第1行给出正整数K (≤100000)；第2行给出K个整数，其间以空格分隔。
输出格式:
    在一行中输出最大子列和。如果序列中所有整数皆为负数，则输出0。
输入样例:
    6
    -2 11 -4 13 -5 -2
输出样例:
    20
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

/*
    接下来是我的思路:

        一个序列存放着seqLen个数，因此其中的连续子序列最少只有1个数值，最多有seqLen个数值。

    如果这个序列存放的整数的数量>=2，要找到【最大】的连续子序列和，需要从【长为2的连续子序列】一直扫描到【长为seqLen的连续子序列】。
        * 长度为1的子序列在读取输入的时候就已经处理了。

    比如对于序列：
        1 -4 5 3 -7 8 3 -4
    最开始寻找长为2的连续子序列和，需要计算：
        1+(-4)  (-4)+5  5+3  ...  3+(-4)  (也就是【每两项】一加)
    接下来找长为3的连续子序列和，需要计算：
        1+(-4)+5   (-4)+5+3   5+3+(-7)   ...   8+3+(-4)   (也就是【每三项】一加)
    怎么样，有没有找到“华点”？没错，每次计算子序列和的时候可以【借助上一次运算的结果】

    在代码中可以看到，我增加了一个subSeqSum数组来【缓存前一次的运算结果】，这样一来：
        求所有长为2的连续子列和只需要运算: seqLen-1 次
        求所有长为3的连续子列和只需要运算: seqLen-2 次
        求所有长为4的连续子列和只需要运算: seqLen-3 次
        ...

    相比不缓存运算结果，能省下一大笔时间开销。（代价是耗费稍微多一点的空间）

    每次求和运算的时候，如果求出来的和大于当前记录的最大值maxSum就更新。

        - SomeBottle 2022.12.7

*/