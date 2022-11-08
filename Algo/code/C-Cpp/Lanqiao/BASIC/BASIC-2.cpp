/*
试题 基础练习 01字串
资源限制
    内存限制：256.0MB   C/C++时间限制：1.0s   Java时间限制：3.0s   Python时间限制：5.0s
问题描述
    对于长度为5位的一个01串，每一位都可能是0或1，一共有32种可能。它们的前几个是：
    00000
    00001
    00010
    00011
    00100
    请按从小到大的顺序输出这32种01串。
输入格式
    本试题没有输入。
输出格式
    输出32行，按从小到大的顺序每行一个长度为5的01串。
样例输出
    00000
    00001
    00010
    00011
    <以下部分省略>

*/
#include <iostream>

using namespace std;
/*
    对于这题，我的思路是用一个字符数组储存各位，然后模拟一下二进制数加法进位的过程。
    当然有暴力解法，直接上五个for循环即可 - SomeBottle
*/
/*
暴力解
int main(){
    for(int i=0;i<2;i++)
        for(int j=0;j<2;j++)
            for(int k=0;k<2;k++)
                for(int l=0;l<2;l++)
                    for(int m=0;m<2;m++)
                        cout<<i<<j<<k<<l<<m<<endl;
    return 0;
}
*/
int main()
{
    // 用一个字符数组从高位到低位（方便输出）表示
    char digits[] = {'0', '0', '0', '0', '0'}; // 通过修改这里，可以改变枚举的位数
    // 计算出位数
    int len = sizeof(digits) / sizeof(digits[0]);
    // 先把初始值输出
    cout << digits << endl;
    while (true)
    {
        bool carry = true; // 是否有进位，初始值置为true代表在最低位上+1
        for (int i = len - 1; i >= 0; i--)
        {
            if (digits[i] == '1')
            {                    // 逢1就进位
                digits[i] = '0'; // 本位置为0
                carry = true;    // 产生新的进位
            }
            else
            { // 逢0，进位完成
                digits[i] = '1';
                carry = false;
            }
            if (!carry)
            { // carry=false代表进位完成
                break;
            }
            else if (i == 0)
            { // 到了最高位仍然carry=true(有进位)，这说明已经枚举完毕，程序结束
                return 0;
            }
        }
        cout << digits << endl;
    }
    return 0;
}