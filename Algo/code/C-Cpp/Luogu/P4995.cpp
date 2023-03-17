#include <cstdio>
#include <algorithm>

using namespace std;

int main()
{
    int N;
    scanf("%d", &N);
    int stones[N]; // 每个石头的高度
    for (int i = 0; i < N; i++)
        scanf("%d", &stones[i]);
    sort(stones, stones + N); // 升序排，把高的石头放在末尾
    int front = 0, rear = N - 1;
    bool flag = true;     // 用于交替变更front和rear
    int currH = 0;        // 现在所在的高度
    long long result = 0; // 体力耗费
    while (front <= rear)
    {
        int targetH; // 跳到哪个高度的石头
        if (flag)
            targetH = stones[rear--];
        else
            targetH = stones[front++];
        int diff = targetH - currH;
        currH = targetH;
        result += diff * diff;
        flag = !flag;
    }
    printf("%lld", result);
    return 0;
}

/*
    这题是比较典型的贪心算法题，每一步要找到【和目前高度相差最远的】一块石头。

    整体的过程描述如下：
        最初主角位于地面，地面的高度为0:
        1. 先从地面跳到最高的一块石头 N 上
        2. 再从最高的一块石头跳到除地面外最低的石头 1 上
        3. 然后再从这个最低的石头 1 上跳到【第二最高的石头】 N-1 上
        4. 再从第二最高的石头跳到石头 2 上
        ...

    可以发现，当我【将表述所有石头的序列按高度进行排序】后，这个过程实际上做的就是“反复横跳”了，即：

    将石头序列按高度进行【升序】排序，在序列首个元素处置一个指针front，在最后一个元素处置一个指针rear，然后交替取front和rear对应的元素值，不断缩小[front,rear]区间，计算高度差并求和即可。

    注意本题的一个需要注意的点：

        * 输出的数值大小可能超过了整型所能表示的范围，因此需要使用long long类型对结果进行统计和储存。

        - SomeBottle 2023.3.17
*/