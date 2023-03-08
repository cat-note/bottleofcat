#include <cstdio>
#define LL long long

using namespace std;

typedef struct Bomb // 炸弹亦或者火箭都可以用一个三元组(x,y,r)表示
{
    LL x; // x,y的规模可能很大
    LL y;
    int r; // 半径
    Bomb *next;
    void operator=(const Bomb &bomb) // 运算符重载，用于拷贝结点值
    {
        x = bomb.x;
        y = bomb.y;
        r = bomb.r;
    }
    Bomb(LL x, LL y, int r)
    {
        this->x = x;
        this->y = y;
        this->r = r;
        next = NULL;
    }
} *BombPtr;

LL findBombs(LL x, LL y, int r); // 声明函数，寻找(x,y,r)内的炸雷数目

BombPtr bombs = new Bomb(0, 0, 0); // 链表头结点

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) // 先读入炸雷
    {
        LL bombX, bombY;
        int bombR;
        scanf("%lld %lld %d", &bombX, &bombY, &bombR);
        BombPtr nBomb = new Bomb(bombX, bombY, bombR);
        nBomb->next = bombs->next; // 头插法
        bombs->next = nBomb;
    }
    LL count = 0;               // 排除掉的炸雷数
    for (int i = 0; i < m; i++) // 读入排雷火箭
    {
        LL rocketX, rocketY;
        int rocketR;
        scanf("%lld %lld %d", &rocketX, &rocketY, &rocketR);
        count += findBombs(rocketX, rocketY, rocketR);
    }
    printf("%lld", count);
    return 0;
}

LL findBombs(LL x, LL y, int r)
{
    LL count = 0;
    BombPtr curr = bombs->next;
    BombPtr prev = bombs; // 指向遍历的结点的前一个结点
    LL left = x - r;      // 左边界
    LL right = x + r;     // 右边界
    LL upper = y + r;     // 上边界
    LL lower = y - r;     // 下边界
    while (curr)
    {
        if (curr->x >= left && curr->x <= right && curr->y >= lower && curr->y <= upper) // 先判断这个炸雷点在不在(x,y)为中心，边长为2R的正方形区域内
        {
            LL xDist = curr->x - x;                     // 横坐标上的距离
            LL yDist = curr->y - y;                     // 纵坐标上的距离
            if (xDist * xDist + yDist * yDist <= r * r) // 勾股定理判断两点间距离是否在半径内
            {
                // 如果在半径内说明此炸雷被排除，从链表移除
                prev->next = curr->next; // 这里没有free掉结点curr，只是从链表中拆了，因此下面可以继续使用。free还是有点耗时的。
                count++;
                count += findBombs(curr->x, curr->y, curr->r); // DFS扫描关联的炸雷
            }
        }
        prev = curr;
        curr = curr->next;
    }
    return count;
}

/*
    本题数据规模可能非常大(5×10^4)，要储存图的话，普通的邻接表亦或者邻接矩阵都很容易内存超限MLE。

    但是看了一下发现半径r的范围竟然挺小的，这里肯定是一个突破口。

    图中的雷其实是在慢慢减少的，已经爆掉的雷就可以移除掉了

    初步想法是用DFS来扫

    在扫到某个炸雷后要将其立刻从图中移除，因此这里很适合使用链表，而且内存肯定是够用的（如果预估每个结点占用29B内存, 5*10^4个结点也才占用不过2MB）。
*/