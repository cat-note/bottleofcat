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
        double middleLeft = xLeft1 > xLeft2 ? xLeft1 : xLeft2;
        double middleRight = xRight1 < xRight2 ? xRight1 : xRight2;
        double middleTop = yTop1 < yTop2 ? yTop1 : yTop2;
        double middleBottom = yBottom1 > yBottom2 ? yBottom1 : yBottom2;
        double area = (middleRight - middleLeft) * (middleTop - middleBottom);
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

         ┌──────────┐ 
         │      ↓   │
         │    → ┏━━━╅────────────────┐
         └──────╄━━━┛ ← middleBottom │
                │   ↑                │
                │ middleRight        │
                └────────────────────┘
*/