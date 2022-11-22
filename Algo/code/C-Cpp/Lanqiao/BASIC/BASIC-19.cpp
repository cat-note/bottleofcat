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
    int middleInd = (strLen - 1) / 2;
    // 字符串长度是否是奇数
    bool isOdd = strLen & 1;
    // 是否已经存在【落单】的字符，在回文串【长度为奇数】时，【有且只能】有一个落单字符
    bool hasAlone = false;
    // symmetry指向【结果中】与【former指向的字符】对称的字符下标
    int symmetry = strLen - 1;
    // former指向一个字符，从0扫描到字符串【中间】
    for (int former = 0; former <= middleInd; former++)
    {
        // latter指向另一个字符，从symmetry扫描到former指向的字符
        for (int latter = symmetry; latter >= former; latter--)
        {
            if (latter == former)
            { // 注：只有当字符串为【奇数长度】时可能出现这种情况
                // 如果latter和former指向的是同一个字符，说明这个字符【落单】了
                if (isOdd && !hasAlone)
                {
                    // 不用刻意移动这个字符，只需要把落单字符【移动到正中间的交换次数】加上即可
                    // 结果只需要一个数字罢了
                    swapCount += (middleInd - former);
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
                symmetry--; // 这个时候才前移与former对称的下标，这样可以【暂且忽视落单的字符】
                break;
            }
        }
    }
    printf("%d", swapCount);
    return 0;
}
/*
    ※ 我第一次写的错误解：https://gist.github.com/SomeBottle/b02a4ea15dd970b20017fc86d22729d0
    这题采用的是贪心策略，但最后要得到一个【最少】的交换次数
    既然采用了贪心策略，每一步要做的事其实就是把【当前字符】的相应字符【移动到对称的位置上】
        【例1】比如abbca：
            1. a和(第二个)a对称了，不用交换操作
            2. b和b不对称，要把第二个b交换到c所在的位置才能对称 (abbca->abcba)
            3. 完成上面第2步后，右侧的c被“挤”到了中间，c是唯一的落单字符，【必须处于中间】，已满足，不需要交换
            4. 程序结束，只做了1次交换
        【例2】比如abccadd：
            1. a和(第二个)a不对称，需要把第二个a交换到最后一个位置才能对称 (abccadd->abccdda, 交换2次)
            2. b是【唯一的落单】字符，加上这是【奇数长度】的字符串，最后【b要处于最中间】，
                计算b移动到中间的交换次数（【并不真的移动b】） (假设abccdda->accbdda，要交换2次)
            3. (此时的字符串：abccdda) c和c不对称 ，因为【忽略了b的移动】，需要把第二个c交换到【倒数第二个d的位置才能对称】 (abccdda->abcddca, 交换2次) 
            4. 接下来扫描到从左至右第一个d，发现和第二个d已经对称了，不需要交换
            5. 到目前为止，a和c各经历了2次交换，共4次。最后将b移动到正中央(abcddca->acdbdca, 交换2次(第2步中计算的结果))
            6. 最终整个字符串经历了4+2=6次交换
        【例3】再比如abdcdcab
            1. a和(第二个)a不对称，第二个a交换到最后一位才行 (abdcdcab->abdcdcba, 交换1次)
            2. b和b对称了，无需进行交换 (abdcdcba)
            3. d和d不对称，需要将第二个d交换到第二个c的位置 (abdcdcba->abdccdba, 交换1次)
            4. c和c已经对称了，无需进行交换
            5. 程序结束，a和d各交换了1次，整个字符串共经历2次交换
        【例4】比如mamdc
            1. m和(第二个)m不对称，第二个m交换到最后一位才行 (mamdc->madcm, 交换1次)
            2. a字符【落单了】，计算a字符交换到正中心需要的次数，【但不真的移动a】 (假设madcm->mdacm, 需要经历1次交换)
            3. (此时的字符串:madcm) d字符【也落单了】，【奇数长】的字符串中【出现了>1个的落单字符】，说明【不可能形成回文串】
            4. 返回Impossible
        【例5】比如faqaaf
            
        【例6】比如afaadaa
                
    从上面这个例子可以看出来
*/
/*
    这题我自己想的几个测试用例：
    6
    chhcdd

    10
    abbeccdade

    7
    abccadd

    7
    accaddb

    7
    ccaabbd

    7
    ewfwsea
*/