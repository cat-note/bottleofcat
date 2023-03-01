#include <cstdio>
#include <cstring>
#define LL long long

using namespace std;

int N, K;
LL nums[20]; // 储存N个数

int solve(LL sum = 0, int start = 0, int depth = 0);

int main()
{
    scanf("%d %d", &N, &K);
    for (int i = 0; i < N; i++)
    {
        scanf("%lld", &nums[i]);
    }
    printf("%d", solve());
    return 0;
}

// (直到当前层的总和，当前层从哪个下标开始遍历，第几层)
int solve(LL sum, int start, int depth) // DFS解决本题
{
    if (depth >= K) // 层数达到K时，说明找到了K个数，判断这K个数的和是否是质数
    {
        for (int i = 2; i * i <= sum; i++)
            if (sum % i == 0) // 如果有能除尽这个和的数，那么这个和就不是质数
                return 0;
        return 1; // 否则统计+1
    }
    int count = 0;                  // 统计和为素数的数量
    for (int i = start; i < N; i++) // 每一层从start下标开始
        count += solve(sum + nums[i], i + 1, depth + 1);
    return count;
}

/*
    题目需要求的是n个数中的【k个数之和】。

    要用人脑去想的话，思路肯定是先确定第一个数，然后【在剩余的数】中寻找第2个数，接着在【除前两个数之外的数】中寻找第3个数...

    可以发现每一个数就像【一棵树中的结点】，而整个寻找过程其实就是一棵树的深度优先搜索遍历过程，只不过这里【限定了最大深度为k】。

    ------------------

    另外还能发现，【并不是每层都要遍历所有n个数字】。

    比如样例给出的数列{3,7,12,19}:

    * 先选定3，k=3个数的和有:

        - 3+7+12=22
        - 3+7+19=29
        - 3+12+19=34

    * 这个时候再从7开始，找三个数的和:

        - 7+3+12=22 (重复了！)
        - 7+3+19=29 (重复了！)
        - 7+12+19=38

    可以看到，对3来说需要在{7,12,19}里进行搜索，而对7来说只需要在{12,19}里搜索即可，这样就能避免重复了。

    也就是说，在本题的DFS中，对于下层来说，只需要从上一层遍历到的下标的【下一位】开始即可，不需要“回头”。

        - SomeBottle 2023.3.1

*/