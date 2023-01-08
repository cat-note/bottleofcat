#include <cstdio>
#include <vector>

using namespace std;

typedef vector<long> Sequence; // 长整型数字序列

void BubbleSort(Sequence &seq);    // 冒泡排序
void SelectionSort(Sequence &seq); // 选择排序
void InsertionSort(Sequence &seq); // 插入排序

int main()
{
    int N;
    scanf(" %d", &N);
    // 初始化数字序列
    Sequence seq(N);
    for (int i = 0; i < N; i++)
        scanf(" %ld", &seq[i]);
    // 进行排序
    // BubbleSort(seq);
    // SelectionSort(seq);
    InsertionSort(seq);
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

// [冒泡排序]
// 冒泡排序其实就是一个【数组内】不断两两交换的过程，因为【仅仅是交换数字】，这个算法的表现【很稳定】
// 最好情况: 第一次扫描时，整个数组已经有序了，O(n)
// 最糟情况: 两个循环都跑完才排序完毕，O(n^2)
//     * 经过测试，冒泡排序在对【数据5】和【数据9】进行排序时会TLE(超时)
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

// [选择排序]
// 选择排序也是数组内交换，不过有一个【选择数字进行交换】的过程，因此也是原地算法。
//     * 因为有这个选择的过程，选择排序是不稳定的，相同值的数字在不同排序过程中可能顺序不同
// 选择排序无论如何都需要把两个循环都跑完, 因此无论最好情况还是最坏情况【时间复杂度全为O(n^2)】，是相对耗时的排序算法
//     * 经过测试，选择排序只能通过【前四个测试点】，数据规模大的情况下非常不建议采用
void SelectionSort(Sequence &seq)
{
    int len = seq.size();
    // 选择排序也是原地完成的
    for (int i = 0; i < len; i++)
    {
        int selection = i;
        // 在[i+1,len-1]下标范围的子序列中“选择”最小的，且比seq[i]小的数
        for (int j = i + 1; j < len; j++)
        {
            if (seq[j] < seq[selection])
                selection = j;
        }
        // 如果找到了比seq[i]更小的数就交换
        if (selection != i)
            Swap(seq[selection], seq[i]);
    }
}

// [插入排序]
// 这里实现插入排序的原地算法，在将数字插入到有序序列中时实际上还是在进行【两两交换】
// 插入排序是稳定的算法
// 最好情况下，未排序序列已经有序，插入排序无需插入元素到有序序列中，时间复杂度O(n)
//     * 在序列【基本有序】的情况下，很适合采用插入排序
// 最坏情况下，整个序列的数字是逆序的，插入排序每次都要将元素插入到有序序列中，时间复杂度O(n^2)
//     * 经过测试，插入排序可以通过所有测试点，表现很不错
//     * 在数据规模较小的情况下，在冒泡、插入、选择中我大抵会选择插入排序。
void InsertionSort(Sequence &seq)
{
    int len = seq.size();
    // i之前的子序列是【有序序列】
    // i及之后的子序列是【未排序序列】
    // 插入排序做的事就是把【未排序序列】的数字【插入到有序序列的指定位置】
    for (int i = 1; i < len; i++)
    {
        // 为求方便，实际实现其实还是交换
        // 因为【有序序列】和【未排序序列】是相邻的，我只需要把当前的i元素向前交换至【有序序列中正确的位置即可】
        for (int j = i; j > 0; j--)
        {
            if (seq[j] > seq[j - 1])
            {
                // 交换到了后一个数字大于前一个，就符合升序了
                break;
            }
            else
            {
                Swap(seq[j], seq[j - 1]);
            }
        }
    }
}