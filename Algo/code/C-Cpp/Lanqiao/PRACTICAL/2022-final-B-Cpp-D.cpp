#include <iostream>

using namespace std;

int main()
{
    int bushNum;
    cin >> bushNum; // 灌木数
    for (int i = 1; i <= bushNum; i++)
    {
        int leftDist = i - 1;                                                // 距离最左端有几棵灌木
        int rightDist = bushNum - i;                                         // 距离最右端有几棵灌木
        int maxHeight = leftDist > rightDist ? leftDist * 2 : rightDist * 2; // 把距离最远的一端的灌木数*2就行
        cout << maxHeight << "\n";
    }
    return 0;
}

/*
    爱丽丝是循环往复地在修剪，对于每棵灌木当然是【越久没被修剪，长得越高】。

    很明显，灌木能长多高，和其到两端的距离息息相关，对于每棵灌木得找到距离【最远的一端】

        - 比如 1 2 3 4 5 6 这六棵灌木:

            对于2来说，被修剪后爱丽丝的【修剪路线】可能如下:

            【情况1】
                  修剪: 2->3->4->5->6->5->4->3->2
                2的高度: 1  2  3  4  5  6  7  8

            【情况2】
                  修剪: 2->1->2
                2的高度: 1  2

    为什么要找最远的一端已经不言而喻。

        - SomeBottle 2023.2.11

*/