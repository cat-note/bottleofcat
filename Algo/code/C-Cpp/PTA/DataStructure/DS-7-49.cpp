#include <cstdio>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

typedef vector<short> Courses; // 储存课程编号的数组

int main()
{
    unordered_map<string, Courses> selections; // 储存每个学生的选课记录
    int queryNum, courseNum;                   // 查询学生数和课程数
    scanf("%d %d", &queryNum, &courseNum);
    // 读入选课信息
    string strTemp; // 临时字符串
    strTemp.resize(5);
    for (int i = 0; i < courseNum; i++)
    {
        short courseId;
        int stuNum; // 课程编号和选课人数
        scanf("%hd %d", &courseId, &stuNum);
        for (int j = 0; j < stuNum; j++)
        {
            scanf("%s", &strTemp[0]);
            selections[strTemp].push_back(courseId);
        }
    }
    // 开始查询
    for (int i = 0; i < queryNum; i++)
    {
        if (i != 0) // 不输出多余空白符
            printf("\n");
        scanf("%s", &strTemp[0]);
        Courses &crs = selections[strTemp]; // 取出这名学生选的课
        sort(crs.begin(), crs.end());       // sort默认递增排序
        printf("%s %d", strTemp.c_str(), crs.size());
        for (Courses::iterator it = crs.begin(); it != crs.end(); it++)
        {
            printf(" %d", *it);
        }
    }
    return 0;
}

/*
    本题应该依旧是考察字符串哈希，利用STL的unordered_map能快速解决，这里不多赘述。

    其实这题说不定用普通的Trie树也是可以写的，毕竟字符串最多只有4个有效字符，涉及到4层Trie树。
    (不过建树的时间开销估计也不少)

        - SomeBottle 2023.2.7
*/

/*
7-49 打印学生选课清单
    假设全校有最多40000名学生和最多2500门课程。现给出每门课的选课学生名单，要求输出每个前来查询的学生的选课清单。注意：每门课程的选课人数不可超过 200 人。

输入格式:
    输入的第一行是两个正整数：N（≤40000），为前来查询课表的学生总数；K（≤2500），为总课程数。此后顺序给出课程1到K的选课学生名单。格式为：对每一门课，首先在一行中输出课程编号（简单起见，课程从1到K编号）和选课学生总数（之间用空格分隔），之后在第二行给出学生名单，相邻两个学生名字用1个空格分隔。学生姓名由3个大写英文字母+1位数字组成。选课信息之后，在一行内给出了N个前来查询课表的学生的名字，相邻两个学生名字用1个空格分隔。
输出格式:
    对每位前来查询课表的学生，首先输出其名字，随后在同一行中输出一个正整数C，代表该生所选的课程门数，随后按递增顺序输出C个课程的编号。相邻数据用1个空格分隔，注意行末不能输出多余空格。
输入样例:
    10 5
    1 4
    ANN0 BOB5 JAY9 LOR6
    2 7
    ANN0 BOB5 FRA8 JAY9 JOE4 KAT3 LOR6
    3 1
    BOB5
    4 7
    BOB5 DON2 FRA8 JAY9 KAT3 LOR6 ZOE1
    5 9
    AMY7 ANN0 BOB5 DON2 FRA8 JAY9 KAT3 LOR6 ZOE1
    ZOE1 ANN0 BOB5 JOE4 JAY9 FRA8 DON2 AMY7 KAT3 LOR6
输出样例:
    ZOE1 2 4 5
    ANN0 3 1 2 5
    BOB5 5 1 2 3 4 5
    JOE4 1 2
    JAY9 4 1 2 4 5
    FRA8 3 2 4 5
    DON2 2 4 5
    AMY7 1 5
    KAT3 3 2 4 5
    LOR6 4 1 2 4 5
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/