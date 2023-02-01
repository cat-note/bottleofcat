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