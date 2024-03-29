#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

int main()
{
    priority_queue<int, vector<int>, greater<int>> pq; // 优先队列
    int blockNum;                                      // 块数
    int totalCost = 0;                                 // 总花费
    scanf("%d", &blockNum);
    // 读入每个木块的长度
    for (int i = 0; i < blockNum; i++)
    {
        int blockSize; // 木头长度
        scanf("%d", &blockSize);
        pq.push(blockSize);
    }
    // 这里其实就是霍夫曼树的构造思想了
    while (pq.size() >= 2)
    {
        // 从序列中选择最小的两块木头组成一块大木头，倒着看切割的过程
        int smallest = pq.top();
        pq.pop();
        int cost = pq.top() + smallest; // 算出切割成最小两块木头时的成本
        pq.pop();
        pq.push(cost);     // 算出来再进入优先队列
        totalCost += cost; // 算入总成本
    }
    printf("%d", totalCost);
    return 0;
}

/*
    这题第一眼上去以为是动态规划。换了一个角度思考后发现可以用【霍夫曼树】的构造思想去解决。

    题目的意思是将一个大木块不断切分成多个小块的木块，所需成本就是【被切分前的大木块的长度】之和

        比如 20 --切分成--> 16 和 4，成本就是20
             16 --切分成--> 10 和 6，成本就是16

    到这里大致能猜到，这题考的是用一种【贪心算法】去解决问题。
    ----------------------------------------------------------------
    正着看很难归纳出贪心的选择策略，但是反着看就能发现神奇的地方:

    切分之后的木块长度如下:
        8 7 5 2
            -> 选择最短的两块木板组成一块长木板: 2+5=7
        8 7 7
            -> 选择最短的两块木板组成一块长木板: 7+7=14
        8 14
            -> 选择最短的两块木板组成一块长木板: 8+14=22
        -> 最低成本=7+14+22=43

    这不就是【霍夫曼】算法的过程嘛！实际上霍夫曼算法就是一种贪心算法，它自下而上不断选择较小的节点，构造出一棵树。

    在这里，我们把整个切木板的过程【倒过来】看作【把两块木板组成一块大木板】，这个问题就迎刃而解了！

    💡 只要我【每次都选择最短的两块木板】来组成一块大木板，组成的【大木板的长度】就能保证是【最短的】
        而每块大木板的长度就是【每次切割时的成本】，因此将【组成的所有的大木板长度都加起来】，得到的就是【总成本】。
    -----------------------------------------------------------------
    本题中我采用了【优先队列】来辅助选出最小的两个节点。

    这题中最关键的思维技巧就是“正难则反”，如果从大问题推到小问题有些困难，不妨【反过来从小问题推到大问题】试试。

        - SomeBottle 2023.1.21
*/

/*
7-29 修理牧场
    农夫要修理牧场的一段栅栏，他测量了栅栏，发现需要N块木头，每块木头长度为整数Li个长度单位，于是他购买了一条很长的、能锯成N块的木头，即该木头的长度是Li的总和。

    但是农夫自己没有锯子，请人锯木的酬金跟这段木头的长度成正比。为简单起见，不妨就设酬金等于所锯木头的长度。例如，要将长度为20的木头锯成长度为8、7和5的三段，第一次锯木头花费20，将木头锯成12和8；第二次锯木头花费12，将长度为12的木头锯成7和5，总花费为32。如果第一次将木头锯成15和5，则第二次锯木头花费15，总花费为35（大于32）。

    请编写程序帮助农夫计算将木头锯成N块的最少花费。

输入格式:
    输入首先给出正整数N（≤10^4），表示要将木头锯成N块。第二行给出N个正整数（≤50），表示每段木块的长度。
输出格式:
    输出一个整数，即将木头锯成N块的最少花费。
输入样例:
    8
    4 5 1 2 1 3 1 1
输出样例:
    49
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/