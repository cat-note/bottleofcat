/*
蓝桥杯基础练习VIP-报时助手
时间限制: 1s 内存限制: 128MB 提交: 5658 解决: 2119

题目描述
    给定当前的时间，请用英文的读法将它读出来。
    时间用时h和分m表示，在英文的读法中，读一个时间的方法是：
    如果m为0，则将时读出来，然后加上“o'clock”，如3:00读作“three  o'clock”。
    如果m不为0，则将时读出来，然后将分读出来，如5:30读作“five  thirty”。
    时和分的读法使用的是英文数字的读法，其中0~20读作：
        0:zero,  1:  one,  2:two,  3:three,  4:four,  5:five,  6:six,  7:seven,  8:eight,  9:nine,  10:ten,  11:eleven,  12:twelve,  13:thirteen,  14:fourteen,  15:fifteen,  16:sixteen,  17:seventeen,  18:eighteen,  19:nineteen,  20:twenty。
    30读作thirty，40读作forty，50读作fifty。
    对于大于20小于60的数字，首先读整十的数，然后再加上个位数。如31首先读30再加1的读法，读作“thirty  one”。
    按上面的规则21:54读作“twenty  one  fifty  four”，9:07读作“nine  seven”，0:15读作“zero  fifteen”。
输入格式
    输入包含两个非负整数h和m，表示时间的时和分。非零的数字前没有前导0。h小于24，m小于60。
输出格式
    输出时间时刻的英文。
样例输入
    0 15
样例输出
    zero fifteen
*/

/*
    这题的题目给出了十分清晰的判断逻辑，整挺好。
    关于时间和英文单词的转换，我主要用了个map容器，可以说是用空间换时间了吧~
        - SomeBottle 2022.11.16
*/

#include <iostream>
#include <map>

using namespace std;

map<short, string> numMap; // 数字到英文单词的映射

string numStr(short num); // 声明函数

int main()
{
    short hour; // 读入时间
    short minute;
    cin >> hour >> minute;
    numMap[0] = "zero";
    numMap[1] = "one";
    numMap[2] = "two";
    numMap[3] = "three";
    numMap[4] = "four";
    numMap[5] = "five";
    numMap[6] = "six";
    numMap[7] = "seven";
    numMap[8] = "eight";
    numMap[9] = "nine";
    numMap[10] = "ten";
    numMap[11] = "eleven";
    numMap[12] = "twelve";
    numMap[13] = "thirteen";
    numMap[14] = "fourteen";
    numMap[15] = "fifteen";
    numMap[16] = "sixteen";
    numMap[17] = "seventeen";
    numMap[18] = "eighteen";
    numMap[19] = "nineteen";
    numMap[20] = "twenty";
    numMap[30] = "thirty";
    numMap[40] = "forty";
    numMap[50] = "fifty";
    if (minute == 0)
    { // 分钟为0，输出整点
        cout << numStr(hour) << " o'clock";
    }
    else
    { // 其他情况输出小时和分钟
        cout << numStr(hour) << " " << numStr(minute);
    }
    return 0;
}

/**
 * @brief 将数字转换为相应的字符串
 *
 * @param num 整数
 * @return string 字符串
 */
string numStr(short num)
{
    if (num <= 19)
    { // 先处理19以下的整数
        return numMap[num];
    }
    else
    {
        short unit = num % 10;        // 取得个位
        short tens = num - unit;      // 减去个位就是十的倍数
        string result = numMap[tens]; // 获得十的倍数的相关单词
        if (unit > 0)
        {
            result += " " + numMap[unit]; // 如果个位不为0就加上相关单词
        }
        return result;
    }
}