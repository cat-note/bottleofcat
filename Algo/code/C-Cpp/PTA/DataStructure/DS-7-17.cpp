#include <iostream>
#include <stack>

using namespace std;

typedef struct PlatePile *Plates;

// 用结构体表示一堆碟子
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
    汉诺塔问题是经典的递归问题
*/