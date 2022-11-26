/*
蓝桥杯基础练习VIP-字符串对比
时间限制: 1s 内存限制: 512MB
题目描述
    给定两个仅由大写字母或小写字母组成的字符串(长度介于1到10之间)，它们之间的关系是以下4中情况之一：

        1：两个字符串长度不等。比如  Beijing  和  Hebei

        2：两个字符串不仅长度相等，而且相应位置上的字符完全一致(区分大小写)，比如  Beijing  和  Beijing

        3：两个字符串长度相等，相应位置上的字符仅在不区分大小写的前提下才能达到完全一致（也就是说，它并不满足情况2）。比如  beijing  和  BEIjing

        4：两个字符串长度相等，但是即使是不区分大小写也不能使这两个字符串一致。比如  Beijing  和  Nanjing

    编程判断输入的两个字符串之间的关系属于这四类中的哪一类，给出所属的类的编号。
输入格式
    包括两行，每行都是一个字符串
输出格式
    仅有一个数字，表明这两个字符串的关系编号
样例输入
    BEIjing
    beiJing
样例输出
    3
*/
#include <iostream>

using namespace std;

int main()
{
    string str1, str2;
    cin >> str1;
    cin >> str2;
    // 字符串长度在1~10, short类型足够了
    short strLen1 = str1.length();
    short strLen2 = str2.length();
    if (strLen1 != strLen2) // 第一种情况：字符串长度不等
    {
        cout << 1;
    }
    else // 其他三种情况字符串长度全都是相同的
    {
        short result = 2; // 至少是情况2了
        for (short i = 0; i < strLen1; i++)
        {
            if (str1[i] != str2[i]) // 有字符不相同
            {
                // 将str2中的当前字符切换成str1中当前字符一样的大小写试试
                // 大写和小写字母在ASCII码中相差32
                char caseSwitched = str1[i] > str2[i] ? str2[i] + 32 : str2[i] - 32;
                if (str1[i] == caseSwitched) // 切换大小写后相同了，情况变为3
                {
                    result = 3;
                }
                else // 切换大小写后还不同，不用看后面了
                {
                    result = 4;
                    break;
                }
            }
        }
        cout << result;
    }
    return 0;
}
/*
    这题在逻辑判断上有两个主要的分界点：
        1. 字符串长度是否相同
        2. 字符串中是否有字符不相同
    总体来说是简单的一题。

        - SomeBottle 2022.11.26
*/