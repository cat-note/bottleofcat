#include <iostream>
#include <queue>

using namespace std;

int main()
{
    int N; // N位顾客
    cin >> N;
    queue<int> aQueue; // A窗口排队
    queue<int> bQueue; // B窗口排队
    for (int i = 0; i < N; i++)
    {
        int customer; // 顾客编号
        cin >> customer;
        bool toA = customer & 1; // 此顾客是否由A受理
        // 顾客入队
        if (toA)
            aQueue.push(customer);
        else
            bQueue.push(customer);
    }
    while (true)
    {
        bool output = false; // 标记是否已经输出内容
        // 优先打印A队出队的两个或一个顾客(如果有的话)
        if (!aQueue.empty())
        {
            cout << aQueue.front();
            aQueue.pop();
            if (!aQueue.empty())
            {
                cout << " " << aQueue.front();
                aQueue.pop();
            }
            output = true; // 输出了内容
        }
        // 然后再打印B队出队一个顾客(如果有)
        if (!bQueue.empty())
        {
            if (output) // 如果前面输出了内容就需要垫一个空格
                cout << " ";
            cout << bQueue.front();
            bQueue.pop();
        }
        // 两队皆空就不输出空格，跳出循环
        if (aQueue.empty() && bQueue.empty())
            break;
        else
            cout << " ";
    }
    return 0;
}

/*
    这题主要考察队列的使用，很贴近生活。

    A窗口和B窗口并行工作，单位时间内【A能处理2个顾客，B能处理1位顾客】，设想A窗口前有一条队，B窗口前也有一条队。

        * 同一单位时间内，A队【每出队两人】的，B队能【出队一人】

        * 因为题目要求优先输出A窗口处理的顾客，因此【如果A队有出队顾客】，就【先输出】，随后【再输出B队的出队顾客】(如果有的话)。

    题目还要求输出内容末尾不能有多余空格，因此对于空格输出要严格把控，详见代码。

        - SomeBottle 2023.1.13
*/
/*
7-18 银行业务队列简单模拟
    设某银行有A、B两个业务窗口，且处理业务的速度不一样，其中A窗口处理速度是B窗口的2倍 —— 即当A窗口每处理完2个顾客时，B窗口处理完1个顾客。给定到达银行的顾客序列，请按业务完成的顺序输出顾客序列。假定不考虑顾客先后到达的时间间隔，并且当不同窗口同时处理完2个顾客时，A窗口顾客优先输出。

输入格式:
    输入为一行正整数，其中第1个数字N(≤1000)为顾客总数，后面跟着N位顾客的编号。编号为奇数的顾客需要到A窗口办理业务，为偶数的顾客则去B窗口。数字间以空格分隔。
输出格式:
    按业务处理完成的顺序输出顾客的编号。数字间以空格分隔，但最后一个编号后不能有多余的空格。
输入样例:
    8 2 1 3 9 4 11 13 15
输出样例:
    1 3 2 9 11 4 13 15
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/