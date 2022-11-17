/*
蓝桥杯基础练习VIP-龟兔赛跑预测
时间限制: 1s 内存限制: 128MB 提交: 7812 解决: 2166

*/
#include <iostream>

using namespace std;

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
    int rabbitLazingUntil = -1; // 兔子得意忘形到多久的时候继续开始跑
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