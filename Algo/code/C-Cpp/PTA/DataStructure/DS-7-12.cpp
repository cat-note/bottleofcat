#include <cstdio>
#include <vector>

using namespace std;

typedef vector<long> Sequence; // 长整型数字序列

void Swap(long &elem1, long &elem2);
void BubbleSort(Sequence &seq);                                           // 冒泡排序
void SelectionSort(Sequence &seq);                                        // 选择排序
void InsertionSort(Sequence &seq);                                        // 插入排序
void ShellSort(Sequence &seq);                                            // 希尔排序
void MergeSort(Sequence &seq);                                            // 归并排序
void recursiveMerge(int start, int end, Sequence &origin, Sequence &buf); // 归并排序的递归体

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
    // InsertionSort(seq);
    // ShellSort(seq);
    MergeSort(seq);
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
//   - 经过测试，插入排序可以通过所有测试点，表现很不错
//   - 在数据规模较小的情况下，在冒泡、插入、选择中我大抵会选择【插入排序】。
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
        int currNum = seq[i]; // 当前待插入有序序列的数字
        int j = i;
        for (; j > 0; j--)
        {
            /*
            if (seq[j] > seq[j - 1])
                // 交换到了后一个数字大于前一个，就符合升序了
                break;
            else
                Swap(seq[j], seq[j - 1]);
            // 这里的写法可以优化
            */
            if (currNum > seq[j - 1]) // 找到了插入的位置就终止
                break;
            // 优化后省去了一大半的赋值工作，也不用反复调用Swap函数，对插入排序来说能省一些时间
            seq[j] = seq[j - 1];
        }
        seq[j] = currNum;
    }
}

// [希尔排序]
// 希尔排序是插入排序的优化版，主要思想是按照增量将元素分为几组，在【组间进行插入排序】，然后不断缩小增量直至为1(此时就是整个数组)
//     * 增量的初始值一般是(序列元素个数/2)，每次缩小都是÷2
//     * 增量缩小的过程中，每组内元素个数增加，可以发现【整个序列在逐渐变得基本有序】，而基本有序是【利于插入排序的】。
// 因为希尔排序中交换的并不是相邻元素，不同组内的插入排序可能会序列中相同的元素，实际上是【不稳定的算法】
// 希尔排序时间复杂度为O(n(logn)^2)，平均时间复杂度接近O(nlogn)，是比较快的算法
//   - 经过测试，希尔排序可以较快地进行排序，速度十分可观
//   - 希尔排序比较适合大规模的数据，平均下来比插入排序快了不是一点半点

void ShellSort(Sequence &seq)
{
    int len = seq.size();
    // 每组的元素的下标间的增量
    // 比如组内第一个元素下标为0，增量为5，那么组内第二个元素下标是0+5=5
    int numGap = len / 2;
    // 增量为1时，组内元素下标相隔1，就是整个seq
    while (numGap >= 1)
    {
        // 中间这块其实就是插入排序，只不过交换的元素不见得是相邻的
        // 插入排序中i从1开始，因为第0个元素是有序序列的初始序列，从1号元素开始都是待排序序列
        // 此处我要对【各组内的元素进行插入排序】，这里的i从numGap开始，也是因为numGap之前的元素【在各组内都是首个元素】
        for (int i = numGap; i < len; i++)
        {
            // i之前的是有序序列，将第i个元素向前交换，插入到到有序序列中
            // j的减量为numGap
            int currNum = seq[i]; // 当前【待插入】有序序列的数字
            int j = i;
            for (; j >= numGap; j -= numGap)
            {
                /*if (seq[j] > seq[j - numGap]) // 满足升序就不必继续
                    break;
                Swap(seq[j], seq[j - 1]);
                // 这里的写法是可以优化的
                */
                if (currNum > seq[j - numGap]) // 找到数字插入的位置
                    break;
                // 优化后省去了一大半的赋值工作，也不用反复调用Swap函数，对希尔排序来说能省超级多的时间
                seq[j] = seq[j - numGap]; // 不满足升序就把元素向后移动
            }
            seq[j] = currNum;
        }
        numGap = numGap / 2; // 不断缩小增量直至为1
    }
}

