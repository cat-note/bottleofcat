#include <cstdio>
#include <string>
#include <vector>
#include <queue>

using namespace std;

// 代表一条学生记录
class Record
{
private:
    int orderBy; // 排序基准
public:
    string id;   // 学号
    string name; // 名字
    short score; // 分数
    Record(int orderBy)
    {
        this->orderBy = orderBy;
    }
    bool operator>(const Record &obj) const
    {
        int cmp;
        if (orderBy == 2 && (cmp = name.compare(obj.name)) != 0)
        {
            // 2-按名字排序且两人名字不同
            return cmp > 0 ? true : false;
        }
        else if (orderBy == 3 && score != obj.score)
        {
            // 3-按分数排序且两人分数不同
            return score > obj.score;
        }
        // 1-否则一律按学号递增排序
        return id.compare(obj.id) > 0 ? true : false;
    };
};

int main()
{
    // 堆排序采用的优先队列
    priority_queue<Record, vector<Record>, greater<Record>> pq;
    int recordNum, sortBy; // 记录数和排序基准
    scanf("%d %d", &recordNum, &sortBy);
    // 读入所有记录
    char tmp[10]; // 临时字符数组
    for (int i = 0; i < recordNum; i++)
    {
        Record student(sortBy);
        scanf("%s", tmp);
        student.id.assign(tmp);
        scanf("%s", tmp);
        student.name.assign(tmp);
        // 读入的是short类型，要用%hd格式
        scanf("%hd", &student.score);
        pq.push(student);
    }
    // 输出优先队列内所有内容即可
    while (!pq.empty())
    {
        printf("%s %s %hd\n", pq.top().id.c_str(), pq.top().name.c_str(), pq.top().score);
        pq.pop();
    }
    return 0;
}

/*
    本题考察对于【包含多字段的元组】的排序。这里我采用了STL中的优先队列，辅以运算符重载实现了【堆排序】，从而解决了问题。

    主要注意下面三个点:

        1. 因为学号高位都是用0来进行填充的，这里的学号可以直接依赖【字典序】来进行比较。

        2. 学号和名字都采用字典序比较，利用STL string的compare方法。

        3. 采用<cstdio>头文件时，读入和输出数据时采用的格式字符串要正确。比如我这里成绩用的short类型，那么格式字符串就是%hd

        - SomeBottle 2023.1.28
*/
/*
7-37 模拟EXCEL排序
Excel可以对一组纪录按任意指定列排序。现请编写程序实现类似功能。

输入格式:
    输入的第一行包含两个正整数N(≤10^5)和C，其中N是纪录的条数，C是指定排序的列号。之后有 N行，每行包含一条学生纪录。每条学生纪录由学号（6位数字，保证没有重复的学号）、姓名（不超过8位且不包含空格的字符串）、成绩（[0, 100]内的整数）组成，相邻属性用1个空格隔开。
输出格式:
    在N行中输出按要求排序后的结果，即：当C=1时，按学号递增排序；当C=2时，按姓名的非递减字典序排序；当C=3时，按成绩的非递减排序。当若干学生具有相同姓名或者相同成绩时，则按他们的学号递增排序。
输入样例:
    3 1
    000007 James 85
    000010 Amy 90
    000001 Zoe 60
输出样例:
    000001 Zoe 60
    000007 James 85
    000010 Amy 90
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/