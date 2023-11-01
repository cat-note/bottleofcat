/*
 * @lc app=leetcode.cn id=1523 lang=cpp
 *
 * [1523] 在区间范围内统计奇数数目
 */

// @lc code=start
class Solution
{
public:
    int countOdds(int low, int high)
    {
        if (low == high) // low和high相等的情况
            return low % 2 == 1 ? 1 : 0;
        int cnt = (high - low - 1) / 2; // 中间有这么多数
        if (low % 2 == 1)               // 左闭为奇数
            cnt++;
        if (high % 2 == 1) // 右闭为奇数
            cnt++;
        if (low % 2 != 1 && high % 2 != 1) // 左右都不是奇数
            cnt++;

        return cnt;
    }
};
// @lc code=end
