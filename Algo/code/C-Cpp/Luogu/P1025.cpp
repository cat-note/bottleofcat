#include <cstdio>

using namespace std;

int solve(int num, int start = 1, int part = 1); // DFS解决问题

int N, K;

int main()
{
    scanf("%d %d", &N, &K);
    printf("%d", solve(N));
    return 0;
}

// (当前剩余的整数大小，这一份从start开始枚举，第几份)
int solve(int num, int start, int part)
{
    if (part == K) // 找到了可行的分法，分成了K份
        return 1;
    int res = 0; // 总的可行方案
    // 除了当前这一份外还有K-layer份，假设后面每份只分一份，bound就是当前能分到的最大数值
    // 比如把7分成3份，那么第一份的数值最大为7-(3-1)=5.
    int bound = num - (K - part);
    for (int i = start; i <= bound; i++) // 从start开始，也算是剪枝
    {
        int numLeft = num - i; // 计算留给后面分的还有多少
        if (i > numLeft)       // 当前这一份比剩余的值要大，就不用继续了，剪枝
            break;
        res += solve(numLeft, i, part + 1);
    }
    return res;
}