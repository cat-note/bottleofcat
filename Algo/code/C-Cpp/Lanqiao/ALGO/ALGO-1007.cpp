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

    4. 每买了一个【与其他印章都不同的】印章，下一回取【与已取印章都不同的】印章的概率就会【变小了】

    程序思路：
        - 用一个数组【buyStatus】记录每一步的购买情况

        - 如果购买次数buyNum不及印章总数badgeNum，则【不可能收集全印章】，直接打印0.0000

        - 其余情况，进入递归，递归从【第二次购买】开始，思路类似DFS。
            每一个递归深度(这里递归深度就是购买次数)中：

            1. 如果【未收集到的印章数<=剩余的购买次数】，说明还有可能集齐

                - 概率P=0

                - 先设定当前这次购买中买到了【新种类】的印章 (buyStatus数组相应元素设为true)

                - 在当前买到了新种类的情况下继续进入递归的更深层。

                - 上面一条递归过程结束，返回这种情况下【可行的购买方式】的概率，加入到P中。

                - 回溯：设定当前这次购买中【没有买到】新种类的印章 (buyStatus数组相应元素设为false)

                - 在当前没有买到新种类印章的情况下进入递归更深层

                - 上面一条递归过程结束，返回这种情况下【可行的购买方式】的概率，加入到P中。

                - 返回概率P

            2. 如果【买到的印章的种类=印章的种类】，说明已经集齐了，【到达了当前递归最深层D】(D从0开始) (最后一次购买对应数组下标为D-1)

                - 此时buyStatus数组记录了从第二次到最后一次的购买情况：true则代表某一次买到了新种类印章，false则代表买到了已有的印章

                    * (最开始买到每种印章的概率是 badgeP = 1/badgeNum )

                - 从数组第二个元素开始遍历到最后一次购买对应的元素(下标范围:[1,D-1])，
                    初始概率P=1，
                    初始买到【新种类印章】的概率 diffP=(badgeNum-1)*badgeP  (因为第一次我已经买了一个印章，要买到不同的印章，概率为 剩余印章数*badgeP)
                    初始买到【相同种类印章】的概率 sameP=1*badgeP (要买到和第一次相同的印章，概率只有badgeP)

                        1. 每遍历到一个true，说明买到了新种类印章

                            - 概率P先乘上diffP(这回买到新种类印章的概率)

                            - 然后，diffP减去一个badgeP(下次再买到新种类印章的概率变小了)，而sameP加上一个badgeP(下次买到已有印章的概率增大)

                        2. 每遍历到一个false，说明买到了已有种类的印章

                            - 概率P乘上sameP(这回买到相同种类印章的概率)

                - 返回概率P，这是当前【可行的购买方式】的总概率

            3. 如果【未收集到的印章数>剩余的购买次数】，说明不可能集齐了

                - 递归不再进入更深层，返回这种购买方式达成目的的概率为0

        - SomeBottle 2022.11.29

        动态规划的题很难用语言完全表述清楚。


*/