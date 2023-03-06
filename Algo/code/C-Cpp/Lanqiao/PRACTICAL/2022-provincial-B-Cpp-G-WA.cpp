#include <cstdio>
#include <cstring>
#define LL long long
#define DIVISOR 1000000007

using namespace std;

int main()
{
    int N;
    scanf("%d", &N);
    LL fib[N + 1];   // 斐波那契数列
    LL dp_I[N + 1];  // 递推数组，针对只有I组成的图案
    LL dp_IL[N + 1]; // 递推数组，针对有I和L组成的图案
    memset(fib, 0, sizeof(fib));
    memset(dp_I, 0, sizeof(dp_I));
    memset(dp_IL, 0, sizeof(dp_IL));
    fib[1] = 1; // 初始化斐波那契数列
    fib[2] = 1;
    dp_I[1] = 1;                 // 2×1只有一种可能
    dp_I[2] = 2;                 // 2×2有两种可能
    dp_IL[2] = 0;                // 2×2时没法放L型
    for (int i = 3; i <= N; i++) // 从N=3开始推，正好从N=3开始能放L型
    {
        fib[i] = (fib[i - 1] + fib[i - 2]) % DIVISOR;
        dp_I[i] = (dp_I[i - 1] + fib[i - 1]) % DIVISOR;
        dp_IL[i] = (dp_IL[i - 1] * 2 % DIVISOR + 2) % DIVISOR;
        if (i >= 5)
            dp_IL[i] = (dp_IL[i] + (fib[i - 4] * 2) % DIVISOR) % DIVISOR;
    }
    printf("%lld", (dp_I[N] + dp_IL[N]) % DIVISOR);
    return 0;
}

/*
    这题画布宽度固定为2，且结果只需要依赖于输入值N进行推算，初步推断这题可以用线性DP解。


*/