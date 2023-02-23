#include <cstdio>

using namespace std;

void toLower(char &chr); // 将字符转换为小写

int main()
{
    char word[11]; // 待查找
    scanf("%s", word);
    int wordLen = 0;
    while (word[wordLen] != '\0') // 计算读入单词的长度，顺便转换为纯小写
    {
        toLower(word[wordLen]);
        wordLen++;
    }
    int matchNum = 0;     // 匹配数
    int firstMatch = 0;   // 首次匹配的位置
    char readChr = '\0';  // 读入的字符
    char wordTmp[100001]; // 临时储存读入的单词(有可能一整段文字没有空格)
    int readLen = 0;      // 记录读入的单词长度
    int readCharNum = 0;  // 记录读入的字符数
    while (true)          // 逐字符读入
    {
        int reader = scanf("%c", &readChr);
        if (readChr == '\n' || readChr == '\r') // 忽略多余的空白符
            continue;
        if (reader == EOF || readChr == ' ') // 读入的是空格
        {
            if (readLen == wordLen) // 只有单词长度相同时才比较
            {
                int front = 0, rear = wordLen - 1;
                bool same = true;     // 是否是相同的单词
                while (front <= rear) // 双指针逐字符比较字符串
                {
                    if (word[front] != wordTmp[front] || word[rear] != wordTmp[rear])
                    {
                        same = false;
                        break;
                    }
                    front++;
                    rear--;
                }
                if (same)
                {
                    if (!matchNum) // 如果是首次匹配
                    {
                        firstMatch = readCharNum - readLen; // 计算这个词的首字母的位置
                    }
                    matchNum++;
                }
            }
            readLen = 0;       // 临时字符串置空
            if (reader == EOF) // 读取完毕的话就跳出循环
                break;
        }
        else
        {
            toLower(readChr); // 统一转换为小写，方便比对
            wordTmp[readLen++] = readChr;
        }
        readCharNum++; // 读入的字符数增加
    }
    if (!matchNum) // 无匹配
        printf("-1");
    else
        printf("%d %d", matchNum, firstMatch);
    return 0;
}

inline void toLower(char &chr)
{
    if (chr >= 'A' && chr <= 'Z')
        chr += ('a' - 'A');
}

/*
    1. 因为本题需要给出【单词首字母在文段中出现的位置】，因此为了方便处理，我采用了【逐字符读入】。
        * 注意这里的写法，处理不当的话，可能会漏掉最后一个词。
        * 另外，题目没有说两个词之间只有一个空格，因此要针对【多空格分隔】的情况进行处理。

    2. 同时也是为了方便处理，所有读入的字母都转换为小写字母进行处理

    3. 比较两个词的时候，如果【长度不同】，就不用逐字符比较了。否则，采用一前一后两个指针对【两个词进行逐字符比较】。

        - SomeBottle
*/

/*
# [NOIP2011 普及组] 统计单词数

## 题目描述

一般的文本编辑器都有查找单词的功能，该功能可以快速定位特定单词在文章中的位置，有的还能统计出特定单词在文章中出现的次数。

现在，请你编程实现这一功能，具体要求是：给定一个单词，请你输出它在给定的文章中出现的次数和第一次出现的位置。注意：匹配单词时，不区分大小写，但要求完全匹配，即给定单词必须与文章中的某一独立单词在不区分大小写的情况下完全相同（参见样例 1），如果给定单词仅是文章中某一单词的一部分则不算匹配（参见样例 2）。

## 输入格式

共 $2$ 行。

第 $1$ 行为一个字符串，其中只含字母，表示给定单词；

第 $2$ 行为一个字符串，其中只可能包含字母和空格，表示给定的文章。

## 输出格式

一行，如果在文章中找到给定单词则输出两个整数，两个整数之间用一个空格隔开，分别是单词在文章中出现的次数和第一次出现的位置（即在文章中第一次出现时，单词首字母在文章中的位置，位置从 $0$ 开始）；如果单词在文章中没有出现，则直接输出一个整数 $-1$。

## 样例 #1

### 样例输入 #1

```
To
to be or not to be is a question
```

### 样例输出 #1

```
2 0
```

## 样例 #2

### 样例输入 #2

```
to
Did the Ottoman Empire lose its power at that time
```

### 样例输出 #2

```
-1
```

## 提示

数据范围

$1\leq $ 第一行单词长度 $\leq10$。

$1\leq $ 文章长度 $\leq10^6$。

noip2011 普及组第 2 题
*/