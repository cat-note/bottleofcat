#include <cstdio>
#include <stack>

using namespace std;

bool opCompare(char op1, char op2); // 比较运算符op1是否优于op2
bool isNum(char chr);               // 检查字符是否是数字

int main()
{
    stack<char> opStack;   // 运算符栈
    char prevChar = ' ';   // 读入的上一个字符
    bool putSpace = false; // 标记是否已经有输出，为true则会开始放置空格
    char readChar;         // 从输入读取的一个字符
    while (scanf("%c", &readChar) != EOF)
    {
        if (readChar == '\n') // 方便命令行测试
            break;
        if (isNum(readChar))
        {
            // case1------------------------------读入的是数字
            // 如果上一个读入的不是数字，且之前已经有输出，就用空格分隔，以免输出多余空格
            if (putSpace && !isNum(prevChar))
                printf(" ");
            // 数字读入结果表达式中
            printf("%c", readChar);
            putSpace = true; // 前面有数字输出的时候才允许填充空格，避免开头就有括号的情况
        }
        else if (!isNum(prevChar) && prevChar != ')' && (readChar == '+' || readChar == '-')) // 上一个读入的是操作符号且不为)，且当前符号是数字前的正负符号
        {
            // case2------------------------------读入的是数字前的符号
            // 比如 1++1就认为是1+(+1)。如果是(1+2)-2的话，这里的)-的-是不能看作负号的
            // 有一种情况肯定要注意，那就是中缀表达式开头就是符号: -1+2
            if (putSpace)
                printf(" ");
            // 只输出负号
            if (readChar == '-')
                printf("%c", readChar);
            putSpace = false; // 先关闭填充空格，因为符号和数字是连为一体的
        }
        else
        {
            // case3------------------------------读入的是运算符
            // 运算符压入运算符栈的情况
            // 1.栈为空
            // 2.新运算符是(
            // 3.优于栈顶运算符, 或栈顶是(
            if (opStack.empty())
            {
                // 栈为空, 直接压入
                opStack.push(readChar);
            }
            else if (readChar == ')')
            {
                // 新运算符是)，弹出栈中元素直至遇到(
                while (!opStack.empty() && opStack.top() != '(')
                {
                    printf(" %c", opStack.top());
                    opStack.pop();
                }
                if (!opStack.empty())
                    opStack.pop(); // 丢弃左括号
            }
            else
            {
                // 如果新运算符不比栈顶运算符优先，且栈顶不是(
                // 就出栈直至新运算符优先于栈顶，或者栈顶是(时
                while (!opStack.empty() && opCompare(opStack.top(), readChar))
                {
                    // 把比较优先的运算符全部出栈
                    printf(" %c", opStack.top());
                    opStack.pop();
                }
                opStack.push(readChar); // 新运算符入栈
            }
        }
        prevChar = readChar;
    }
    // 清空运算符栈
    while (!opStack.empty())
    {
        printf(" %c", opStack.top());
        opStack.pop();
    }
    return 0;
}

// 比较运算符op1是否优于op2
// 注意，当op1=op2时就认为op1>op2，两运算符相同时就弹出之前的运算符
bool opCompare(char op1, char op2)
{
    if ((op1 == '*' || op1 == '/' || op1 == '+' || op1 == '-') && (op2 == '-' || op2 == '+'))
        return true;
    return false; // 这样写巧妙处理了栈顶是左括号(的情况
}

bool isNum(char chr)
{
    return (chr >= '0' && chr <= '9') || chr == '.'; // 小数点也考虑在内
}

