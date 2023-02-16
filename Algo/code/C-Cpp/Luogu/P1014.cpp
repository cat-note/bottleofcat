#include <iostream>

using namespace std;

int main()
{
    int N;
    cin >> N;
    int upper = 1;         // 当前的上界，达到上界后
    int nume = 1;          // 分子
    int deno = 1;          // 分母
    bool alterDeno = true; // 标记在转角处是否变更分母，在过程中是否执行分母+1
    for (int i = 0; i < N - 1; i++)
    {
        // 碰到上界
        if (deno >= upper && alterDeno) // 比如2/1到3/1的转变
        {
            deno++;
            upper++; // 上界增高
            alterDeno = !alterDeno;
        }
        else if (nume >= upper && !alterDeno) // 比如1/3到1/4的转变
        {
            nume++;
            upper++; // 上界增高
            alterDeno = !alterDeno;
        }
        else // 没碰到上界
        {
            if (!alterDeno)
            { // alterDeno=false, 过程中分母值减少
                deno--;
                nume++;
            }
            else
            { // alterDeno=true, 过程中分母值增加
                deno++;
                nume--;
            }
        }
    }
    cout << nume << "/" << deno;
    return 0;
}

/*
    本题是一个模拟题。观察题中的枚举规律可以发现一个简单粗暴的思路：

        分子/分母的数值变化方向始终是相反的，而【在转角处】，需要单独对分子/分母进行操作。

        很明显，我需要一个【标志变量】来指定【转角处的操作】和【到底是分母增、分子减，还是分母减、分子增】

    详见上面代码的注释。

    这题还有更优的解法，我这里就按模拟的解法写了。

        - SomeBottle 2023.2.16
*/

/*
# [NOIP1999 普及组] Cantor 表

## 题目描述

现代数学的著名证明之一是 Georg Cantor 证明了有理数是可枚举的。他是用下面这一张表来证明这一命题的：

$1/1$ ,   $1/2$ ,   $1/3$ ,   $1/4$,    $1/5$,   …

$2/1$,   $2/2$ ,   $2/3$,    $2/4$,    …

$3/1$ ,   $3/2$,    $3/3$,    …

$4/1$,    $4/2$,    …

$5/1$,   …

…

我们以 Z 字形给上表的每一项编号。第一项是 $1/1$，然后是 $1/2$，$2/1$，$3/1$，$2/2$，…

## 输入格式

整数$N$（$1 \leq N \leq 10^7$）。

## 输出格式

表中的第 $N$ 项。

## 样例 #1

### 样例输入 #1

```
7
```

### 样例输出 #1

```
1/4
```
*/