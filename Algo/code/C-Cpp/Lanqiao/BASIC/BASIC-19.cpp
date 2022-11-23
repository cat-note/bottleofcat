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
            2. b是【落单】的字符，加上这是【奇数长度】的字符串，最后【b要处于最中间】，
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
            4. 输出Impossible
        【例5】比如faqaaf
            1. f和(第二个)f已经对称了，无需进行交换 (faqaaf)
            2. a和倒数第一个a已经对称了，无需进行交换
            3. q字符【落单】了，但这是【偶数长】的字符串，因此【不可能形成回文串】
            4. 输出Impossible
        【例6】比如afaaaaa
            1. 第一个a和(倒数第一个)a已经对称了，无需进行交换
            2. f是【落单字符】，接下来【并不真的移动f】，而是【计算f交换到正中间】要经历的交换次数。
                (假设afaaaaa->aaafaaa, 需要经历两次交换)  
            3. (此时的字符串:afaaaaa) 第二个a字符和倒数第二个a字符已经对称，无需交换 (afaaaaa)
            4. 第三个a字符和倒数第三个a字符已经对称，无需交换 (afaaaaa)
            5. 综上，整个字符串中只用【把落单字符交换到正中间】即可，共经历2次交换。

    🤔 从上面这些例子可以看出来几条比较关键的思路：
        1. 程序需要分别用两个变量former, symmetry来解决这个问题，former从字符串首部开始扫描到【字符串正中间】，而symmetry代表【和former】对称的下标
            每次former扫描到一个【非落单】字符，会额外定义一个变量latter，latter从symmetry开始【向字符串前面扫描】，直到找到【和former指向的字符相同】的字符 ，
            接着，latter指向的这个字符会被交换直到symmetry下标这里。
            接着symmetry-1，former+1，整体看下来，former和symmetry呈一种【从两边向中间夹】的动态。
                这个过程还是得举个例子，比如字符串 mamacc：
                    [1] m a m a c c - 此时former指向首个m，symmetry是和former相对称的下标，指向最后一个c
                        ↑         ↑     (前一个箭头是former, 后一个箭头是symmetry)
                    [2] m a m a c c - latter从symmetry开始向前找，找到了第二个m
                        ↑   ↑     ↑     (中间的一个箭头是latter)
                    [3] m a a c c m - m从latter指向的位置交换移动到symmetry指向的位置 (经历3次交换)
                        ↑         ↑     (latter带着m不停交换到symmetry, 此时latter和symmetry重合)
                    [4] m a a c c m - former+1且symmetry-1，借此保持symmetry和former对称
                        ↑     ↑  
                    [5] m a a c c m - 仍然是派出latter从symmetry开始向前，找到了第二个a
                        ↑ ↑   ↑       (中间一个箭头是latter)
                    [6] m a c c a m - latter带着字符a交换到symmetry的位置 (经历2次交换)
                        ↑     ↑
                    [7] m a c c a m - former+1且symmetry-1，可以发现c和c已经对称了，无需进行更多交换
                            ↑ ↑
                字符串共经历3+2=5次交换

        2. former只需要扫描到【字符串正中间】就够了，这多亏了回文串的【对称】结构。
            从上面的例子可以看出来，former每扫描到一个字符，程序都会设法将【另一个相同的字符】移动到symmetry这个对称的位置上
            【除了遇到落单字符】外，former和symmetry总是在一起变化的，【没有处理的字符】只会处于[former,symmetry]这个区间内
            而无论如何，总有symmetry<=former的一刻，而这一刻就在former【扫描到字符串正中间】后出现
                当symmetry<=former时，说明所有字符已经处理完毕。
        3. 对于【奇数长】的回文串来说，肯定【有且仅有一个】落单的字符，💡 且其在最后的回文串中一定会【位于正中间】
            问题来了，当former扫描到落单字符时，该怎么处理？
                ——答案是：略过，只计算【将落单字符移动到字符串正中间】需要的【交换次数】
            为什么要这样做？要是我先将这个落单字符交换到了正中间，可能在处理其他字符时，【处于正中间的落单字符】会被【再次移动】，处理起来会麻烦很多！
                来，举个【反例】，对于字符串abccadd:
                    [1] abccadd - former指向首个字符a, symmetry指向最后一个字符d
                        ↑     ↑
                    [2] abccdda - 这里略过latter找到字符a并移动的过程
                        ↑     ↑
                    [3] abccdda - former+1,symmetry-1，可以发现，former指向了落单字符b (因为latter向前找不到和b相同的字符)
                         ↑   ↑
                    [4] accbdda - 把b移动到正中间, former+1,symmetry-1，可以发现从这里开始就不对劲了，有个c字符被忽略了！
                          ↑ ↑
                    [5] 
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