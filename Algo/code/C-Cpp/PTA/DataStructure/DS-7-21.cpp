#include <cstdio>
#include <stack>

using namespace std;

// 逆转字符串
void reverseStr(char *str, int strLen);

int main()
{
    stack<float> numStack; // 操作数栈
    char expression[31];   // 前缀表达式
    int expLen = 0;        // 表达式长度
    // 因为求前缀表达式要倒着读，这里先把表达式全读进来
    while (scanf("%c", &expression[expLen++]) != EOF)
        if (expression[expLen - 1] == '\n')
        {
            expression[expLen - 1] = '\0'; // 忽略换行符
            break;
        };
    expLen--; // 修正表达式字符串长度
    // 倒着读表达式，计算前缀值
    char tempStr[31];       // 临时字符串
    short tempLen = 0;      // 临时字符串长度
    char prevChr = ' ';     // 前一个读取到的字符
    bool errorFlag = false; // 是否发生错误
    for (int i = expLen - 1; i >= -1; i--)
    {
        char readChr = expression[i];
        // 如果遇到了一个空格，或者读到了表达式末尾，说明一个数据读取完毕，接下来判断这个数据是操作数还是运算符
        if (i == -1 || readChr == ' ')
        {
            if ((prevChr >= '0' && prevChr <= '9') || tempLen >= 2)
            {
                // ---------------------------------------1. 如果前一个读入的字符是数字，说明这个数据是数字
                // 注意，读入的数字都是反的，需要进行逆转
                reverseStr(tempStr, tempLen); // 逆转临时字符串，得到正常的数字，比如411-逆转后就是-114
                float num;
                char buf[20]; // 临时字符串
                int scanNum;  // 匹配的数量
                // 将数字读入为浮点数
                // 1. 如果sscanf返回0说明浮点数解析失败，数字非法
                // 2. 如果sscanf匹配的数量>1，说明数字后面还有非法字符
                // 3. 如果数字前有多余的符号，比如--110，sscanf是无法识别出浮点数的，数字非法
                if (!(scanNum = sscanf(tempStr, "%f%[^\n]", &num, buf)) || scanNum > 1)
                {
                    errorFlag = true;
                    break;
                }
                numStack.push(num); // 将数字加入队尾
            }
            else
            {
                // ---------------------------------------2. 否则，前一个读入的是符号
                // 符号只有一位，直接访问0号字符即可
                char op = tempStr[0];
                float num1, num2; // 两个操作数
                // 栈中操作数<2，说明操作数不够用，错误
                if (numStack.size() < 2)
                {
                    errorFlag = true;
                    break;
                }
                num1 = numStack.top();
                numStack.pop();
                num2 = numStack.top();
                numStack.pop();
                bool opSuccess = true; // 运算是否成功
                // 进行运算
                switch (op)
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
                    num1 /= num2;
                    // 除数为0，计算失败
                    if (num2 == 0)
                        opSuccess = false;
                    break;
                default:
                    opSuccess = false;
                }
                if (!opSuccess)
                {
                    errorFlag = true;
                    break;
                }
                // 运算之后将结果num1重新压入栈中
                numStack.push(num1);
            }
            // 读取完毕后清空临时字符串
            tempStr[0] = '\0';
            tempLen = 0;
            if (readChr == ' ')
                continue;
            else
                break; // i=-1时说明读取结束，循环终止
        }
        // 将读入的字符放入临时字符串
        tempStr[tempLen++] = readChr;
        tempStr[tempLen] = '\0'; // 保证字符串终止的位置没错
        prevChr = readChr;
    }
    // 到最后栈中只会剩一个结果，否则就出现了错误
    if (numStack.size() != 1)
        errorFlag = true;
    if (errorFlag)
    {
        printf("ERROR");
    }
    else
    {
        printf("%.1f\n", numStack.top());
    }
    return 0;
}

