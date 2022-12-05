/*
本题要求实现二分查找算法，待实现方法如下：
    Position BinarySearch( List L, ElementType X );
L是用户传入的一个线性表，其中ElementType元素可以通过>、==、<进行比较，并且题目保证传入的数据是递增有序的。函数BinarySearch要查找X在Data中的位置，即数组下标（注意：元素从下标1开始存储）。找到则返回下标，否则返回一个特殊的失败标记NotFound。
输入样例1：
    5
    12 31 55 89 101
    31
输出样例1：
    2
输入样例2：
    3
    26 78 233
    31
输出样例2：
    0
代码长度限制
    16 KB
时间限制
    100 ms
内存限制
    64 MB
*/
#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 10
#define NotFound 0
typedef int ElementType;

typedef int Position;
typedef struct LNode *List;
struct LNode
{
    ElementType Data[MAXSIZE];
    Position Last; /* 保存线性表中最后一个元素的位置 */
};

List ReadInput(); /* 裁判实现，细节不表。元素从下标1开始存储 */
Position BinarySearch(List L, ElementType X);

int main()
{
    List L;
    ElementType X;
    Position P;

    L = ReadInput();
    scanf("%d", &X);
    P = BinarySearch(L, X);
    printf("%d\n", P);

    return 0;
}

// ----------------以下是题解代码

Position BinarySearch(List L, ElementType X)
{
    Position mid;
    Position end = L->Last; // 末尾元素下标
    Position start = 1;     // 起始元素下标（注意本题是从1开始）
    do
    {
        mid = (start + end) / 2; // 中间元素
        if (L->Data[mid] == X)
            return mid;
        else if (L->Data[mid] < X) // X在右侧
            start = mid + 1;
        else if (L->Data[mid] > X) // X在左侧
            end = mid - 1;
    } while (start <= end);
    return NotFound;
}

// ----------------我顺便实现一下ReadInput()函数

List ReadInput()
{
    List L = (List)malloc(sizeof(struct LNode));
    scanf(" %d", &L->Last);
    for (int i = 1; i <= L->Last; i++)
    {
        scanf(" %d", &L->Data[i]);
    }
    return L;
}

/*
    注意本题中元素在顺序表中是从下标1开始储存的。
    二分查找是老生常谈的算法了。
        - SomeBottle 2022.12.5
*/