#include <cstdio>

using namespace std;

inline int strLen(char *str); // 获得字符串长度

int main()
{
    char a[502], b[502]; // a和b两个数，先用字符串表示
    scanf("%s %s", a, b);
    int aPtr = strLen(a) - 1, bPtr = strLen(b) - 1; // 指向a和b的数值最低位
    int result[502];                                // 结果（从低位向高位储存）
    int resLen = 0;                                 // 结果长度
    int carry = 0;                                  // 进位数值
    for (; resLen < 502; resLen++)                  // 从最低位开始运算
    {
        int aDigit = 0, bDigit = 0;
        // 注意这里是当进位都没有的时候才算运算结束
        if (aPtr < 0 && bPtr < 0 && carry == 0) // 运算结束的条件
            break;
        if (aPtr >= 0)
            aDigit = (int)(a[aPtr--] - '0');
        if (bPtr >= 0)
            bDigit = (int)(b[bPtr--] - '0');
        int addition = aDigit + bDigit + carry; // 先把十进制位相加
        if (addition >= 10)                     // 产生进位
        {
            carry = addition / 10; // 高位作为进位传递给下一位
            addition %= 10;        // 保留低位
        }
        else
        {
            carry = 0; // 否则无进位
        }
        result[resLen] = addition; // 保存本位的运算结果
    }
    for (int i = resLen - 1; i >= 0; i--)
        printf("%d", result[i]);
    return 0;
}

int strLen(char *str)
{
    int cnt = 0;
    while (str[cnt] != '\0')
        cnt++;
    return cnt;
}

/*
## 想法

这题需要实现一个高精度的**无符号数十进制整数**加法，思路想起来不难：

* 每位按**字符**读入（因为C++中没有数据类型能表示如此大的数值）

* **从低位向高位**，将两数的**每个十进制位**进行**相加**

* **每次相加**的时候：

    1. 如果有**来自低位的进位**，需要把这个进位值也加上。

   2. 如果相加后得到的值 $\gt 9$，那么就**只保留个位**，**高位作为进位传递给下一位**。

--------

运算需要**满足以下三条**的时候才能终止：

1. 已经用掉了 $a$ 数的**所有位**。

2. 已经用掉了 $b$ 数的**所有位**。

3. **没有进位**了。

基于上面的运算过程和运算终止条件，这个代码就很快能码出来了。

    - SomeBottle 2023.3.13
*/

/*
# A+B Problem（高精）

## 题目描述

高精度加法，相当于 a+b problem，**不用考虑负数**。

## 输入格式

分两行输入。$a,b \leq 10^{500}$。

## 输出格式

输出只有一行，代表 $a+b$ 的值。

## 样例 #1

### 样例输入 #1

```
1
1
```

### 样例输出 #1

```
2
```

## 样例 #2

### 样例输入 #2

```
1001
9099
```

### 样例输出 #2

```
10100
```
*/