#include <iostream>
#include <string>

using namespace std;

int toDec(int base9);
int shunzi(); // 获得2022年的顺子日期

int main()
{
    int ans[] = {
        toDec(2022), // 双引号中替换为 A 题的答案
        shunzi(),    // 双引号中替换为 B 题的答案
    };
    char T;
    cin >> T;
    cout << ans[T - 'A'] << endl;
    return 0;
}

int toDec(int base9)
{
    int weight = 1;
    int result = 0;
    while (base9)
    {
        result += base9 % 10 * weight;
        base9 /= 10;
        weight *= 9;
    }
    return result;
}

int shunzi()
{
    // 2022显然不是闰年，2月只有28天
    // 这题有歧义，012算不算？321又算不算？不过210肯定不算
    // 洛谷这要AC的话是要考虑012, 123这种情况的。
    string dates[] = {
        "-", "01", "02", "03", "04", "05", "06", "07",
        "08", "09", "10", "11", "12", "13", "14", "15",
        "16", "17", "18", "19", "20", "21", "22", "23",
        "24", "25", "26", "27", "28", "29", "30", "31"};
    int days[] = {
        0, 31, 30, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int result = 0;
    for (int i = 1; i <= 12; i++)
    {
        string temp = "2022";
        temp += dates[i]; // 月份
        for (int j = 1; j <= days[i]; j++)
        {
            string temp2 = temp + dates[j]; // 日
            if (temp2.find("012") != string::npos || temp2.find("123") != string::npos)
            {
                result++;
            }
        }
    }
    return result;
}

/*
# [蓝桥杯 2022 省 B] 填空问题

## 题目描述

## 试题 A：九进制转十进制

### 【问题描述】

九进制正整数 $(2022)_{9}$ 转换成十进制等于多少?

### 【答案提交】

这是一道结果填空的题，你只需要算出结果后提交即可。本题的结果为一个整数，在提交答案时只填写这个整数，填写多余的内容将无法得分。

## 试题 B：顺子日期

### 【问题描述】

小明特别喜欢顺子。顺子指的就是连续的三个数字：`123`、`456` 等。顺子日期指的就是在日期的 `уyyymmdd` 表示法中，存在任意连续的三位数是一个顺子的日期。例如 `20220123` 就是一个顺子日期，因为它出现了一个顺子：`123`; 而 `20221023` 则不是一个顺子日期，它一个顺子也没有。小明想知道在整个 2022 年份中，一共有多少个顺子日期。

### 【答案提交】

这是一道结果填空的题，你只需要算出结果后提交即可。本题的结果为一个整数，在提交答案时只填写这个整数，填写多余的内容将无法得分。

## 输入格式

输入格式输入一个大写字母，表示第几个问题。

## 输出格式

根据所输入的问题编号，输出对应问题的答案。

## 提示

答题模板，可供参考。

```cpp
#include<iostream>
using namespace std;
int main() {
    string ans [] = {
        "The answer of task A", // 双引号中替换为 A 题的答案
        "The answer of task B", // 双引号中替换为 B 题的答案
    };
    char T;
    cin >> T;
    cout << ans[T - 'A'] << endl;
    return 0;
}
```
*/