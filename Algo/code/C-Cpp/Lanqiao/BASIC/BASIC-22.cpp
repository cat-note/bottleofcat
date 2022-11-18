/*
蓝桥杯基础练习VIP-FJ的字符串
时间限制: 1s 内存限制: 128MB
题目描述
    FJ在沙盘上写了这样一些字符串：
    A1  =  “A”
    A2  =  “ABA”
    A3  =  “ABACABA”
    A4  =  “ABACABADABACABA”
    …  …
    你能找出其中的规律并写所有的数列AN吗？
输入格式
    仅有一个数：N ≤ 26。
输出格式
    请输出相应的字符串AN，以一个换行符结束。输出中不得含有多余的空格或换行、回车符。
样例输入
    3
样例输出
    ABACABA
*/
#include <iostream>

using namespace std;
/*
    通过观察发现，第n个字符串形如：【第n-1个字符串】【第n个英文字母】【第n-1个字符串】
    发现规律后，这道题其实就是简单的字符串循环拼接了。
        - SomeBottle 2022.11.18
*/
int main()
{
    string result = "A";
    short n; // 要推算第几个字符串
    cin >> n;
    for (short i = 1; i < n; i++) // 从第二个开始推(i从1开始)
    {
        string temp = result;
        temp.append(1, 'A' + i); // 将字符加入到字符串尾部
        result = temp + result;  // 再在尾部拼接一个result
    }
    cout << result << "\n"; // 题目要求以换行符结束
    return 0;
}