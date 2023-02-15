#include <cstdio>
#include <string>

using namespace std;

// 转换为字符串
string toString(int num)
{
    string temp;
    temp.resize(8);
    sprintf(&temp[0], "%d", num);
    return temp;
}

// 检查字符串str中chr出现的次数
int countChar(string &str, char chr)
{
    int cnt = 0;
    for (string::iterator it = str.begin(); it != str.end(); it++)
    {
        if (*it == chr)
            cnt++;
    }
    return cnt;
}

int main()
{
    int n, x;
    scanf("%d %d", &n, &x);
    // 生成字符串并检查是否有x出现
    int cnt = 0; // 计数
    for (int i = 1; i <= n; i++)
    {
        string str = toString(i);
        cnt += countChar(str, '0' + x);
    }
    printf("%d", cnt);
    return 0;
}

/*
    这题需要统计一串数字序列中【某个0-9的数字】出现了多少次，很明显需要用到【字符串】来辅助处理。

    转换字符串我能想到两种方式:
        1. C语言标准输入输出库的sprintf
        2. 对数字进行整除与取余，逐位转换为字符串(这个方法较快)

        - SomeBottle 2023.2.15
*/

/*
# [NOIP2013 普及组] 计数问题

## 题目描述

试计算在区间 $1$ 到 $n$ 的所有整数中，数字 $x$（$0\le x\le9$）共出现了多少次？例如，在 $1$ 到 $11$ 中，即在 $1,2,3,4,5,6,7,8,9,10,11$ 中，数字 $1$ 出现了 $4$ 次。

## 输入格式

$2$ 个整数 $n,x$，之间用一个空格隔开。

## 输出格式

$1$ 个整数，表示 $x$ 出现的次数。

## 样例 #1

### 样例输入 #1

```
11 1
```

### 样例输出 #1

```
4
```

## 提示

对于 $100\%$ 的数据，$1\le n\le 10^6$，$0\le x \le 9$。
*/