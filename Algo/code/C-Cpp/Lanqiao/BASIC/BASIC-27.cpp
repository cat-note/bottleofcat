#include <iostream>
#include <vector>
/*
    题目描述我觉得有点不准确，应该说任意【和对角线平行的斜线】上都只能有一个同一种的皇后
*/
using namespace std;
short n; // 棋盘有n×n个格子(n<=8)，用short完全没问题
// 注意在C++11前，如果两个尖括号写在一起(>>)可能会被认为是移位运算而编译失败
vector<vector<short>> chessBoard; // 象棋棋盘
vector<bool> blackColStatus;      // 记录每一列是否已经放过黑皇后
vector<bool> blackDiagonalLBRT;   // 记录从左下(LeftBottom)到右上(RightTop)的斜线是否放过黑皇后
vector<bool> blackDiagonalLTRB;   // 记录从左上(LeftTop)到右下(RightBottom)的斜线是否放过黑皇后
vector<bool> whiteColStatus;      // 记录每一列是否已经放过白皇后
vector<bool> whiteDiagonalLBRT;   // 记录从左下(LeftBottom)到右上(RightTop)的斜线是否放过白皇后
vector<bool> whiteDiagonalLTRB;   // 记录从左上(LeftTop)到右下(RightBottom)的斜线是否放过白皇后

// 函数声明
int placeBlackQueen(int currentRow);
int placeWhiteQueen(int currentRow);

int main()
{
    cin >> n;                               // 读入棋盘
    chessBoard.resize(n, vector<short>(n)); // 将棋盘二维数组塑形成n×n
    whiteColStatus.resize(n, false);        // 每一列目前没有棋，全是0
    blackColStatus.resize(n, false);
    blackDiagonalLBRT.resize(2 * n - 1, false); // 初始化斜线状态储存数组
    blackDiagonalLTRB.resize(2 * n - 1, false); // 下面放置皇后的时候能发现下标最多到2n-2，因此数组要有2n-1这么大
    whiteDiagonalLBRT.resize(2 * n - 1, false);
    whiteDiagonalLTRB.resize(2 * n - 1, false);
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            cin >> chessBoard[row][col];
        }
    }
    // 开始摆皇后
    cout << placeBlackQueen(0); // 从下标为0的行(深度)开始
    return 0;
}
/**
 * @brief 放置黑皇后
 *
 * @param currentRow 当前行(深度)
 * @return int 有多少种可行解
 */
int placeBlackQueen(int currentRow)
{
    if (currentRow >= n) // 当深度达到n时，遍历已经透过所有行(深度)，说明产生了放黑皇后的可行解
    {
        return placeWhiteQueen(0); // 黑皇后放完了，接着放白皇后
    }
    int solutions = 0; // 可行解数量
    // 遍历这一行的每一列
    for (int col = 0; col < n; col++)
    {
        /*
            chessBoard[currentRow][col]=1时，说明棋盘的这个位置可以放置皇后
            blackColStatus[col]=false时，说明棋盘的这一行还没有放过黑皇后。
                * 因为我是逐行深入(DFS)的，每一行上一定不会有重复，只用考虑列。
            blackDiagonalLBRT[currentRow + col]=false时，说明当前的左下到右上的斜线没有放过黑皇后
                * 观察可知左下到右上的斜线中，每一格的横纵下标加起来的值是确定的，在[0,2n-2]这个范围内
            blackDiagonalLTRB[currentRow - col + n - 1]=false时，说明当前的左上到右下的斜线中没有放过黑皇后
                * 观察可以发现，左上到右下的斜线中，每一格的横纵下标之差都是一致的，在[1-n,n-1]这个范围内
                  如果有负数的话就不方便处理了，如果再加上n-1，就能把下标范围移动到[0,2n-2]范围内，同LBRT的一样
            以上四个条件均满足，才可以放置黑皇后
        */
        int LBRTIndex = currentRow + col;
        int LTRBIndex = currentRow - col + n - 1;
        if (chessBoard[currentRow][col] == 1 && !blackColStatus[col] && !blackDiagonalLBRT[LBRTIndex] && !blackDiagonalLTRB[LTRBIndex])
        {
            chessBoard[currentRow][col] = 2;              // 标记这一格放了黑皇后
            blackColStatus[col] = true;                   // 标记这一列放过了黑皇后
            blackDiagonalLBRT[LBRTIndex] = true;          // 标记这根左下到右上的斜线放过了黑皇后
            blackDiagonalLTRB[LTRBIndex] = true;          // 标记这根左上到右下的斜线放过了黑皇后
            solutions += placeBlackQueen(currentRow + 1); // 在当前状态下，下移一行(增加深度)，再继续遍历
            // 因为当前列下的遍历已经结束，接下来要向下推移一列继续遍历，需要回复到【遍历当前列之前的状态】
            chessBoard[currentRow][col] = 1;      // 回溯：这一格没有放过黑皇后，可以放别的皇后
            blackColStatus[col] = false;          // 回溯：标记这一列没有放过黑皇后
            blackDiagonalLBRT[LBRTIndex] = false; // 回溯：标记这根左下到右上的斜线没有放过黑皇后
            blackDiagonalLTRB[LTRBIndex] = false; // 回溯：标记这根左上到右下的斜线没有放过黑皇后
        }
    }
    return solutions;
}
/**
 * @brief 放置白皇后
 *
 * @param currentRow 当前行(深度)
 * @return int 有多少可行解
 */
int placeWhiteQueen(int currentRow)
{
    if (currentRow >= n) // 当深度达到n时，遍历已经透过所有行(深度)，说明【在已经放了黑皇后的情况下】产生了放白皇后的可行解
    {
        return 1; // 白皇后放完了，可行解+1
    }
    int solutions = 0; // 可行解数量
    // 遍历这一行的每一列
    for (int col = 0; col < n; col++)
    {
        /*
           chessBoard[currentRow][col]==1时，说明这个格子可以放白皇后
                * 因为放了黑皇后的格子值=2，放了黑皇后肯定就不能再放白皇后了
        */
        int LBRTIndex = currentRow + col;
        int LTRBIndex = currentRow - col + n - 1;
        if (chessBoard[currentRow][col] == 1 && !whiteColStatus[col] && !whiteDiagonalLBRT[LBRTIndex] && !whiteDiagonalLTRB[LTRBIndex])
        {
            whiteColStatus[col] = true;                   // 标记这一列放过了白皇后
            whiteDiagonalLBRT[LBRTIndex] = true;          // 标记这根左下到右上的斜线放过了白皇后
            whiteDiagonalLTRB[LTRBIndex] = true;          // 标记这根左上到右下的斜线放过了白皇后
            solutions += placeWhiteQueen(currentRow + 1); // 在当前状态下，下移一行(增加深度)，再继续遍历
            // 因为当前列下的遍历已经结束，接下来要向下推移一列继续遍历，需要回复到【遍历当前列之前的状态】
            whiteColStatus[col] = false;          // 回溯：标记这一列没有放过白皇后
            whiteDiagonalLBRT[LBRTIndex] = false; // 回溯：标记这根左下到右上的斜线没有放过白皇后
            whiteDiagonalLTRB[LTRBIndex] = false; // 回溯：标记这根左上到右下的斜线没有放过白皇后
        }
    }
    return solutions;
}