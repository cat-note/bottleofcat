#include <cstdio>
#include <algorithm>

using namespace std;

int main()
{
    int bound, N;
    scanf("%d %d", &bound, &N);
    int souvenirs[N];
    for (int i = 0; i < N; i++) // 读入纪念品价格
        scanf("%d", &souvenirs[i]);
    sort(souvenirs, souvenirs + N); // 将纪念品按价格升序排序
    int groupNum = 0;               // 分组数
    int currSum = 0;                // 当前组内的价格之和
    int front = 0, rear = N - 1;
    while (front <= rear)
    {
        if (currSum + souvenirs[front] > bound)
        {
            groupNum++;
            currSum = 0;
        }
        currSum += souvenirs[front];
        front++;
        while (front < rear && currSum + souvenirs[rear] > bound)
        {
            groupNum++;
            rear--;
        }
        if (front < rear)
        {
            currSum += souvenirs[rear];
            rear--;
        }
    }
    if (currSum != 0) // 最后未填满一组的也要算入
        groupNum++;
    printf("%d", groupNum);
    return 0;
}

/*
    这题也是经典的贪心+排序优化模式。

    本题需要在【不超过每组上限】的情况下【让每组的价格之和尽量大】。

    也就是说，每次做决策的时候，我需要找到【价格加起来最接近上限的几个纪念品】作为一组。

    我的思路是：

    - 先将纪念品按价格升序排序，低价的纪念品排在前，而高价纪念品排在后。

    - 用两个指针front,rear，分别指向纪念品数组的前部和尾部。

    - 维护一个currSum整型变量，储存当前组内的现有纪念品的价格总和。

    - front向后遍历(front>=rear为止)，对于每个纪念品souvenirs[front]，在尾部寻找一个和其相加的价格<=上限的纪念品souvenirs[rear]

        * currSum加上当前纪念品souvenirs[front]的价格。

        * 如果找不到，就一直rear--，直至找到或者front>=rear为止。

        * 如果souvenirs[rear]和souvenirs[front]的价格相加如果超过了上限的话，那么front后面的纪念品价格和souvenirs[rear]相加【一定会超过上限】，因此把【无法成组】的纪念品souvenirs[rear]都单独作为一组。
            (【分组数目+1】)

    - 如果currSum超过了上限，就将【分组数目+1】，并置currSum=0。

        - SomeBottle 2023.3.18
*/

/*
# [NOIP2007 普及组] 纪念品分组

## 题目描述

元旦快到了，校学生会让乐乐负责新年晚会的纪念品发放工作。为使得参加晚会的同学所获得 的纪念品价值相对均衡，他要把购来的纪念品根据价格进行分组，但每组最多只能包括两件纪念品， 并且每组纪念品的价格之和不能超过一个给定的整数。为了保证在尽量短的时间内发完所有纪念品，乐乐希望分组的数目最少。

你的任务是写一个程序，找出所有分组方案中分组数最少的一种，输出最少的分组数目。

## 输入格式

共 $n+2$ 行：

第一行包括一个整数 $w$，为每组纪念品价格之和的上限。

第二行为一个整数 $n$，表示购来的纪念品的总件数 $G$。

第 $3\sim n+2$ 行每行包含一个正整数 $P_i$ 表示所对应纪念品的价格。

## 输出格式

一个整数，即最少的分组数目。

## 样例 #1

### 样例输入 #1

```
100 
9 
90 
20 
20 
30 
50 
60 
70 
80 
90
```

### 样例输出 #1

```
6
```

## 提示

$50\%$ 的数据满足：$1\le n\le15$。

$100\%$ 的数据满足：$1\le n\le3\times10^4$，$80\le w\le200$，$5 \le  P_i  \le  w$。
*/