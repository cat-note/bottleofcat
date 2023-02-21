#include <cstdio>
#include <cstring>

using namespace std;

int main()
{
    int N, M, K; // N个人，M天能打比赛，总共有K天
    scanf("%d %d %d", &N, &M, &K);
    // 这里比赛天数和模拟题都从1开始编号
    int matches[K + 1][K + 1]; // 至多有K天的比赛，每天可能至多有K套模拟题
    memset(matches, 0, sizeof(matches));
    // 读入N个人的数据
    for (int i = 0; i < N; i++)
    {
        for (int no = 1; no <= M; no++)
        {
            int day;
            scanf("%d", &day);     // 第i位参赛者在第day天做第no套模拟题
            if (!matches[day][no]) // 第day天还没有人做第no套模拟题，需要多准备一套
                matches[day][0]++; // 0号下标元素用来储存第day天要用多少套模拟题
            matches[day][no]++;
        }
    }
    // 输出从第1天道第K天需要准备的模拟题套数
    for (int i = 1; i <= K; i++)
    {
        if (i != 1)
            printf(" "); // 不输出多余空格
        printf("%d", matches[i][0]);
    }
    return 0;
}

/*
    本题的重点在于统计【每天】需要用到【多少套不同的模拟题】。

    一共有N位参赛者，第n位参赛者在第day天会做第no套模拟题（no=1,2,3,...,K）

    很容易能发现，咱们可以用一个二维数组matches来记录:

    * 【某天】的【某套模拟题】是否有人使用
    * 【某天】共有【多少套不同的模拟题】

    这个题解中，我将【某天】作为二维数组matches的第一维下标，而将【某套模拟题】作为二维数组的第二维下标。

    * 其中【某天】和【某套模拟题】都是从1开始编号的

    二维数组第二维的【0号下标元素】全部用来统计【某天】共有【多少套不同的模拟题】。

    迭代过程中，只要【某一套模拟题是首次有人做】(matches[day][no]==0)，那么就将这一天的【模拟题用量】加一（matches[day][0]++）。

        - SomeBottle 2023.2.21
*/