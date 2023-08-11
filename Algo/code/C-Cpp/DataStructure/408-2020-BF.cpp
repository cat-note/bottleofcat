#include <cstdio>

using namespace std;

int main()
{
    // input:
    int S1[] = {-1, 0, 9};
    int S2[] = {-25, -10, 10, 11};
    int S3[] = {2, 9, 17, 30, 41};
    int S1Len = sizeof(S1) / sizeof(S1[0]);
    int S2Len = sizeof(S2) / sizeof(S2[0]);
    int S3Len = sizeof(S3) / sizeof(S3[0]);
    // algorithm:
    int minDist = -1;
    int minTuple[3];
    int tempDist = -1;
    int a, b, c;
    for (int i = 0; i < S1Len; i++)
        for (int j = 0; j < S2Len; j++)
            for (int k = 0; k < S3Len; k++)
            {
                a = S1[i];
                b = S2[j];
                c = S3[k];
                // 三目运算取绝对值相加
                tempDist = (a > b ? a - b : b - a) + (b > c ? b - c : c - b) + (c > a ? c - a : a - c);
                // 算出最小值则更新
                if (minDist == -1 || tempDist < minDist)
                {
                    minDist = tempDist;
                    minTuple[0] = a;
                    minTuple[1] = b;
                    minTuple[2] = c;
                }
            }
    // output:
    printf("minDist: %d, tuple: (%d, %d, %d)\n", minDist, minTuple[0], minTuple[1], minTuple[2]);
    return 0;
}

/*
    这题要短时间想出优解并写正确，是很难的。
    受不了了！直接一拳干上三层循环!
    暴力枚举所有组合来计算最小值。

    时间复杂度：O(N^3)级别，实际上是O(S1Len*S2Len*S3Len)
    空间复杂度：O(1)

        - SomeBottle 2023.8.11
*/