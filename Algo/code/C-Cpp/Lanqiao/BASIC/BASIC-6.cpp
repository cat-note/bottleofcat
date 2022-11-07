/*
试题 基础练习 杨辉三角形
资源限制
    内存限制：256.0MB   C/C++时间限制：1.0s   Java时间限制：3.0s   Python时间限制：5.0s
问题描述
    杨辉三角形又称Pascal三角形，它的第i+1行是(a+b)i的展开式的系数。

    它的一个重要性质是：三角形中的每个数字等于它两肩上的数字相加。

    下面给出了杨辉三角形的前4行：
    1
    1 1
    1 2 1
    1 3 3 1
    给出n，输出它的前n行。
输入格式
    输入包含一个数n。
输出格式
    输出杨辉三角形的前n行。每一行从这一行的第一个数开始依次输出，中间使用一个空格分隔。请不要在前面输出多余的空格。
样例输入
    4
样例输出
    1
    1 1
    1 2 1
    1 3 3 1
数据规模与约定
    1 <= n <= 34。
*/
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int layerCount; // 层数
    cin >> layerCount;
    // 这里要注意一下，在C++11之前，这里的两个尖括号>不能写在一起(>>)，得加个空格(> >)，不然会被认为是移位
    vector< vector<long> > triangle(layerCount);                         // 储存杨辉三角(初始化layerCount层先)
    triangle[0].push_back(1);                                          // 先在第一层放个1
    cout << 1 << endl;                                                 // 因为n至少为1，肯定有顶层这个1，我先输出为敬!
    for (int row = 1, prevRow = 0; row < layerCount; row++, prevRow++) // prevRow是上一行
    {
        for (int col = 0; col <= row; col++)
        {
            long num = 0;          // [row,col]这一位要填入的数字
            int prevCol = col - 1; // 上一行对应的上一列
            if (prevCol >= 0)      // 如果左肩存在
            {
                // prevCol不会超出下标上边界，只用考虑是否小于下边界
                num += triangle[prevRow][prevCol]; // 这里先加上[row,col]这个数字左肩上的数字[row-1,col-1]
            }
            if (col <= prevRow)
            {                                  // 如果右肩存在(prevRow也是上一行的最大下标)
                num += triangle[prevRow][col]; // 加上这个数字右肩上的数字[row-1,col]
            }
            triangle[row].push_back(num); // 将算出的数字放入[row,col]
            cout << num << " ";
        }
        cout << endl;
    }
    return 0;
}