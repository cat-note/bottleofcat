/*
蓝桥杯算法训练VIP-s01串
时间限制: 1s 内存限制: 128MB

题目描述
    s01串初始为" 0"
    按以下方式变换
    0变1，1变01
输入格式
    1个整数(0~19)
数据规模和约定
    0~19
输出格式
    n次变换后s01串
样例输入
    3
样例输出
    101
*/
#include <iostream>

using namespace std;

int main()
{
    string result = "0";
    int transTime;
    cin >> transTime;
    for (int i = 0; i < transTime; i++)
    {
        string temp = "";
        for (int ind = 0, len = result.length(); ind < len; ind++)
        {
            temp += result[ind] == '0' ? "1" : "01";
        }
        result = temp;
    }
    cout << result;
    return 0;
}

/*
    这题如果用C++写，就是简单的逻辑判断和字符string的拼接
    如果用C语言写需要使用strcpy这类内存复制函数。
        - SomeBottle 2022.12.3
*/