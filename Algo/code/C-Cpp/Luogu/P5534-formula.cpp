#include <cstdio>
#define LL long long

using namespace std;

int main()
{
    LL a1, a2, n;
    scanf("%lld%lld%lld", &a1, &a2, &n);
    LL d = a2 - a1; // 公差
    LL S = n * a1 + n * (n - 1) * d / 2;
    printf("%lld", S);
    return 0;
}

/*
    这题用等差数列求和公式就能秒杀23333
    
*/