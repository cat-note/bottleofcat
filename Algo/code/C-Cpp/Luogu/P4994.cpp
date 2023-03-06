#include <cstdio>

using namespace std;

int main()
{
    int M;
    scanf("%d", &M);
    int itemN = 1;   // 第n项
    int itemNP1 = 1; // 第n+1项
    int n = 1;       // 当前的n
    while (true)
    {
        int modN = itemN % M;         // f(n) mod 2
        int modNP1 = itemNP1 % M;     // f(n+1) mod 2
        if (modN == 0 && modNP1 == 1) // 找到满足要求的n了
            break;
        itemN = itemNP1;         // 第n+1项
        itemNP1 = modN + modNP1; // 第n+2项（取模后再运算不会影响结果）
        n++;
    }
    printf("%d", n);
    return 0;
}

/*
    这题需要从递推出的斐波那契数列中找出匹配项。

    观察可以发现，自始至终运算其实只涉及到了第n项和第n+1项，实际上我只需要利用两个变量进行递推即可，没有必要开一个数组。

    程序中还会用到模运算的性质：(a+b) mod p = (a mod p + b mod p) mod p

    借此我可以让两个变量的值一直保持在[0,M)的范围内，使得程序中用整型int数据类型就能进行表示和运算。

    * 注：题目输出限定n是>0的正整数。

        - SomeBottle 2023.3.6
*/