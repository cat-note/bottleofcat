#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

// 表示一位农民
struct Peasant
{
    int price;  // 单价
    int amount; // 一天最多能提供的牛奶量
    bool operator<(const Peasant &obj)
    {
        return price < obj.price;
    }
};

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);
    vector<Peasant> providers(m);
    for (int i = 0; i < m; i++)
        scanf("%d %d", &providers[i].price, &providers[i].amount);
    sort(providers.begin(), providers.end()); // 按牛奶单价升序排序，让定价低的农民先提供牛奶
    int cost = 0;                             // 总费用
    for (int i = 0; i < m; i++)
    {
        if (n >= providers[i].amount) // 如果剩余的需求量比当前农户提供的量大
        {
            n -= providers[i].amount; // 这位农户提供出一天能卖出的所有量
            cost += providers[i].amount * providers[i].price;
        }
        else // 剩余需求量不足当前农户提供的量
        {
            cost += n * providers[i].price; // 从这位农户这买n份牛奶即可。
            break;
        }
    }
    printf("%d", cost);
    return 0;
}

/*
    这题是比较基础的贪心算法题，每次决策的时候只【找当前定价最低】且牛奶尚有余量的农户。

        - SomeBottle 2023.3.12
*/