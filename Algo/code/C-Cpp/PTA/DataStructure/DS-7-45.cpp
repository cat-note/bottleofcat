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
        size = (1 << 15); // 表长2^15
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
            if (i == 5) // 前6位算完就跳到末四位
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

/*
7-45 航空公司VIP客户查询
    不少航空公司都会提供优惠的会员服务，当某顾客飞行里程累积达到一定数量后，可以使用里程积分直接兑换奖励机票或奖励升舱等服务。现给定某航空公司全体会员的飞行记录，要求实现根据身份证号码快速查询会员里程积分的功能。

输入格式:
    输入首先给出两个正整数N（≤10^5）和K（≤500）。其中K是最低里程，即为照顾乘坐短程航班的会员,航空公司还会将航程低于K公里的航班也按K公里累积。随后N行，每行给出一条飞行记录。飞行记录的输入格式为：18位身份证号码（空格）飞行里程。其中身份证号码由17位数字加最后一位校验码组成，校验码的取值范围为0~9和x共11个符号；飞行里程单位为公里，是(0, 15 000]区间内的整数。然后给出一个正整数M（≤10^5），随后给出M行查询人的身份证号码。
输出格式:
    对每个查询人，给出其当前的里程累积值。如果该人不是会员，则输出No Info。每个查询结果占一行。
输入样例:
    4 500
    330106199010080419 499
    110108198403100012 15000
    120104195510156021 800
    330106199010080419 1
    4
    120104195510156021
    110108198403100012
    330106199010080419
    33010619901008041x
输出样例:
    800
    15000
    1000
    No Info
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/