#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

typedef struct HNode *List;

struct HNode
{
    int64_t number; // 记录电话号码,int64_t为64位整型
    int record;     // 通话次数
    List next;
};

// 哈希表，链地址法解决冲突
class HashTable
{
private:
    mutable vector<List> H; // 哈希表底层数组
    int divisor;

public:
    HashTable()
    {
        // 2的14次方，因为用的是位运算取余，除数必须是2^N
        // 这里并不是说2的14次方最好，只是我感觉10000-20000左右的数字应该差不多。
        divisor = (1 << 14);
        // 节省内存，各链表就不用头节点了
        H.resize(divisor, NULL);
    }
    ~HashTable()
    {
        // 析构，释放堆内存
        for (int i = 0, len = H.size(); i < len; i++)
        {
            if (H[i])
            {
                List curr = H[i];
                while (curr)
                {
                    List next = curr->next;
                    delete curr;
                    curr = next;
                }
            }
        }
    }
    // 创建新链表节点
    List newNode(const int64_t &number) const
    {
        List node = new HNode();
        node->record = 0;
        node->next = NULL;
        node->number = number;
        return node;
    }
    // 根据电话号码进行处理
    int &operator[](const int64_t &number) const
    {
        int hash = (number & (divisor - 1)); // 位与取余法计算哈希
        List target = NULL;                  // 找到待更新的结点
        // 寻找位置
        if (!H[hash])
        {
            // 如果此处链表无元素
            List node = newNode(number);
            H[hash] = node;
            target = node;
        }
        else
        {
            // 顺着链表找电话号码
            List curr = H[hash];
            while (true)
            {
                if (curr->number == number)
                {
                    target = curr;
                    break;
                }
                // 如果下一个NULL就中断，便于尾部插入新节点
                if (!curr->next)
                    break;
                curr = curr->next;
            }
            // 没找到电话号码，说明是新插入的
            if (!target)
            {
                List node = newNode(number);
                curr->next = node; // 接入链表尾部
                target = node;
            }
        }
        return target->record; // 返回引用
    }
};

int main()
{
    HashTable table;    // 初始化哈希表
    int recordNum;      // 记录数
    int64_t madman = 0; // 狂人的电话号码
    int maxRecord = 0;  // 最高通话记录
    int madNum = 0;     // 狂人数
    cin >> recordNum;
    recordNum *= 2; // 因为每行有两个电话号码
    for (int i = 0; i < recordNum; i++)
    {
        int64_t number;
        cin >> number;
        // table[number]取得某个电话号码的通话次数并进行修改
        int currRecord = ++table[number];
        if (currRecord > maxRecord)
        {
            // 大于最高记录
            madNum = 1;             // 狂人数为1
            maxRecord = currRecord; // 更新最高记录
            madman = number;        // 记录电话号码
        }
        else if (currRecord == maxRecord)
        {
            // 等于最高记录，说明有新的狂人
            madNum++;            // 狂人数增加
            if (number < madman) // 保证输出狂人的电话号码是最小的
                madman = number;
        }
    }
    cout << madman << " " << maxRecord;
    // 不止一个狂人，就输出狂人数目
    if (madNum > 1)
        cout << " " << madNum;
    return 0;
}

/*
    这题考察的是哈希表的基本存取操作。

    这题的数据规模可能比较大，且电话号码有11位，而有多个并列狂人的时候还需要比较出最小的电话号码，用字符串操作实际上是很不方便，且效率相对较低的。
    因此这题我用的是int64_t(相当于long long)这种固定64位宽的整型来储存11位的电话号码。

    哈希表的哈希函数我采用了位与取余，因为比较快。
    至于哈希表底层数组的大小...我选择了2的14次方，一个处于10000-20000间的数字。

        * 位于取余: [ 被除数 & (除数 - 1) ]， 这里的除数必须是2的N次方。

    另外，哈希碰撞的处理我采用了链地址法。

        - SomeBottle 2023.1.10
*/
/*
7-14 电话聊天狂人
给定大量手机用户通话记录，找出其中通话次数最多的聊天狂人。

输入格式:
    输入首先给出正整数N（≤10^5），为通话记录条数。随后N行，每行给出一条通话记录。简单起见，这里只列出拨出方和接收方的11位数字构成的手机号码，其中以空格分隔。
输出格式:
    在一行中给出聊天狂人的手机号码及其通话次数，其间以空格分隔。如果这样的人不唯一，则输出狂人中最小的号码及其通话次数，并且附加给出并列狂人的人数。
输入样例:
    4
    13005711862 13588625832
    13505711862 13088625832
    13588625832 18087925832
    15005713862 13588625832
输出样例:
    13588625832 3
代码长度限制
    16 KB
时间限制
    600 ms
内存限制
    64 MB
*/