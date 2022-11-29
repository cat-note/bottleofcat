/*
试题 算法训练 拿金币
资源限制
    内存限制：256.0MB   C/C++时间限制：1.0s   Java时间限制：3.0s   Python时间限制：5.0s
问题描述
　　有一个N x N的方格,每一个格子都有一些金币,只要站在格子里就能拿到里面的金币。你站在最左上角的格子里,每次可以从一个格子走到它右边或下边的格子里。请问如何走才能拿到最多的金币。
输入格式
　　第一行输入一个正整数n。
　　以下n行描述该方格。金币数保证是不超过1000的正整数。
输出格式
　　最多能积攒的金币数量。
样例输入
    3
    1 3 3
    2 2 2
    3 1 2
样例输出
    11
数据规模和约定
　　n<=1000
*/
#include <cstdio>
#include <vector>

using namespace std;

int main()
{
    int N;
    scanf("%d", &N);
    // N*N的硬币地图
    // 这里两个尖括号之间加个空格，免得编译器不支持C++11
    vector< vector<int> > coinMap(N, vector<int>(N));
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
        {
            scanf("%d", &coinMap[row][col]); // 读入(row,col)这一格有多少金币
            if (row > 0 && col > 0)          // 在读入初始值后就可以边读边动态规划了
            {
                int leftCoinNum = coinMap[row - 1][col]; // (row,col)的左边这一格的金币数
                int upCoinNum = coinMap[row][col - 1];   // (row,col)的上面这一格的金币数
                // 如果左边的格(row-1,col)金币数比较多，上一步就是【向右】走到当前这一格(row,col)的
                // 如果上边的格(row,col-1)的金币数比较多，上一步就是【向下】走到当前这一格(row,col)的
                // 为了使【到当前格(row,col)为止的金币数最多】，要保证【上一步所在的格的金币数最多】
                coinMap[row][col] += leftCoinNum > upCoinNum ? leftCoinNum : upCoinNum;
            }
            else if (col > 0)
            {
                // 对第一行的值进行处理
                coinMap[row][col] += coinMap[row][col - 1];
            }
            else if (row > 0)
            {
                // 对第一列的值进行处理
                coinMap[row][col] += coinMap[row - 1][col];
            }
        }
    printf("%d", coinMap[N - 1][N - 1]);
    return 0;
}
/*
    又是一道动态规划题。这题输入的N规模是可能比较大的，如果用递归的话非常容易TLE(执行超时)。

    题目在评测的时候会输入一个N*N矩阵，定义了每一格金币的【初始值】。

    初始值
        coinMap是储存这些【金币初始值】的二维数组，也是动态规划使用的数组

    数组元素的含义
        coinMap[row][col] 代表【从最左上角(0,0)】走到(row,col)这一格，【一共能积攒coinMap[row][col]个金币】。

    元素之间的关系
        1. 对于首行元素有 coinMap[0][col]=coinMap[0][col]+coinMap[0][col-1]
            ( 因为在首行只能向右走，在(0,col)处已经积攒的金币数=(0,col)处的初始值+(0,col-1)的金币数 )
            (注意，col>0时此式才有效)

        2. 对于首列元素有 coinMap[row][0]=coinMap[row][0]+coinMap[row-1][0]
            ( 因为在首列只能向下走，在(row,0)处已经积攒的金币数=(row,0)处的初始值+(row-1,0)的金币数 )
            (注意，row>0时此式才有效)

        3. 对于其他元素有 coinMap[row][col]=coinMap[row][col] + MAX( coinMap[row-1][col], coinMap[row][col-1] )
            ( 在(row,col)处【能积攒到的最多金币数】=(row,col)处的初始值 + 上一步【金币数最多】的那一格)
            (注意，row,col均>0时此式才有效)

    根据元素之间的关系很快就能写出程序了。

    * 注：使用了cstdio头而不是iostream，能比较明显地降低时间消耗(80~100ms左右)

        - SomeBottle 2022.11.29
*/