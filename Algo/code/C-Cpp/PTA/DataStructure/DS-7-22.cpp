#include <iostream>

using namespace std;

template <typename T>
class Stack
{
private:
    int maxSize; // 数组最大容量
    int topPos;  // 栈顶指针, 指向栈顶元素的下一位
    T *arr;      // 栈的底层数组
public:
    Stack()
    {
        topPos = 0;
        arr = NULL;
    }
    ~Stack()
    {
        delete[] arr; // 释放内存
    }
    int size()
    {
        return topPos;
    }
    void setSize(int size)
    {
        if (arr)
            return;
        arr = new T[size]; // 初始化数组
        maxSize = size;
    }
    // 栈是否已满
    bool full()
    {
        return topPos == maxSize;
    }
    // 栈是否为空
    bool empty()
    {
        return topPos == 0;
    }
    // 压元素入栈(无操作检查)
    void push(T elem)
    {
        arr[topPos++] = elem;
    }
    // 弹元素出栈(无操作检查)
    T pop()
    {
        return arr[--topPos];
    }
};

template <typename T>
class StackQueue
{
private:
    Stack<T> enqueue; // 入队栈
    Stack<T> dequeue; // 出队栈
public:
    StackQueue(int size1, int size2)
    {
        // 较小容量的栈决定了队列的最大容量
        // 较小容量的栈作为入队栈，较大的作为出队栈
        // 出队栈容量>入队栈容量
        if (size1 < size2)
        {
            enqueue.setSize(size1); // 初始化栈大小
            dequeue.setSize(size2);
        }
        else
        {
            enqueue.setSize(size2); // 初始化栈大小
            dequeue.setSize(size1);
        }
    }
    // 加入队尾(返回是否成功)
    bool push(T elem)
    {
        if (enqueue.full())
        {
            // 如果入队栈满了
            if (dequeue.empty())
            {
                // 此时如果出队栈是空的，把入队栈的元素弹出压入出队栈
                while (!enqueue.empty())
                    dequeue.push(enqueue.pop());
            }
            else
            {
                return false; // 否则队满了
            }
        }
        // 入队栈有了空间，将新元素压入栈
        enqueue.push(elem);
        return true;
    }
    // 返回是否队空
    bool empty()
    {
        // 如果入队栈和出队栈均为空，则队空
        return enqueue.empty() && dequeue.empty();
    }
    // 元素出队
    T pop()
    {
        T returnVal;
        if (!dequeue.empty())
        {
            // -----如果出队栈不为空，就弹出出队栈的元素
            returnVal = dequeue.pop();
        }
        else if (!enqueue.empty())
        {
            // -----如果出队栈为空，但入队栈不为空
            // 将入队栈的元素弹出压入出队栈
            while (!enqueue.empty())
                dequeue.push(enqueue.pop());
            returnVal = dequeue.pop();
            // 如果有剩余的元素就从出队栈倒回入队栈
            while (!dequeue.empty())
                enqueue.push(dequeue.pop());
        }
        return returnVal;
    }
};

int main()
{
    int N1, N2;
    cin >> N1 >> N2;
    StackQueue<int> myQueue(N1, N2); // 初始化队列
    while (true)
    {
        char operation; // 操作
        cin >> operation;
        if (operation == 'A')
        {
            // 数字入队
            int num;
            cin >> num;
            if (!myQueue.push(num))
            {
                // 入队失败
                cout << "ERROR:Full\n";
            }
        }
        else if (operation == 'D')
        {
            // 出队
            if (myQueue.empty())
            {
                // 队已经空了，出队失败
                cout << "ERROR:Empty\n";
                continue;
            }
            cout << myQueue.pop() << "\n";
        }
        else
        {
            break; // 读取完毕
        }
    }
    return 0;
}

