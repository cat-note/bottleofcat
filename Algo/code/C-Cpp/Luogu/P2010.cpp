#include <cstdio>

using namespace std;

bool isLeap(int year);   // 判断是否是闰年
int chrToInt(char chr);  // 字符转换为一位整数
int getYear(char *date); // 从日期中提取年份
int dateInt(char *date); // 将日期转换为8位数整型

int main()
{
    char startDate[9], endDate[9]; // 起始日期和结束日期
    scanf("%s %s", startDate, endDate);
    int startYear = getYear(startDate), startInt = dateInt(startDate);
    int endYear = getYear(endDate), endInt = dateInt(endDate);
    int lastFour[4];                                                         // 临时储存日期的后四位
    int count = 0;                                                           // 统计有多少回文日期
    int monthDays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // 每月的天数（非闰年时）
    for (int y = startYear; y <= endYear; y++)                               // 枚举年份
    {
        int temp = y;
        for (int i = 0; i < 4; i++) // 年份倒转过来作为后四位
        {
            lastFour[i] = temp % 10;
            temp /= 10;
        }
        int month = lastFour[0] * 10 + lastFour[1];   // 月份
        int day = lastFour[2] * 10 + lastFour[3];     // 天数
        int currDate = y * 10000 + month * 100 + day; // 整型日期
        if (currDate < startInt || currDate > endInt) // 超出了日期范围
            continue;
        if (month > 12) // 超过12月了
            continue;
        if (isLeap(y) && month == 2)
        {
            if (day > 29) // 闰年的时候如果2月超过了29天就不合法
                continue;
        }
        else if (day > monthDays[month]) // 天数超过了这个月的上限
        {
            continue;
        }
        // 日期合法，说明是正确的回文数字，统计在内
        count++;
    }
    printf("%d", count);
    return 0;
}

bool isLeap(int year)
{
    return year % 100 == 0 && year % 400 == 0 || year % 4 == 0;
}

inline int chrToInt(char chr)
{
    return (int)(chr - '0');
}

inline int getYear(char *date)
{
    return chrToInt(date[0]) * 1000 + chrToInt(date[1]) * 100 + chrToInt(date[2]) * 10 + chrToInt(date[3]);
}

inline int dateInt(char *date)
{
    return getYear(date) * 10000 + chrToInt(date[4]) * 1000 + chrToInt(date[5]) * 100 + chrToInt(date[6]) * 10 + chrToInt(date[7]);
}

/*
    可以发现，回文日期其实和【年份】的关系很大。

    因此这里我主要枚举的是年份，把年份倒转过来作为后面四位看合不合法即可。
    这里我主要将日期给转换为数值进行处理了。

    需要注意的点如下:

    1. 年份，月份和天数还不能超出两个日期形成的范围。
    2. 月份不可能超过12月
    3. 闰年的时候2月天数不可能超过29天
    4. 其他情况，所有月的天数都不能超过其上限

        - SomeBottle 2023.2.24
*/