#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

/*
    分析样例能发现，本题要求的是总回报而不是净回报
        * 也就是说，算出来的结果【忽略倒贴】扣除的部分。
        * 可以理解成我尽量“避免”倒贴的情况，题目也没说一定要用完所有优惠券
*/

int main()
{
    vector<long> coupons; // 优惠券
    vector<long> prices;  // 商品价值
    int couponNum;        // 优惠券数量
    scanf("%d", &couponNum);
    // 读入优惠券
    for (int i = 0; i < couponNum; i++)
    {
        long cpVal;
        scanf("%ld", &cpVal);
        coupons.push_back(cpVal);
    }
    int goodsNum; // 商品数
    scanf("%d", &goodsNum);
    // 读入商品，免费和收费商品分开储存
    for (int i = 0; i < goodsNum; i++)
    {
        long gdVal;
        scanf("%ld", &gdVal);
        prices.push_back(gdVal);
    }
    // 对两部分分别进行快排(均排成升序)
    sort(coupons.begin(), coupons.end());
    sort(prices.begin(), prices.end());
    // 开始处理
    long result = 0; // 最大回报
    int cpStart = 0; // 优惠券数组[ cpStart,size() )
    int prStart = 0; // 商品价值数组[ prStart,size() )
    // 先处理负数部分，从前往后扫(因为是升序排列，尽量让负负得正的值更大)
    while (cpStart < coupons.size() && prStart < prices.size())
    {
        if (coupons[cpStart] < 0)
        {
            // 当前扫到一张负优惠券
            if (prices[prStart] < 0)
            {
                // 且还有负商品，负负得正，回报增加。
                result += coupons[cpStart] * prices[prStart];
                prStart++; // 数组起始下标后移
            }
            // 如果没有负商品，就忽略负优惠券
            cpStart++;
        }
        else if (prices[prStart] < 0)
        {
            // 优惠券是正的，但是商品是负价值，我不买不就完事了！
            // 这里忽略商品
            prStart++;
        }
        else
        {
            // 如果此时【优惠券和商品都只有正的了】，说明负数部分处理完成
            break;
        }
    }
    // 再处理正数部分
    // 此时优惠券的[ cpStart,size() )和商品价值[ prStart,size() )全为正
    // 从后往前扫，尽量让正正得正的值更大
    while (cpStart < coupons.size() && prStart < prices.size())
    {
        result += coupons.back() * prices.back();
        coupons.pop_back(); // 弹掉末尾元素
        prices.pop_back();
    }
    printf("%ld", result);
    return 0;
}

/*
    本题本质上考察的也是排序的应用。
    ---------------------------------
    💡 本题【没有说要用完所有优惠券】，只需要【求出最大回报】即可，【回避所有扣除的情况】。

    目前能想到的有两种思路:

        1. 快排+正向/逆向扫描处理

        2. 利用四个优先队列进行处理

    从测试效果上来看，第一种从耗时看来是更快的。
    -------------------------------------------
    这个题解我采用了第1种思路:

        1. 准备两个数组，分别读入数据:
            coupons - 储存所有优惠券面值
            prices - 储存所有商品价值

        2. 利用algorithm库的sort对两个数组的数据按【递增】序进行排序。

            * 排完后，序列头部是【绝对值最大的负数】，而序列尾部是【最大的正数】，就像这样:
                [Example]
                    coupons: -4 -2 1 4 7 8 9
                    prices: -7 5 6 8

        3. 先【从前往后扫描】，取出成对的负数(如果有的话)，负负得正加入结果:

                coupons: [-4] -2 1 4 7 8 9  -> -4*-7=28
                prices: [-7] 5 6 8

            如果有落单的负数就忽略:

                coupons: [-2] 1 4 7 8 9  -> 忽略-2
                prices: 5 6 8

            直到coupons和prices中【没有负数】为止。

        4. 此时coupons和prices中【只有正数】了，这个适合从尾部开始【成对取出正数并相乘】:

                coupons: 1 4 {7} <8> [9]
                prices: {5} <6> [8]

                9*8+8*6+7*5=155

            直到coupons和prices没有成对正数为止:

                coupons: 1 4
                prices: (空)

        5. 将负负得正和正正得正的结果相加:

            28+155=183 <- 得到结果
    ----------------------------------------
    第二种利用优先队列的思路就很明显了。

    可以看到上面快排的时候我是将【负数部分和正数部分分开处理】的，
    优先队列也是这样，不过还要细分一下，因为:

        - 对于【负数部分】需要【从小往大】成对取出，进行运算 -> 小根堆

        - 对于【正数部分】需要【从大往小】成对取出以进行运算 -> 大根堆

    可以发现，这里需要维护两种堆，而【每种堆】又需要2个来储存优惠券和商品的信息。

        * 因此一共要4个优先队列。

            - 负值优惠券 - 小根堆
            - 负值商品 - 小根堆
            - 正值优惠券 - 大根堆
            - 正值商品 - 大根堆

    接下来的操作就很简单了，把数据按照数值正负分别读入对应的堆中，然后就是【成对出堆】并进行计算了，这里就不多赘述。
    ---------------------------------
    注: 我写的这个还不算是最优解。

        - SomeBottle 2023.1.29

*/

/*
我自己想的一个测试例:
7 -4 1 -2 8 7 9 4
4 5 8 6 -7
输出结果:
183
*/