/*
-----------------------------
反思: 函数调用与时间消耗

    最开始我在写【插入排序】和【希尔排序】时内层都是用的Swap函数，运行程序的时候我发现希尔排序在大规模数据上的表现甚至还要慢于插入排序...

    琢磨了一会儿，我突然发现问题所在了: 我在有多层循环的排序中反复调用了Swap函数来不停交换元素！
    于是我将交换数字的程序直接写在了循环体内，且优化了交换的过程，减少了赋值工作的量:

    *************************
        原本是: [1 2 3 5 6 7 8] 4 9 10 11
                |<-有序序列-->|<-待插入->|

            接下来要把4插入有序序列，就需要先把4和8比，4<8，于是交换4和8；接着再把4和7比，4<7，交换4和7....

        现在是: [1 2 3 5 6 7 8] 4 9 10 11
                |<-有序序列-->|<-待插入->|

            先用一个变量保存当前待插入元素的值: currNum=4，
                * 先比较currNum和8，发现4<8，于是将8赋给后一位:
                                   ↓
                    [1 2 3 5 6 7 8 8] 9 10 11
                    |<---有序序列-->|<-待插入>|

                * 然后再比较currNum和7，发现4<7，于是将7赋给后一位:
                                 ↓
                    [1 2 3 5 6 7 7 8] 9 10 11
                    |<---有序序列-->|<-待插入>|

                * 直到扫描到元素3:
                           ↓
                    [1 2 3 5 5 6 7 8] 9 10 11
                    |<---有序序列-->|<-待插入>|

                    这里3<4，符合顺序，因此把currNum赋给后一位:
                           ↓
                    [1 2 3 4 5 6 7 8] 9 10 11
                    |<---有序序列-->|<-待插入>|

                插入完成
    ****************************
    在将插入排序中的Swap函数调用改为上面这个流程后，效果还不算很明显。
    但是当我把【希尔排序】的函数调用改成这个流程的时候可谓是效果拔群！对10^5个逆序整数的排序要【快了250多倍！】

    这大概是因为，希尔排序中实际上涉及到了三层循环(比插排还要多一层)，函数的调用过程本来就是相对耗时的，加上Swap函数内有多次赋值操作，整体的赋值操作翻倍了，自然时间消耗就大。

    这次咱得到的教训就是，不能一味地采用已经写好的函数来简化代码，有的时候要关注一下循环层数这些影响算法效率的关键因素。

-----------------------------
*/

void MergeSort(Sequence &seq)
{
    int len = seq.size();
    // 临时数组，用于子序列完毕后的【合并】
    // 如果写在recursiveMerge中，在递归过程中每一层都要存放一个容器，开销很大
    Sequence buf(len);
    // 开始递归
    recursiveMerge(0, len - 1, seq, buf);
}

// 处理从start到end下标的序列，origin是排序序列，buf是一个临时数组
void recursiveMerge(int start, int end, Sequence &origin, Sequence &buf)
{
    if (start >= end) // 已经划分成最小的序列(只有一个元素)，就回溯
        return;
    int mid = start + (end - start) / 2; // 防溢出写法
    // 将[start,end]的序列二分为[start,mid]和[mid+1,end]这两个序列分别进行排序
    recursiveMerge(start, mid, origin, buf);   // 对左半边进行排序，操作原数组的[start,mid]序列
    recursiveMerge(mid + 1, end, origin, buf); // 对右半边进行排序，操作原数组的[mid+1,end]序列
    // 到这里，[start,mid]和[mid+1,end]这两个序列已经【各自被排序好了】，要将这两个合并
    // 合并方式是将两个序列的元素【按顺序】放入buf数组
    int bufSize = 0;       // 记录用到的临时数组大小
    int lSeqPos = start;   // 左半边序列元素指针
    int rSeqPos = mid + 1; // 右半边序列元素指针
    for (int i = start; i <= end; i++)
    {
        if (lSeqPos > mid)
        {
            // 左半边序列已经扫描完毕，此时右半边序列肯定还剩
            buf[bufSize++] = origin[rSeqPos++];
        }
        else if (rSeqPos > end)
        {
            // 右半边序列已经扫描完毕, 左半边序列肯定还有
            buf[bufSize++] = origin[lSeqPos++];
        }
        else if (origin[lSeqPos] < origin[rSeqPos])
        {
            // 两半序列都还有元素，如果左序列此时的元素比右序列此时的元素小，就把左序列元素加入buf
            // 这样做，最后输出的是升序排序序列
            buf[bufSize++] = origin[lSeqPos++];
        }
        else
        {
            // 右序列元素加入buf
            buf[bufSize++] = origin[rSeqPos++];
        }
    }
    // 将buf临时数组的结果更新到【原数组】的[start,end]这段序列中
    for (int i = start, j = 0; i <= end; i++)
        origin[i] = buf[j++];
}