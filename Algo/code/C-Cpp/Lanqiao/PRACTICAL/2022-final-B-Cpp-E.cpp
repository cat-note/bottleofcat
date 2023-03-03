#include <iostream>
#include <vector>
#define DIVISOR 1000000007

using namespace std;

typedef vector<long long> VecLong;

int main()
{
    int N;           // 最高进制
    int Ma, Mb;      // A和B的位数
    VecLong aDigits; // 储存A的各位
    VecLong bDigits; // 储存B的各位
    cin >> N >> Ma;
    aDigits.resize(Ma);
    for (int i = Ma - 1; i >= 0; i--) // 从低位到高位读入
        cin >> aDigits[i];
    cin >> Mb;
    bDigits.resize(Mb);
    for (int i = Mb - 1; i >= 0; i--) // 从低位到高位读入
        cin >> bDigits[i];
    // 开始处理
    int maxLen = Ma > Mb ? Ma : Mb; // 找到A和B中最长的一个的位数
    VecLong systems(maxLen);        // 储存从低到高位各位的进制
    for (int i = 0; i < maxLen; i++)
    {
        // 找到A、B相同位上最大的数+1作为这位的进制
        long long sys = 2; // 最低为2进制
        if (i < aDigits.size() && aDigits[i] >= sys)
            sys = aDigits[i] + 1;
        if (i < bDigits.size() && bDigits[i] >= sys)
            sys = bDigits[i] + 1;
        /*
        if(sys>N) // 我原本写了句这个，但是无法AC
            sys=(long long)N;
        */
        systems[i] = sys;
    }
    // 转换为10进制数相减
    long long weights[maxLen]; // 各位的权重
    weights[0] = 1;
    for (int i = 1; i < maxLen; i++) // 推算出权重
        weights[i] = (weights[i - 1] * systems[i - 1]) % DIVISOR;
    long long decimalA = 0, decimalB = 0; // A和B的十进制数
    for (int i = aDigits.size() - 1; i >= 0; i--)
        decimalA = (decimalA + aDigits[i] * weights[i]) % DIVISOR;
    for (int i = bDigits.size() - 1; i >= 0; i--)
        decimalB = (decimalB + bDigits[i] * weights[i]) % DIVISOR;
    cout << (decimalA - decimalB + DIVISOR) % DIVISOR; // 可能减出负数，先加上模数再模
    return 0;
}

/*
    这题的N没有任何作用，我最开始限制每位的进制必须在N及以内竟然还没法AC...非常神奇

    对于一个X进制的三位数:
        D3 D2 D1
    对应的进制为
        S3 S2 S1
    那么这个数转换为十进制数就是:
        D3*S2*S1 + D2*S1 + D1
    ------------------------
    要使得A-B结果尽可能小，需要保证每位的进制要【尽量小】，且【保证A和B是有效的数】
    因此本题其实是采用了一种贪心思想。
    ------------------------
    本题需要注意:
        1. 位权运算，转换为十进制数 这两部分运算都容易【溢出】，因此运算时需要模上1000000007

        2. 最终结果可能A<B，那么减出来是一个负数。因此结果需要先加上1000000007再模。

    - SomeBottle 2023.2.12
*/

/*
# [蓝桥杯 2022 省 B] X 进制减法

## 题目描述

进制规定了数字在数位上逢几进一。

$X$ 进制是一种很神奇的进制，因为其每一数位的进制并不固定！例如说某种 $X$ 进制数，最低数位为二进制，第二数位为十进制，第三数位为八进制，则 $X$ 进制数 `321` 转换为十进制数为 `65`。

现在有两个 $X$ 进制表示的整数 $A$ 和 $B$，但是其具体每一数位的进制还不确定，只知道 $A$ 和 $B$ 是同一进制规则，且每一数位最高为 $N$ 进制，最低为二进制。请你算出 $A-B$ 的结果最小可能是多少。

请注意，你需要保证 $A$ 和 $B$ 在 $X$ 进制下都是合法的, 即每一数位上的数字要小于其进制。

## 输入格式

第一行一个正整数 $N$，含义如题面所述。

第二行一个正整数 $M_{a}$，表示 $X$ 进制数 $A$ 的位数。

第三行 $M_{a}$ 个用空格分开的整数，表示 $X$ 进制数 $A$ 按从高位到低位顺序各个数位上的数字在十进制下的表示。

第四行一个正整数 $M_{b}$，表示 $X$ 进制数 $B$ 的位数。

第五行 $M_{b}$ 个用空格分开的整数，表示 $X$ 进制数 $B$ 按从高位到低位顺序各个数位上的数字在十进制下的表示。

请注意，输入中的所有数字都是十进制的。

## 输出格式

输出一行一个整数，表示 $X$ 进制数 $A-B$ 的结果的最小可能值转换为十进制后再模 $1000000007$（即 $10^9+7$）的结果。

## 样例 #1

### 样例输入 #1

```
11
3
10 4 0
3
1 2 0
```

### 样例输出 #1

```
94
```

## 提示

**【样例说明】**

当进制为：最低位 $2$ 进制, 第二数位 $5$ 进制, 第三数位 $11$ 进制时, 减法得到的差最小。此时 $A$ 在十进制下是 $108$，$B$ 在十进制下是 $14$，差值是 $94$。

**【评测用例规模与约定】**

对于 $30 \%$ 的数据，$N \leq 10,M_{a}, M_{b} \leq 8$.

对于 $100 \%$ 的数据，$2 \leq N \leq 1000,1 \leq M_{a}, M_{b} \leq 10^5,A \geq B$。

蓝桥杯 2022 省赛 B 组 E 题。
*/