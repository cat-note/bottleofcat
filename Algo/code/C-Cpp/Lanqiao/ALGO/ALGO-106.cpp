/*
蓝桥杯算法训练VIP-判定字符位置
时间限制: 1s 内存限制: 128MB

题目描述
    返回给定字符串s中元音字母的首次出现位置。英语元音字母只有‘a’、‘e’、‘i’、‘o’、‘u’五个。
    若字符串中没有元音字母，则返回0。
    只考虑小写的情况。
输入格式
    输入一串字符串
输出格式
    一个数
样例输入
    and
样例输出
    1
*/
#include <iostream>

using namespace std;

int main()
{
    bool map[118]{false}; // 空间换时间
    map['a'] = true;
    map['e'] = true;
    map['i'] = true;
    map['o'] = true;
    map['u'] = true;
    string myStr;
    cin >> myStr;
    int pos = 0;
    for (int i = 0, len = myStr.length(); i < len; i++)
    {
        if (map[myStr[i]])
        {
            pos = i + 1;
            break;
        }
    }
    cout << pos;
    return 0;
}
/*
    直接通过访问数组元素得知字母是否是元音字母

        * 注意，本题中的字符位置是从1开始的

        - SomeBottle 2022.11.30
*/