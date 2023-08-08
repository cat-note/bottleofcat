#include <cstdio>
#include <cstring>

using namespace std;

int main()
{
    // input:
    int N = 8; // 假设N=8
    int seq[N] = {0, 5, 5, 3, 5, 7, 5, 5};
    // int seq[N] = {0, 5, 5, 3, 5, 7, 1, 5};
    // algorithm:
    int pivot = -1; // 主元素
    // 这题也是明确限制了数据范围，因此可以直接用空间换时间，上哈希表
    int ht[N + 1];
    memset(ht, 0, sizeof(ht));
    // 扫描序列
    for (int i = 0; i < N; i++)
        if (++ht[seq[i]] > N / 2)
        {                   // 数值作为下标来直接定址，进行计数
            pivot = seq[i]; // 如果计数超过了N/2就找到了这个主元
            break;
        }
    // output:
    printf("%d\n", pivot);
    return 0;
}

/*
    题目中明确地限制了数据的取值范围，因此这里可以直接用空间换时间。
    利用哈希表对各数值出现的次数进行统计，选出出现次数超过序列长度一半的元素即可。

    时间复杂度：O(N)级别
    空间复杂度：O(N)级别 <- 按理来说应该还有优化方法，不用哈希表

        - SomeBottle 2023.8.8

*/