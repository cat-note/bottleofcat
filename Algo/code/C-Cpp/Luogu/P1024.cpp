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
            double end = (double)upper;
            while (end - start > TOLERANCE)
            {
                double mid = start + (end - start) / 2; // 算出中间值
                if (F(mid) * F(start) < 0)              // 看看根在不在[start,mid]这个区间
                    end = mid;                          // 在的话就左移右边界
                else
                    start = mid; // 否则移动左边界
            }
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