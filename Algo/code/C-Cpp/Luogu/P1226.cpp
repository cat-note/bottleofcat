#include <cstdio>
#define LL long long

using namespace std;

LL powerMod(LL a, LL b, LL p);

int main()
{
    LL a, b, p;
    scanf("%lld %lld %lld", &a, &b, &p);
    printf("%lld^%lld mod %lld=%lld", a, b, p, powerMod(a, b, p));
    return 0;
}

LL powerMod(LL a, LL b, LL p)
{
    LL result = 1;
    LL an = a; // 初始值: a^1=a^{2^0}
    while (b != 0)
    {
        if (b % 2) // 如果当前二进制位为1
        {
            result = (result * an) % p; // 结果乘上 a^{2^m}，然后模上p
        }
        an = (an * an) % p; // 递推a^{2^m}=a^{2^{m-1}} \times a^{2^{m-1}}，并模上p，防止溢出
        b /= 2;             // 进制转换的过程
    }
    return result;
}

/*
## 想法
这题主要考察**快速幂**的写法，关键是要算出 $a^n$ 。我认为快速幂算法中有以下两个要素：

1. **进制转换**（二进制）
2. **递推**

----------

### 进制转换

首先通过进制转换将指数 $n$ 转换为**二进制数**，也就是用几位二进制来表示一个十进制数 $n$。接着通过二进制数位来**拆解**指数 $n$。

* 比如算 $3^9$ ( $a=3$, $n=9$ )

    那么就可以把 $n=9$ 转换为成二进制数 $1001$ , 可以发现二进制数 $1001$ 告诉了我这个指数 $n$ **可以拆解成**十进制数 $8 + 1$ 。

    因此我可以把 $3^9$ 拆解成 $3^{(8+1)}$ ，通过幂运算公式进一步展开就成为了： $3^8 \times 3^1$ 。

    列成表格可以更清晰地看到 $n$ 的二进制位和十进制数的关系：

    | 二进制位 | 1 | 0 | 0 | 1 |
    | :---: | :---: | :---: | :---: | :---: |
    | 位权 $2^m$ | $2^3$ | $2^2$ | $2^1$ | $2^0$ |
    | $3^{n}=3^{2^m}$ | $3^8=3^{2^3}$ | $3^4=3^{2^2}$ | $3^2=3^{2^1}$ | $3^1=3^{2^0}$ |

    因此运算过程可以看成:

    $ 3^9=3^8 \times 1 + 3^4 \times 0 + 3^2 \times 0 + 3^1 \times 1=3^8 \times 3^1$

### 递推

观察上面的计算过程可以发现：

$$3^2=(3^1)^2=3^1 \times 3^1$$

$$ 3^4=(3^2)^2=3^2 \times 3^2$$

$$ 3^{2^m}=(3^{2^{m-1}})^2=3^{2^{m-1}} \times 3^{2^{m-1}}\quad(m\gt1)$$

$$ a^n=a^{2^m}=(a^{2^{m-1}})^2=a^{2^{m-1}} \times a^{2^{m-1}}\quad(m\gt1)$$

有这样的递推关系，也就是说我可以边将 $n$ 转换为二进制，边**递推**计算乘幂 $a^{2^m}$。

当当前迭代中算出来的二进制位为 $1$ 时，就把结果**乘上**当前的 $3^{2^m}$ 值（像上面这个例子就是结果 $=3^9=3^8 \times 3^1$）

    - SomeBottle 2023.3.11
*/

/*
# 【模板】快速幂||取余运算

## 题目描述

给你三个整数 $a,b,p$，求 $a^b \bmod p$。

## 输入格式

输入只有一行三个整数，分别代表 $a,b,p$。

## 输出格式

输出一行一个字符串 `a^b mod p=s`，其中 $a,b,p$ 分别为题目给定的值， $s$ 为运算结果。

## 样例 #1

### 样例输入 #1

```
2 10 9
```

### 样例输出 #1

```
2^10 mod 9=7
```

## 提示

**样例解释**

$2^{10} = 1024$，$1024 \bmod 9 = 7$。

**数据规模与约定**

对于 $100\%$ 的数据，保证 $0\le a,b < 2^{31}$，$a+b>0$，$2 \leq p \lt 2^{31}$。
*/