/*
https://pintia.cn/problem-sets/15/exam/problems/713
这里我采用了自下向上的建堆方式，可惜PTA不认，这题PTA只认自顶向下插入建堆的方式
但这个题解代码我写都写了，还是把代码留着作为以后的参考吧。
*/
#include <iostream>
#include <vector>

using namespace std;

void Swap(int &src, int &dest);           // 交换结点值
bool SiftDown(vector<int> &H, int i);     // 下滤结点
void AdjustHeap(vector<int> &H);          // 调整完全二叉树为小根堆
void PrintPath(vector<int> &H, int dest); // 打印从根结点到dest的路径

int main()
{
    int insertNum, pathNum;
    cin >> insertNum >> pathNum; // 输入元素个数和路径个数
    vector<int> H(insertNum);    // 初始化存放堆的数组
    // 建堆方法采用自下而上建堆
    // 先把所有结点构造成完全二叉树存放于数组
    for (int i = 0; i < insertNum; i++)
    {
        cin >> H[i];
    }
    // 接下来从最底层的父节点开始自下而上逐层调整
    AdjustHeap(H);
    // 打印从根节点到指定下标结点的路径
    for (int i = 0; i < pathNum; i++)
    {
        int destInd;
        cin >> destInd;
        PrintPath(H, destInd);
        if (i != pathNum - 1)
            cout << "\n";
    }
    return 0;
}
// 对需要移动的父节点进行下滤操作
void AdjustHeap(vector<int> &H)
{
    int len = H.size(); // 结点数
    // 从最后一个元素开始，找最接近底层的父节点
    // 注意只用遍历到下标为1的结点，下标为0的是根结点
    for (int i = len - 1; i >= 1; i--)
    {
        // 如果当前遍历到的是某个右子节点，就跳过左结点（因为左右子结点的父节点是一致的）
        if (SiftDown(H, i))
            i--;
    }
}
// 交换结点值
void Swap(int &src, int &dest)
{
    int temp = src;
    src = dest;
    dest = temp;
}

// 下滤结点，第一步是将from下标结点和to结点交换
// 返回true/false代表当前结点是/否是右结点
bool SiftDown(vector<int> &H, int i)
{
    int parentInd = (i - 1) / 2;     // 当前结点的父节点下标
    bool isRight = (i - 1) % 2 == 1; // 当前结点是否是右子节点
    bool returnVal = isRight;
    int nodeNum = H.size(); // 节点数量
    while (i < nodeNum)
    {
        if (!isRight)
        {
            // 只有左子的情况
            // 当前结点是左子的话，直接和父节点比较
            // 如果左子比父节点小，就和父节点交换
            if (H[parentInd] > H[i])
            {
                Swap(H[parentInd], H[i]);
                parentInd = i; // 继续和下层比较
            }
            else
            {
                break;
            }
        }
        else if (H[parentInd] > H[i] && H[i] < H[i - 1])
        {
            // 父节点大于右子，且右子小于左子，把右子和父节点交换，保证父节点小于子节点
            Swap(H[parentInd], H[i]);
            parentInd = i; // 继续和下层比较
        }
        else if (H[parentInd] > H[i - 1] && H[i - 1] < H[i])
        {
            // 父节点大于左子，且左子小于右子，把左子和父节点交换
            Swap(H[parentInd], H[i - 1]);
            parentInd = i - 1; // 继续和下层比较
        }
        else
        {
            break;
        }
        // 更新parentInd后再更新i为这个父节点的左子结点
        i = parentInd * 2 + 1;
        if (i >= nodeNum) // 如果左结点都没了就说明到底了，break
            break;
        if (i + 1 < nodeNum)
        { // 如果有右子就i++，更新i为右子节点
            i++;
            isRight = true; // 标记：有右结点
        }
        else
        {
            isRight = false; // 标记：非右结点
        }
    }
    return returnVal;
}

void PrintPath(vector<int> &H, int dest)
{
    int i = dest - 1; // 因为我的堆的下标是从0开始，但题目是1
    for (; i > 0; i = (i - 1) / 2)
    {
        cout << H[i] << " ";
    }
    cout << H[i];
}