#include <cstdio>
#include <cstring>
#define DIVISOR 100003

using namespace std;

int main()
{
    int N, K;
    scanf("%d %d", &N, &K);
    int steps[N + 1]; // steps[i]代表跳到第i级台阶有多少种方式
    memset(steps, 0, sizeof(steps));
    steps[0] = 1;                // 从最底部跳到第1-K级台阶至少有1种方式
    steps[1] = steps[0];         // 跳到第1级台阶只有1种方式
    for (int i = 2; i <= N; i++) // 递推（从第二级台阶开始）
    {
        steps[i] = 2 * steps[i - 1];
        if (i > K) // 请注意，这里的steps[i - 1 - K]可能会比steps[i]大，因此先加上除数再相减，以免变成负数
            steps[i] = (steps[i] + DIVISOR - steps[i - 1 - K]) % DIVISOR;
        steps[i] %= DIVISOR;
    }
    printf("%d", steps[N]);
    return 0;
}

/*
    实际上这题的时间复杂度可以优化到O(n)级别。

    k为3的情况下:
    dp[1]=dp[0]
    dp[2]=dp[1]+dp[0]=2dp[1]
    dp[3]=dp[2]+dp[1]+dp[0]=2dp[2]
    dp[4]=dp[3]+dp[2]+dp[1]=2dp[3]-dp[0]
    ...
    dp[8]=dp[7]+dp[6]+dp[5]
    dp[9]=dp[8]+dp[7]+dp[6]=2dp[8]-dp[5]
    ...
    归纳：
    dp[i]=2dp[i-1]  (1<i<=K)
    dp[i]=2dp[i-1]-dp[i-1-k] (i>K)

    得出了这样的递推式，这题显而易见就是道线性动态规划了~

    值得注意的是，这里的2*steps[i-1]可能比steps[i - 1 - K]小，为了防止模之前出现了负数，得先把【被减数2*steps[i-1]加上除数】，接着减去steps[i - 1 - K]，然后再模上除数。

        - SomeBottle 2023.3.3
*/

/*
# 台阶问题

## 题目描述

有 $N$ 级台阶，你一开始在底部，每次可以向上迈 $1\sim K$ 级台阶，问到达第 $N$ 级台阶有多少种不同方式。

## 输入格式

两个正整数 $N,K$。

## 输出格式

一个正整数 $ans\pmod{100003}$，为到达第 $N$ 级台阶的不同方式数。

## 样例 #1

### 样例输入 #1

```
5 2
```

### 样例输出 #1

```
8
```

## 提示

- 对于 $20\%$ 的数据，$1\leq N\leq10$，$1\leq K\leq3$；
- 对于 $40\%$ 的数据，$1\leq N\leq1000$；
- 对于 $100\%$ 的数据，$1\leq N\leq100000$，$1\leq K\leq100$。
*/