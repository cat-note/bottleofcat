/*
蓝桥杯基础练习VIP-回形取数
时间限制: 1s 内存限制: 128MB 提交: 5118 解决: 1717

题目描述
    回形取数就是沿矩阵的边取数，若当前方向上无数可取或已经取过，则左转90度。一开始位于矩阵左上角，方向向下。
输入格式
    输入第一行是两个不超过200的正整数m,  n，表示矩阵的行和列。接下来m行每行n个整数，表示这个矩阵。
输出格式
    输出只有一行，共mn个数，为输入矩阵回形取数得到的结果。数之间用一个空格分隔，行末不要有多余的空格。
样例输入
    3 3
    1 2 3
    4 5 6
    7 8 9
样例输出
    1 4 7 8 9 6 3 2 5
*/
#include <iostream>
#include <vector>

using namespace std;
/*
    这道题我暂时只能想到这种利用循环的解法了，不过这样整个代码写下来思路很清晰，我先放个6×5矩阵在这里：
        1  2  3  4  5
        6  7  8  9  10
        11 12 13 14 15
        16 17 18 19 20
        21 22 23 24 25
        26 27 28 29 30
    可以发现回形取数就是一环一环地逆时针读数，以上面这个矩阵为例：
        最外环(layer=0)的取数顺序：1->26->30->5->2
        最外环取完后，进入第二环(layer=1)，取数顺序：7->22->24->9->8
        再向内一环(layer=2)，取数顺序：13->18
    可以发现，【除了最内部一环】外，外面每一环都要经过【四个方向】上的数字遍历，因此对于每一环【可以采用四个for循环】
    而在这四个for循环外，还要套一层对于环数处理的循环。
    等等...【最内部一环可能没有经过四个方向】该怎么办?
        - 我为此额外加了个计数器counter，只要counter达到了m × n，说明【所有数字】已经全部输出，【后续循环不再继续】

        - SomeBottle 2022.11.17
*/
int main()
{
    int m; // 行数
    int n; // 列数
    cin >> m >> n;
    vector< vector<int> > matrix(m, vector<int>(n)); // m×n的矩阵
    int totalNum = m * n;                          // 总数目
    int shortest = m > n ? n : m;                  // 找到m和n中最短的边
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            cin >> matrix[i][j];

    // 开始绕圈输出，从最外层(layer=0)开始，每层绕一圈
    // 关于循环继续条件，shortest为奇数时：layer<=shortest/2；而shortest为偶数时：layer<shortest/2 (因为是仅保留整数的除法)
    // 还是写成layer*2<shortest吧，这样无论shortest是奇数还是偶数，循环继续条件都是一样的
    int counter = 0; // 计数器，记录输出了多少个数
    for (int layer = 0; layer * 2 < shortest; layer++)
    {
        // 从左上角开始，先向下直到底部(如果layer为1，row从1开始，结束于m-1-1，相当于往内收缩了一圈)
        for (int row = layer; counter < totalNum && row < m - layer; row++)
        {
            // 这里的layer实际上是0+layer，最外圈layer=0, 也就是遍历[0][0]至[m-1][0]。
            cout << matrix[row][layer] << " ";
            counter++;
        }
        // 从左下角开始向右, col=layer+1是为了防止重复输出角落数
        for (int col = layer + 1; counter < totalNum && col < n - layer; col++)
        {
            // m-1-layer，最底部坐标随层数增加而减少
            cout << matrix[m - 1 - layer][col] << " ";
            counter++;
        }
        // 从右下角开始向上
        // 这里的m-layer-2其实是m-1-layer-1，一样是为了防止重复输出角落数
        for (int row = m - layer - 2; counter < totalNum && row >= layer; row--)
        {
            cout << matrix[row][n - 1 - layer] << " ";
            counter++;
        }
        // 从右上角开始向左
        // 这里的n-layer-2实际上是n-1-layer-1
        // 值得注意的是，结束条件是col<=layer，左上角数字已经输出过了
        for (int col = n - layer - 2; counter < totalNum && col >= layer + 1; col--)
        {
            cout << matrix[layer][col] << " ";
            counter++;
        }
    }
    return 0;
}