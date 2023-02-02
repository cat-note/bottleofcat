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

// 哈希表
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
                int hashForward = (hash + inc * inc) % size; // 向后平方探测 1^2, 2^2, 3^2, ..., maxInc^2
                if (hashAvailable(hashForward, str))
                {
                    // 如果找到了表项就返回位置
                    hash = hashForward;
                    break;
                }
                // 💡注意这里要防止越界，hash在减去inc^2时还要加上size再取余
                int hashBackward = (size + hash - inc * inc) % size; // 向前平方探测 -1^2, -2^2, -3^2, ..., -maxInc^2
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