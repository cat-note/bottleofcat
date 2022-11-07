/*
试题 基础练习 特殊的数字
资源限制
    内存限制：512.0MB   C/C++时间限制：1.0s   Java时间限制：3.0s   Python时间限制：5.0s
问题描述
　　153是一个非常特殊的数，它等于它的每位数字的立方和，即153=1*1*1+5*5*5+3*3*3。编程求所有满足这种条件的三位十进制数。
输出格式
　　按从小到大的顺序输出满足条件的三位十进制数，每个数占一行。
*/
#include <iostream>

using namespace std;

int pow3(int n)
{
    return n * n * n;
}

int main()
{
    for (int hundreds = 1; hundreds < 10; hundreds++)
        for (int tens = 0; tens < 10; tens++)
            for (int units = 0; units < 10; units++)
            {
                if (pow3(hundreds) + pow3(tens) + pow3(units) == hundreds * 100 + tens * 10 + units)
                    cout << hundreds << tens << units << endl;
            }
    return 0;
}