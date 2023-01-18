#include <iostream>
#include <map>
#include <set>

using namespace std;

typedef set<int> FriendSet;

int main()
{
    // 储存每位学生的朋友数
    map<int, FriendSet> friends;
    int stuNum, clubNum; // 学生总数和俱乐部数
    int maxCircle = 0;   // 最大朋友圈人数
    cin >> stuNum >> clubNum;
    for (int i = 0; i < clubNum; i++)
    {
        int clubStu; // 读入俱乐部中的学生数量
        cin >> clubStu;
        // 读入俱乐部学生
        int temp[clubStu]; // 临时存放俱乐部学生
        for (int j = 0; j < clubStu; j++)
        {
            cin >> temp[j];
        }
        for (int j = 0; j < clubStu; j++)
        {
            for (int k = 0; k < clubStu; k++)
            {
                if (j != k)
                {
                    FriendSet &setTmp = friends[temp[j]];
                    setTmp.insert(temp[k]); // 将俱乐部第k个学生加入第j个学生的朋友圈
                    if (setTmp.size() > maxCircle)
                        maxCircle = setTmp.size(); // 更新最大朋友圈
                }
            }
        }
    }
    cout << maxCircle + 1; // 最大朋友圈人数
    return 0;
}

/*
    这里我尝试用STL中的map和set搭配解决问题，但输入数据规模大时是顶不住的，因为这些容器还有额外的内存开销，会内存超限。

    实际上这题考察的是【并查集】。

        - SomeBottle 2023.1.18
*/