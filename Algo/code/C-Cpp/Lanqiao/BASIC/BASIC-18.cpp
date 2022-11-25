/*
蓝桥杯基础练习VIP-矩形面积交
时间限制: 1s 内存限制: 128MB

题目描述
    平面上有两个矩形，它们的边平行于直角坐标系的X轴或Y轴。对于每个矩形，我们给出它的一对相对顶点的坐标，请你编程算出两个矩形的交的面积。
输入格式
    输入仅包含两行，每行描述一个矩形。 
    在每行中，给出矩形的一对相对顶点的坐标，每个点的坐标都用两个绝对值不超过10^7的实数表示。
输出格式
    输出仅包含一个实数，为交的面积，保留到小数后两位。 
样例输入
    1 1 3 3 
    2 2 4 4 
样例输出
    1.00
*/
#include <cstdio>

using namespace std;
// 声明函数
void swapper(double &x1, double &x2);

/* 对于每个矩形，大概是这样：
                                  xRight
                                    ↓  (xRight,yTop)
                 ┌──────────────────┐ ← yTop
                 │                  │
       yBottom → └──────────────────┘
 (xLeft,yBottom) ↑
               xLeft
*/
int main()
{
    double xLeft1, yBottom1; // 第一个矩形的左边界x坐标和下边界y坐标
    double xRight1, yTop1;   // 第一个矩形的右边界x坐标和上边界y坐标
    double xLeft2, yBottom2; // 第二个矩形的左边界x坐标和下边界y坐标
    double xRight2, yTop2;   // 第二个矩形的右边界x坐标和上边界y坐标
    scanf(" %lf %lf %lf %lf", &xLeft1, &yBottom1, &xRight1, &yTop1);
    scanf(" %lf %lf %lf %lf", &xLeft2, &yBottom2, &xRight2, &yTop2);
    swapper(xLeft1, xRight1); // 确保左边界x坐标<右边界x坐标
    swapper(yBottom1, yTop1); // 确保下边界y坐标<上边界y坐标
    swapper(xLeft2, xRight2);
    swapper(yBottom2, yTop2);
    if (xRight1 < xLeft2 || xLeft1 > xRight2 || yBottom1 > yTop2 || yTop1 < yBottom2)
    {
        /*
            几种两个矩形不相交的情况
            1. 第一个矩形的【右边界】x坐标小于第二个矩形的【左边界】的，这说明第一个矩形在第二个矩形左边
            2. 第一个矩形的【左边界】x坐标大于第二个矩形的【右边界】的，这说明第一个矩形在第二个矩形右边
            3. 第一个矩形的【下边界】y坐标大于第二个矩形的【上边界】的，这说明第一个矩形在第二个矩形上方
            4. 第一个矩形的【上边界】y坐标小于第二个矩形的【下边界】的，这说明第一个矩形在第二个矩形下方
        */
        printf("0.00");
    }
    else
    {
        double midLeft = xLeft1 > xLeft2 ? xLeft1 : xLeft2;           // 相交部分左边界的x坐标
        double midRight = xRight1 < xRight2 ? xRight1 : xRight2;      // 相交部分右边界的x坐标
        double midTop = yTop1 < yTop2 ? yTop1 : yTop2;                // 相交部分上边界的y坐标
        double midBottom = yBottom1 > yBottom2 ? yBottom1 : yBottom2; // 相交部分下边界的y坐标
        double area = (midRight - midLeft) * (midTop - midBottom);    // 算出相交部分的面积
        printf("%.2lf", area);
    }
    return 0;
}

/**
 * @brief 交换x1和x2的值，保证x1<x2
 *
 * @param x1 x1
 * @param x2 x2
 */
void swapper(double &x1, double &x2)
{
    if (x1 > x2)
    {
        double temp = x2;
        x2 = x1;
        x1 = temp;
    }
}
/*
    题目给出的是一对【相对的顶点】的坐标，其实就是对角顶点的坐标（左上角和右下角，或左下角和右上角）
    具体给的是哪个顶点的坐标并不重要：
        - 我们只需要找出【两个顶点中最小的】x和y坐标，就能得到【这个矩形的左下角顶点的坐标】(xLeft,yBottom)
        - 同理，只要我们找出两个顶点中【最大的】x和y坐标，就能得到【这个矩形右上角顶点的坐标】(xRight,yTop)
        - 可以发现，xLeft可以代表【矩形的左边】，xRight代表【矩形的右边】，而yTop和yBottom则分别代表矩形的【上边和下边】
    在【矩形相交】的前提下，主要情况可以归纳成如下两种：

        1. 部分相交
         ┌────────────────┐
         │       midLeft  │
         │          ↓     │
         │  midTop →┏━━━━━╅─────────────┐
         └──────────╄━━━━━┛← midBottom  │
                    │     ↑             │
                    │ midRight          │
                    └───────────────────┘
        2. 包含
          ┌──────────────────────────────────────┐
          │           midLeft                    │
          │              ↓                       │
          │              ┏━━━━━━━━━━┓← midTop    │
          │              ┃          ┃            │
          │              ┃          ┃            │
          │   midBottom →┗━━━━━━━━━━┛            │
          │                         ↑            │
          │                     midRight         │
          │                                      │
          └──────────────────────────────────────┘
    可以发现，无论是部分相交，还是一个矩形包含另一个，相交形成的矩形区域中我们关心的只是：
        - 左边界的x坐标midLeft
        - 右边界的x坐标midRight
        - 上边界的y坐标midTop
        - 下边界的y坐标midBottom
    其中：
        - midLeft可以从两个矩形的左边界中【选一个最大的】
        - midRight可以从两个矩形的右边界中【选一个最小的】
            * 因为对每个矩形来说恒有xLeft<xRight(左边界x坐标<右边界的)，所以可以保证midLeft < midRight
        - midTop则可以从两个矩形的上边界中【选一个最小的】
        - midBottom则可以从两个矩形的下边界中【选一个最大的】
            * 因为对每个矩形来说恒有yBottom<yTop(下边界y坐标<上边界的)，所以可以保证midBottom < midTop

    最后，midRight-midLeft得到相交部分的【长度】，而midTop-midBottom得到相交部分的【高度】，长度×高度则得出面积

    吐槽：这道题在DotCpp上标注为了入门题，但我前几次的题解都没AC。看了一眼DotCpp上的统计，通过率只有30%。这波啊，是入门劝退题！

            - SomeBottle2022.11.25
*/