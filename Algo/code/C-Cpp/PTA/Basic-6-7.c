// 题目：https://pintia.cn/problem-sets/14/problems/739
#include <stdio.h>
#include <math.h>

int IsTheNumber(const int N);

int HasTwoSameNum(const int number);

int main() {
    int number;
    printf("Input an integer: ");
    fflush(stdout);
    scanf("%d", &number);
    printf("IsTheNumber = %d\n", IsTheNumber(number));

    return 0;
}

int HasTwoSameNum(const int number) {
    int quotient = number; // 商
    int remainder; // 余数
    int arrLen = 0;
    int arr[10]; // int型最多10位数字
    int i;
    while (quotient > 0) {
        remainder = quotient % 10; // 余数
        quotient = quotient / 10; // 商
        for (i = 0; i < arrLen; i++) {
            if (arr[i] == remainder)
                return 1;
        }
        arr[arrLen++] = remainder;
    }
    return 0;
}

int IsTheNumber(const int N) {
    int squareRoot = (int) sqrt(N);
    // 如果平方根都不是，就没必要调用HasTwoSameNum了
    if (squareRoot * squareRoot == N && HasTwoSameNum(N))
        return 1;
    else
        return 0;
}