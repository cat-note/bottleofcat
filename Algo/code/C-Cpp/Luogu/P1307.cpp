#include <cstdio>

using namespace std;

int main()
{
    char num[12];
    scanf("%s", num);
    int strLen = 0;
    while (num[strLen] != '\0') // 计算字符串长度
        strLen++;
    int begin = 0; // 指向字符串中首个有效数字位
    int end = strLen - 1;
    bool sign = false; // 是否有符号
    if (num[0] == '-') // 负号
    {
        sign = true;
        begin++;
    }
    if (!(begin == end && num[begin] == '0')) // 原数不为0才进行运算
    {
        int start = begin;
        while (start < end)
        {
            char temp = num[end];
            num[end] = num[start]; // 双指针交换【对称位置上的字符】
            num[start] = temp;
            start++;
            end--;
        }
        // 去除头部的0
        for (int i = begin; i < strLen; i++)
        {
            if (num[i] == '0') // 遇到0就把begin指针后移
                begin++;
            else
                break;
        }
    }
    // 有负号就在前面输出
    printf(sign ? "-%s" : "%s", num + begin);
    return 0;
}

/*
    本题将数字作为字符串处理较为方便。

    反转字符串只需要同时用两个指针，一个从前往后扫描，另一个从后往前扫描，交换二者指到的字符即可。

    注意本题要求输出的是合理的整数，高位不能有0。
    为了防止写错，这里我把字符串反转后再【去除开头多余的0】

        * 注意负号要特别处理，这里我还考虑到了【-0】的情况，休想给我挖坑！

        - SomeBottle 2023.2.16
*/

/*
# [NOIP2011 普及组] 数字反转

## 题目描述

给定一个整数 $N$，请将该数各个位上数字反转得到一个新数。新数也应满足整数的常见形式，即除非给定的原数为零，否则反转后得到的新数的最高位数字不应为零（参见样例 2）。

## 输入格式

一个整数 $N$。

## 输出格式

一个整数，表示反转后的新数。

## 样例 #1

### 样例输入 #1

```
123
```

### 样例输出 #1

```
321
```

## 样例 #2

### 样例输入 #2

```
-380
```

### 样例输出 #2

```
-83
```

## 提示

**【数据范围】**

$-1,000,000,000\leq N\leq 1,000,000,000 $。

noip2011 普及组第一题
*/