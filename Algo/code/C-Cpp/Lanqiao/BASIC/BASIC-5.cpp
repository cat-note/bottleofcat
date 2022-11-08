/*
资源限制
    内存限制：256.0MB   C/C++时间限制：1.0s   Java时间限制：3.0s   Python时间限制：5.0s
问题描述
    给出一个包含n个整数的数列，问整数a在数列中的第一次出现是第几个。
输入格式
    第一行包含一个整数n。
    第二行包含n个非负整数，为给定的数列，数列中的每个数都不大于10000。
    第三行包含一个整数a，为待查找的数。
输出格式
    如果a在数列中出现了，输出它第一次出现的位置(位置从1开始编号)，否则输出-1。
样例输入
    6
    1 9 4 8 3 9
    9
样例输出
    2
数据规模与约定
    1 <= n <= 1000。
*/
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int len;
    vector<int> arr;
    int numToFind;
    cin >> len;
    for (int i = 0; i < len; i++)
    {
        int curr;
        cin >> curr;
        arr.push_back(curr);
    }
    cin >> numToFind;
    for (int i = 0; i < len; i++)
    {
        if (arr[i] == numToFind)
        {
            cout << i + 1 << endl;
            return 0;
        }
    }
    cout << -1 << endl;
    return 0;
}