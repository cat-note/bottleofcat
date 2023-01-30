#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>
#define TOLERANCE 1e-6

using namespace std;

// 表示一个国家
struct Country
{
    int gold;       // 金牌总数
    int medal;      // 奖牌总数
    int population; // 国民总数
};

vector<Country> countries; // 储存国家

bool doubleEquals(double a, double b); // 判断浮点数a和b是否相等

template <typename T>
class Greater
{
public:
    bool operator()(const T a, const T b) const
    {
        return a > b;
    }
};

// 为了方便unique方法，特地写个浮点数比较类
struct Double
{
    double val;
    bool operator>(const Double &db) const
    {
        return val > db.val;
    }
    bool operator==(const Double &db) const
    {
        return doubleEquals(val, db.val);
    }
    bool operator!=(const Double &db) const
    {
        return !doubleEquals(val, db.val);
    }
};

int main()
{
    int countryNum, queryNum; // 国家数和查询数
    scanf("%d %d", &countryNum, &queryNum);
    countries.resize(countryNum);
    vector<int> goldOrder(countryNum);   // 按金牌数降序排序的序列
    vector<int> medalOrder(countryNum);  // 按奖牌数降序排序的序列
    vector<Double> goldPer(countryNum);  // 按人均金牌数降序排序的序列
    vector<Double> medalPer(countryNum); // 按人均奖牌数降序排序的序列
    // 读入国家数据
    for (int i = 0; i < countryNum; i++)
    {
        Country ct;
        scanf("%d", &ct.gold);
        scanf("%d", &ct.medal);
        scanf("%d", &ct.population);
        countries[i] = ct;
        // 排序用的序列中储存的是国家编号
        goldOrder[i] = ct.gold;
        medalOrder[i] = ct.medal;
        goldPer[i] = Double{(double)ct.gold / ct.population};
        medalPer[i] = Double{(double)ct.medal / ct.population};
    }
    // 按照不同标准分别进行降序排序
    sort(goldOrder.begin(), goldOrder.end(), Greater<int>());
    sort(medalOrder.begin(), medalOrder.end(), Greater<int>());
    sort(goldPer.begin(), goldPer.end(), Greater<Double>());
    sort(medalPer.begin(), medalPer.end(), Greater<Double>());
    // 去重
    unique(goldOrder.begin(), goldOrder.end());
    unique(medalOrder.begin(), medalOrder.end());
    unique(goldPer.begin(), goldPer.end());
    unique(medalPer.begin(), medalPer.end());
    // 输出最优计算方法和排名
    for (int i = 0; i < queryNum; i++)
    {
        int orderBy = 0; // 计算方式编号
        int ranking = 0; // 排名
        short queryInd;  // 查询的国家编号
        scanf("%hd", &queryInd);
        // 先找到待查询国家的四项指标
        int queryGold = countries[queryInd].gold;
        int queryMedal = countries[queryInd].medal;
        double queryGoldPer = (double)countries[queryInd].gold / countries[queryInd].population;
        double queryMedalPer = (double)countries[queryInd].medal / countries[queryInd].population;
        // 同时扫描四个序列
        for (int j = 0; j < countryNum; j++)
        {
            ranking = j + 1; // 排名
            if (goldOrder[j] == queryGold)
            {
                orderBy = 1; // 金牌榜
                break;
            }
            if (medalOrder[j] == queryInd)
            {
                orderBy = 2; // 奖牌榜
                break;
            }
            if (doubleEquals(goldPer[j].val, queryGoldPer))
            {
                orderBy = 3; // 国民人均金牌榜
                break;
            }
            if (doubleEquals(medalPer[j].val, queryMedalPer))
            {
                orderBy = 4; // 国民人均奖牌榜
                break;
            }
        }
        if (i != 0)
            printf(" "); // 不输出多余空格
        printf("%d:%d", ranking, orderBy);
    }
    return 0;
}

// 判断两浮点数是否相等
bool doubleEquals(double a, double b)
{
    // 只要浮点数相差在误差范围内就认为相等
    return abs(a - b) < TOLERANCE;
}
