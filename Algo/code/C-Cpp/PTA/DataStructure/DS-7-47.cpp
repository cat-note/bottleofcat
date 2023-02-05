#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

string names[40000]; // 储存学生名字字符串

// 学生比较类
struct StuLess
{
    bool operator()(const int &a, const int &b)
    {
        return names[a].compare(names[b]) < 0 ? true : false;
    }
};

// 用set容器储存学生名，既防止重复也能自动按字典序排序
typedef vector<int> Students;

int main()
{
    int stuNum, courseNum; // 总学生数和总课程数
    scanf("%d %d", &stuNum, &courseNum);
    Students courses[courseNum]; // 每门课对应的学生集合
    // 读取选课名单
    char strBuf[5]; // 临时字符数组
    for (int i = 0; i < stuNum; i++)
    {
        int num; // 选这门课的学生数
        scanf("%s %d", strBuf, &num);
        names[i].assign(strBuf); // 储存学生名
        for (int j = 0; j < num; j++)
        {
            int courseId;
            scanf("%d", &courseId);
            courseId--;                     // 处理的时候课程按0~N-1编号
            courses[courseId].push_back(i); // 把学生序号加入课程选择者集合中
        }
    }
    // 输出结果
    for (int i = 0; i < courseNum; i++)
    {
        printf("%d %d\n", i + 1, courses[i].size()); // 输出课程编号和选课人数
        if (courses[i].size() > 0)                   // 对非空的桶进行排序
        {
            sort(courses[i].begin(), courses[i].end(), StuLess());
            for (Students::iterator it = courses[i].begin(); it != courses[i].end(); it++)
            {
                printf("%s\n", names[*it].c_str()); // 根据学生序号取出学生名
            }
        }
    }
    return 0;
}

/*
    本题考察排序，且有点类似桶排序，只不过题目告诉了【桶里该装哪些元素】。

    每个课程就像是一个桶，把所有【选课同学】加入桶中，对非空的桶的元素进行排序即可。

    本题每个学生名字字符串长度恒为4，占5字节的空间。
    这里我给每个学生编了号，用4字节的整型标记一个学生，而【桶中储存的则是学生编号】。

    对非空桶元素进行排序的时候，我直接用了STL的sort方法。
    --------------------------------------
    吐槽:
        这题我最开始是用STL的set容器去实现的，set容器能默认按字典升序排序且不会有重复项。
        但提交之后我怎么也过不了数据规模最大的一个测试点，十分令人匪夷所思。

        我的感觉就是，能用普通排序解决的，就不要瞎用set这类容器。

        - SomeBottle 2023.2.5
*/

/*
7-47 打印选课学生名单
    假设全校有最多40000名学生和最多2500门课程。现给出每个学生的选课清单，要求输出每门课的选课学生名单。

输入格式:
    输入的第一行是两个正整数：N（≤40000），为全校学生总数；K（≤2500），为总课程数。此后N行，每行包括一个学生姓名（3个大写英文字母+1位数字）、一个正整数C（≤20）代表该生所选的课程门数、随后是C个课程编号。简单起见，课程从1到K编号。
输出格式:
    顺序输出课程1到K的选课学生名单。格式为：对每一门课，首先在一行中输出课程编号和选课学生总数（之间用空格分隔），之后在第二行按字典序输出学生名单，每个学生名字占一行。
输入样例:
    10 5
    ZOE1 2 4 5
    ANN0 3 5 2 1
    BOB5 5 3 4 2 1 5
    JOE4 1 2
    JAY9 4 1 2 5 4
    FRA8 3 4 2 5
    DON2 2 4 5
    AMY7 1 5
    KAT3 3 5 4 2
    LOR6 4 2 4 1 5
输出样例:
    1 4
    ANN0
    BOB5
    JAY9
    LOR6
    2 7
    ANN0
    BOB5
    FRA8
    JAY9
    JOE4
    KAT3
    LOR6
    3 1
    BOB5
    4 7
    BOB5
    DON2
    FRA8
    JAY9
    KAT3
    LOR6
    ZOE1
    5 9
    AMY7
    ANN0
    BOB5
    DON2
    FRA8
    JAY9
    KAT3
    LOR6
    ZOE1
代码长度限制
    16 KB
时间限制
    1000 ms
内存限制
    64 MB
*/