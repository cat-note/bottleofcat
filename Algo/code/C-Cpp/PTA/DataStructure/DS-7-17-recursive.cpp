#include <iostream>

using namespace std;

void SolveHanoi(int plateNum, char start, char buffer, char end);

int main()
{
    int plateNum; // 盘子数
    cin >> plateNum;
    SolveHanoi(plateNum, 'a', 'b', 'c');
    return 0;
}

/**
 * @brief 解决汉诺塔问题的递归函数
 *
 * @param plateNum 放在起始柱上的盘子数
 * @param start 起始柱
 * @param buffer 中转柱
 * @param end 目标柱
 */
void SolveHanoi(int plateNum, char start, char buffer, char end)
{
    if (plateNum == 1)
    { // 如果这一摞只剩一个盘子，就直接移动到目标柱
        cout << start << " -> " << end << "\n";
        return;
    }
    // 否则进行三步
    SolveHanoi(plateNum - 1, start, end, buffer); // 先将上面的N-1个盘子移动到中转柱
    cout << start << " -> " << end << "\n";       // 再把最底下的盘子移动到目标柱
    SolveHanoi(plateNum - 1, buffer, start, end); // 最后将上面的N-1个盘子从中转柱移动到目标柱
}

/*
    很有意思的是，7-17这道题就算用递归也是可以AC的
*/