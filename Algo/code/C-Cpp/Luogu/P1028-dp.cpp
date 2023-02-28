#include <cstdio>
#include <cstring>

using namespace std;

int main()
{
    int N;
    scanf("%d", &N);
    int dp[N + 1];               // 动态规划数组
    memset(dp,0,sizeof(dp));
    dp[1]=1; // 初始化
    for (int i = 2; i <= N; i++) // 从i=2推到i=N
    {
        int halfN = i / 2;
        int count = 0;
        for (int j = 1; j <= halfN; j++) // 求dp[1]+...+dp[i/2]
        {
            count += dp[j];
        }
        dp[i] = count + 1;
    }
    printf("%d", dp[N]);
    return 0;
}

/*
    观察样例给出的数列:
    6
    6,1
    6,2
    6,3
    6,2,1
    6,3,1
    可以发现递推规律。

    设n=i的合法数列个数有dp[i]个：

    * 很容易能得到dp[1]=1

    * 再手动枚举一下n=2的合法数列个数:

        2
        2 1

        可以发现，其实合法数列个数就是 dp[1]+1 = 2

    * 接着再看一下n=3的:

        3
        3 1

        一样是 dp[1]+1 = 2

    * 那么n=4呢？

        4
        4 1
        4 2
        4 2 1

        可以发现 dp[4]=dp[1] + dp[2] + 1 = 4

    * 最后观察一下样例的n=6的情况能得到:

        dp[6] = dp[1] + dp[2] + dp[3] + 1 = 6

    * 很快咱们便能归纳出动态规划的递推式（状态转移方程）:

        $dp\left[ i \right] = 1 + \sum_{k=1}^{\lfloor \frac {i}{2} \rfloor} dp\left[k\right]$

        写的通俗一点就是:

        dp[i] = (dp[1]+dp[2]+...+dp[i/2]) + 1

        式子中的+1其实就是数列中只包含 n 一个数的情况。

    初始化时将dp[1]置为1，而递推顺序自然就是从dp[1]开始向后推了。（毕竟要先知道dp[1]才能推dp[2]和dp[3]）

        - SomeBottle 2023.2.28
*/