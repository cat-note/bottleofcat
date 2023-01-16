#include <cstdio>
#include <string>
#include <stack>

using namespace std;

int main()
{
    stack<string> expression; // 储存表达式的栈
    stack<float> numStack;    // 储存操作数的栈
    char tempStr[31];         // 临时字符数组，用于读入数据
    string data;              // 临时字符串
    // scanf扫描字符串时直到遇到空白符为止
    while (scanf("%s", tempStr) != EOF)
    {
        data.assign(tempStr);  // 转换为C++ string
        expression.push(data); // 压入栈
    }
    bool errorFlag = false; // 是否出现错误
    // 出栈，借此实现从右向左读取表达式
    while (!expression.empty())
    {
        data = expression.top(); // 取出一个数据
        expression.pop();
        if (data.length() == 1 && (data[0] < '0' || data[0] > '9'))
        {
            // 数据长度为1且这个字符不是数字，那就是符号
            if (numStack.size() < 2)
            {
                // 如果操作数栈的操作数小于2个，不够用了
                errorFlag = true;
                break;
            }
            // 从栈中取出两个操作数
            float num1 = numStack.top();
            numStack.pop();
            float num2 = numStack.top();
            numStack.pop();
            bool opSuccess = true; // 运算是否成功
            switch (data[0])
            {
            case '+':
                num1 += num2;
                break;
            case '-':
                num1 -= num2;
                break;
            case '*':
                num1 *= num2;
                break;
            case '/':
                if (num2 == 0) // 很容易忽略的情况，当除数为0时无法正常运算
                    opSuccess = false;
                else
                    num1 /= num2;
                break;
            default:
                opSuccess = false; // 未知运算符
            }
            // 运算不成功就不继续了
            if (!opSuccess)
            {
                errorFlag = true;
                break;
            }
            // 运算成功就把结果压入操作数栈
            numStack.push(num1);
        }
        else
        {
            // 否则视作数字处理
            float num;   // 读取的数字
            int scanNum; // sscanf匹配的数量
            if (!(scanNum = sscanf(data.c_str(), "%f%s", &num, tempStr)) || scanNum > 1)
            {
                // 如果匹配的数量=0或者>1，说明数字非法
                errorFlag = true;
                break;
            }
            // 数字合法，压入栈
            numStack.push(num);
        }
    }
    // 算完后操作数栈只有一个结果才是正常的
    if (numStack.size() != 1)
        errorFlag = true;
    if (errorFlag)
        printf("ERROR");
    else
        printf("%.1f", numStack.top());
    return 0;
}

/*
    DS-7-21.cpp我写的有点繁琐了，这里咱重写了一下，利用了C++的string，代码要简洁一些。

        - SomeBottle 2023.1.16
*/