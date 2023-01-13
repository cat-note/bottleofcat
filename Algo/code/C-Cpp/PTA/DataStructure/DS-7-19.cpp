#include <cstdio>
#include <queue>

using namespace std;

int main()
{
    int K;              // 倒数第K项
    int targetNum = -1; // 在目标位置上的数字
    int readNum = 0;    // 读入的数字数
    queue<int> window;  // 滑动窗口，窗口内是倒数的K项
    scanf(" %d", &K);
    while (true)
    {
        int read; // 读入的数字
        scanf(" %d", &read);
        // 注意，题目说的是“以一个负整数表示结尾”，而不是-1
        if (read < 0)
            break;
        window.push(read); // 入队
        // 读入的数字数量起码要>=K才有倒数第K项
        if (++readNum >= K)
        {
            // 此时队头就是倒数第K项
            targetNum = window.front();
            window.pop();
        }
    }
    if (targetNum == -1) // 不存在就输出NULL
        printf("NULL");
    else
        printf("%d", targetNum);
    return 0;
}

/*
    这题用链式线性表写的话，解题方法就是利用头插法将元素一个个插入到链表，然后访问第K项即可。
    ---------------------
    仔细想一想，这题其实有比链表省心很多的解法——用队列。

    想知道倒数第K项是什么数字:

        0. 初始化一个队列window，我称其为滑动窗口。程序【每读入一个数字都加入到其队尾】。

        1. 要读到第K项，首先【程序读入的数字数量就要至少有K个】

        2. 当程序【读入的数字数量 >=K】时，每读取一个数字，就就弹出队首，【此时的队首就是倒数第K个数字】

            * 可以发现此时队列window中储存的数字一直是【倒数的K个数字】

    利用队列，只要将所有数字读完就可以得出结果。

        - SomeBottle 2023.1.13
*/

/*
7-19 求链式线性表的倒数第K项
    给定一系列正整数，请设计一个尽可能高效的算法，查找倒数第K个位置上的数字。

输入格式:
    输入首先给出一个正整数K，随后是若干非负整数，最后以一个负整数表示结尾（该负数不算在序列内，不要处理）。
输出格式:
    输出倒数第K个位置上的数据。如果这个位置不存在，输出错误信息NULL。
输入样例:
    4 1 2 3 4 5 6 7 8 9 0 -1
输出样例:
    7
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/