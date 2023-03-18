#include <cstdio>
#include <algorithm>

using namespace std;

int main()
{
    int bound, N;
    scanf("%d %d", &bound, &N);
    int souvenirs[N];
    for (int i = 0; i < N; i++) // 读入纪念品价格
        scanf("%d", &souvenirs[i]);
    sort(souvenirs, souvenirs + N); // 将纪念品按价格升序排序
    int groupNum = 0;               // 分组数
    int currSum = 0;                // 当前组内的价格之和
    int front = 0, rear = N - 1;
    while (front <= rear)
    {
        if (currSum + souvenirs[front] > bound)
        {
            groupNum++;
            currSum = 0;
        }
        currSum += souvenirs[front];
        front++;
        while (front < rear && currSum + souvenirs[rear] > bound)
        {
            groupNum++;
            rear--;
        }
        if (front < rear)
        {
            currSum += souvenirs[rear];
            rear--;
        }
    }
    if (currSum != 0) // 最后未填满一组的也要算入
        groupNum++;
    printf("%d", groupNum);
    return 0;
}