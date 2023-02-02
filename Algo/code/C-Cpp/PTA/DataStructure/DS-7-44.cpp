#include <cstdio>
#include <string>
#include <vector>

using namespace std;

typedef vector<string> Words; // 储存单词的数组

// 哈希表项
typedef struct TableItem
{
    string key;  // 键值
    bool access; // 储存这个词是否被访问
    TableItem *next;
    TableItem(const string &key)
    {
        this->key = key;
        access = false;
        next = NULL;
    }
} *List;

// 哈希表
class HashTable
{
private:
    int size;           // 哈希表长度, 2的n次方，方便取余运算
    vector<List> table; // 哈希表底层数组
public:
    HashTable()
    {
        size = (1 << 13); // 2^13 = 8192，经过测试，2^13或2^14是最适合这题的
        table.resize(size, NULL);
    }
    ~HashTable() // 析构，释放内存
    {
        for (int i = 0; i < size; i++)
        {
            if (table[i])
            {
                List node = table[i];
                while (node)
                {
                    List next = node->next;
                    delete node;
                    node = next;
                }
            }
        }
    }
    // 重置访问位access=0
    void resetAccess()
    {
        for (int i = 0; i < size; i++)
        {
            if (table[i])
            {
                List node = table[i];
                while (node)
                {
                    node->access = false;
                    node = node->next;
                }
            }
        }
    }
    // 通过字符串键值访问哈希表元素
    bool &operator[](const string &key)
    {
        // 计算字符串哈希
        int hash = 0;
        // 这里给11位，防止访问未知内存
        int weights[11] = {1}; // 递推位权32^n
        for (int i = 0, len = key.length(); i < len; i++)
        {
            int tmpHash = weights[i] * (int)(key[i] - 'a');
            weights[i + 1] = weights[i] * 32; // 位权32^n
            // 注意，因为最多有10个字符，这里可能会有溢出
            if (tmpHash < 0)
                tmpHash = -tmpHash; // 如果溢出到负数就取相反数
            hash += tmpHash;        // 计算哈希
        }
        if (hash < 0)
            hash = -hash;         // 溢出的时候保证是正数
        hash = hash & (size - 1); // 位与取余，得到0~4095范围的哈希值
        if (!table[hash])
        {
            // 如果对应的表还没有创建，就新建一下
            List node = new TableItem(key);
            table[hash] = node;
            return node->access; // 返回引用
        }
        else
        {
            // 如果对应的地方已经有表了
            List curr = table[hash];
            while (curr)
            {
                if (curr->key == key)
                {                        // 已经存在了这一项
                    return curr->access; // 返回引用
                }
                curr = curr->next;
            }
            // 没有找到键key对应的项，就创建
            List node = new TableItem(key);
            node->next = table[hash]->next; // 头插法
            table[hash]->next = node;
            return node->access; // 返回引用
        }
    }
};

