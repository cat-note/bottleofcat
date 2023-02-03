#include <cstdio>

using namespace std;

// 链地址结点
typedef struct TableItem
{
    long miles;   // 里程数
    char key[19]; // 身份证号作为键
    TableItem *next;
    TableItem()
    {
        miles = 0; // 代表首次被访问
        next = NULL;
    }
    // 赋值字符串
    void operator=(const char *str)
    {
        int i = 0;
        while (str[i] != '\0')
        {
            key[i] = str[i];
            i++;
        }
        key[i] = '\0';
    }
    // 比较字符串
    bool operator==(const char *str)
    {
        int i = 0;
        while (key[i] != '\0' && str[i] != '\0')
            if (key[i] != str[i])
                return false;
            else
                i++;
        // 两字符串长度不同
        if (key[i] != '\0' || str[i] != '\0')
            return false;
        return true;
    }
} *List;

class HashTable
{
private:
    int size;        // 表长
    List *table;     // 哈希表底层数组
    int weights[20]; // 计算字符串哈希时用到的位权
public:
    HashTable()
    {
        size = (1 << 15); // 2^15
        table = new List[size];
        weights[0] = 1;              // 首位位权为1
        for (int i = 1; i < 20; i++) // 初始化位权数组
            weights[i] = 0;
        for (int i = 0; i < size; i++) // 初始化所有值为-1，代表无此表项
            table[i] = NULL;
    }
    // 析构，释放内存
    ~HashTable()
    {
        for (int i = 0; i < size; i++)
        {
            List curr = table[i];
            while (curr)
            {
                List next = curr->next;
                delete curr;
                curr = next;
            }
        }
        delete[] table;
    }
    // 计算哈希
    int getHash(const char *str)
    {
        int hash = 0;
        int exp = 0; // 指数
        // 取身份证前6位和末4位进行哈希
        for (int i = 0; i < 18; i++)
        {
            int chrCode = (int)(str[i] - '0'); // 数字代号
            if (str[i] == 'x')
                chrCode = 10; // x就当作10来算
            int hashTmp = weights[exp] * chrCode;
            if (!weights[exp + 1])                    // 如果已经推算出来就不再重复算了
                weights[exp + 1] = weights[exp] * 11; // 当成11进制数
            if (hashTmp < 0)                          // 溢出了也要保证是正的
                hashTmp = -hashTmp;
            hash += hashTmp;
            exp++;
            if (i == 5) // 跳到末四位
                i = 14;
        }
        return hash & (size - 1); // 位与取余
    }
    // 字符串为键，访问哈希表元素
    long &operator[](const char *str)
    {
        int hash = getHash(str);
        if (!table[hash])
        {
            // 如果表中没有有此项，就新建
            List node = new TableItem();
            *node = str; // 字符串赋值
            table[hash] = node;
            return node->miles;
        }
        else
        {
            List curr = table[hash];
            // 在链表中找到目标
            while (curr)
            {
                if (*curr == str)
                    return curr->miles;
                curr = curr->next;
            }
            // 如果不存在此项就创建
            List node = new TableItem();
            *node = str;
            // 头插法
            node->next = table[hash]->next;
            table[hash]->next = node;
            return node->miles;
        }
    }
};

int main()
{
    HashTable table;        // 初始化哈希表
    int recordNum, minMile; // 记录数和最低里程
    scanf("%d %d", &recordNum, &minMile);
    // 读入记录
    char strTemp[19]; // 身份证号临时数组
    for (int i = 0; i < recordNum; i++)
    {
        int miles;
        scanf("%s %d", strTemp, &miles);
        if (miles < minMile) // 不足最低里程的按最低里程算
            miles = minMile;
        table[strTemp] += (long)miles;
    }
    // 查询记录
    int queryNum;
    scanf("%d", &queryNum);
    for (int i = 0; i < queryNum; i++)
    {
        scanf("%s", strTemp);
        long query = table[strTemp];
        if (!query)
            printf("No Info\n"); // 无此会员
        else
            printf("%ld\n", query); // 输出累积里程数
    }
    return 0;
}

/*
    本题仍然考察的是字符串哈希。

        - 将身份证号作为字符串处理，每位看作11进制的数，转换为10进制后通过位与取余收束到哈希表范围内。

    身份证号后四位加上前六位，重复率就已经较低了，因此这题我针对身份证号的【前四位】和【末六位】进行了哈希计算。

    其他的就不多赘述了。

        - SomeBottle 2023.2.3
*/