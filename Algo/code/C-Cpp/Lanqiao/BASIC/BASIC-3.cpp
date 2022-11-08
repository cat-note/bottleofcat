/*
试题 基础练习 字母图形
资源限制
    内存限制：256.0MB   C/C++时间限制：1.0s   Java时间限制：3.0s   Python时间限制：5.0s
问题描述
    利用字母可以组成一些美丽的图形，下面给出了一个例子：
    ABCDEFG
    BABCDEF
    CBABCDE
    DCBABCD
    EDCBABC
    这是一个5行7列的图形，请找出这个图形的规律，并输出一个n行m列的图形。
输入格式
    输入一行，包含两个整数n和m，分别表示你要输出的图形的行数的列数。
输出格式
    输出n行，每个m个字符，为你的图形。
样例输入
    5 7
样例输出
    ABCDEFG
    BABCDEF
    CBABCDE
    DCBABCD
    EDCBABC
数据规模与约定
    1 <= n, m <= 26。
*/
#include <iostream>

using namespace std;
/*
    通过观察可以发现本题生成的“图形”形似理发店门口的旋转灯（憋载着理发店！）
    规模限定为1 <= n, m <= 26，我的理解是m和n均在[1,26]这个范围内
    m是列数，这说明一行最多能装下A-Z这26个英文字母
    而n是行数，也是一行中A的向右偏移量，<=26说明顶多偏移到首位为Z
*/
int main()
{
    int rowNum;
    int colNum;
    cin >> rowNum >> colNum;
    for (int row = 0; row < rowNum; row++)
    {
        for (int col = 0; col < colNum; col++)
        {
            // 观察可以发现，row可以代表一行中A的位置下标
            int offset = col > row ? col - row : row - col; // 算出当前列和A所在位置的偏移量(绝对值)
            char chr = 'A' + offset;                        // 推出当前列应该填入的字符
            cout << chr;                                    // 输出
        }
        cout << endl;
    }
    return 0;
}