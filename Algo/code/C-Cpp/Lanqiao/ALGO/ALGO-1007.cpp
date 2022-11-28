#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

vector<bool> buyStatus; // 购买情况，true代表买到了【不同于已有】种类的徽章
short badgeNum;         // 总共有多少种印章
short buyNum;           // 买了多少张印章
double badgeP;          // 取到一个印章的概率
// 函数声明
double buy(short diffNum, short depth);

int main()
{
    cin >> badgeNum >> buyNum;
    buyStatus.resize(buyNum);
    buyStatus[1] = true; // 第一次买的印章其实不用管
    badgeP = 1.0 / badgeNum;
    if (buyNum < badgeNum) // 不可能集齐的情况
    {
        printf("0.0000");
    }
    else
    {
        printf("%.4f", buy(1, 1)); // 忽略第一次买的概率，diffNum=1代表已经得到1种印章，depth=1代表从第二次购买开始处理(数组元素下标为1)
    }
    return 0;
}

double buy(short diffNum, short depth)
{
    double probability = 0;
    if (diffNum >= badgeNum)
    {
        probability = 1;
        double diffBadge = (badgeNum - 1) * badgeP;
        double sameBadge = 1 * badgeP;    // 买到相同印章的概率
        for (short i = 1; i < depth; i++) // 从第二次购买开始计算
        {
            if (buyStatus[i]) // 每买到一个【不同于所有已有印章种类】的印章
            {
                probability *= diffBadge;
                diffBadge -= badgeP; // 减少下次买到【不同于所有已买到的印章种类的印章】的概率
                sameBadge += badgeP; // 增加下次买到【已买到的种类的印章】的概率
            }
            else
            {
                probability *= sameBadge;
            }
            // cout << (buyStatus[i] ? 1 : 0) << " ";
        }
        // cout << "\n";
    }
    else if (badgeNum - diffNum <= buyNum - depth) // 未收集到的印章数<=剩余的购买次数，说明还有可能收集所有的印章
    {
        buyStatus[depth] = true; // 先假定当前买到的徽章
        probability += buy(diffNum + 1, depth + 1);
        buyStatus[depth] = false;
        probability += buy(diffNum, depth + 1);
    }
    return probability;
}

/*
    (badgeNum=N,  buyNum=M)
    1. 印章总共有badgeNum种，最好的情况下也要买badgeNum次才能集齐，
        因此如果购买次数buyNum不及印章总数badgeNum，则【不可能收集全印章】

    2. 每一步购买的时候，我关心的是这次买到的印章的种类【是否不同于所有已买到的印章】，因此可以用一个buyStatus数组来记录购买的情况，
        数组每一个元素只有true/false两种可能，代表【某一次】买到的印章的种类【是否不同于所有已买到的印章】。

    3. 第一次买印章的概率可以忽略，上面第2条也说了，我关心的是是否买到了新种类印章。
        而第一次买的印章是没有对比对象的（因为此时【我已经拥有的印章种类为0个】），不会影响到最终概率。

            * 因此程序中也要注意，递归处理也是从第二次购买开始处理的。

    2. 递归思路：
        - 用一个数组【buyStatus】记录购买数量

    每取了一个【与其他印章都不同的】印章，下一回取【与已取印章都不同的】印章的概率就会变小了
*/