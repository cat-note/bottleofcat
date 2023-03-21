#include <cstdio>
#define LL long long

using namespace std;

LL N;
LL factorials[50]; // 递推阶乘

inline LL C(LL m, LL n); // 组合数计算

int main()
{
    scanf("%lld", &N);
    factorials[0] = 1;  // 0!=1
    LL row = 1;         // 从第二行（下标为1）开始
    bool found = false; // 是否找到
    if (N == 1)         // 如果N=1直接就输出（在这里处理第一行）
    {
        printf("1");
        found = true;
    }
    while (!found)
    {
        // 递推阶乘
        factorials[row] = factorials[row - 1] * row;
        LL midInd = row / 2;                       // 计算这一层中间的一个数的下标
        if (N >= C(1, row) && N <= C(midInd, row)) // 如果数N在这层的[左边第一个数,中间的一个数]区间内，就用二分查找进行搜索
        {
            // 二分查找
            LL start = 1;
            LL end = midInd;
            while (start <= end)
            {
                LL half = start + (end - start) / 2;
                LL halfNum = C(half, row);
                if (N > halfNum)
                {
                    start = half + 1;
                }
                else if (N < halfNum)
                {
                    end = half - 1;
                }
                else
                {
                    printf("%lld", (1 + row) * row / 2 + half + 1);
                    found = true;
                    break;
                }
            }
        }
        row++;
    }
    return 0;
}

LL C(LL m, LL n)
{
    LL res = factorials[n] / (factorials[m] * factorials[n - m]);
    if (res > N)
        res = N + 1;
    return res;
}