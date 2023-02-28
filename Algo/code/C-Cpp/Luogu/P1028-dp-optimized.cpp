#include <cstdio>
#include <cstring>

using namespace std;

int main()
{
    int N;
    scanf("%d", &N);
    int dp[N + 1]; // 动态规划数组
    memset(dp, 0, sizeof(dp));
    dp[1] = 1;                   // 初始化
    for (int i = 2; i <= N; i++) // 从i=2推到i=N
    {
        if (i % 2 == 0)
            dp[i] = dp[i - 1] + dp[i / 2];
        else
            dp[i] = dp[i - 1];
    }
    printf("%d", dp[N]);
    return 0;
}

/*
    再进一步，可以发现并不需要从dp[1]一直加到dp[i/2]:

    * n=1 -> dp[1] = 1
    * n=2 -> dp[2] = dp[1] + dp[1] = 2
    * n=3 -> dp[3] = dp[2] = 2
    * n=4 -> dp[4] = dp[3] + dp[2]= 4
    * n=5 -> dp[5] = dp[4] = 4
    * n=6 -> dp[6] = dp[5] + dp[3] = 6

    由此可以得到更优化的递推公式:

    $$dp\left[i\right]=\begin{cases} dp\left [  i-1 \right ] & ( i \mod{2} = 1)  \\ dp\left [  i-1 \right ] + dp\left [  \frac {i}{2}   \right ] & ( i \mod{2} = 0) \end{cases}$$


        - SomeBottle 2023.2.28
*/