#include <iostream>
using namespace std;
/*
    谢谢你，让我回忆起了小学学的多位数的具体读法😂
    (真没想到还会考到这类知识)
    别看这道题标的是“简单题”，这逻辑写出来要想一遍AC，真的不容易...挺绕的
    我认为主要需要注意到的几个地方：
        1. 从低位向高位，四位【作为一级】进行处理，高位最后可能不满四位
            - 注：每一级的四个位我还是分别称作【千百十个】
        2. 某【一级】如果全为0(四个0)，就不读出来，比如20 0000 0000读作二十亿
        3. 处理【某一级】的【连续的0】时：
            a. 如果是【从个位开始的】连续0，直接忽略不读，比如：
                * 1100 读作 一千一百而不是一千一百零
            b. 【不是从个位开始的】连续0，只当作一个0读出来，比如：
                * 1 0001 读作 一万零一
                * 1 0010 读作 一万零一十
        4. 处理高位的时候可能不满4个数位，比如 12 0000，按四位分割后高位只有12
            当剩余的位数最多就到【十位】的时候，且十位上的数值是1，就读成【十...】，比如：
                * 12 0000 读成 十二万 而不是 一十二万
                * 19 读成 十九 而不是 一十九
    - SomeBottle 2022.11.20
*/

string readDigits(string digits);
// 0-9到拼音的映射
string numMap[10]{"ling ", "yi ", "er ", "san ", "si ", "wu ", "liu ", "qi ", "ba ", "jiu "};
// 个十百千位的拼音（“个”不用读出来，放个空字符串在这）
string digitNames[4]{"", "shi ", "bai ", "qian "};
// 级的拼音，主要是万、亿。千及以下是不用在后面加后缀的，留为空字符串
string levelNames[3]{"", "wan ", "yi "};
int main()
{
    string number;
    cin >> number; // 以字符串的形式读入
    string result = "";
    int numLen = number.length(); // 数字位数
    /*
        用中文将这个数字读出来的时候，是按每四个数位来读为一级的
        这一部分做的事就是将数字拆为四位一读，比如：1235123412
            1. 先处理最后四位3412
            2. 再处理中间四位3512
            3. 最后处理高位剩下的两位12
    */
    // level是当前的级别（千及以下level=0，万level=1，亿level=2）
    for (int i = numLen, level = 0; i > 0; i -= 4, level++)
    {
        int startPos = i - 4; // 从i-4开始读4位
        int readFor = 4;      // 读四位
        if (startPos < 0)
        {
            readFor += startPos; // 改变读取的位数(因为这里不满四位)
            startPos = 0;        // 防止越界
        }
        // 四位一读
        string fourDigits = readDigits(number.substr(startPos, readFor));
        // 如果readDigits返回了空字符串，说明这一级省略不读了
        // 比如20 0000 0000，应该读成二十亿而不是二十亿万
        if (fourDigits.length() != 0)
            fourDigits += levelNames[level];
        result = fourDigits + result;
    }
    cout << result << endl;
    return 0;
}

/*
    四位一读，这里列几个典型例子
        7301 七千三百零一
        7009 七千零九
        7010 七千零一十
        0010 零一十(前面还有两个0，说明高位还有非零位)
          10 十（10是最高的两位，就读十）
*/
string readDigits(string digits)
{
    // ----------------------💡特殊情况1：字符串全为0，不读这一级，返回空字符串，比如 10 0000直接就读十万（个十百千位不读）
    if (digits == "0000")
    {
        return "";
    }
    // ----------------------💡其他情况，需要注意多个零只读一个
    string result = "";
    int digitsNum = digits.length(); // 数字位数，除最高的几位外，全都是4
    // 仍然是从低位向高位读
    // i是从digits.length() - 1到0
    // 而pos是从0到digits.length()-1，代表 个十百千 位
    bool justZero = false; // 上一次读到的是不是0，借此实现多个0只读一个
    for (int i = digitsNum - 1, pos = 0; i >= 0; i--, pos++)
    {
        short currentDigit = (short)(digits[i] - '0'); // 当前数位的值(0-9)
        string temp = "";
        // ---------------------💡特殊情况2：如果字符串最高位是十位，直接读成“十”，比如12 0100，读成十二万零一百；12读成十二而不是一十二
        // digitsNum=2说明只有十、个位可读
        // pos=1说明正在读十位
        // currentDigit=1说明十位上的数字是1
        if (!(digitsNum == 2 && pos == 1 && currentDigit == 1))
            temp += numMap[currentDigit]; // 获得数值的拼音
        if (currentDigit == 0)            // 读到了0
        {
            // 💡【处理多个0】的情况1：如果【上一次读到的是0】，这次还是0，就跳过, 比如1001读成一千零一，而不是一千零零一
            if (justZero)
                continue;
            justZero = true; // 否则标记justZero=true，【这一次读到的是0】
            // 💡【处理多个0】的情况2：如果是从个位开始的连续0，就不读，比如1100读成一千一百而不是一千一百零
            if (pos == 0) // pos=0, 说明是个位
                continue; // 个位是0，直接跳过，也就是从个位开始的连续0不读
        }
        else
        {
            justZero = false; // 标记【这一次读到的不是0】
            // 如果是0，就不用加后缀，比如1010读成一千零一十而不是一千零百一十
            // 不是0就要加上后缀：
            temp += digitNames[pos];
        }
        result = temp + result;
    }
    return result;
}