/*
蓝桥杯基础练习VIP-龟兔赛跑预测
时间限制: 1s 内存限制: 128MB 提交: 7812 解决: 2166

题目描述
    话说这个世界上有各种各样的兔子和乌龟，但是  研究发现，所有的兔子和乌龟都有一个共同的特点——喜欢赛跑。于是世界上各个角落都不断在发生着乌龟和兔子的比赛，小华对此很感兴趣，于是决定研究不同兔  子和乌龟的赛跑。他发现，兔子虽然跑比乌龟快，但它们有众所周知的毛病——骄傲且懒惰，于是在与乌龟的比赛中，一旦任一秒结束后兔子发现自己领先t米或以  上，它们就会停下来休息s秒。对于不同的兔子，t，s的数值是不同的，但是所有的乌龟却是一致——它们不到终点决不停止。

    然而有些比赛相当漫长，全程观看会耗费大量时间，而小华发现只要在每场比赛开始后记录下兔子和乌龟的数据——兔子的速度v1（表示每秒兔子能跑v1米），乌龟的速度v2，以及兔子对应的t，s值，以及赛道的长度l——就能预测出比赛的结果。但是小华很懒，不想通过手工计算推测出比赛的结果，于是他找  到了你——清华大学计算机系的高才生——请求帮助，请你写一个程序，对于输入的一场比赛的数据v1，v2，t，s，l，预测该场比赛的结果。

输入格式
    输入只有一行，包含用空格隔开的五个正整数v1，v2，t，s，l，其中(v1,v2< =100;t< =300;s< =10;l< =10000且为v1,v2的公倍数)

输出格式
    输出包含两行，第一行输出比赛结果——一个大写字母“T”或“R”或“D”，分别表示乌龟获胜，兔子获胜，或者两者同时到达终点。
    第二行输出一个正整数，表示获胜者（或者双方同时）到达终点所耗费的时间（秒数）。
样例输入
    10 5 5 2 20
样例输出
    D
    4
*/
#include <iostream>

using namespace std;
/*
    小华你小子...真的懒得抽筋! 好好学学隔壁李华吧。
    本题实际上就是将整个比赛过程模拟一遍，鉴于数值规模并不大且正好跑道长度还是参赛者速度的公倍数，【一个循环加上一些加减/逻辑运算】就能解决了。
    * 乌龟一直在跑，我们可以先将其晾在一边(好无情！)
    * 小兔叽才是我们要着重关注的对象，因为这小崽子精得很，并不总是在跑。
        我通过一个记录变量rabbitLazingUntil来解决这个问题：
            - 兔子在跑的时候，rabbitLazingUntil标记为-1
            - 兔子超过乌龟metersAhead后，直接算出兔子要休息到什么时候(rabbitLazingUntil)
            - 当计时器到达rabbitLazingUntil时，将rabbitLazingUntil置为-1，从下一秒开始兔子接着跑
        - SomeBottle 2022.11.18
*/
int main()
{
    int rabbitPace;   // 兔子速度
    int tortoisePace; // 乌龟速度
    int metersAhead;  // 兔子领先多少米时得意忘形
    int lazeTime;     // 兔子得意忘形后休息的时长
    int totalLen;     // 跑道总长度
    cin >> rabbitPace >> tortoisePace >> metersAhead >> lazeTime >> totalLen;
    int rabbitProgress = 0;     // 兔子已经跑了多远
    int tortoiseProgress = 0;   // 乌龟已经跑了多远
    int time = 0;               // 耗费时间
    int rabbitLazingUntil = -1; // 兔子得意忘形直到哪个时刻（在这个时刻的下一秒，也就是rabbitLazingUntil+1，兔子会开跑）
    while (true)
    {
        time++;                           // 时间+1秒
        tortoiseProgress += tortoisePace; // 乌龟一直在跑
        if (rabbitLazingUntil == -1)
        { // 兔儿没偷懒
            rabbitProgress += rabbitPace;
            // 兔子发现自己超过了乌龟metersAhead或以上，开摆！
            if (rabbitProgress >= tortoiseProgress + metersAhead)
                rabbitLazingUntil = time + lazeTime; // 兔子一直摆到rabbitLazingUntil
        }
        else if (time >= rabbitLazingUntil)
        { // 时间到了rabbitLazingUntil，兔子在下一秒要开始继续跑了，置rabbitLazingUntil为-1
            rabbitLazingUntil = -1;
        }
        // 以下是赢家判断
        bool rabbitFinish = rabbitProgress == totalLen;     // 兔子到了终点
        bool tortoiseFinish = tortoiseProgress == totalLen; // 乌龟到了终点
        if (rabbitFinish && tortoiseFinish)
        {
            cout << "D\n";
            break;
        }
        if (rabbitFinish)
        {
            cout << "R\n";
            break;
        }
        if (tortoiseFinish)
        {
            cout << "T\n";
            break;
        }
    }
    cout << time << "\n";
    return 0;
}