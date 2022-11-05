/**
 * 试题 基础练习 十六进制转八进制
 * 资源限制
 *     内存限制：512.0MB   C/C++时间限制：1.0s
 *  问题描述
　　    给定n个十六进制正整数，输出它们对应的八进制数。
    输入格式
　　    输入的第一行为一个正整数n （1<=n<=10）。
　　    接下来n行，每行一个由0~9、大写字母A~F组成的字符串，表示要转换的十六进制正整数，每个十六进制数长度不超过100000。
    输出格式
    　　输出n行，每行为输入对应的八进制正整数。
    　　【注意】
    　　输入的十六进制数不会有前导0，比如012A。
    　　输出的八进制数也不能有前导0。
    样例输入
    　　2
    　　39
    　　123ABC

    样例输出
    　　71
    　　4435274
 */
#include <iostream>
#include <stack>

using namespace std;

string hexToBin(char hexDigit);

int binToOct(string binDigits);

int main() {
    int lineNum;
    cin >> lineNum;
    for (int i = 0; i < lineNum; i++) {
        string hexNum; // 原十六进制数
        string binNum; // 转换成的二进制数
        stack<int> octNum; // 转换的结果八进制数，每一位压入栈
        cin >> hexNum; // 接受十六进制数
        // 先转换为二进制数,按**低位到高位**的顺序读入（而不是像在纸上写时用的高位到低位）
        for (int j = hexNum.length() - 1; j >= 0; j--) {
            binNum += hexToBin(hexNum[j]);
        }
        // 不足三位就补零
        int zeroNum = binNum.length() % 3;
        if (zeroNum != 0)
            zeroNum = 3 - zeroNum; // 算出缺0的个数
        for (int j = 0; j < zeroNum; j++)
            binNum += "0"; // 因为二进制数是从低位到高位储存着的，所以在末尾补零就是在高位补零
        // **三位一读**成八进制的一位
        for (int j = 0, len = binNum.length(); j < len; j += 3) {
            // 三位一读，值得注意的是这里的八进制位读出来也是从低位往高位排的
            // 所以我用栈来将其从高位到低位输出，先在这里压入栈
            octNum.push(binToOct(binNum.substr(j, 3)));
        }
        // 如果栈顶为0就弹了（因为题目有要求输出的八进制数也不能有前导0）
        // 因为输入时也没有前导0，所以结果中至多只会有一个多余的0出现，如果出现了弹掉就行。
        if (octNum.top() == 0)
            octNum.pop();
        // 弹栈输出八进制数
        for (int j = 0, len = octNum.size(); j < len; j++) {
            cout << octNum.top();
            octNum.pop();
        }
        cout << endl;
    }
    return 0;
}

// 3位二进制读成一位八进制
int binToOct(string binDigits) {
    int result = 0;
    for (int i = 0; i < 3; i++) {
        int priority = 1;
        for (int j = 0; j < i; j++)
            priority *= 2; // 计算位权
        result += (((int) (binDigits[i] - '0')) * priority);
    }
    return result;
}

// 一位十六进制读成4位二进制（倒序）
string hexToBin(char hexDigit) {
    switch (hexDigit) {
        case '0':
            return "0000"; // 这里的二进制数是倒序的
        case '1':
            return "1000";
        case '2':
            return "0100";
        case '3':
            return "1100";
        case '4':
            return "0010";
        case '5':
            return "1010";
        case '6':
            return "0110";
        case '7':
            return "1110";
        case '8':
            return "0001";
        case '9':
            return "1001";
        case 'A':
            return "0101";
        case 'B':
            return "1101";
        case 'C':
            return "0011";
        case 'D':
            return "1011";
        case 'E':
            return "0111";
        case 'F':
            return "1111";
    }
    return "0000";
}
