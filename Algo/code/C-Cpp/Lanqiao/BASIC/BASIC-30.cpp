/*
蓝桥杯基础练习VIP-阶乘计算
    时间限制: 1s 内存限制: 128MB 提交: 8610 解决: 4294
题目
    输入一个正整数n，输出n!的值。
    其中n!=1*2*3*…*n。
算法描述
    n!可能很大，而计算机能表示的整数范围有限，需要使用高精度计算的方法。使用一个数组A来表示一个大整数a，A[0]表示a的个位，A[1]表示a的十位，依次类推。
    将a乘以一个整数k变为将数组A的每一个元素都乘以k，请注意处理相应的进位。
    首先将a设为1，然后乘2，乘3，当乘到n时，即得到了n!的值。
输入格式
    输入包含一个正整数n，n< =1000。
输出格式
    输出n!的准确值。
样例输入
    10
样例输出
    3628800
*/
#include <iostream>
#include <vector>

using namespace std;
/*
    这道题容易遗漏的地方是 当前位在加上来自低位的进位后，可能又要再向高位进一位
    这是咱第二次遇到这个题目了，上一回是在PTA那边遇到的，具体可以看看我写的笔记：
    https://www.cnblogs.com/somebottle/p/big_number_fatorial.html
        - SomeBottle 2022.11.12
*/
int main()
{
    int n;
    cin >> n;                                   // 注意题目中说n为正整数，不用考虑0!的情况了
    vector<short> result{1};                    // 用一个vector容器按**从低位到高位**的顺序储存结果每一位数字
    int digitsNum = 1;                          // 储存位数，而不是老是用vector的size方法
    for (int factor = 2; factor <= n; factor++) // 从2开始乘
    {
        int carry = 0;     // 进位数
        int currDigit = 0; // 当前处理到的位数
        do
        {
            int mulDigit = 0;
            if (currDigit < digitsNum) // 不需要新增数位
            {
                mulDigit = result[currDigit] * factor; // 算出当前位乘上因子的结果
            }
            else
            { // 新增一位（在进位的时候会发生这种情况）
                result.push_back(0);
                digitsNum++;
            }
            int newDigit = mulDigit % 10 + carry % 10; // 将carry的最低位加上上面结果(mulDigit)的最低位，得到当前位的新值
            carry = carry / 10 + mulDigit / 10;        // 更新carry，除去最低位，并加上(multDigit)除最低位外的数
            if (newDigit > 9)
            {
                // 进位后，新的一位又>9了，需要再进到高位
                carry += newDigit / 10;
                newDigit = newDigit % 10;
            }
            result[currDigit] = (short)newDigit;       // 更新当前位
            currDigit++;                               // 处理下一位
        } while (currDigit < digitsNum || carry != 0); // 位数没处理完，或者还有进位，循环就要继续
    }
    for (int i = digitsNum - 1; i >= 0; i--)
    {
        cout << result[i];
    }
    return 0;
}