/*
蓝桥杯基础练习VIP-时间转换
时间限制: 1s 内存限制: 128MB

题目描述
    给定一个以秒为单位的时间t，要求用  “<H>:<M>:<S>”的格式来表示这个时间。<H>表示小时，<M>表示分钟，而<S> 表示秒，它们都是整数且没有前导的“0”。例如，若t=0，则应输出是“0:0:0”；若t=3661，则输出“1:1:1”。
输入格式
    输入只有一行，是一个整数t（0<=t<=86399）。
输出格式
    输出只有一行，是以“<H> :<M> :<S> ”的格式所表示的时间，不包括引号。
样例输入
    5436
样例输出
    1:30:36
*/
#include <iostream>

using namespace std;

int main()
{
    int time; // 时间：相对0点的秒数
    cin >> time;
    short secondsRemain = time % 60;    // 剩余多少秒
    short minutes = time / 60;          // 总共有多少分钟, 60秒一分钟
    short hours = minutes / 60;         // 60分钟一小时
    short minutesRemain = minutes % 60; // 剩余多少分钟
    cout << hours << ":" << minutesRemain << ":" << secondsRemain;
    return 0;
}
/*
    这题是真·入门题了，就没什么好注释的啦
        - SomeBottle 2022.11.27
*/