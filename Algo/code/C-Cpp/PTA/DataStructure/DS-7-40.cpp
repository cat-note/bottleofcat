#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>
#define INF 114514
#define DOUBLE_TOLERANCE 1e-6

using namespace std;

// 表示一个国家
struct Country
{
    int gold;        // 金牌总数
    int medal;       // 奖牌总数
    double goldAvg;  // 国民人均金牌数
    double medalAvg; // 国民人均奖牌数
    int rank[4];     // 存放上面四种计算方式下的排名
};

vector<Country> countries; // 储存国家
vector<short> rankings;    // 临时储存排名

bool goldSort(short a, short b);       // 按金牌数比较的函数
bool medalSort(short a, short b);      // 按奖牌数比较的函数
bool goldAvgSort(short a, short b);    // 按人均金牌数比较的函数
bool medalAvgSort(short a, short b);   // 按人均奖牌数比较的函数
bool doubleEquals(double a, double b); // 判断两个浮点数是否相同
void calcRank(int type);               // 根据计算方法type计算所有国家的排名，如果能更新则更新

int main()
{
    int countryNum, queryNum; // 国家数和查询数
    scanf("%d %d", &countryNum, &queryNum);
    countries.resize(countryNum);
    rankings.resize(countryNum);
    // 读入国家数据
    for (short i = 0; i < countryNum; i++)
    {
        Country ct; // 最开始排名都是无穷
        scanf("%d", &ct.gold);
        scanf("%d", &ct.medal);
        int population;
        scanf("%d", &population);
        ct.goldAvg = (double)ct.gold / population;
        ct.medalAvg = (double)ct.medal / population;
        countries[i] = ct;
        rankings[i] = i;
    }
    // 按四种计算方法算出对应的排名
    calcRank(1);
    calcRank(2);
    calcRank(3);
    calcRank(4);
    // 输出最优计算方法和排名
    for (int i = 0; i < queryNum; i++)
    {
        short queryInd; // 查询的国家编号
        scanf("%hd", &queryInd);
        int ranking = INF; // 排序
        int orderBy;       // 计算方式
        // 从当前国家的四种计算方法的排序中找出最佳的
        for (int type = 0; type < 4; type++)
        {
            if (countries[queryInd].rank[type] < ranking)
            {
                ranking = countries[queryInd].rank[type];
                orderBy = type + 1;
            }
        }
        if (i != 0)
            printf(" "); // 不输出多余空格
        printf("%d:%d", ranking, orderBy);
    }
    return 0;
}

void calcRank(int type)
{
    // 先按计算方法排序
    switch (type)
    {
    case 1: // 按金牌数降序
        sort(rankings.begin(), rankings.end(), goldSort);
        break;
    case 2: // 按奖牌数降序
        sort(rankings.begin(), rankings.end(), medalSort);
        break;
    case 3: // 按人均金牌数降序
        sort(rankings.begin(), rankings.end(), goldAvgSort);
        break;
    case 4: // 按人均奖牌数降序
        sort(rankings.begin(), rankings.end(), medalAvgSort);
        break;
    }
    int countryNum = countries.size(); // 国家数
    // 处理并列排名
    for (int i = 0; i < countryNum; i++)
    {
        int currRank = i + 1;     // 当前的排名
        short curr = rankings[i]; // 当前国家编号
        if (i > 0)
        {
            short prev = rankings[i - 1];
            bool parallel = false; // 是否并列
            switch (type)
            {
            case 1: // 如果金牌数相同，说明和序列中前一个国家并列了
                parallel = countries[curr].gold == countries[prev].gold;
                break;
            case 2: // 如果奖牌数相同，说明和序列中前一个国家并列了
                parallel = countries[curr].medal == countries[prev].medal;
                break;
            case 3: // 如果人均金牌数相同，说明和序列中前一个国家并列了
                parallel = doubleEquals(countries[curr].goldAvg, countries[prev].goldAvg);
                break;
            case 4: // 如果人均奖牌数相同，说明和序列中前一个国家并列了
                parallel = doubleEquals(countries[curr].medalAvg, countries[prev].medalAvg);
                break;
            }
            // 如果和前一个国家并列了，排名也要和前一个国家一样
            if (parallel)
                currRank = countries[prev].rank[type - 1]; // 读取【前一个国家】在【相同计算方法下】的排名
        }
        // 更新当前国家的排名
        countries[curr].rank[type - 1] = currRank;
    }
}

