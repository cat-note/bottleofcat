/*
 * 试题 基础练习 十六进制转十进制
    问题描述
    　　从键盘输入一个不超过8位的正的十六进制数字符串，将它转换为正的十进制数后输出。
    　　注：十六进制数中的10~15分别用大写的英文字母A、B、C、D、E、F表示。
    资源限制
        内存限制：512.0MB   C/C++时间限制：1.0s   Java时间限制：3.0s   Python时间限制：5.0s
    样例输入
        FFFF
    样例输出
        65535
 */
#include <iostream>
#include <map>

using namespace std;

int main() {
    map<char, int> hexMap;
    // 映射16进制位到十进制数
    hexMap['1'] = 1;
    hexMap['2'] = 2;
    hexMap['3'] = 3;
    hexMap['4'] = 4;
    hexMap['5'] = 5;
    hexMap['6'] = 6;
    hexMap['7'] = 7;
    hexMap['8'] = 8;
    hexMap['9'] = 9;
    hexMap['A'] = 10;
    hexMap['B'] = 11;
    hexMap['C'] = 12;
    hexMap['D'] = 13;
    hexMap['E'] = 14;
    hexMap['F'] = 15;
    string hexNum; // 输入的16进制数
    cin >> hexNum;
    unsigned long int decNum = 0; // 结果十进制数(题目给的16进制数不会超过八位，但这也比较大了，用unsigned long顶一下）
    // 从低位向高位读
    for (int i = hexNum.length() - 1, exp = 0; i >= 0; i--, exp++) {
        int digit = hexMap[hexNum[i]]; // 找到当前十六进制位对应的十进制数
        unsigned int priority = 1; // 位权
        for (int j = 0; j < exp; j++)
            priority *= 16;
        decNum += digit * priority;
    }
    cout << decNum << endl;
    return 0;
}
