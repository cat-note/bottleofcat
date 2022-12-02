/*
蓝桥杯算法训练VIP-学生成绩
时间限制: 1s 内存限制: 128MB
题目描述
    定义一个学生结构体类型student，包括4个字段，姓名、性别、年龄和成绩。然后在主函数中定义一个结构体数组（长度不超过1000），并输入每个元素的值，程序使用冒泡排序法将学生按照成绩从小到大的顺序排序，然后输出排序的结果。
输入格式
    第一行是一个整数N（N<1000），表示元素个数；接下来N行每行描述一个元素，姓名、性别都是长度不超过20的字符串，年龄和成绩都是整型。
输出格式
    按成绩从小到大输出所有元素，若多个学生成绩相同则成绩相同的同学之间保留原来的输入顺序。
样例输入
    3
    Alice female 18 98
    Bob male 19 90
    Miller male 17 92
样例输出
    Bob male 19 90
    Miller male 17 92
    Alice female 18 98
*/
#include <iostream>
#include <vector>

using namespace std;

typedef struct
{
    string name;
    string sex;
    int age;
    int score;
} student;

int main()
{
    int itemNum; // 项目数
    cin >> itemNum;
    vector<student> stuArr(itemNum); // 学生数组
    for (int i = 0; i < itemNum; i++)
    {
        cin >> stuArr[i].name;
        cin >> stuArr[i].sex;
        cin >> stuArr[i].age;
        cin >> stuArr[i].score;
    }
    // 冒泡排序
    bool hasSwap = true;
    // 如果在某次遍历中全程都没发生交换(hasSwap=false)，说明已经排序完成，可以提前结束了！
    for (int end = itemNum - 1; end >= 0 && hasSwap; end--)
    {
        hasSwap = false;
        // 不停对比i和i+1下标的元素，如果i下标元素更大，就交换一下
        for (int i = 0; i <= end - 1; i++)
        {
            if (stuArr[i].score > stuArr[i + 1].score)
            {
                // C++里的swap更快捷，这里还是按C的写吧
                student tmp = stuArr[i];
                stuArr[i] = stuArr[i + 1];
                stuArr[i + 1] = tmp;
                hasSwap = true; // 在这一次遍历过程中发生了交换
            }
        }
    }
    // 输出结果
    for (int i = 0; i < itemNum; i++)
    {
        cout << stuArr[i].name << " " << stuArr[i].sex << " " << stuArr[i].age << " " << stuArr[i].score << "\n";
    }
    return 0;
}
/*
    这题的重点在冒泡排序这里。
    冒泡排序实质上就是多次的【相邻元素】比较与交换过程，本题中是【把较大的元素】往【数组后面】交换，大元素慢慢从数组后方“冒出来”。
        - 外层循环从数组尾部遍历到头部
            - 内层循环每次都从数组开头遍历到【外层遍历到的位置】
            （这是因为每次不停交换都把最大的元素换到了尾部，最大元素的位置一旦确定下来就不用再管了)

    💡 值得注意的是，冒泡排序可能外层循环进行到半途时【元素已经排序完成】，这个时候就不需要再把循环继续下去了，省时间。
        - 只要在某一次内层循环中【全程都没有交换】，就可以跳出外层循环了。

      - SomeBottle 2022.12.2
*/