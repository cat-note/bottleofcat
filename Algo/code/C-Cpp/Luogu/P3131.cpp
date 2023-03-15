#include <cstdio>

using namespace std;

int main()
{
    int N;
    scanf("%d", &N);
    int prefix[N + 1]; // 前缀和
    prefix[0] = 0;
    for (int i = 1; i <= N; i++)
    {
        int num;
        scanf("%d", &num);
        prefix[i] = prefix[i - 1] + num; // 计算前缀和
    }
    // 要找最长的，且其和能被7整除的序列
    int len = N;        // 从最长的开始找
    bool found = false; // 找到了就不继续循环了
    for (; len > 0; len--)
    {
        for (int i = len; i < N; i++)
        {
            if ((prefix[i] - prefix[i - len]) % 7 == 0)
            {
                found = true;
                break;
            }
        }
        if (found)
            break;
    }
    printf("%d", len);
    return 0;
}

/*
    O(n^2)算法会TLE，得想办法优化到O(n)
*/