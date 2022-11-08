/*
试题 基础练习 数列特征
资源限制
    内存限制：256.0MB   C/C++时间限制：1.0s   Java时间限制：3.0s   Python时间限制：5.0s
问题描述
    给出n个数，找出这n个数的最大值，最小值，和。
输入格式
    第一行为整数n，表示数的个数。
    第二行有n个数，为给定的n个数，每个数的绝对值都小于10000。
输出格式
    输出三行，每行一个整数。第一行表示这些数中的最大值，第二行表示这些数中的最小值，第三行表示这些数的和。
样例输入
    5
    1 3 -2 4 5
样例输出
    5
    -2
    11
数据规模与约定
    1 <= n <= 10000。
*/
#include <iostream>

using namespace std;

int main()
{
    int amount;
    cin >> amount;
    int sum = 0;
    int max = 0;
    int min = 0;
    // 完全没有必要用到数组，一遍循环解决
    for (int i = 0; i < amount; i++)
    {
        int current;
        cin >> current;
        if (i == 0)
        { // i=0时初始化max/min
            max = current;
            min = current;
        }
        else
        {
            // 更新最大值和最小值
            if (current > max)
                max = current;
            else if (current < min)
                min = current;
        }
        sum += current;
    }
    cout << max << endl;
    cout << min << endl;
    cout << sum << endl;
    return 0;
}