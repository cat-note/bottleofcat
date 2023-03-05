#include <cstdio>
#include <cstring>
#define LL long long
#define DIVISOR 1000000007

using namespace std;

int main()
{
    int N;
    scanf("%d", &N);
    LL dp[N + 3]; // 因为初始化时给dp[1..3]赋值了，因此这里分配时至少要有4个元素(N>=1)
    memset(dp, 0, sizeof(dp));
    dp[1] = 1;
    dp[2] = 2;
    dp[3] = 5;
    for (int i = 4; i <= N; i++)
        dp[i] = ((dp[i - 1] * 2) % DIVISOR + dp[i - 3]) % DIVISOR;
    printf("%lld", dp[N]);
    return 0;
}

/*
    这题画布宽度固定为2，且结果只需要依赖于输入值N进行推算，初步推断这题可以用线性DP解。

    打表得出的结果:
    dp[1]=1
    dp[2]=2
    dp[3]=5
    dp[4]=11
    dp[5]=24

    然后就是加来减去好久，终于发现了个可行的规律: dp[i]=dp[i-1]*2 + dp[i-3]

    打表果然还是爱着我的 😭

        - SomeBottle 2023.3.5
*/