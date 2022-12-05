/*
本题要求在一个数组中实现两个堆栈，需要实现的方法如下：
    Stack CreateStack( int MaxSize );
    bool Push( Stack S, ElementType X, int Tag );
    ElementType Pop( Stack S, int Tag );
其中Tag是堆栈编号，取1或2；MaxSize堆栈数组的规模。

注意：如果堆栈已满，Push函数必须输出“Stack Full”并且返回false；如果某堆栈是空的，则Pop函数必须输出“Stack Tag Empty”（其中Tag是该堆栈的编号），并且返回ERROR。

输入样例：
    5
    Push 1 1
    Pop 2
    Push 2 11
    Push 1 2
    Push 2 12
    Pop 1
    Push 2 13
    Push 2 14
    Push 1 3
    Pop 2
    End
输出样例：
    Stack 2 Empty
    Stack 2 is Empty!
    Stack Full
    Stack 1 is Full!
    Pop from Stack 1: 1
    Pop from Stack 2: 13 12 11
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR 1e8
typedef int ElementType;
typedef enum
{
    push,
    pop,
    end
} Operation;
typedef enum
{
    false,
    true
} bool;
typedef int Position;
struct SNode
{
    ElementType *Data;
    Position Top1, Top2;
    int MaxSize;
};
typedef struct SNode *Stack;

Stack CreateStack(int MaxSize);
bool Push(Stack S, ElementType X, int Tag);
ElementType Pop(Stack S, int Tag);

Operation GetOp();                 /* details omitted */
void PrintStack(Stack S, int Tag); /* details omitted */

Operation GetOp()
{
    char Push[] = "Push";
    char Pop[] = "Pop";
    char End[] = "End";
    char s[100];
    scanf("%s", s);

    if (strcmp(Push, s) == 0)
        return push;
    if (strcmp(Pop, s) == 0)
        return pop;
    if (strcmp(End, s) == 0)
        return end;
}

int main()
{
    int N, Tag, X;
    Stack S;
    int done = 0;

    scanf("%d", &N);
    S = CreateStack(N);
    while (!done)
    {
        switch (GetOp())
        {
        case push:
            scanf("%d %d", &Tag, &X);
            if (!Push(S, X, Tag))
                printf("Stack %d is Full!\n", Tag);
            break;
        case pop:
            scanf("%d", &Tag);
            X = Pop(S, Tag);
            if (X == ERROR)
                printf("Stack %d is Empty!\n", Tag);
            break;
        case end:
            PrintStack(S, 1);
            PrintStack(S, 2);
            done = 1;
            break;
        }
    }
    return 0;
}

// 以下是题解代码

Stack CreateStack(int MaxSize)
{
    Stack myStack = (Stack)malloc(sizeof(struct SNode));
    // 动态分配数组元素
    myStack->Data = (ElementType *)malloc(sizeof(ElementType) * MaxSize);
    myStack->Top1 = -1;      // 栈1顶下标(指向栈顶元素的下一位)
    myStack->Top2 = MaxSize; // 栈2顶下标(指向栈顶元素下一位)
    myStack->MaxSize = MaxSize;
    return myStack;
}

bool Push(Stack S, ElementType X, int Tag)
{
    if (S->Top1 + 1 == S->Top2)
    { // 栈满
        printf("Stack Full\n");
        return false;
    }
    if (Tag == 1)
    { // 压入栈1
        S->Data[++S->Top1] = X;
    }
    else if (Tag == 2)
    { // 压入栈2
        S->Data[--S->Top2] = X;
    }
    return true;
}

ElementType Pop(Stack S, int Tag)
{
    if (Tag == 1 && S->Top1 > -1) // 栈1不为空
        return S->Data[S->Top1--];
    else if (Tag == 2 && S->Top2 < S->MaxSize) // 栈2不为空
        return S->Data[S->Top2++];
    printf("Stack %d Empty\n", Tag);
    return ERROR;
}

void PrintStack(Stack S, int Tag)
{
    printf("Pop from Stack %d:", Tag);
    if (Tag == 1)
    {
        while (S->Top1 != -1)
        {
            printf(" %d", S->Data[S->Top1--]);
        }
    }
    else
    {
        while (S->Top2 != S->MaxSize)
        {
            printf(" %d", S->Data[S->Top2++]);
        }
    }
    putchar('\n');
}

