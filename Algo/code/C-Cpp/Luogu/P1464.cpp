#include <cstdio>
#include <cstring>
#define LL long long

using namespace std;

LL memory[21][21][21]; // 记忆数组memory[a][b][c]储存w(a,b,c)的值

LL w(LL a, LL b, LL c); // 函数w(a,b,c)

int main()
{
    memset(memory, 0, sizeof(memory));
    memory[0][0][0] = 1;
    while (true)
    {
        LL a, b, c;
        scanf("%lld %lld %lld", &a, &b, &c);
        if (a == -1 && b == -1 && c == -1)
            break;
        printf("w(%lld, %lld, %lld) = %lld\n", a, b, c, w(a, b, c));
    }
    return 0;
}

LL w(LL a, LL b, LL c)
{
    if (a <= 0 || b <= 0 || c <= 0) // 条件1
        return 1;
    if (a > 20 || b > 20 || c > 20) // 条件2
        return w(20, 20, 20);
    // 从这里开始，a,b,c只有可能在[0,20]内
    if (memory[a][b][c]) // 如果已经算过w(a,b,c)，就直接返回结果
        return memory[a][b][c];
    // 接下来是没算过w(a,b,c)的情况
    if (a < b && b < c) // 条件3
    {
        memory[a][b][c] = w(a, b, c - 1) + w(a, b - 1, c - 1) - w(a, b - 1, c); // 用记忆数组储存已经运算出的结果
    }
    else // 其他情况
    {
        memory[a][b][c] = w(a - 1, b, c) + w(a - 1, b - 1, c) + w(a - 1, b, c - 1) - w(a - 1, b - 1, c - 1);
    }
    return memory[a][b][c];
}

/*
    这题看似输入的数值规模很大，但实际上a,b,c是在[0,20]的范围内的。

    本题函数的递归调用过程中实际上【重复访问了】多次【之前已经计算出】的子问题的结果（状态），要是用一个数组对这些已经运算出的结果进行储存，便能减少很多函数的递归调用。

    因此主要的解题思路就是——记忆化搜索！
    -------------------
    这题也可以像动态规划那样先把【整个记忆数组】都推出来，但这样把【需要用到的结果】和【不需要用到的结果】都推出来了，在某些情况下不一定比递归函数【边算边储存】的时间效率高。

        - SomeBottle 2023.3.2
*/