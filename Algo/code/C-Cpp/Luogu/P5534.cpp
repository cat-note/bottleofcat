#include <cstdio>
#define LL long long

using namespace std;

int main()
{
    LL a1, a2, n;
    scanf("%lld%lld%lld", &a1, &a2, &n);
    LL d = a2 - a1; // 公差
    LL S = a1 + a2;
    LL prevA = a2;
    for (int i = 3; i <= n; i++)
        S += (prevA += d);
    printf("%lld", S);
    return 0;
}
/*
    这个解法就是递推了，相比公式直接求出结果来说要平均慢了1ms
        - SomeBottle 2023.3.3
*/

/*
# 【XR-3】等差数列

## 题目描述

小 X 给了你一个等差数列的前两项以及项数，请你求出这个等差数列各项之和。

等差数列：对于一个 $n$ 项数列 $a$，如果满足对于任意 $i \in [1,n)$，有 $a_{i+1} - a_i = d$，其中 $d$ 为定值，则称这个数列为一个等差数列。

## 输入格式

一行 $3$ 个整数 $a_1, a_2, n$，表示等差数列的第 $1,2$ 项以及项数。

**数据范围：**

- $|a_1|,|a_2| \le 10^6$。
- $3 \le n \le 10^6$。

## 输出格式

一行一个整数，表示答案。

## 样例 #1

### 样例输入 #1

```
1 2 3
```

### 样例输出 #1

```
6
```

## 样例 #2

### 样例输入 #2

```
-5 -10 5
```

### 样例输出 #2

```
-75
```

## 提示

【样例 $1$ 说明】

这个等差数列为 `1 2 3`，其各项之和为 $6$。
*/