/*
    吐槽：我最开始写的是让top指向栈顶元素下标的后一个下标，结果PTA这小子他不认，非得我把top指向栈顶元素下标
        来看看PTA的PrintStack是怎么写的：
        void PrintStack(Stack S, int Tag)
        {
            printf("Pop from Stack %d:", Tag);
            if (Tag == 1){
                while (S->Top1 != -1){
                    printf(" %d", S->Data[S->Top1--]);
                }
            }
            else {
                while (S->Top2 != S->MaxSize){
                    printf(" %d", S->Data[S->Top2++]);
                }
            }
            putchar('\n');
        }
        太不灵活了，实际上我们直接把栈中剩余元素直接全Pop出来不就行了，哥们儿你怎么想的啊！
*/

/*
Stack CreateStack(int MaxSize)
{
    Stack myStack = (Stack)malloc(sizeof(struct SNode));
    // 动态分配数组元素
    myStack->Data = (ElementType *)malloc(sizeof(ElementType) * MaxSize);
    myStack->Top1 = -1;      // 栈1顶下标(指向栈顶元素的下一位)
    myStack->Top2 = MaxSize; // 栈2顶下标(指向栈顶元素下一位)
    myStack->MaxSize = MaxSize;
    return myStack;
}

bool Push(Stack S, ElementType X, int Tag)
{
    if (S->Top1 + 1 == S->Top2)
    { // 栈满
        printf("Stack Full\n");
        return false;
    }
    if (Tag == 1)
    { // 压入栈1
        S->Data[++S->Top1] = X;
    }
    else if (Tag == 2)
    { // 压入栈2
        S->Data[--S->Top2] = X;
    }
    return true;
}

ElementType Pop(Stack S, int Tag)
{
    if (Tag == 1 && S->Top1 > -1) // 栈1不为空
        return S->Data[S->Top1--];
    else if (Tag == 2 && S->Top2 < S->MaxSize) // 栈2不为空
        return S->Data[S->Top2++];
    printf("Stack %d Empty\n", Tag);
    return ERROR;
}
void PrintStack(Stack S, int Tag)
{
    printf("Pop from Stack %d:", Tag);
    if (Tag == 1){
        while (S->Top1 != -1){
            printf(" %d", S->Data[S->Top1--]);
        }
    }
    else {
        while (S->Top2 != S->MaxSize){
            printf(" %d", S->Data[S->Top2++]);
        }
    }
    putchar('\n');
}
*/
/*
我最开始的题解代码
这里让Pop在栈内没元素的时候不打印信息，然后让PrintStack直接调用Pop打印出栈。

Stack CreateStack(int MaxSize)
{
    Stack myStack = (Stack)malloc(sizeof(struct SNode));
    // 动态分配数组元素
    myStack->Data = (ElementType *)malloc(sizeof(ElementType) * MaxSize);
    myStack->Top1 = 0;           // 栈1顶下标(指向栈顶元素的下一位)
    myStack->Top2 = MaxSize - 1; // 栈2顶下标(指向栈顶元素下一位)
    myStack->MaxSize = MaxSize;
    return myStack;
}

bool Push(Stack S, ElementType X, int Tag)
{
    if (S->Top1 > S->Top2)
    { // 栈满
        printf("Stack Full\n");
        return false;
    }
    if (Tag == 1)
    { // 压入栈1
        S->Data[S->Top1++] = X;
    }
    else if (Tag == 2)
    { // 压入栈2
        S->Data[S->Top2--] = X;
    }
    return true;
}

ElementType Pop(Stack S, int Tag)
{
    if (Tag == 1 && S->Top1 > 0) // 栈1不为空
        return S->Data[--S->Top1];
    else if (Tag == 2 && S->Top2 < S->MaxSize - 1) // 栈2不为空
        return S->Data[++S->Top2];
    return ERROR;
}

void PrintStack(Stack S, int Tag)
{
    printf("Pop from Stack %d:", Tag);
    ElementType elem;
    while ((elem = Pop(S, Tag)) != ERROR)
    {
        printf(" %d", elem);
    }
    printf("\n");
}
*/