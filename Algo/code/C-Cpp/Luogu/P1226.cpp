#include <cstdio>
#define LL long long

using namespace std;

LL powerMod(LL a, LL b, LL p);

int main()
{
    LL a, b, p;
    scanf("%lld %lld %lld", &a, &b, &p);
    printf("%lld^%lld mod %lld=%lld", a, b, p, powerMod(a, b, p));
    return 0;
}

LL powerMod(LL a, LL b, LL p)
{
    LL result = 1;
    LL an = a; // 初始值: a^1
    while (b != 0)
    {
        if (b % 2) // 当前二进制位为1
        {
            result = (result * an) % p;
        }
        an = (an * an) % p; // 模上p，防止溢出
        b /= 2;
    }
    return result;
}