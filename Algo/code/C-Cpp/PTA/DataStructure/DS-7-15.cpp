#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

typedef struct LNode *List;

// 链表节点
struct LNode
{
    int64_t qq; // QQ号
    string pwd; // 密码
    List next;
};
// 实现简单的哈希表
class HashTable
{
private:
    mutable vector<List> H;
    int divisor = (1 << 14); // 2的14次方
public:
    HashTable()
    {
        H.resize(divisor, NULL); // 初始所有链表为空
    }
    // 析构时释放内存
    ~HashTable()
    {
        for (int i = 0, len = H.size(); i < len; i++)
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
    // 取得哈希
    int getHash(const int64_t &qqNum) const
    {
        return qqNum & (divisor - 1); // 位与取余计算哈希
    }
    // 创建新的表项
    bool put(const int64_t &qqNum, const string &password)
    {
        int hash = getHash(qqNum);
        List node = new LNode();
        node->qq = qqNum;
        node->pwd = password;
        node->next = NULL;
        if (!H[hash])
        {
            H[hash] = node;
            return true; // 成功创建
        }
        List curr = H[hash];
        while (true)
        {
            if (curr->qq == qqNum)
            {
                delete node;
                return false; // 表项已经存在，创建失败
            }
            if (!curr->next)
                break;
            curr = curr->next;
        }
        curr->next = node;
        return true;
    }
    // 通过中括号运算符访问哈希表
    List operator[](const int64_t &qqNum) const
    {
        int hash = getHash(qqNum);
        if (!H[hash])
        {
            // 暂时还没这个链表就返回NULL
            return NULL;
        }
        List curr = H[hash];
        while (curr)
        {
            if (curr->qq == qqNum)
                return curr;
            curr = curr->next;
        }
        // 没找到，返回NULL
        return NULL;
    }
};

int main()
{
    HashTable table; // 初始化哈希表
    int instNum;     // 指令数
    cin >> instNum;
    for (int i = 0; i < instNum; i++)
    {
        char action;
        int64_t qqNum;
        string password;
        cin >> action >> qqNum >> password;
        if (action == 'N')
        {
            // 创建账号
            if (table.put(qqNum, password))
                cout << "New: OK\n";
            else
                cout << "ERROR: Exist\n"; // 账户已存在
        }
        else if (action == 'L')
        {
            // 登录账号
            List node = table[qqNum];
            if (node)
            {
                // 密码一致
                if (node->pwd == password)
                {
                    cout << "Login: OK\n";
                }
                else
                {
                    cout << "ERROR: Wrong PW\n";
                }
            }
            else
            {
                cout << "ERROR: Not Exist\n"; // 账户不存在
            }
        }
    }
    return 0;
}

/*
    本题套路类似DS-7-14，都是用哈希表(散列表)存取数据。本题主打模拟创建/登录账号的过程。

    就...不多赘述了捏~

        - SomeBottle 2023.1.11
*/
/*
7-15 QQ帐户的申请与登陆
    实现QQ新帐户申请和老帐户登陆的简化版功能。最大挑战是：据说现在的QQ号码已经有10位数了。

输入格式:
    输入首先给出一个正整数N（≤10^5），随后给出N行指令。每行指令的格式为：“命令符（空格）QQ号码（空格）密码”。其中命令符为“N”（代表New）时表示要新申请一个QQ号，后面是新帐户的号码和密码；命令符为“L”（代表Login）时表示是老帐户登陆，后面是登陆信息。QQ号码为一个不超过10位、但大于1000（据说QQ老总的号码是1001）的整数。密码为不小于6位、不超过16位、且不包含空格的字符串。
输出格式:
    针对每条指令，给出相应的信息：
    1）若新申请帐户成功，则输出“New: OK”；
    2）若新申请的号码已经存在，则输出“ERROR: Exist”；
    3）若老帐户登陆成功，则输出“Login: OK”；
    4）若老帐户QQ号码不存在，则输出“ERROR: Not Exist”；
    5）若老帐户密码错误，则输出“ERROR: Wrong PW”。
输入样例:
    5
    L 1234567890 myQQ@qq.com
    N 1234567890 myQQ@qq.com
    N 1234567890 myQQ@qq.com
    L 1234567890 myQQ@qq
    L 1234567890 myQQ@qq.com
输出样例:
    ERROR: Not Exist
    New: OK
    ERROR: Exist
    ERROR: Wrong PW
    Login: OK
代码长度限制
    16 KB
时间限制
    1200 ms
内存限制
    64 MB
*/