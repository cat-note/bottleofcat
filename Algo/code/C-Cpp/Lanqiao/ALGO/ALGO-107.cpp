#include <iostream>

using namespace std;

typedef struct complexNode complex;
struct complexNode
{
    int real;      // 实部
    int imaginary; // 虚部
    complex *next; // 指向下一个
};

int main()
{
    complex *head = new complex(); // 头部
    complex *tail = head;
    for (int i = 0; i < 10; i++)
    {
        complex *node = new complex();
        cin >> node->real;      // 读入实部
        cin >> node->imaginary; // 读入虚部
        node->next = NULL;
        tail->next = node;
        tail = node; // 移动尾指针
    }
    complex *temp = head->next;
    int realTotal = 0;
    int imaginaryTotal = 0;
    while (temp != NULL)
    {
        // 复数相加，实部、虚部分别相加
        realTotal += temp->real;
        imaginaryTotal += temp->imaginary;
        temp = temp->next;
    }
    cout << realTotal << "+" << imaginaryTotal << "i";
    return 0;
}