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
        prefix[i] = (prefix[i - 1] + num) % 7; // 计算前缀和
    }
    // 要找最长的，且其和能被7整除的序列
    int front;
    int back;
    int longest = 0;
    for (int i = 0; i < 7; i++) // 枚举余数
    {
        front = 0;
        back = N;
        for (; front <= N; front++)
            if (prefix[front] == i)
                break;
        for (; back >= 1; back--)
            if (prefix[back] == i)
                break;
        int len = back - front;
        if (len > longest)
            longest = len;
    }
    printf("%d", longest);
    return 0;
}

/*
    O(n^2)算法会TLE，得想办法优化到O(n)
*/