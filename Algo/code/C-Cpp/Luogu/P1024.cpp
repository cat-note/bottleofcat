#include <cstdio>
#define TOLERANCE 1e-3
// 容差，两浮点数相减<TOLERANCE就认为相等

using namespace std;

double a, b, c, d;

inline double F(double x);

int main()
{
    scanf("%lf %lf %lf %lf", &a, &b, &c, &d);
    // 因为根与根之间相差绝对值>=1，说明一个大小为1的区间内不可能有【超过一个解】
    // 因此枚举[-100,100]中的每个大小为1的区间即可
    for (int lower = -100; lower < 100; lower++)
    {
        int upper = lower + 1; // 区间[lower,upper]
        double f1 = F((double)lower);
        double f2 = F((double)upper);
        if (f1 == 0 || f1 > 0 && f1 < TOLERANCE || f1 < 0 && 0 - f1 < TOLERANCE) // 可能根是一个整值，那么f(x)算出来就会是0
        {
            printf("%.2lf ", (double)lower);
            continue;
        }
        if (f1 * f2 < 0) // [lower,upper]区间内有一个根
        {
            // 以0.001为粒度s在这个大小为1的区间内进行二分搜寻
            double start = (double)lower;
            double end = (double)upper; // 防止上界重复
            while (end - start > TOLERANCE)
            {
                double mid = start + (end - start) / 2; // 算出中间值
                if (F(mid) * F(start) < 0)              // 看看根在不在[start,mid]这个区间
                    end = mid;                          // 在的话就左移右边界
                else
                    start = mid; // 否则移动左边界
            }
            // 每一次会先处理左边界并输出可行解，而这次迭代的右边界就是下次迭代的左边界
            // 因此这里如果start趋近于右边界，就不输出了，以免重复输出
            if ((double)upper - start >= TOLERANCE)
                printf("%.2lf ", start);
        }
    }
    return 0;
}

// f(x)
double F(double x)
{
    return a * x * x * x + b * x * x + c * x + d;
}

/*
    这题有两个关键提示:

    1. 根与根之差的绝对值>=1，这说明一个大小为1的区间内【至多】只有一个根。

    2. x1<x2 时，若有f(x1)*f(x2)<0，说明x1与x2之间有一个根。

    第一个条件说明我可以遍历[-100,100]内的所有大小为1的区间进行查找。

    第二个条件说明每遍历到一个【大小为1的区间】，我可以按折半查找的方式进行搜寻（本题限定了精度为小数点后2位）。

    * 要注意的是，根可能出现在【当前区间的边界上】，因此每次可以先判断【左边界】是否是一个根，如果是的话就不用在这个区间进行折半查找了。

    * 如果折半查找到的根在【当前区间的右边界上】，就不用输出了，因为下一个区间的【左边界】就是这个区间的右边界。

        - SomeBottle 2023.3.10
*/

/*
# [NOIP2001 提高组] 一元三次方程求解

## 题目描述

有形如：$a x^3 + b x^2 + c x + d = 0$  这样的一个一元三次方程。给出该方程中各项的系数（$a,b,c,d$ 均为实数），并约定该方程存在三个不同实根（根的范围在 $-100$ 至 $100$ 之间），且根与根之差的绝对值 $\ge 1$。要求由小到大依次在同一行输出这三个实根(根与根之间留有空格)，并精确到小数点后 $2$ 位。

提示：记方程 $f(x) = 0$，若存在 $2$ 个数 $x_1$ 和 $x_2$，且 $x_1 < x_2$，$f(x_1) \times f(x_2) < 0$，则在 $(x_1, x_2)$ 之间一定有一个根。

## 输入格式

一行，$4$ 个实数 $a, b, c, d$。

## 输出格式

一行，$3$ 个实根，从小到大输出，并精确到小数点后 $2$ 位。

## 样例 #1

### 样例输入 #1

```
1 -5 -4 20
```

### 样例输出 #1

```
-2.00 2.00 5.00
```

## 提示

**【题目来源】**

NOIP 2001 提高组第一题
*/