// 逆转字符串的算法
void reverseStr(char *str, int strLen)
{
    int front = 0;
    int rear = strLen - 1;
    // 双指针逆转字符串
    while (front < rear)
    {
        char temp = str[front];
        str[front] = str[rear];
        str[rear] = temp;
        front++;
        rear--;
    }
}

/*
    这题可以说也是很“坑”，坑在判断表达式计算错误的情况上。
    ---------------------
    计算前缀表达式的结果的步骤很简单:

        【从右至左】扫描表达式:
            1. 扫描到了数字，将其压入【操作数栈】。

            2. 扫描到了符号，就从【操作数栈】弹出【两个数字】，以进行运算。运算后的结果【重新压入栈】。
                (本题涉及到的均为二元运算符)

        重复执行上面两步直至扫描完整个表达式。最后【操作数栈】中【只会有一个数值】作为结果。
    ---------------------
    对于【从右至左】扫描表达式，我这里的做法是:

        1. 将前缀表达式逐字符读入【字符数组】

        2. 倒着【逐字符】读取字符数组

            - 初始化一个字符串tempStr

            - 每读到一个字符，就将其加入tempStr

            - 每遇到空格，或者读到结尾，就:

                把tempStr拿出来，判断tempStr是数字还是运算符:

                    - 如果是数字，将tempStr逆转一下(因为我是倒着读的，要逆转才能得到正确的数字！)，然后压入栈

                    - 如果是运算符，就进行相应运算

    这里我真的是写复杂了，读入的时候其实可以不用逐字符读。
    无论是cin还是scanf，在读取字符串时【都是按空格分隔】的，而题目给的前缀表达式的各个部分正好是空格分隔，因此只需要创建一个【字符串栈】stack<string>来储存前缀表达式的各部分，就可以更方便地进行【从右至左】的读取了。
    -----------------------
    本题的坑点: 输出ERROR的情况

        当前缀表达式无法被计算时，题目要求输出ERROR。
        当题目不保证前缀表达式的正确性时，这难度噌噌的就往上涨了。

    这里我把能考虑到的坑点全考虑进去了:

        1. 数字后方有非法符号，例如 + 1 3- , 这里3后面多了个非法的符号；又例如 + 1+1 3，这里1后面还有个非法的+1

        2. 数字前面有非法符号，例如 + +-1 3 , 这里1前面有两个符号

        3. 符号不止一个字符，例如 +* 1 3，本题涉及的运算符全都是只占一个字符的，这里的+*显然错误

        4. 扫描到运算符，进行运算时发现【操作数不够】(栈中操作数<2)，无法完成计算。例如 + 3

        5. 运算除法时【除数为0】，例如 / 3 0 。
            ⚠💡注意！！！！这是非常容易忽略的点，题目的【测试点2】考察的就是这个!

        6. 运算符不存在

        7. 运算数多余。所有运算符对应的运算执行完后，操作数栈中理应【只有1个结果】。但凡不为1，说明运算数多余。
    -------------------------------------
    后缀表达式的运算和这里前缀表达式的运算是真的【非常相似】了，唯一的不同点就是：

        💡 后缀表达式运算时是【从左至右】扫描的。

    --------------------------------------

        - SomeBottle 2023.1.15
*/

/*
7-21 求前缀表达式的值
    算术表达式有前缀表示法、中缀表示法和后缀表示法等形式。前缀表达式指二元运算符位于两个运算数之前，例如2+3*(7-4)+8/4的前缀表达式是：+ + 2 * 3 - 7 4 / 8 4。请设计程序计算前缀表达式的结果值。

输入格式:
    输入在一行内给出不超过30个字符的前缀表达式，只包含+、-、*、/以及运算数，不同对象（运算数、运算符号）之间以空格分隔。
输出格式:
    输出前缀表达式的运算结果，保留小数点后1位，或错误信息ERROR。
输入样例:
    + + 2 * 3 - 7 4 / 8 4
输出样例:
    13.0
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/