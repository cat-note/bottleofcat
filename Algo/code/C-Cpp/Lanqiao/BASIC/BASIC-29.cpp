/*
蓝桥杯基础练习VIP-高精度加法
    时间限制: 1s 内存限制: 128MB 提交: 9822 解决: 4560
题目描述
    输入两个整数a和b，输出这两个整数的和。a和b都不超过100位。
输入格式
    输入包括两行，第一行为一个非负整数a，第二行为一个非负整数b。两个整数都不超过100位，两数的最高位都不是0。
输出格式
    输出一行，表示a + b的值。
样例输入
    20100122201001221234567890
    2010012220100122
样例输出
    20100122203011233454668012
*/
#include <iostream>
#include <vector>

using namespace std;
/*
    这题的思路和BASIC-30大规模阶乘计算思路很类似，都是依赖数组储存结果，手动实现进位
    不过这题很有意思的是：
        输入的数值规模可能就难以用C++的基本数据类型表达
    对此我的解决办法是将输入的A和B数以字符串的形式处理
        - SomeBottle 20221112
*/
int main()
{
    vector<short> result;
    string numA;
    string numB;
    cin >> numA; // 因为输入数值可能位数很多，按字符串读入
    cin >> numB;
    int lenA = numA.length(); // 获得两数的长度
    int lenB = numB.length();
    int currDigit = 0; // 正在处理的结果(result)的数位(也就是数组下标)
    int carry = 0;     // 待进入高位的数值
    int numAIndex, numBIndex;
    do
    {
        // 从低位向高位扫描A和B数的每个数位，进行相加
        numAIndex = lenA - 1 - currDigit; // 根据currDigit计算正在处理的A和B的数位下标
        numBIndex = lenB - 1 - currDigit;
        short resultDigit = 0; // 算出来的结果位
        if (numAIndex >= 0)    // 如果没有超出A的最高位，就将A当前位加到结果位上
            resultDigit += (short)(numA[numAIndex] - '0');
        if (numBIndex >= 0) // 如果没有超出B的最高位，就将B当前位加到结果位上
            resultDigit += (short)(numB[numBIndex] - '0');
        resultDigit += carry % 10; // 把从低位进过来的位(carry的最低位)加入到结果位上
        carry /= 10;               // 去除已经进了的最低位，为接下来的进位作准备
        if (resultDigit > 9)
        {                              // 算出来发现当前结果位>9了，产生了进位
            carry += resultDigit / 10; // 新产生的进位加入到carry中
            resultDigit %= 10;         // 结果位只保留最低的个位
        }
        result.push_back(resultDigit);                      // 将结果位加入结果中
        currDigit++;                                        // 准备处理结果的下一位
    } while (numAIndex > 0 || numBIndex > 0 || carry != 0); // 如果A和B的位数还没处理完成，或者还有进位没有处理完，就继续循环
    for (int i = result.size() - 1; i >= 0; i--)
    { // 从高位往低位输出
        cout << result[i];
    }
    return 0;
}