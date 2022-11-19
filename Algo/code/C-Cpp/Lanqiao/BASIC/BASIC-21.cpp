/*
蓝桥杯基础练习VIP-Sine之舞
时间限制: 1s 内存限制: 128MB 提交: 4202 解决: 2553

题目描述
    最近FJ为他的奶牛们开设了数学分析课，FJ知道若要学好这门课，必须有一个好的三角函数基本功。所以他准备和奶牛们做一个“Sine之舞”的游戏，寓教于乐，提高奶牛们的计算能力。
    不妨设
    An=sin(1–sin(2+sin(3–sin(4+...sin(n))...)
    Sn=(...(A1+n)A2+n-1)A3+...+2)An+1
    FJ想让奶牛们计算Sn的值，请你帮助FJ打印出Sn的完整表达式，以方便奶牛们做题。
输入格式
    仅有一个数：N<201。
输出格式
    请输出相应的表达式Sn，以一个换行符结束。输出中不得含有多余的空格或换行、回车符。
样例输入
    3
样例输出
    ((sin(1)+3)sin(1-sin(2))+2)sin(1-sin(2+sin(3)))+1
*/
#include <iostream>
#include <cstdio>

using namespace std;
/*
    这题虽然蓝桥杯OJ里给的关键词是递归/递推，但完全可以用非递归写法
    这里我就用的是非递归写法了。
    在知道规律后这题其实就是字符串拼接和迭代该怎么写的问题了。
    值得注意的点：
        1. An中的加减交替
        2. Sn是从A1+n开始的
    * 注：本题中整数转字符串我用的是stdio库中的sprintf，这样可以兼容C++98
    * 不过现在蓝桥杯是支持C++11特性了，所以也可以用to_string，写起来要简单多了
        - SomeBottle 2022.11.19

    吐槽：这题目...机翻的吧，奶牛是什么鬼啊喂！
*/
int main()
{
    int N;
    cin >> N;
    string An = "sin(1-sin(2";
    string Sn = "sin(1)+";
    char tempStr[4];      // 用于转换数字为字符串的临时字符串
    bool subFlag = false; // false的话，An下一次迭代为+；否则为-
    sprintf(tempStr, "%d", N);
    Sn += tempStr; // 先把Sn=A1+n的结果算出来
    for (int i = 1; i < N; i++)
    {
        sprintf(tempStr, "%d", N - i); // 数字转为字符串
        Sn = "(" + Sn + ")" + An;      // 先用括号把之前的Sn包起来，然后再粘上An
        // 补齐An后面的反括号)，比如sin(1-sin(2，就要补两个反括号变成sin(1-sin(2))
        for (int j = 0; j < i + 1; j++)
            Sn += ")";
        Sn += "+"; // Sn尾部的+n
        Sn += tempStr;
        // 接下来算出下一次迭代的An
        An += (subFlag ? "-" : "+"); // An下一次是-还是+
        An += "sin(";
        sprintf(tempStr, "%d", i + 2);
        An += tempStr;
        subFlag = !subFlag; // An符号+/-交替
    }
    cout << Sn << "\n";
    return 0;
}
