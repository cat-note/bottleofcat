/*
蓝桥杯基础练习VIP-矩阵乘法
时间限制: 1s 内存限制: 128MB

题目描述
    给定一个N阶矩阵A，输出A的M次幂（M是非负整数）
    例如：
    A =
        1 2
        3 4
    那么A的2次幂 =
        7 10
        15 22
输入格式
    第一行是一个正整数N、M（1<=N<=30,  0<=M<=5），表示矩阵A的阶数和要求的幂数
    接下来N行，每行N个绝对值不超过10的非负整数，描述矩阵A的值
输出格式
    输出共N行，每行N个整数，表示A的M次幂所对应的矩阵。相邻的数之间用一个空格隔开
样例输入
    2 2
    1 2
    3 4
样例输出
    7 10
    15 22
*/
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    short order; // 矩阵阶数
    short power; // 要求矩阵的多少次幂
    cin >> order >> power;
    vector<vector<short>> initMatrix(order, vector<short>(order)); // 最初的N*N矩阵，vector存储列向量
    vector<long *> resultMatrix(order);                            // 最终要输出的结果矩阵，vector存储行向量
    for (short i = 0; i < order; i++)
    {
        resultMatrix[i] = new long[order]; // 初始化这一行行向量
        for (short j = 0; j < order; j++)
        {
            short numRead;
            cin >> numRead;
            if (power == 0)
            {
                // 如果是0次幂
                resultMatrix[i][j] = i == j ? 1 : 0; // 左上角到右下角的对角线为1，其余全为0
            }
            else
            {
                resultMatrix[i][j] = (long)(numRead); // 行向量作为一个数组元素
            }
            initMatrix[j][i] = numRead; // 列向量作为一个数组元素
        }
    }
    // 本题有可能出现0次幂(在上面已经处理了)，而矩阵的1次幂就是它本身，这里仅处理>=2次幂
    if (power > 1)
    {
        long *rowTemp;                             // 指向一个临时行向量，每一次行向量和列向量相乘的结果暂且放在这里
        for (short time = 1; time < power; time++) // 迭代power-1次, 算出power次幂的resultMatrix
        {
            // 这里的row(行)和col(列)指的是resultMatrix的行和列
            for (short row = 0; row < order; row++)
            {
                rowTemp = new long[order]; // 新分配一个行向量，储存此行的运算结果
                // initRow指的是initMatrix的行，是原矩阵的列
                // 原矩阵的每一列的各数值分别与resultMatrix的相乘并相加
                for (short initRow = 0; initRow < order; initRow++)
                {
                    rowTemp[initRow] = 0;
                    for (short col = 0; col < order; col++)
                    {
                        rowTemp[initRow] += resultMatrix[row][col] * (long)initMatrix[initRow][col];
                    }
                }
                delete[] resultMatrix[row];  // 释放这一行原本的内存
                resultMatrix[row] = rowTemp; // 替换为rowTemp指向的内存(行向量)
            }
        }
    }
    // 输出结果矩阵resultMatrix
    for (short i = 0; i < order; i++)
    {
        for (short j = 0; j < order; j++)
            cout << resultMatrix[i][j] << " ";
        delete[] resultMatrix[i]; // 释放内存好习惯
        cout << "\n";
    }
    return 0;
}
/*
                原矩阵第一列
                     ↓
    ┌         ┐   ┌         ┐     ┌         ┐
    │  1   2  │   │  1   2  │     │  1   3  │ <- initRow
    │         │ × │         │     │         │ (这里的第一行对应原矩阵第一列)
    │  3   4  │   │  3   4  │     │  2   4  │
    └         ┘   └         ┘     └         ┘
    resultMatrix      原矩阵        initMatrix(原矩阵转置)

    -----------

    💡 值得注意的是，initMatrix我是将原矩阵【转置】(行列交换)后储存的。

    最开始，将数据读入resultMatrix和initMatrix，
    后面就是不断的 resultMatrix = resultMatrix × initMatrix 操作了

    -----------

    ⭐运算过程
        ┌                     ┐   ┌           ┐
        │  1×1+3×3   1×2+2×4  │   │  7    10  │
        │                     │ = │           │  <-此乃结果也！
        │  3×1+4×3   3×2+4×4  │   │  15   22  │
        └                     ┘   └           ┘
        1. resultMatrix[0][0] * initMatrix[0][0] + resultMatrix[0][1] * initMatrix[0][1]
            = 1×1 + 3×3
            = 7   ->  放入临时行向量rowTemp[0]

        2. resultMatrix[0][0] * initMatrix[1][0] + resultMatrix[0][1] * initMatrix[1][1]
            = 1×2 + 2×4
            = 10  ->  放入临时行向量rowTemp[1]

        3. 更新resultMatrix[0]为rowTemp指向的临时行向量
            然后新建一个临时行向量，让rowTemp指向新的临时行向量

        4. resultMatrix[1][0] * initMatrix[0][0] + resultMatrix[1][1] * initMatrix[0][1]
            = 3×1 + 4×3
            = 15  ->  放入临时行向量rowTemp[0]

        5. resultMatrix[1][0] * initMatrix[1][0] + resultMatrix[1][1] * initMatrix[1][1]
            = 3×2 + 4×4
            = 22  ->  放入临时行向量rowTemp[1]

        6. 更新resultMatrix[1]为rowTemp指向的临时行向量

        7. 运算结束

    -------------------

    ⚠还要注意一点，题目给出的M可能=0，也就是可能要算矩阵的【0次幂】

        - 矩阵的【0次幂】其实就是N×N的【单位阵】(Identity Matrix), 特点是除左上到右下这条对角线上的元素为1外，其余元素皆为0。


    - SomeBottle 2022.11.25
*/