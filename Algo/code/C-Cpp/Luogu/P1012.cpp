#include <cstdio>
#include <string>
#include <algorithm>

using namespace std;

bool cmp(const string &a, const string &b);

int main()
{
    int N; // 数字个数
    scanf("%d", &N);
    // 读入各数字（按字符串处理）
    string nums[20];  // 最多20个
    char readTmp[11]; // 临时字符数组
    for (int i = 0; i < N; i++)
    {
        scanf("%s", readTmp);
        nums[i].assign(readTmp);
    }
    // 按字典序进行排序
    sort(begin(nums), begin(nums) + N, cmp);
    // 接下来顺序输出即可实现拼接
    for (int i = 0; i < N; i++)
    {
        printf("%s", nums[i].c_str());
    }
    return 0;
}

bool cmp(const string &a, const string &b)
{
    int i = 0;
    // 先比较等长的部分
    while (i < a.length() && i < b.length())
    {
        if (a[i] < b[i])
        { // 如果b更大就返回false
            return false;
        }
        else if (a[i] > b[i])
        { // 如果a比b更大就返回true
            return true;
        }
        i++;
    }
    // 处理不等长的部分
    return a.length() < b.length() && b[i] < b[0] || a.length() > b.length() && a[i] > a[0];
}

/*
    按字符串字典序来比较数值大小时，是【从高位向低位】进行比较的。

    因此如果我按【从高位向低位】的拼接顺序对数值进行拼接，就可以先按字符串字典序对代表数值的所有字符串进行【降序排序】，然后直接拼接即可。

        * 这样能保证高位数字尽可能大

    但是要注意，这里的排序并不是排排字典序就完事了，需要额外注意一个情况:

    当两字符串长度不相等的时候：

    1. 如果两字符串的【等长部分】能决定出字典序大小，就按字典序排

    2. 如果两字符串的等长部分无法决定字典序，需要看【较长一个字符串】的多出来的【一个字符】:  

        
*/