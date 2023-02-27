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
        逆天，看了题解才知道这题的范文也有退格键<，这题面...
        我最开始是按【边读入边和范文逐字符对比】的方式去写的，但我怎么改都没法AC...
        于是我索性【读完一行后】对所有退格键 < 进行处理，再和范文逐字符对比，这样一写就能AC了...想不到就是这样简单，我还以为会有TLE惊喜来着.

    现在看来，这题其实是不错的模拟题。
    ---------------------------------
    这题我的思路就是：
        1. 先逐行且逐字符读入字符串，然后遇到'<'时，读取指针【就往回退一位】，反之则前进一位，以模拟退格过程。
            * 注意回退的时候【不要越出下界】。
            * 每读完一行字符串，就存入表示范文的数组中。
            * 遇到EOF时，范文读取完毕。

        2. 然后一样是逐行且逐字符读入字符串，和读入范文时一样【处理'<'退格的情况】。
            * 此时【每读完一行字符串】，就和【范文中的同一行字符串进行逐字符比较】，统计相同的字符数即可。
            * 注意，用户输入的行数可能超出范文行数，在这种情况下需要【忽略多余的行数】。
            * 遇到EOF时，用户输入读取完毕。

        3. 最后计算每分钟用户【输入正确的】字符数，这里我用了cmath库的round函数实现了四舍五入。

    这样应该是比较保险的写法，不过实际上运行的效果也还不错。
    我觉得边读入边比较应该也是可以实现的，能省一些时间开销。
    --------------------------------
        - SomeBottle 2023.2.27
*/

/*
# 打字练习

## 题目描述

R 君在练习打字。

有这样一个打字练习网站，给定一个范文和输入框，会根据你的输入计算准确率和打字速度。可以输入的字符有小写字母、空格和 `.`（英文句号），输入字符后，光标也会跟着移动。

输入的文本有多行，R 君可以通过换行键来换行，换行后光标移动到下一行的开头。

R 君也可以按退格键（为了方便，退格键用 `<` 表示），以删除上一个打的字符，并将光标回移一格。特殊的，如果此时光标已经在一行的开头，则不能继续退格（即忽略此时输入的退格键）。

网站的比较方式遵循以下两个原则：

- 逐行比较，即对于范文和输入的每一行依次比较，不同行之间不会产生影响，多余的行会被忽略。
- 逐位比较，即对于两行的每一个字符依次比较，当且仅当字符相同时才会被算作一次正确，否则会被算作错误。计算答案时，只统计相同的字符个数。

需要注意的是，回车键不会被计入正确的字符个数。

R 君看到网站上显示他花了 $T$ 秒完成了这次的打字游戏，请你计算出他的 KPM（Keys per minutes，每分钟输入的字符个数），答案四舍五入保留整数部分。

## 输入格式

R 君会依次告诉你网站的范文，他的输入和花费的时间。

其中范文和输入将会这样读入：给定若干行字符串，以单独的一行 `EOF` 结束，其中 `EOF` 不算入输入的文本。

最后一行一个整数 $T$，表示他打字花费了 $T$ 秒。

可以参考样例输入输出文件和样例解释辅助理解。

## 输出格式

一行一个整数，表示 KPM。

## 样例 #1

### 样例输入 #1

```
hello world.
aaabbbb
x
EOF
heelo world.
aaacbbbb
y<x
EOF
60
```

### 样例输出 #1

```
18
```

## 提示

#### 样例解释

第一行的正确字符数为 11。
第二行的正确字符数为 6，错误的字符 `c` 仍会占据一个位置。
第三行的正确字符数为 1，R 君使用退格键删除了被打错的字符 `y`

#### 数据范围

对于 $20\%$ 的数据，不存在换行键。
对于 $40\%$ 的数据，不存在退格键。
对于 $100\%$ 的数据，$T \leq 10^3$，保证每个文本段的总字符数（包括换行）不超过 $10^5$ 个且总行数不超过 $10^4$。
*/