/*
    栈是只能在【一端进行元素压入/弹出操作】的逻辑结构，而队列则是只能【在一端进行加入元素，另一端进行弹出元素操作】的逻辑结构。

    要用栈实现队列，就需要把两个栈的元素不断【倒来倒去】。可以把这两个栈看作两个杯子，本题中就是把杯子中的水倒来倒去(可能不太贴切23333)。
    ----------------------------
    在开始动手写代码前，先观察题目给出的测试样例:
        [样例输入]
            3 2
            A 1 A 2 A 3 A 4 A 5 D A 6 D A 7 D A 8 D D D D T
        [输出如下]
            ERROR:Full
            1
            ERROR:Full
            2
            3
            4
            7
            8
            ERROR:Empty

        - 首先，输入第一行规定了两个栈的容量

        - 从输入第二行可以发现，要入队的元素数量是【多于任何一个栈的容量】的

        - 从输出可以看到，直到【A 5】这个指令才队满，也就是说【队中此时最多能存放4个元素】

        - 但是在A 5后扫描到D，弹出了一个元素，而后面【执行A 6】时却还是队满

        - 在A 6后再弹出一个元素(D)，后面的【A 7 D A 8】就都正常了

    * 一般来说，用栈实现队列我会用到两个栈，一个栈称为【入队栈】，而另一个栈称作【出队栈】。

        - 入队的时候，元素【压入】入队栈
        - 出队的时候，将【入队栈元素全部弹出，压入出队栈】，然后弹出【出队栈的栈顶】，然后再把剩余的元素弹出，【压回】入队栈。

    但这题显然不是这么简单！！
    琢磨一会儿可以发现，本题两个栈容量不同，💡为了【出队时所有元素都能进入出队栈】，出队栈的容量必须要 >= 入队栈的容量。

    那么入队栈的容量就是整个队列的容量吗？当然不是！上面的样例中前面加入了4个元素都没事呢！

    ——于是我的思路就转向了【出队栈】，有没有可能在入队的时候【利用一下出队栈】呢？
    ------------------------------------------
    💡根据样例我总结如下:

    【入队操作】:

        1. 如果【入队栈】没有满，就直接【将新元素压入】入队栈。

        2. 如果【入队栈】满了，看【出队栈】是不是空的:

            - 如果出队栈是【空的】，将【入队栈元素弹出，压入出队栈】，然后再把新元素压入【入队栈】。

            - 如果出队栈【不是空的】，说明【队满了】，入队失败！

                * 一旦出队栈中有元素，入队栈的元素就不能再进入了，不然【会影响出队顺序】！

                * 这也是为什么上面样例中D弹出一个元素后，执行A 6却发现队还是满的。

    【出队操作】:

        1. 如果【出队栈】不是空的，直接将【出队栈栈顶】弹出。

        2. 如果【出队栈】是空的，但【入队栈不是空的】:

            - 把入队栈所有元素依次弹出，并压入【出队栈】

            - 弹出此时的【出队栈栈顶】

            - 把出队栈【剩余的元素依次弹出，压回入队栈中】。

    ---------------------------------------------------
    可以发现，这题中，【较小的栈】的容量决定了【队列容量的上限】。

        * 比如本题样例给的最小栈容量是2，那么队列容量的【上限是 2 × 2 = 4 个元素】。

    - SomeBottle 2023.1.16
*/
/*
7-22 堆栈模拟队列
    设已知有两个堆栈S1和S2，请用这两个堆栈模拟出一个队列Q。
    所谓用堆栈模拟队列，实际上就是通过调用堆栈的下列操作函数:
        int IsFull(Stack S)：判断堆栈S是否已满，返回1或0；
        int IsEmpty (Stack S )：判断堆栈S是否为空，返回1或0；
        void Push(Stack S, ElementType item )：将元素item压入堆栈S；
        ElementType Pop(Stack S )：删除并返回S的栈顶元素。
    实现队列的操作，即入队void AddQ(ElementType item)和出队ElementType DeleteQ()。
输入格式:
    输入首先给出两个正整数N1和N2，表示堆栈S1和S2的最大容量。随后给出一系列的队列操作：A item表示将item入列（这里假设item为整型数字）；D表示出队操作；T表示输入结束。
输出格式:
    对输入中的每个D操作，输出相应出队的数字，或者错误信息ERROR:Empty。如果入队操作无法执行，也需要输出ERROR:Full。每个输出占1行。
输入样例:
    3 2
    A 1 A 2 A 3 A 4 A 5 D A 6 D A 7 D A 8 D D D D T
输出样例:
    ERROR:Full
    1
    ERROR:Full
    2
    3
    4
    7
    8
    ERROR:Empty
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/