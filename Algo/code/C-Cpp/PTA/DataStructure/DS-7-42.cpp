#include <cstdio>

using namespace std;

// 表项
struct Item
{
    int key;
    Item()
    {
        key = -1; // 代表此处无元素
    }
};

int main()
{
    int keyNum, tableSize; // 关键字数和表长
    scanf("%d %d", &keyNum, &tableSize);
    Item table[tableSize]; // 初始化哈希表
    for (int i = 0; i < keyNum; i++)
    {
        int inputKey;
        scanf("%d", &inputKey);
        int hash = inputKey % tableSize;
        // 1. 表中已经有了inputKey, 输出已有的表项的下标
        // 2. 表中没有inputKey，找到插入位置，输出下标
        while (table[hash].key != inputKey && table[hash].key != -1)
            hash = (hash + 1) % tableSize; // 找到位置
        table[hash].key = inputKey;        // 标记hash处对应键名inputKey
        if (i != 0)
            printf(" "); // 不输出多余空格
        printf("%d", hash);
    }
    return 0;
}

/*
    这题考察基本的哈希表实现，冲突解决方法是【开放定址法】

    要根据键值key找到表项:

        1. 求出Hash = key % 表长

        2. 从Hash下标开始向后【循环查找】整张表中【首个】空位进行插入; 或者能找到key对应的表项(说明key键表项已经存在)。

    这题的实现挺简单的。
    -------------------------
        - SomeBottle 2023.2.1
*/

/*
7-42 整型关键字的散列映射
    给定一系列整型关键字和素数P，用除留余数法定义的散列函数H(Key)=Key将关键字映射到长度为P的散列表中。用线性探测法解决冲突。

输入格式:
    输入第一行首先给出两个正整数N（≤1000）和P（≥N的最小素数），分别为待插入的关键字总数、以及散列表的长度。第二行给出N个整型关键字。数字间以空格分隔。
输出格式:
    在一行内输出每个整型关键字在散列表中的位置。数字间以空格分隔，但行末尾不得有多余空格。
输入样例:
    4 5
    24 15 61 88
输出样例:
    4 0 1 3
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/