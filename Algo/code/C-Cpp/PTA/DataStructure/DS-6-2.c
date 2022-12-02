/*
本题要求实现顺序表的操作集。
List MakeEmpty();
Position Find( List L, ElementType X );
bool Insert( List L, ElementType X, Position P );
bool Delete( List L, Position P );

各个操作函数的定义为：
    List MakeEmpty()：创建并返回一个空的线性表；

    Position Find( List L, ElementType X )：返回线性表中X的位置。若找不到则返回ERROR；

    bool Insert( List L, ElementType X, Position P )：将X插入在位置P并返回true。若空间已满，则打印“FULL”并返回false；如果参数P指向非法位置，则打印“ILLEGAL POSITION”并返回false；

    bool Delete( List L, Position P )：将位置P的元素删除并返回true。若参数P指向非法位置，则打印“POSITION P EMPTY”（其中P是参数值）并返回false。
输入样例
    6
    1 2 3 4 5 6
    3
    6 5 1
    2
    -1 6
输出样例
    FULL Insertion Error: 6 is not in.
    Finding Error: 6 is not in.
    5 is at position 0.
    1 is at position 4.
    POSITION -1 EMPTY Deletion Error.
    FULL Insertion Error: 0 is not in.
    POSITION 6 EMPTY Deletion Error.
    FULL Insertion Error: 0 is not in.
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/
#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 5
#define ERROR -1
typedef enum
{
    false,
    true
} bool;
typedef int ElementType;
typedef int Position;
typedef struct LNode *List;
struct LNode
{
    ElementType Data[MAXSIZE];
    Position Last; /* 保存线性表中最后一个元素的位置 */
};

List MakeEmpty();
Position Find(List L, ElementType X);
bool Insert(List L, ElementType X, Position P);
bool Delete(List L, Position P);

int main()
{
    List L;
    ElementType X;
    Position P;
    int N;

    L = MakeEmpty();
    scanf("%d", &N);
    while (N--)
    {
        scanf("%d", &X);
        if (Insert(L, X, 0) == false)
            printf(" Insertion Error: %d is not in.\n", X);
    }
    scanf("%d", &N);
    while (N--)
    {
        scanf("%d", &X);
        P = Find(L, X);
        if (P == ERROR)
            printf("Finding Error: %d is not in.\n", X);
        else
            printf("%d is at position %d.\n", X, P);
    }
    scanf("%d", &N);
    while (N--)
    {
        scanf("%d", &P);
        if (Delete(L, P) == false)
            printf(" Deletion Error.\n");
        if (Insert(L, 0, P) == false)
            printf(" Insertion Error: 0 is not in.\n");
    }
    return 0;
}

//---------------题解代码如下

List MakeEmpty()
{
    List newList = (List)malloc(sizeof(struct LNode));
    newList->Last = -1; // 当前一个元素都没有
    return newList;
}

Position Find(List L, ElementType X)
{
    if (L->Last != -1)
    { // 顺序表有元素
        for (int i = 0; i <= L->Last; i++)
        {
            if (L->Data[i] == X)
                return i;
        }
    }
    return ERROR;
}

bool Insert(List L, ElementType X, Position P)
{
    if (L->Last + 1 >= MAXSIZE)
    { // 顺序表已满
        printf("FULL");
        return false;
    }
    if (P < 0 || P > L->Last + 1)
    { // 位置P得要有效才行
        printf("ILLEGAL POSITION");
        return false;
    }
    // 把从位置P开始的元素全后移一位
    for (int i = L->Last; i >= P; i--)
        L->Data[i + 1] = L->Data[i];
    // 不要忘记了将Last+1
    L->Last++;
    L->Data[P] = X; // 插入新元素
    return true;
}

bool Delete(List L, Position P)
{
    if (P < 0 || P > L->Last)
    { // 位置P必须要有效且有元素
        printf("POSITION %d EMPTY", P);
        return false;
    }
    // 直接把P后面所有元素前移一个单位，把P位置原来的元素覆盖即可
    for (int i = P; i < L->Last; i++)
        L->Data[i] = L->Data[i + 1];
    // 别忘了Last-1
    L->Last--;
    return true;
}

/*
    这题主要考察的是顺序表的四个基本操作，包括创建、查找、插入、删除元素。
    在插入/删除时需要注意：
        - 操作位置P是否合法
        - 不要忘了改变指向最后元素下标的变量Last
        - 插入时不要忘了判断顺序表是否已满
        - 别忘了移动元素，顺序表中的元素在内存中是顺序储存的。

      - SomeBottle 2022.12.2
*/