/*
    做这题最开始我想到了可能会卡【多位整数】的情况，但我万万没想到他还卡了【小数】和【负数】的情况，岂可修！

    -------------------------------
    关于测试点:
        - 写完中缀转后缀后只能过【0号(sample 6种运算符)】和【4号(只有1个数字)】测试点

        - 注意【测试点1(嵌套括号)】会卡一种【中缀表达式开头有括号】的情况，比如: (2+3*(7-4)+8/4)，要保证【不输出多余空格】

        - 【测试点2(运算数超过1位整数且有非整数出现)】会卡【多位整数】和【小数】的情况

        - 【测试点3(运算数前有正负号)】会卡正负号的处理

            * 注意: 正数的符号不需要输出，【只输出负号】

    本题最难的其实是【保证输出格式的正确】。
    --------------------------------
    关于中缀转后缀: 【从左至右】扫描中缀表达式

        - 扫描到【新符号】时:

            1. 扫描到的新符号是')'
                *弹出栈中所有符号，【输出到后缀表达式中】，直到遇到'('为止，并丢弃左括号(。

            2. 栈为空
                *将符号压入栈

            3. 栈顶为左括号'('
                *将符号压入栈

            4. 栈顶符号优先级高于新符号【且不为左括号'('】
                *不断将栈顶元素出栈，【输出到后缀表达式中】，直至【栈顶元素优先级低于新符号，或者栈顶为'('】

                *注意: 栈顶元素=新符号的时候，认为栈顶元素>新符号，因而弹出栈顶

            观察代码可以发现，我把上面的第3和4步放在一起处理了:
                - 如果栈顶为'(', 循环就会直接终止

                - 否则循环会不停弹出栈顶符号，直至栈顶为'('或者栈顶符号优先级低，此时循环也会终止

            -----------------------------------
            注: 这里在输出到后缀表达式时，【每个符号前面都填了一个空格】
                * 因为后缀表达式开头第一位必不可能是符号，且最后一位一定是符号，不用担心有多余空格输出的问题
            -----------------------------------

        - 扫描到【连续两个符号】时   💡过测试点3

            - 当扫到的两个符号是)-或)+时，一切正常，不额外处理，情况形如(1+2)-3

            - 否则，如果扫到的第二个符号是+或者-，说明扫到了【某个操作数的符号】

                * 此时将这个符号直接【输出到后缀表达式】即可
                * 标记这个符号后面不要输出空格，因为后面是数字

        - 扫描到【数字或者小数点】时     💡过测试点0,4

            - 如果标记要输出空格，且【上一个读到的字符不是数字】，就输出一个空格(以免多余空格输出)

                * 比如 234，这是一个三位数，那么输出的时候就不能输出成2 3 4

            - 当扫到数字或小数点时直接【输出到后缀表达式即可】 💡过测试点2

    💡关于测试点1(卡嵌套括号)的情况，可以参考代码【第28行】。
    ---------------------------------
    关于将中缀表达式转换为前缀表达式，其实操作和这里的思路是比较相似的，只不过有几个地方要反着来
        * 比如中缀转后缀需要【从左至右】扫描中缀表达式，但中缀转前缀就是【从右至左】了。

    详情可以看这篇博客，有个表格总结的很好: https://www.cnblogs.com/unixfy/p/3344550.html

    ---------------------------------

    总的来说这是一道非常坑的题，坑就坑在输出格式这部分。

    不过本题还是很适合复习 中缀表达式 和 后缀表达式 的转换的，这玩意纸上写起来不难，但写成代码就容易掉坑里。

        - SomeBottle 2023.1.14

*/

/*
7-20 表达式转换
    算术表达式有前缀表示法、中缀表示法和后缀表示法等形式。日常使用的算术表达式是采用中缀表示法，即二元运算符位于两个运算数中间。请设计程序将中缀表达式转换为后缀表达式。

输入格式:
    输入在一行中给出不含空格的中缀表达式，可包含+、-、*、/以及左右括号()，表达式不超过20个字符。
输出格式:
    在一行中输出转换后的后缀表达式，要求不同对象（运算数、运算符号）之间以空格分隔，但结尾不得有多余空格。
输入样例:
    2+3*(7-4)+8/4
输出样例:
    2 3 7 4 - * + 8 4 / +
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/