#include <cstdio>

using namespace std;

// 矩阵和前缀二维数组，为了方便处理（以免越界），从下标1开始储存
long long matrix[502][502], prefix[502][502]{0};

int main()
{
    int N, M;
    long long K;
    scanf("%d %d %lld", &N, &M, &K);
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            scanf("%lld", &matrix[i][j]);
            // 顺带把前缀和算了
            // 从(1,1)到(i,j)的矩阵元素和 = 左边一项的值 + 上面一项的值 - 左上角一项的值 + 矩阵中(i,j)的值
            // 减去左上角是因为左上角在【左边一项】和【上面一项】中重复计算了。
            prefix[i][j] = prefix[i][j - 1] + prefix[i - 1][j] - prefix[i - 1][j - 1] + matrix[i][j];
        }
    }
    long long count = 0; // 计数
    // 分别代表 上边界j坐标、下边界j坐标、左边界i坐标，右边界i坐标
    int up, down, left, right;
    for (up = 1; up <= N; up++) // 上边界下移
    {
        for (down = up; down <= N; down++) // 下边界下移，从1x..到2x.., 3x..
        {
            // 此时上下边界已经确定，专心处理左右边界即可。
            left = 1; // 左边界最开始在最左边
            right = 1;
            while (left <= M) // 左指针移动到最右边为止
            {
                // 子矩阵左上角(up,left), 右下角(down,right)
                // 利用前缀求子矩阵的和
                long long sum = prefix[down][right] - prefix[up - 1][right] - prefix[down][left - 1] + prefix[up - 1][left - 1];
                if (sum > K || right >= M) // 如果算出来的子矩阵和>K，或右指针到顶
                {
                    left++;       // 左指针右移
                    right = left; // 右指针回退
                }
                else // 是满足要求的子矩阵，右指针右移
                {
                    right++;
                }
                if (sum <= K) // 统计符号要求的子矩阵
                    count++;
            }
        }
    }
    printf("%lld", count);
    return 0;
}

/*
    这个写法能得到80分，最后两个测试例时间超限，岂可休！

    在一个大矩阵中，我可以用四条边界确定一块子矩阵
*/