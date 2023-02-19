#include <cstdio>

using namespace std;

// 矩阵和前缀二维数组，为了方便处理（以免越界），从下标1开始储存
long long matrix[502][502], prefix[502][502]{0};

long long sumMatrix(int up, int down, int left, int right); // 计算矩阵和

int main()
{
    int N, M;
    long long K;
    scanf("%d %d %lld", &N, &M, &K);
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            scanf("%lld", &matrix[i][j]);
            // 顺带把前缀和算了
            // 从(1,1)到(i,j)的矩阵元素和 = 左边一项的值 + 上面一项的值 - 左上角一项的值 + 矩阵中(i,j)的值
            // 减去左上角是因为左上角在【左边一项】和【上面一项】中重复计算了。
            prefix[i][j] = prefix[i][j - 1] + prefix[i - 1][j] - prefix[i - 1][j - 1] + matrix[i][j];
        }
    }
    long long count = 0; // 计数
    // 分别代表 上边界j坐标、下边界j坐标、左边界i坐标，右边界i坐标
    int up, down, left, right;
    for (up = 1; up <= N; up++) // 上边界下移
    {
        for (down = up; down <= N; down++) // 下边界下移，从1x..到2x.., 3x..
        {
            // 此时上下边界up, down已经确定，专心处理左右边界即可。
            left = 1; // 左边界最开始在最左边
            right = 1;
            while (right <= M) // 右指针右移到最右边为止
            {
                // 子矩阵左上角(up,left), 右下角(down,right)
                while (left <= right && sumMatrix(up, down, left, right) > K) // 左、右指针围起来的矩阵和>K，左指针右移以缩小矩阵
                {
                    left++;
                }
                // 此时有几种可行的矩阵，宽度从1到(right-left+1)
                count += (right - left + 1);
                right++;
            }
        }
    }
    printf("%lld", count);
    return 0;
}

// 计算左上角(up,left), 右下角(down,right)的子矩阵和
long long sumMatrix(int up, int down, int left, int right)
{
    // 利用前缀求子矩阵的和
    return prefix[down][right] - prefix[up - 1][right] - prefix[down][left - 1] + prefix[up - 1][left - 1];
}

/*
    之前的一个80分写法中，右指针right会回退，时间开销更大。
    而这种写法中不会有指针回退，能完全AC。

    在一个大矩阵中，我可以用四条边界确定一块子矩阵

                          left       right
                            │          │
                    ┌───────┼──────────┼───────┐
                    │       │          │       │
                    │       │          │       │
                ────┼───────╆━━━━━━━━━━╅───────┼─── up
                    │       ┃          ┃       │
                    │       ┃          ┃       │
                ────┼───────╄━━━━━━━━━━╃───────┼─── down
                    └───────┼──────────┼───────┘
                            │          │

    具体的解题思路是:
        1. 通过【分别】移动子矩阵上边界up和下边界down，枚举【所有】子矩阵的【高度】(down-up+1)。

        2. 每枚举一对up和down，就确定了一排子矩阵(从宽度为1到宽度为M)的高度，接下来用left和right指针寻找这些子矩阵中【哪些符合要求】:

            - 最开始时left=1, right=1

            - 一直向右移动right指针，子矩阵宽度是(right-left+1)

                - 当(down-up+1) x (right-left+1)的子矩阵和>K时，说明当前矩阵以及【比当前更大的矩阵】都不会满足要求了，

                此时，【left指针右移】直到(down-up+1) x (right-left+1)的子矩阵和 <= K为止。

                    * 此时从宽度为1到宽度为(right-left+1)的子矩阵【全都是符合要求的矩阵】
                    * 所以代码中写的是count += (right - left + 1)

            - right指针到最右边为止
                * 此时就枚举出了【当前高度(down-up+1)】下所有符合要求的子矩阵数量。

    这题的AC思路还是挺巧妙的，值得回顾。
    --------------------------
    这里我是先确定高度(up/down)再移动left/right寻找同高度下满足要求的子矩阵。其实先确定宽度(left/right)再移动up/down寻找同宽度下满足要求的子矩阵也是可行的。

        - SomeBottle 2023.2.19

*/

/*
# [蓝桥杯 2022 省 B] 统计子矩阵

## 题目描述

给定一个 $N \times M$ 的矩阵 $A$，请你统计有多少个子矩阵 (最小 $1 \times 1$, 最大 $N \times M)$ 满足子矩阵中所有数的和不超过给定的整数 $K$。

## 输入格式

第一行包含三个整数 $N, M$ 和 $K$。

之后 $N$ 行每行包含 $M$ 个整数, 代表矩阵 $A$。

## 输出格式

一个整数代表答案。

## 样例 #1

### 样例输入 #1

```
3 4 10
1 2 3 4
5 6 7 8
9 10 11 12
```

### 样例输出 #1

```
19
```

## 提示

**【样例说明】**

满足条件的子矩阵一共有 $19$，包含:

大小为 $1 \times 1$ 的有 $10$ 个。

大小为 $1 \times 2$ 的有 $3$ 个。 大小为 $1 \times 3$ 的有 $2$ 个。

大小为 $1 \times 4$ 的有 $1$ 个。

大小为 $2 \times 1$ 的有 $3$ 个。

**【评测用例规模与约定】**

对于 $30 \%$ 的数据, $N, M \leq 20$.

对于 $70 \%$ 的数据, $N, M \leq 100$.

对于 $100 \%$ 的数据, $1 \leq N, M \leq 500,0 \leq A_{i j} \leq 1000,1 \leq K \leq 2.5\times10^8$.

蓝桥杯 2022 省赛 B 组 F 题。
*/