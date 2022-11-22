#include <cstdio>
#include <vector>

using namespace std;

int main()
{
    int strLen; // 字符串长度
    scanf(" %d", &strLen);
    vector<char> myStr(strLen); // 根据长度初始化字符vector长度
    char temp;
    for (int i = 0; i < strLen; i++)
    { // 从标准输入读入字符, 忽略前面的空白符\n
        scanf(" %c", &temp);
        myStr[i] = temp;
    }
    // 纪录交换次数
    int swapCount = 0;
    // 字符串中间的下标
    int middleInt = (strLen - 1) / 2;
    // 字符串长度是否是奇数
    bool isOdd = strLen & 1;
    // 是否已经存在【落单】的字符，在回文串【长度为奇数】时，【有且只能】有一个落单字符
    bool hasAlone = false;
    // symmetry指向【结果中】与【former指向的字符】对称的字符下标
    int symmetry = strLen - 1;
    // former指向一个字符，从0扫描到字符串尾部
    for (int former = 0; former < strLen; former++)
    {
        // latter指向另一个字符，从symmetry扫描到former指向的字符
        for (int latter = symmetry; latter >= former; latter--)
        {
            if (latter == former)
            { // 如果latter和former指向的是同一个字符，说明这个字符【落单】了
                if (isOdd && !hasAlone)
                {
                    // 不用刻意移动这个字符，只需要把落单字符【移动到正中间的交换次数】加上即可
                    // 结果只需要一个数字罢了
                    swapCount += (middleInt - former);
                }
                else
                {
                    printf("Impossible");
                    return 0;
                }
            }
            else if (myStr[latter] == myStr[former]) // 如果找到了相同的字符
            {
                // latter和former不是同一个字符，就把latter指向的字符移动到和former对称的symmetry处
                for (; latter < symmetry; latter++)
                {
                    char temp = myStr[latter + 1];
                    myStr[latter + 1] = myStr[latter];
                    myStr[latter] = temp;
                    swapCount++;
                }
                symmetry--;
                break;
            }
        }
    }
    printf("%d", swapCount);
    return 0;
}