int main()
{
    int fileNum, queryNum; // 文件数和查询数
    scanf("%d", &fileNum);
    HashTable table; // 初始化哈希表, 储存词被访问的情况
    //  ----------------------------------------------------------开始读取文件内容
    int currentFile = 0;  // 当前正在读取哪个文件的内容(编号0~N-1)
    char wordBuf[11];     // 单词缓存，一个单词最多只考虑10个字母
    int wordLen = 0;      // 单词长度
    Words files[fileNum]; // 储存所有文件的单词
    string wordTmp;       // 临时储存单词的字符串
    while (currentFile < fileNum)
    {
        char readChar; // 读入的字符
        scanf("%c", &readChar);
        if (readChar >= 'A' && readChar <= 'Z')
        {
            // 如果是大写字符，就转换为小写
            readChar += ('a' - 'A');
        }
        if (readChar >= 'a' && readChar <= 'z')
        {
            // 如果读入的是英文字母，说明正在读取字符
            if (wordLen < 10) // 长度>10的单词只考虑前10个字母
                wordBuf[wordLen++] = readChar;
        }
        else
        {
            // 否则读到的就是非英文字母，这说明【一个单词读取完毕】
            if (wordLen >= 3)
            {
                // 只考虑长度>=3的单词
                wordBuf[wordLen] = '\0'; // 封装成字符串
                wordTmp.assign(wordBuf); // 转换为string
                bool &access = table[wordTmp];
                if (!access) // 如果词未被访问(去重)
                {
                    access = true;                         // 标记词语被访问过
                    files[currentFile].push_back(wordTmp); // 将单词加入到文件的单词集合中
                }
            }
            if (readChar == '#')
            { // 一个文件读取完毕
                currentFile++;
                table.resetAccess(); // 重置所有access=0
            }
            wordLen = 0; // 清空单词缓存
        }
    }
    // ----------------------------------------------------------开始对比文件
    scanf("%d", &queryNum);
    for (int i = 0; i < queryNum; i++)
    {
        int index1, index2; // 两个文件的编号
        scanf("%d %d", &index1, &index2);
        index1--; // 转换为0~N-1的编号
        index2--;
        int commonNum = 0; // 公共单词数(不包含重复词)
        int totalNum = 0;  // 总单词数(不包含重复词)
        // 扫描文件1的所有单词
        for (Words::iterator it = files[index1].begin(); it != files[index1].end(); it++)
        {
            table[*it] = true; // 标记单词存在于文件1
            totalNum++;        // 总单词数增加
        }
        // 扫描文件2的所有单词
        for (Words::iterator it = files[index2].begin(); it != files[index2].end(); it++)
        {
            bool &access = table[*it];
            if (access) // 当前单词同时存在于文件1和文件2，就是共有单词
            {
                commonNum++;
            }
            else // 单词只存在于文件2
            {
                totalNum++;
            }
        }
        table.resetAccess(); // 重置access=0
        printf("%.1lf%%\n", ((double)commonNum / totalNum) * 100);
    }
    return 0;
}

/*
    看到这种统计词频的题，首先能想到的解决方法就是哈希表。

        - 总词汇量指的是两个文件所有的单词【组成的集合】的元素数，也就是【没有重复单词】；
        - 可以推测，公共词汇应该也是【不算重复单词】的。
            * 因此还要考虑一种情况，那就是一个单词在【同一个文件中出现多次】的时候，不能重复计算。

    本题有一个测试点涉及到大规模的数据，且对时间复杂度也有要求，因此本题的目的是【快速地进行统计】。
    ---------------------------
    我的解题方案:

        - 手写实现一个字符串哈希表，把字符串看成32进制数进行哈希，采用链地址解决碰撞问题。

        - 读入文件:

            - 逐字母读入，最多一次读入10个字母，至少读入3个字母才能作为单词

            - 将读入的大写字母【全转换为小写】

            - 每次遇到【非字母】的字符时，说明【一个单词读取完毕】了:

                1. 将这个单词作为键，查询哈希表，检查【这个单词是否已经加入文件】
                    * 这一步就是【去重】工作

                2. 如果没有加入文件，就标记 哈希表[单词]=true (已访问)，并且将单词【加入相应文件编号的单词数组中】

            - 每次遇到#号时说明一个文件读取完毕:

                1. 文件编号++

                2. 【重置哈希表】

        - 对比文件

            在读入文件的时候，我已经保证了每个文件的单词数组中【不会有重复单词】。

            每次对比的时候能拿到一对文件的编号: index1, index2，分别对应文件1，文件2，
            只需要扫描这两个文件【各自的单词数组】，辅以【哈希表】对单词访问情况进行标记，就能很快得出【公共词汇数】和【总词汇数】
            (详见代码)

    个人感觉，这题的【关键点】其实是去重。

    本题如果采用C++ STL的map/unordered_map应该可以更快速地解决。这里我想锻炼一下写字符串哈希的能力，所以自己实现了一下。

    绞尽脑汁去解决TLE(时间超限)和MLE(内存超限)问题真的挺痛苦...苦露西
    ----------------------------------------

        - SomeBottle 2023.2.2

*/