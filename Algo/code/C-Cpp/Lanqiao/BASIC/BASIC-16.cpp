/*
蓝桥杯基础练习VIP-分解质因数
时间限制: 1s 内存限制: 128MB

题目描述
    求出区间[a,b]中所有整数的质因数分解。 
    *提示
        先筛出所有素数，然后再分解。
        数据规模和约定
输入格式
    输入两个整数a，b。 
    2<=a<=b<=10000
输出格式
    每行输出一个数的分解，形如k=a1*a2*a3...(a1<=a2<=a3..., k也是从小到大的)(具体可看样例) 
样例输入
    3 10
样例输出
    3=3
    4=2*2
    5=5
    6=2*3
    7=7
    8=2*2*2
    9=3*3
    10=2*5
*/
#include <iostream>
#include <map>
#include <vector>
#include <cmath>

using namespace std;
// 质因数，顾名思义就是质数因子。值得一提的是，质数亦称素数，因此本题重要的地方在于【找出素数】
int main()
{
    int from;
    int to;
    cin >> from >> to;
    map<int, bool> primeMap; // 储存是否是素数
    vector<int> primeNums;   // 储存素数数组
    // 无论怎样，这题涉及的质数只会在[2,to]之间出现，那么就先把这之间的质数给揪出来吧
    for (int num = 2; num <= to; num++)
    {
        int squareRoot = (int)sqrt((double)num);
        bool isPrime = true;
        for (int factor = 2; factor <= squareRoot; factor++)
        {
            // 只要找到了因数，就不是质数
            if (num % factor == 0)
            {
                isPrime = false;
                break;
            }
        }
        if (isPrime)
        {
            primeMap[num] = isPrime;  // 标记num为素数(质数)
            primeNums.push_back(num); // 将素数加入素数数组
        }
    }
    // 开始分解质因数
    for (int num = from; num <= to; num++)
    {
        cout << num << "=";
        if (primeMap[num])
        {
            // 如果num就是质数，直接输出即可
            cout << num;
        }
        else
        {
            int quotient = num;
            // 商不为1说明没有分解完毕
            while (quotient != 1)
            {
                // 直接拿所有[2,to]范围内的质数来试，找到其中【第一个】当前quotient的因子
                for (int i = 0, len = primeNums.size(); i < len; i++)
                {
                    if (quotient % primeNums[i] == 0)
                    {
                        quotient /= primeNums[i]; // 算出商quotient留到下一次迭代进行运算
                        cout << primeNums[i];
                        break;
                    }
                }
                // 没有分解完毕就输出乘号
                if (quotient != 1)
                    cout << "*";
            }
        }
        // 输出完了一行分解结果，换行
        cout << "\n";
    }
    return 0;
}
/*
    这题主要的考点其实还是【找素数】（质数也就是素数）
        * 素数or质数指的是【除了1和其本身外】没有其他因数的数。
    首先可以确定的是：
        既然要对[from,to]这个范围内的每个数做【质因数】分解，那么肯定会用到[2,to]范围内的质数
        ( 因为所谓质因数分解，简单来说就是把一个数拆解成【多个质数(素数)相乘的式子】)

    所以我先把[2,to]范围内的质数全找出来, 分别用：
        - primeMap(键值对)储存某个数【是否是质数】
        - 另外用primeNums数组【储存所有[2,to]范围内的质数】
        --------------
        💡 关于找素数的捷径：
                                                                                                        
            因为任何数拆成两个因数，这两个因数【不可能全都】大于这个数的平方根，所以对于正整数number，只要[2,√number)]内没有任何number的因数，number就是素数。

            假设有两个因数a,b，且a*b=x                 
                                               a     √x
                - 等式两边同时除以 b*√x ，得到  ── =  ──
                                              √x     b
                - 要维持等式成立，如果a<=√x  , 则b>=√x ；若a>=√x  , 则b<=√x
                                                                
                - 因此如果某个数x有除1和其自身之外的因数的话，在<=√x 这个范围内肯定能找到一个因数

    接下来扫描[from,to]范围内的数:

        - 如果【当前数num】在primeMap中已经被标记为【质数(素数)】，那么直接打印num=num

        - 其他情况，遍历存放所有素数的数组primeNums，一个一个尝试【当前数num】是否能【被某个质数】除尽，如果可以则说明【找到了一个质因数】
            将num除以这个质因数后得到的商quotient也是【num的一个因数】
            接下来将quotient带入下一次迭代，寻找quotient的第一个质因数...
            将得到的这些质因数用乘号连起来就形成了这个数num的质因数分解式。

            比如：
                primeNums数组：[2,3,5,...]  (不包含100)
                primeMap有如下几对键值对：{2:true} {3:true} {5:true} ... (不包含{100:true})

                1. 如果要分解的数num=5，首先查询primeMap[num]发现primeMap[5]=true, 5已经是质数
                    直接输出：5=5

                2. 如果要分解的数num=100, 首先查询primeMap[num]发现primeMap[100]=0(false), 100不是质数
                    那么就遍历primeNums数组，直到质数2：
                        - 100 / 2 = 50 <- 商quotient       质数2可以整除100，输出2*
                    再遍历primeNums数组，直到质数2：
                        - 50 / 2 = 25                      质数2可以整除50，输出2*
                    再遍历primeNums数组，直到质数5：
                        - 25 / 5 = 5                       质数5可以整除25，输出5*
                    再遍历primeNums数组，直到质数5：
                        - 5 / 5 = 1 <- 商为1，迭代结束      质数5可以整除5，输出5 (迭代结束，尾部不输出星号)
                    最后得到分解式：
                        - 100 = 2*2*5*5
        本题思路主要如上

        - SomeBottle 2022.11.26

*/