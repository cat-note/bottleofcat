#include <cstdio>

using namespace std;

inline int strLen(char *str); // 获得字符串长度

int main()
{
    char a[502], b[502]; // a和b两个数，先用字符串表示
    scanf("%s %s", a, b);
    int aPtr = strLen(a) - 1, bPtr = strLen(b) - 1; // 指向a和b的数值最低位
    int result[502];                                // 结果（从低位向高位储存）
    int resLen = 0;                                 // 结果长度
    int carry = 0;                                  // 进位数值
    for (; resLen < 502; resLen++)                  // 从最低位开始运算
    {
        int aDigit = 0, bDigit = 0;
        // 注意这里是当进位都没有的时候才算运算结束
        if (aPtr < 0 && bPtr < 0 && carry == 0) // 运算完毕
            break;
        if (aPtr >= 0)
            aDigit = (int)(a[aPtr--] - '0');
        if (bPtr >= 0)
            bDigit = (int)(b[bPtr--] - '0');
        int addition = aDigit + bDigit + carry; // 先把十进制位相加
        if (addition >= 10)                     // 产生进位
        {
            carry = addition / 10;
            addition %= 10;
        }
        else
        {
            carry = 0; // 否则无进位
        }
        result[resLen] = addition;
    }
    for (int i = resLen - 1; i >= 0; i--)
        printf("%d", result[i]);
    return 0;
}

int strLen(char *str)
{
    int cnt = 0;
    while (str[cnt] != '\0')
        cnt++;
    return cnt;
}