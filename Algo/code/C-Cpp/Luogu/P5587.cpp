#include <cstdio>
#include <string>
#include <cmath>

using namespace std;

int readText(string *arr);                 // 读入文本，返回行数
int inputText(string *model, int lineNum); // 读入用户输入，进行对比，返回用户打对的字符数

int main()
{
    string model[10001];                        // 范文
    int lineNum = readText(model);              // 读入范文
    int correctNum = inputText(model, lineNum); // 检验用户输入
    double seconds;
    scanf("%lf", &seconds);        // 读入耗时
    double minutes = seconds / 60; // 转换为分钟
    double result = (double)correctNum / minutes;
    printf("%d", (int)round(result));
    return 0;
}

int readText(string *arr)
{
    char tmp[100001]; // 临时字符数组
    int tmpLen = 0;   // 临时字符数
    int line = 0;     // 当前读取的是第line行
    while (true)
    {
        scanf("%c", &tmp[tmpLen]);                      // 逐字符读入
        if (tmp[tmpLen] == '\r' || tmp[tmpLen] == '\n') // 如果遇到了换行符，说明一行读取结束
        {
            tmp[tmpLen] = '\0'; // 忽略换行符
            // 遇到EOF，就说明读完了当前文本段
            if (tmp[0] == 'E' && tmp[1] == 'O' && tmp[2] == 'F')
                break;
            arr[line++].assign(tmp); // 读入文本
            tmpLen = 0;              // 临时字符数归零
        }
        else
        {
            tmpLen++;
        }
    }
    return line;
}

int inputText(string *model, int lineNum)
{
    char tmp[100001];    // 临时字符数组
    int tmpLen = 0;      // 临时字符数
    int cmpInd = 0;      // 比较指针
    int line = 0;        // 当前读取的是第line行
    int correct = 0;     // 记录输入正确的字符数
    bool ignore = false; // 是否开始忽略
    while (true)
    {
        scanf("%c", &tmp[tmpLen]); // 逐字符读入
        if (line >= lineNum)       // 输入的行数多于了范文，忽略多余内容
            ignore = true;
        if (tmp[tmpLen] == '\r' || tmp[tmpLen] == '\n') // 如果遇到了换行符，说明一行读取结束
        {
            tmp[tmpLen] = '\0'; // 忽略换行符
            // 遇到EOF，就说明读完了当前文本段
            if (tmp[0] == 'E' && tmp[1] == 'O' && tmp[2] == 'F')
                break;
            tmpLen = 0; // 临时字符数归零
            cmpInd = 0; // 比较指针归零
            line++;
        }
        else if (ignore) // 忽略多余的输入
        {
            tmpLen++;
        }
        else
        {
            if (tmp[tmpLen] == '<')
            {
                if (cmpInd > 0) // 比较指针最多回退到0
                    cmpInd--;   // 比较指针回退，模拟退格
            }
            else if (model[line][cmpInd] == tmp[tmpLen]) // 如果指定位置上的字符一致，就统计在内
            {
                correct++;
                cmpInd++;
            }
            else
            {
                cmpInd++;
            }
            tmpLen++;
            // 注意，length方法返回的是无符号整数，需要转换一下
            // 不然编译器会把cmpInd转换为无符号。
            if (cmpInd >= (int)model[line].length()) // 防止用户输入过长导致越界
                cmpInd = (int)model[line].length();
        }
    }
    return correct; // 返回输入正确的数量
}

/*
    因为退格键仅限当前行，且回车符不会计入正确字符数，因此本题按行处理比较合适，而且很适合【边读入边处理】。

    逆天，看了题解才知道这题的范文也有退格键<，这什么鬼题面...
*/