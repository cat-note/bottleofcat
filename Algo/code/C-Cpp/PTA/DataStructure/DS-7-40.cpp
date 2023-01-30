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
    /* TEST
    for (int i = 0; i < countryNum; i++)
    {
        printf("COUNTRY %d:\n", i);
        printf("\t GOLD ORDER: %d\n", countries[i].rank[0]);
        printf("\t MEDAL ORDER: %d\n", countries[i].rank[1]);
        printf("\t GOLDAVG ORDER: %d\n", countries[i].rank[2]);
        printf("\t MEDALAVG ORDER: %d\n", countries[i].rank[3]);
    }*/
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