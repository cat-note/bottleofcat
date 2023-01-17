#include <cstdio>
#include <map>

using namespace std;

int main()
{
    map<string, int> recordMap;
    int recordNum; // 记录数
    scanf("%d", &recordNum);
    char temp[31];  // 临时字符数组
    string tempStr; // 临时字符串
    for (int i = 0; i < recordNum; i++)
    {
        getchar(); // 取走前一个换行符
        scanf("%[^\n]", temp);
        tempStr.assign(temp); // 转为C++ string
        recordMap[tempStr]++;
    }
    for (auto i = recordMap.begin(); i != recordMap.end(); i++)
    {
        printf("%s %.4lf%%\n", i->first.c_str(), (double)i->second / recordNum * 100);
    }
    return 0;
}

/*
    如果用C++的话，这题考察的就是STL中map容器的使用。

    map底层是红黑树实现的，而红黑树是有序的。当用字符串作为键时，map会自动按字典排序。
    因此本题只需要用字符串作为键，然后根据键进行统计就OK了

    最后利用迭代器输出map容器中的内容即可。

        - SomeBottle 2023.1.17
*/
/*
7-24 树种统计
    随着卫星成像技术的应用，自然资源研究机构可以识别每一棵树的种类。请编写程序帮助研究人员统计每种树的数量，计算每种树占总数的百分比。
输入格式:
    输入首先给出正整数N（≤10^5），随后N行，每行给出卫星观测到的一棵树的种类名称。种类名称由不超过30个英文字母和空格组成（大小写不区分）。
输出格式:
    按字典序递增输出各种树的种类名称及其所占总数的百分比，其间以空格分隔，保留小数点后4位。
输入样例:
    29
    Red Alder
    Ash
    Aspen
    Basswood
    Ash
    Beech
    Yellow Birch
    Ash
    Cherry
    Cottonwood
    Ash
    Cypress
    Red Elm
    Gum
    Hackberry
    White Oak
    Hickory
    Pecan
    Hard Maple
    White Oak
    Soft Maple
    Red Oak
    Red Oak
    White Oak
    Poplan
    Sassafras
    Sycamore
    Black Walnut
    Willow
输出样例:
    Ash 13.7931%
    Aspen 3.4483%
    Basswood 3.4483%
    Beech 3.4483%
    Black Walnut 3.4483%
    Cherry 3.4483%
    Cottonwood 3.4483%
    Cypress 3.4483%
    Gum 3.4483%
    Hackberry 3.4483%
    Hard Maple 3.4483%
    Hickory 3.4483%
    Pecan 3.4483%
    Poplan 3.4483%
    Red Alder 3.4483%
    Red Elm 3.4483%
    Red Oak 6.8966%
    Sassafras 3.4483%
    Soft Maple 3.4483%
    Sycamore 3.4483%
    White Oak 10.3448%
    Willow 3.4483%
    Yellow Birch 3.4483%
代码长度限制
    16 KB
时间限制
    1500 ms
内存限制
    64 MB
*/