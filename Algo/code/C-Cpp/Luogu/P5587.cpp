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
        tmp[tmpLen] = getchar();                        // 逐字符读入
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
            if (tmp[tmpLen] == '<') // 如果读入的是一个退格键
            {
                if (tmpLen > 0) // 防止越下界
                    tmpLen--;   // 模拟回退
            }
            else
            {
                tmpLen++;
            }
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
    bool ignore = false; // 是否开始忽略多余的行
    while (true)
    {
        tmp[tmpLen] = getchar();                        // 逐字符读入
        if (tmp[tmpLen] == '\r' || tmp[tmpLen] == '\n') // 如果遇到了换行符，说明一行读取结束
        {
            tmp[tmpLen] = '\0';                                  // 忽略换行符
            if (tmp[0] == 'E' && tmp[1] == 'O' && tmp[2] == 'F') // 遇到EOF，就说明读完了当前文本段
            {
                break;
            }
            else if (!ignore) // 否则，如果读入的内容没有超出范文的行数，就逐字符比较读入的内容和范文
            {
                int minLen = tmpLen > model[line].length() ? model[line].length() : tmpLen; // 找出较短的一条字符串的长度
                for (int i = 0; i < minLen; i++)
                {
                    if (tmp[i] == model[line][i])
                        correct++;
                }
            }
            tmpLen = 0; // 临时字符数归零
            cmpInd = 0; // 比较指针归零
            line++;
            if (line >= lineNum) // 输入的行数多于了范文，忽略多余内容
                ignore = true;
        }
        else
        {
            if (tmp[tmpLen] == '<')
            {
                if (tmpLen > 0) // 模拟回退
                    tmpLen--;   // 读取指针回退，模拟退格
            }
            else
            {
                tmpLen++;
            }
        }
    }
    return correct; // 返回输入正确的数量
}

/*
    因为退格键仅限当前行，且回车符不会计入正确字符数，因此本题按行处理比较合适。

    * 吐槽:  
        逆天，看了题解才知道这题的范文也有退格键<，这什么鬼题面...
        我最开始是按【边读入边和范文逐字符对比】的方式去写的，但我怎么改都没法AC...
        于是我索性【读完后】再和范文逐字符对比，这样一写就能AC了...想不到就是这样简单，我还以为会有TLE惊喜来着.

    现在看来，这题其实是不错的模拟题。
    ---------------------------------
    这题我的思路就是：
        1. 先逐行且逐字符读入字符串，然后遇到'<'时，读取指针【就往回退一位】，反之则前进一位，以模拟退格过程。
            *
    
*/