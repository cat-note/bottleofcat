#include <cstdio>
#define LL long long

using namespace std;

int main()
{
    LL a1, a2, n;
    scanf("%lld%lld%lld", &a1, &a2, &n);
    LL d = a2 - a1; // 公差
    LL S = a1 + a2;
    LL prevA = a2;
    for (int i = 3; i <= n; i++)
        S += (prevA += d);
    printf("%lld", S);
    return 0;
}
/*
    这个解法就是递推了，相比公式直接求出结果来说要平均慢了1ms
        - SomeBottle 2023.3.3
*/