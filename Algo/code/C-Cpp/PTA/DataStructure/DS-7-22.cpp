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

    
*/