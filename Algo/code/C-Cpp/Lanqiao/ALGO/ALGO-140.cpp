/*
蓝桥杯算法训练VIP-提货单
时间限制: 1s 内存限制: 128MB
题目描述
    有一份提货单，其数据项目有：商品名（MC）、单价（DJ）、数量（SL）。定义一个结构体prut，其成员是上面的三项数据。在主函数中定义一个prut类型的结构体数组，输入每个元素的值，计算并输出提货单的总金额。
输入格式
    第一行是数据项个数N(N< 100)，接下来每一行是一个数据项。商品名是长度不超过100的字符串，单价为double类型，数量为整型。
输出格式
    double类型的总金额。
样例输入
    4
    book 12.5 3
    pen 2.5 10
    computer 3200 1
    flower 47 5
样例输出
    3497.500000
*/
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

typedef struct
{
    string MC; // 商品名
    double DJ; // 单价
    int SL;    // 数量
} prut;

int main()
{
    int itemNum; // 数据项个数
    cin >> itemNum;
    vector<prut> prutArr(itemNum);
    for (int i = 0; i < itemNum; i++)
    {
        cin >> prutArr[i].MC;
        cin >> prutArr[i].DJ;
        cin >> prutArr[i].SL;
    }
    double totalAmount = 0; // 总金额
    for (int i = 0; i < itemNum; i++)
    {
        totalAmount += (prutArr[i].DJ * prutArr[i].SL);
    }
    printf("%.6lf", totalAmount);
    return 0;
}
/*
    实际上完全可以不用结构体...
    还是给题目个面子吧
        - SomeBottle 2022.12.1
*/