#include <cstdio>

using namespace std;

// 表项
struct Item
{
    char key[9]; // 关键字(字符数组)
    Item()
    {
        key[0] = '\0'; // 初始为空字符串
    }
};

class HashTable
{
private:
    int size;
    Item *table;

public:
    HashTable(int tableSize)
    {
        table = new Item[tableSize];
        size = tableSize;
    }
    // 析构
    ~HashTable()
    {
        delete[] table; // 释放内存
    }
    // 判断两字符串是否相等
    bool strEquals(const char *a, const char *b)
    {
        int ptr = 0;
        // 两个字符串都没有到尾部的时候比较每个字符
        while (a[ptr] != '\0' && b[ptr] != '\0')
        {
            if (a[ptr] != b[ptr])
                return false;
            ptr++;
        }
        // 如果两字符串长度不相等，也是不等的
        if (a[ptr] != '\0' || b[ptr] != '\0')
            return false;
        return true;
    }
    // 复制字符串
    void strCpy(const char *from, char *to)
    {
        int ptr = 0;
        while (from[ptr] != '\0')
        {
            to[ptr] = from[ptr];
            ptr++;
        }
        to[ptr] = '\0';
    }
    // 判断str对应的hash是否可用
    bool hashAvailable(int hash, const char *str)
    {
        return strEquals(table[hash].key, str) || table[hash].key[0] == '\0';
    }
    // 返回关键字对应的下标
    int operator[](const char *str)
    {
        int strLen = 0; // 字符串长度
        while (str[strLen] != '\0')
            strLen++;
        // 将倒数三个字符转换为十进制整型key
        int key = 0;
        int exp = 0;     // 指数
        int charNum = 0; // 读取的字符数
        // 💡可能字符串有不足三个字符的情况
        for (int i = strLen - 1; i >= 0 && charNum < 3; i--) // 从低位向高位(从右至左)扫描【至多三个字符】
        {
            int weight = 1; // 位权
            for (int j = 0; j < exp; j++)
                weight *= 32;                    // 计算位权32^exp
            key += weight * (int)(str[i] - 'A'); // 位权×字符代号
            exp++;
            charNum++;
        }
        // 除留余数法算出哈希
        int hash = key % size;
        // 处理哈希碰撞（平方探测法）
        int inc = 1;                   // 增量
        int maxInc = size / 2;         // 增量<=表长/2
        if (!hashAvailable(hash, str)) // 如果hash处有碰撞，就处理碰撞
            while (true)
            {
                int hashForward = (hash + inc * inc) % size; // 向前平方探测 1^2, 2^2, 3^2, ..., maxInc^2
                if (hashAvailable(hashForward, str))
                {
                    // 如果找到了表项就返回位置
                    hash = hashForward;
                    break;
                }
                // 💡注意这里要防止越界，hash在减去inc^2时还要加上size再取余
                int hashBackward = (size + hash - inc * inc) % size; // 向后平方探测 -1^2, -2^2, -3^2, ..., -maxInc^2
                if (hashAvailable(hashBackward, str))
                {
                    // 如果找到了表项就返回位置
                    hash = hashBackward;
                    break;
                }
                inc++;            // 增量增加
                if (inc > maxInc) // 超过最大增量直接返回-1，寻位失败
                    return -1;
            };
        strCpy(str, table[hash].key); // 将字符串存入表项作为关键字
        return hash;                  // 返回存储位置
    }
};

int main()
{
    int keyNum, tableSize; // 关键字总数和散列表长度
    scanf("%d %d", &keyNum, &tableSize);
    HashTable ht(tableSize); // 初始化哈希表
    char tmpStr[9];          // 临时字符数组
    for (int i = 0; i < keyNum; i++)
    {
        scanf("%s", tmpStr);
        if (i != 0)
            printf(" "); // 不输出多余空格
        printf("%d", ht[tmpStr]);
    }
    return 0;
}

/*
    本题仍然考察哈希表，相比上一题，这一题聚焦于字符串的哈希，哈希函数采用【除留余数法】，处理碰撞的方法是【平方探测法】。

    字符串的哈希常用方法就是:
        - 将串中【部分或每一个字符看作某一个进制的数】
        - 然后将字符串作为一个数转换为【十进制数】
        - 对这个十进制数进行哈希，得到一个【哈希值】。

    题目的意思其实是把字符串转换成【32进制的数字】
    为什么是32进制？因为每个字符有26种可能，需要用5位二进制进行表示
        * 5位二进制表示一个字符，一个字符其实就可以看作是一个32进制数位。
    ----------------------------
    值得注意，本题计算哈希时只取了字符串【后三个字符】:
        - 也就是说，NNK和INNK这样的字符串就会产生冲突。
        - 因此，表项中要【储存关键字字符串】用作对比。
    ----------------------------
    如果哈希值hash对应的位置【出现冲突】，才进行碰撞处理。
    关于平方探测法:
        - 指定一个增量d
        - 查询hash+d^2和hash-d^2对应的位置是否有冲突
            * 我愿称之为“向前探测”和“向后探测”
        - d+1
    直到找到【没有冲突的位置】为止。

    💡 注意一点: 平方探测【向后探测】的时候小心下标越界，在减去增量时得加上个size，表达式形如(size+hash-增量)%size。
    -----------------------------

    永远要记住，出题人很狡猾。本题中他只说了字符串最长是多少，但【没说最短有多短】，
    因此本题肯定会考察到字符串长度<3的情况！

        - 处理方法其实也不难，从字符串尾部开始扫描【至多三个字符】进行处理即可，也就是从低位向高位扫描，转换为十进制数
        * 详见代码。

    -----------------------------

        - SomeBottle 2023.2.1
*/

/*
我自己想的一个测试例，如果平方探测写错了(比如越界了)，这个可以测试出来。
输入:
8 11
ABCD ABC BC C AC ACBD ABCDABCD ADB
正确输出:
2 1 0 3 6 4 9 10
*/