bool goldSort(short a, short b)
{
    return countries[a].gold > countries[b].gold;
}

bool medalSort(short a, short b)
{
    return countries[a].medal > countries[b].medal;
}

bool goldAvgSort(short a, short b)
{
    return countries[a].goldAvg > countries[b].goldAvg;
}

bool medalAvgSort(short a, short b)
{
    return countries[a].medalAvg > countries[b].medalAvg;
}

// 浮点数表示不精确，只要两浮点数相差值小于容差，就认为相等
bool doubleEquals(double a, double b)
{
    return abs(a - b) < DOUBLE_TOLERANCE;
}

/*
    这题如果没有并列的情况，是很容易解决的。
    难就难在出题人这老伙计要求有对并列情况的处理，哇，我真想用靴子狠狠地踢他的屁股！
    -------------------------------------

    千辛万苦写出来后，发现思路其实还是比较顺的:

        1. 先把所有国家的信息读入结构体。

        2. 拿出一个rankings数组，其中每个元素代表【一个国家的编号】。

        3. 【分别】计算出【四种计算方式】下每个国家各自的【排名】:

            a. 先对rankings数组按【某种计算方式】进行降序排序(sort函数提供的快速排序)

            b. 从头到尾扫描rankings数组，看看【在这种计算方式下】有没有【重复项】(并列项)
                * 有序序列的重复两项中，【后一项的排名】等于前一项的排名。

            c. 更新国家【在当前计算方式下】的排名

        4. 经过了第3步，每个国家【在每种计算方式】下的排名都算了出来

        5. 在查询某个国家的排名时，只需要【找到四种计算方式排名】中【排名最靠前的一种计算方式】即可。
    ------------------------------

        - SomeBottle 2023.1.30

*/

/*
7-40 奥运排行榜
    每年奥运会各大媒体都会公布一个排行榜，但是细心的读者发现，不同国家的排行榜略有不同。比如中国金牌总数列第一的时候，中国媒体就公布“金牌榜”；而美国的奖牌总数第一，于是美国媒体就公布“奖牌榜”。如果人口少的国家公布一个“国民人均奖牌榜”，说不定非洲的国家会成为榜魁…… 现在就请你写一个程序，对每个前来咨询的国家按照对其最有利的方式计算它的排名。

输入格式:
    输入的第一行给出两个正整数N和M（≤224，因为世界上共有224个国家和地区），分别是参与排名的国家和地区的总个数、以及前来咨询的国家的个数。为简单起见，我们把国家从0 ~ N−1编号。之后有N行输入，第i行给出编号为i−1的国家的金牌数、奖牌数、国民人口数（单位为百万），数字均为[0,1000]区间内的整数，用空格分隔。最后面一行给出M个前来咨询的国家的编号，用空格分隔。
输出格式:
    在一行里顺序输出前来咨询的国家的【排名:计算方式编号】。其排名按照对该国家最有利的方式计算；计算方式编号为：金牌榜=1，奖牌榜=2，国民人均金牌榜=3，国民人均奖牌榜=4。输出间以空格分隔，输出结尾不能有多余空格。

    若某国在不同排名方式下有相同名次，则输出编号最小的计算方式。
输入样例:
    4 4
    51 100 1000
    36 110 300
    6 14 32
    5 18 40
    0 1 2 3
输出样例:
    1:1 1:2 1:3 1:4
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/