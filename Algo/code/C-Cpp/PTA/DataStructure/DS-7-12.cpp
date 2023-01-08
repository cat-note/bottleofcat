#include <cstdio>
#include <vector>

using namespace std;

typedef vector<long> Sequence; // 长整型数字序列

void BubbleSort(Sequence &seq); // 冒泡排序

int main()
{
    int N;
    scanf(" %d", &N);
    // 初始化数字序列
    Sequence seq(N);
    for (int i = 0; i < N; i++)
        scanf(" %ld", &seq[i]);
    // 进行排序
    BubbleSort(seq);
    // 输出序列
    for (int i = 0; i < N; i++)
    {
        printf("%ld", seq[i]);
        if (i != N - 1)
            printf(" ");
    }
    return 0;
}

// 交换元素
void Swap(long &elem1, long &elem2)
{
    long tmp = elem1;
    elem1 = elem2;
    elem2 = tmp;
}

// 冒泡排序
// 冒泡排序其实就是一个不断交换的过程
// 经过测试，冒泡排序在对【数据5】和【数据9】进行排序时会TLE(超时)
void BubbleSort(Sequence &seq)
{
    // 冒泡排序原地完成，不需要额外空间
    int len = seq.size();
    for (int i = 0; i < len; i++)
    {
        bool exchanged = false; // 是否发生交换
        // 如果要把最小值排到数组前面，冒泡排序的两个下标i,j的变化方向是相反的
        // 这里i是递增，而j是递减
        for (int j = len - 1; j > i; j--)
        {
            // 如果前一个数大于后一个数就需要交换
            if (seq[j - 1] > seq[j])
            {
                Swap(seq[j - 1], seq[j]);
                exchanged = true;
            }
        }
        // 如果没发生交换说明已经排序完成
        if (!exchanged)
            break;
    }
}