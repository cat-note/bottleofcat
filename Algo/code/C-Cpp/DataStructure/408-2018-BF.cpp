#include <cstdio>
#include <cstring>

using namespace std;

int main()
{
    // input:
    int N = 8;
    int seq[N] = {-5, 1, 2, 3, 4, 5, 6};
    // algorithm:
    int maxVal = -1;
    for (int i = 0; i < N; i++) // 找到序列中最大的数值
        if (seq[i] > maxVal)
            maxVal = seq[i];
    bool ht[maxVal + 2]; // 利用数值直接定址
    memset(ht, 0, sizeof(ht));
    for (int i = 0; i < N; i++)
    {
        if (seq[i] > 0) // 只标记正整数
            ht[seq[i]] = true;
    }
    // output:
    for (int i = 1; i <= maxVal + 1; i++)
        if (!ht[i])
        {
            printf("%d\n", i);
            break;
        }
    return 0;
}

/*
    这题我一时只能想到利用哈希表的暴力解。
    思路就是把出现的正整数都进行标记，最后扫描一遍哈希表序列，找到首个未被标记的正整数即可。
    这里只是简单写了一下思路，因为题目没有给出数值大小限制，这里的maxVal可能会超级大！
    按理来说应该要实现一个非直接定址的哈希表，比如STL的unordered_map（当然考试里不太能用）

    时间复杂度：O(maxVal) 难以预估，如果是非直接定址哈希表的话就是O(N)
    空间复杂度：O(maxVal) 同上，可以是O(N)

        - SomeBottle 2023.8.10
*/