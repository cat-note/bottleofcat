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
    int front = 0, rear = 1;
    int longest = 0;
    for (; front < N; front++)
    {
        for (rear = front + 1; rear <= N; rear++)
        {
            int span = rear - front;
            if (span > longest && (prefix[rear] - prefix[front]) % 7 == 0)
                longest = span;
        }
    }
    printf("%d", longest);
    return 0;
}

/*
    O(n^2)算法会TLE
*/