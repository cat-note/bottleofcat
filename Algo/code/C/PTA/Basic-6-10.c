// 题目地址：https://pintia.cn/problem-sets/14/problems/742
#include <stdio.h>

// 易错点；N=12

void Print_Factorial(const int N);

int main() {
    int N;

    scanf("%d", &N);
    Print_Factorial(N);
    return 0;
}

void Print_Factorial(const int N) {
    if (N < 0 || N > 1000) {
        printf("Invalid input");
        return;
    }
    /*
        如果阶乘结果大到C语言中任意一种基本数据类型都无法表达，
        不妨考虑一下能不能用某种数据结构来解决问题
        这里采用数组
    */
    /* 1000个1000相乘：1*(10^(3*1000))=1e+3000,
        结果是1000000000....(3000个零)
        而本题N不超过1000，阶乘结果肯定也达不到1e+3000,
        这里就给数组分配3000个元素
    */
    // 从数组第一个元素为个位开始，往后位数升高
    int arr[3000] = {}; // 全初始化为0
    arr[0] = 1; // 个位为1
    int arrLen = 1; // 标记数组目前元素个数(数字位数)
    int factor = 2; // 从2开始乘，因为arr[0]=1
    for (; factor <= N; factor++) {
        // 将数组每一位都乘i，并进行进位处理(超过9的数字往高处进)
        int j;
        int carry = 0; // 要进到高位的数字
        int multiplied; // 用于临时储存数组中每一位数字乘了之后的值
        int calcDigit; // 用于临时储存新计算出来的某一位数字
        for (j = 0; j < arrLen; j++) {
            multiplied = arr[j] * factor; // 每一位都乘i
            // multiplied % 10取最低位，比如6*3=18>9，这个时候取出8，而1要进到高位
            // carry % 10 将进位数字的最低位取出，这一位是进到当前j这个位的
            calcDigit = multiplied % 10 + carry % 10;
            carry = carry / 10 + multiplied / 10; // 进位数字去掉最低位，加上multiplied要进到高位的数字
            // 一种很容易错的情况，虽然multiplied % 10和carry % 10分别不会>=10，但是他们加起来是可能>=10的！
            // 这种时候还要再进一次位
            if (calcDigit >= 10) {
                carry += calcDigit / 10;
                calcDigit %= 10;
            }
            arr[j] = calcDigit;
            if (j >= arrLen - 1 && carry > 0) // 位数不够了，但有需要进到高位的数字，那么就增加位数（增加数组元素）
                arrLen++;
        }
    }
    // 打印结果数字
    int i;
    // 因为随着下标增加，数字位数升高，要打印出来就得倒着遍历数组
    for (i = arrLen - 1; i >= 0; i--) {
        printf("%d", arr[i]);
    }
}

/* N=12出现错误的代码
void Print_Factorial(const int N) {
    if (N < 0) {
        printf("Invalid input");
        return;
    }
    // 从数组第一个元素为个位开始，往后位数升高
    int arr[3000] = {}; // 全初始化为0
    arr[0] = 1; // 个位为1
    int arrLen = 1; // 标记数组目前元素个数(数字位数)
    int factor = 2; // 从2开始乘，因为arr[0]=1
    for (; factor <= N; factor++) {
        // 将数组每一位都乘i，并进行进位处理(超过9的数字往高处进)
        int j;
        int carry = 0; // 要进到高位的数字
        int multiplied; // 用于储存数组中每一位数字乘了之后的值
        for (j = 0; j < arrLen; j++) {
            multiplied = arr[j] * factor; // 每一位都乘i
            // multiplied % 10取最低位，比如6*3=18>9，这个时候取出8，而1要进到高位
            // carry % 10 将进位数字的最低位取出，这一位是进到当前j这个位的
            arr[j] = multiplied % 10 + carry % 10;
            carry = carry / 10 + multiplied / 10; // 进位数字去掉最低位，加上multiplied要进到高位的数字
            if (j >= arrLen - 1 && carry > 0) // 位数不够了，但有需要进到高位的数字，那么就增加位数（增加数组元素）
                arrLen++;
        }
    }
    int i;
    for (i = arrLen - 1; i >= 0; i--) {
        printf("%d", arr[i]);
    }
}
 */