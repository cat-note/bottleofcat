#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

/*
    注意！本题也要考察对并列排名的处理。
*/

using namespace std;

// 学生信息，包括学号,得分,考点编号，考点排名，总排名
struct Stu
{
    int site;         // 考点编号
    string no;        // 学号
    int score;        // 得分
    int siteRanking;  // 考点排名
    int totalRanking; // 总排名
    bool operator<(const Stu &node) const
    {
        if (score == node.score) // 分数相同，按考号递增排序
            return no.compare(node.no) < 0;
        return score > node.score; // 分数不同，按分数递减排序
    }
};

void calcRanking(vector<Stu> &arr, int begin, int end, bool forSite); // 计算排名，处理【有序序列】中的并列排名

int main()
{
    int siteNum; // 考点数
    scanf("%d", &siteNum);
    char noTmp[14];            // 考号临时字符数组
    vector<Stu> report(30000); // 最终的总成绩单，最多有300*100=30000名学生
    int reportSize = 0;        // 总成绩单大小
    for (int i = 0; i < siteNum; i++)
    {
        int stuNum; // 考点学生总数
        scanf("%d", &stuNum);
        int beginInd = reportSize;        // 考点在成绩单中的开始下标
        int endInd = reportSize + stuNum; // 考点在成绩单中的结束下标(在最后一个元素的后一位)
        // 读入考点所有考生信息
        for (int j = beginInd; j < endInd; j++)
        {
            scanf("%s", noTmp);            // 读入考号
            scanf("%d", &report[j].score); // 读入分数
            report[j].no.assign(noTmp);    // 转换考号为string
            report[j].site = i + 1;        // 考点编号
        }
        reportSize = endInd; // 更新成绩单大小
        // 先对考点考生信息进行排序
        sort(report.begin() + beginInd, report.begin() + endInd);
        // 计算并处理考点成绩中并列排名的情况
        calcRanking(report, beginInd, endInd, true);
    }
    // 对总成绩单进行排序处理
    sort(report.begin(), report.begin() + reportSize);
    // 计算排名，并处理总成绩单的并列排名情况
    calcRanking(report, 0, reportSize, false);
    // 输出结果
    printf("%d\n", reportSize); // 考生总数
    for (int i = 0; i < reportSize; i++)
        printf("%s %d %d %d\n", report[i].no.c_str(), report[i].totalRanking, report[i].site, report[i].siteRanking);
    return 0;
}

// 计算【有序序列】中并列项的排名
// forSite=true代表是在【考点内排名】
void calcRanking(vector<Stu> &arr, int begin, int end, bool forSite)
{
    for (int i = begin; i < end; i++)
    {
        int ranking = i - begin + 1; // 当前排名
        // 如果当前学生和序列中前一个学生成绩相同，说明是并列的
        if (i > begin && arr[i - 1].score == arr[i].score)
        {
            if (forSite)                          // 如果是考点内排名
                ranking = arr[i - 1].siteRanking; // 当前学生和前一个学生【在考点内的】排名并列
            else
                ranking = arr[i - 1].totalRanking; // 当前学生和前一个学生【在总成绩中】排名并列
        }
        // 更新学生的排名
        if (forSite)
            arr[i].siteRanking = ranking;
        else
            arr[i].totalRanking = ranking;
    }
}

/*
    这道题是上一题(DS-7-40)的升级版，仍然考察的是排序。

    本题需要进行【分区排序】，同时要【处理并列的名次】。
    处理并列名次这一块和7-40题是非常相似的，具体看代码注释即可。
    ---------------------------------
    关于分区排序，本题要求结果中有【考场内排名】和【总排名】。
    自然而然能想到，我应该先对考场内同学进行排名，然后再对所有同学进行排名。

    我的处理办法是，相同考场的同学【下标连续】，按考场把考生信息【储存在同一个数组中】。

        * 每个考场的【首个考生】和【最后一位考生】分别【用一个下标进行表示】，两个下标begin, end则在数组中指出了【当前考场的考生】。

        * 每读入一个考场的考生，就对【数组中[begin,end)部分】的同学进行排序，算出排名，并进行【并列排名的处理】。

    读完所有考场后，【对整个数组进行排序】，算出所有考生的排名，并进行【并列排名的处理】即可。
    ---------------------------------
    💡！！！！！注意！有个我很容易写错的点！！！！！！

    STL string的比较方法compare:

        如果要判断string1<string2是否成立，千万千万不要写成string1.compare(string2)<-1啊哥！不要看走眼了哥！是<=-1或者<0啊！！！！

    ---------------------
        - SomeBottle 2023.1.31

*/
/*
7-41 PAT排名汇总
有个考试叫PAT。
每次考试会在若干个不同的考点同时举行，每个考点用局域网，产生本考点的成绩。考试结束后，各个考点的成绩将即刻汇总成一张总的排名表。
现在就请你写一个程序自动归并各个考点的成绩并生成总排名表。

输入格式:
    输入的第一行给出一个正整数N（≤100），代表考点总数。随后给出N个考点的成绩，格式为：首先一行给出正整数K（≤300），代表该考点的考生总数；随后K行，每行给出1个考生的信息，包括考号（由13位整数字组成）和得分（为[0,100]区间内的整数），中间用空格分隔。
输出格式:
    首先在第一行里输出考生总数。随后输出汇总的排名表，每个考生的信息占一行，顺序为：考号、最终排名、考点编号、在该考点的排名。其中考点按输入给出的顺序从1到N编号。考生的输出须按最终排名的非递减顺序输出，获得相同分数的考生应有相同名次，并按考号的递增顺序输出。
输入样例:
    2
    5
    1234567890001 95
    1234567890005 100
    1234567890003 95
    1234567890002 77
    1234567890004 85
    4
    1234567890013 65
    1234567890011 25
    1234567890014 100
    1234567890012 85
输出样例:
    9
    1234567890005 1 1 1
    1234567890014 1 2 1
    1234567890001 3 1 2
    1234567890003 3 1 2
    1234567890004 5 1 4
    1234567890012 5 2 2
    1234567890002 7 1 5
    1234567890013 8 2 3
    1234567890011 9 2 4
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/