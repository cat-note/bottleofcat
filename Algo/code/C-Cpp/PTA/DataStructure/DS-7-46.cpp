#include <cstdio>
#include <string>
#include <unordered_map>

using namespace std;

// 哈希表项，代表一个话题被访问的情况
struct TopicAccess
{
    int access;   // 话题访问次数
    int lastPost; // 最后一次访问的是哪一条微博(用于去重)
    TopicAccess()
    {
        access = 0;
        lastPost = -1;
    }
};

typedef unordered_map<string, TopicAccess> HashTable;

int main()
{
    int postNum;              // 微博条数
    scanf("%d%*c", &postNum); // 顺带取走行末的换行符
    // 开始读取微博
    HashTable table;           // 哈希表，话题字符串作为键
    bool readingTopic = false; // 标记是否正在读取#和#之间的话题
    bool readingWord = false;  // 标记是否正在读取【一个话题】中的单词，用于辅助空格分隔
    char strBuf[141];          // 临时字符数组
    int tmpLen = 0;            // 临时字符数组元素数
    int postId = 0;            // 当前读取的微博ID(用于去重)(0~N-1)
    string strTemp;            // 临时字符串
    while (postId < postNum)
    {
        char readChr; // 读入的字符
        int reader = scanf("%c", &readChr);
        if (readChr == '\n' || reader == EOF) // 读到了行末，说明一条微博读取完毕
        {
            postId++;
            continue;
        }
        if (readChr == '#') // 遇到#说明开始或结束读取一个话题
        {
            if (readingTopic)
            {
                // 一个话题读取结束
                strBuf[tmpLen] = '\0'; // 封装成字符串
                strTemp.assign(strBuf);
                TopicAccess &topic = table[strTemp];
                if (topic.lastPost != postId) // 判断上一次包含这个话题的微博是不是当前这条
                {
                    // 防止同一条微博出现多个相同的话题，去个重
                    topic.lastPost = postId;
                    topic.access++; // 话题访问次数增加
                }
                tmpLen = 0; // 清空字符串缓存
            }
            else // 开始读取一个话题
            {
                // 标记开始读取话题中的一个词
                readingWord = true; // 为true代表不在话题字符串头部输出空格
            }
            readingTopic = !readingTopic;
            continue;
        }
        if (readingTopic) // 正在读取当前微博中的【一个话题】
        {
            if (readChr >= 'A' && readChr <= 'Z') // 将大写字母全转换为小写
                readChr += ('a' - 'A');
            if (readChr >= 'a' && readChr <= 'z' || readChr >= '0' && readChr <= '9')
            {
                // 读入的是有效字符
                if (!readingWord)
                {
                    // 如果上一个读入的字符不是字母或数字，就填充一个空格
                    strBuf[tmpLen++] = ' ';
                    readingWord = true; // 标记正在读取一个单词
                }
                strBuf[tmpLen++] = readChr; // 读入字符
            }
            else
            {
                // 否则读入的是其他字符
                readingWord = false; // 标记不在读单词
            }
        }
    }
    // 开始处理输出
    string hottest;      // 最热门话题
    int referNum = 0;    // 被提及的次数
    int parallelNum = 0; // 并列被提及最多的热门话题数
    for (HashTable::iterator it = table.begin(); it != table.end(); it++)
    {
        if (it->second.access > referNum)
        {
            // 找到了被访问更多次的话题
            referNum = it->second.access;
            hottest = it->first;
            parallelNum = 0; // 此时并列数也要清零
        }
        else if (it->second.access == referNum)
        {
            // 并列数增加
            parallelNum++;
            if (it->first.compare(hottest) < 0)
            {
                // 如果这个并列的字典序更小，就优先输出它
                hottest = it->first;
            }
        }
    }
    // 输出结果
    if (hottest[0] >= 'a' && hottest[0] <= 'z') // 首字母大写
        hottest[0] -= ('a' - 'A');
    printf("%s\n%d\n", hottest.c_str(), referNum);
    if (parallelNum)
        printf("And %d more ...", parallelNum);
    return 0;
}

/*
    又是一道考字符串哈希的题，但这题的侧重点其实是【分词】的处理。
    这题的字符串哈希的实现与之前的相差无几，就不多写了，直接上unordered_map!

    判断话题相同: 去掉所有非英文字母和数字的符号、并忽略大小写区别后，它们是相同的字符串且同时它们有完全相同的分词

        - 这说明储存的时候:
            1. 先抹除字符串大小写区别
            2. 然后进行分词
            3. 去除所有符号后，将分词用空格拼接起来【作为键】

        - 同一条微博中出现多个相同话题的话，只算一个，因此需要进行【去重】操作

    -------------------------------------
    实际上程序在读取的过程中就可以【将每条微博】转换为【空格分隔的分词字符串】:

    整个输入过程中，程序都是【逐字符读入】的:

        0. 遇到换行符或者输入结束时

            - 微博编号++

        1. 遇到#号时:

            - 如果之前没有在读取话题，就【标记正在读取话题】
                * 且标记【上一次读取的字符属于单词】，保证【话题开头不输出多余空格】

            - 如果之前正在读取话题，就【标记停止读取话题】

                * 此时一个话题读取完毕，将字符串缓存strBuf封装成string，作为【键】访问哈希表元素

                * 取出哈希表元素，其中包含【上一次拥有这个话题的微博的编号】、【话题提及次数】两个字段

                    - 如果 (上一次拥有这个话题的微博的编号 = 当前微博编号)，说明这个话题在当前微博中【重复出现】了，不作重复处理。

                    - 如果不等于的话，就把【话题提及次数】++

            下面的2、3步的前提都是【正在读取话题】。

        2. 读到A-Z, a-z, 0-9时:

            - 如果【上一次读取的字符不属于单词】，就先在【字符串缓存strBuf尾部加一个空格】，达到空格分隔的效果

            - 如果是A-Z就转换为小写a-z

            - 加入字符串缓存strBuf

        3. 读其他符号时:

            - 标记一个词读取完毕

    -------------------------
    输出的时候只需要【遍历哈希表元素】，找到符合题目要求的最小/最大值即可。这里就不多赘述。

        - SomeBottle 2023.2.4
*/