#include <iostream>
#include <stack>

using namespace std;

typedef struct PlatePile *Plates;

// 用结构体表示一摞碟子的状态
struct PlatePile
{
    int num;     // 碟子数量
    char start;  // 这堆碟子现在放在哪个柱子上(起始柱)
    char buffer; // 中转柱
    char end;    // 目标柱
};

// 汉诺塔是经典递归问题，要用非递归实现就得理解原理
int main()
{
    stack<Plates> process; // 用栈来辅助处理移动盘子的过程
    // 初始的一堆盘子
    Plates initPlates = new PlatePile();
    cin >> initPlates->num;   // 读入初始盘数
    initPlates->start = 'a';  // 这堆盘子最开始放在a柱
    initPlates->buffer = 'b'; // 借助b中转
    initPlates->end = 'c';    // 目标柱c
    process.push(initPlates); // 压入栈中，初始化
    while (!process.empty())
    {
        Plates currPlates = process.top(); // 当前处理的一摞盘子(栈顶)
        process.pop();
        if (currPlates->num == 1)
        {
            // 整个问题最后会被拆解成多个这样的小问题
            // 某起始柱只有一个盘子了，直接移动到其目标柱
            cout << currPlates->start << " -> " << currPlates->end << "\n";
        }
        else
        {
            // 这一摞盘子有N>1个盘子
            Plates startToBuf = new PlatePile();
            Plates startToEnd = new PlatePile();
            Plates bufToEnd = new PlatePile(); // 把N-1个盘子从中转柱移动到目标柱
            // 把上面N-1个盘子从开始柱start移动到中转柱buffer
            startToBuf->num = currPlates->num - 1; // N-1
            startToBuf->start = currPlates->start; // N-1盘子的起始柱
            startToBuf->buffer = currPlates->end;  // N-1盘子的中转柱
            startToBuf->end = currPlates->buffer;  // N-1盘子的目标柱
            // 把最底下的(第N个)盘子从开始柱start移动到目标柱end
            startToEnd->num = 1;
            startToEnd->start = currPlates->start; // 最底下盘子的起始柱
            startToEnd->buffer = currPlates->buffer;
            startToEnd->end = currPlates->end; // 最底下盘子的目标柱
            // 把在中转柱buffer的N-1个盘子移动到目标柱end
            bufToEnd->num = currPlates->num - 1;  // N-1
            bufToEnd->start = currPlates->buffer; // N-1盘子的起始柱
            bufToEnd->buffer = currPlates->start; // N-1盘子的中转柱子
            bufToEnd->end = currPlates->end;      // N-1盘子的目标柱
            // 注意这里用的是栈，三个过程要【倒过来】压入栈
            process.push(bufToEnd);   // 第三步
            process.push(startToEnd); // 第二步
            process.push(startToBuf); // 第一步
        }
        delete currPlates; // 用完就释放内存
    }
    return 0;
}

/*
    汉诺塔问题是经典的递归问题，关于递归:

        * 我认为，递归的过程其实就是将大问题【逐层分解】成【多个小问题】，然后从最深的一层小问题开始解决。每层的小问题解决完后就回溯到上层，直到最浅的一层，最终“组装”成大问题的结果。

            - 逐层分解肯定是有个终点的，【递归的终点】就是【最小子问题】的情况，最小子问题的结果是已知的。

            - 可以发现，递归的思想可以结合【树结构】来理解。

        * 设计递归的思想其实就是将【解决大问题的过程】归纳成几个子问题，且【我们要坚信这几个子问题都能被解决】，【不要去深究这几个子问题】。

            - 因为子问题是靠函数调用自身来进行解决的。函数不断调用自身，层数增加直至递归终止条件(分解成最小子问题)为止。只要过程没写错，子问题肯定是能得出结果的，因此设计递归时我们不需要让大脑里也开始“压栈”。

    ----------------------
    【本题的汉诺塔问题】并不需要结果，只需要输出解决最小子问题的操作即可。

        * 汉诺塔的最小子问题就是将【某个柱子上有且仅有的最后一个盘子】移动到【目标柱】。

    用递归解决汉诺塔问题的核心思路就是:

        1. 【把起始柱的盘子分为两部分】
            假设起始柱上共有N个盘子，可以分成两部分：【上面的N-1个盘子】，【最底下的1个盘子】

        2. 针对这两部分设计递归的子问题:

            1. 将【上面的N-1个盘子】移动到【中转柱】

                * 只有这样，最底下的盘子才能移动到【目标柱】

            2. 将【最底下的盘子】移动到【目标柱】

                * 这是一个最小子问题，它的解我们已经知道了

            3. 将在中转柱上的【N-1个盘子】移动到【目标柱】

                * 至此N个盘子全从起始柱移动到了目标柱

            这样就设计出了递归体，涉及到三个子问题。
            N-1个盘子是怎么移动到中转柱的？N-1个盘子又是怎么从中转柱移动到目标柱的？——这些问题我们无需深究，【坚信它们都能被解决】就好了。

        3. 最小子问题就是【起始柱上的盘子只剩1个】，操作就是将这个盘子移动到目标柱。

    ------------------------
    说回来，本题要求用非递归方式实现，其实也不难。将递归转换为非递归的时候，往往会使用【栈】来进行辅助。

    观察递归函数可以发现每一层的问题中我们需要知道当前这个子问题的状态:

        1. 当前这一摞盘子的起始柱
        2. 当前这一摞盘子的中转柱
        3. 当前这一摞盘子的目标柱
        4. 当前这一摞盘子的盘子数目

    用非递归来实现的话就需要【保存每一摞盘子的状态】，因此我采用了一个结构体PlatePile来【保存这4个状态字段】。

    和递归体一样，我仍然把汉诺塔的每个问题分成三个子问题。

        * 注：这里的每一个子问题都对应一摞盘子的状态，用结构体PlatePile储存

    但在这里不同的是，实际上程序在利用栈结构【将汉诺塔问题不断划分为多个最小子问题并压入栈】。当栈中【全都是最小子问题后】，依次弹栈输出最小子问题的操作，就是这题的结果。

    注意，栈结构是先进后出的，因此要让三个子问题按序被处理，在压入栈时必须要【将三个子问题倒序压入栈】。

        - SomeBottle 2023.1.12
*/
/*
7-17 汉诺塔的非递归实现
    借助堆栈以非递归（循环）方式求解汉诺塔的问题（n, a, b, c），即将N个盘子从起始柱（标记为“a”）通过借助柱（标记为“b”）移动到目标柱（标记为“c”），并保证每个移动符合汉诺塔问题的要求。

输入格式:
    输入为一个正整数N，即起始柱上的盘数。
输出格式:
    每个操作（移动）占一行，按柱1 -> 柱2的格式输出。
输入样例:
    3
输出样例:
    a -> c
    a -> b
    c -> b
    a -> c
    b -> a
    b -> c
    a -> c
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/