// 题目地址：https://pintia.cn/problem-sets/14/problems/741
#include <stdio.h>

int Count_Digit ( const int N, const int D );

int main()
{
    int N, D;

    scanf("%d %d", &N, &D);
    printf("%d\n", Count_Digit(N, D));
    return 0;
}

int Count_Digit(const int N, const int D) {
    int quotient = N;
    int remainder;
    int appears = 0; // 出现次数
    if (quotient < 0)
        quotient *= -1; // 负数转为正数
        // 这里十分容易错！
    else if (quotient == 0) // 考虑数值为0的情况
        appears++;
    while (quotient != 0) {
        remainder = quotient % 10;
        quotient = quotient / 10;
        if (remainder == D)
            appears++;
    }
    return appears;
}