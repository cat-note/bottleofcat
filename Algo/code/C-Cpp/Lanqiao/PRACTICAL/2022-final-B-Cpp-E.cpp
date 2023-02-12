#include <iostream>
#include <vector>
#define MODULUS 1000000007

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
        weights[i] = (weights[i - 1] * systems[i - 1]) % MODULUS;
    long long decimalA = 0, decimalB = 0; // A和B的十进制数
    for (int i = aDigits.size() - 1; i >= 0; i--)
        decimalA = (decimalA + aDigits[i] * weights[i]) % MODULUS;
    for (int i = bDigits.size() - 1; i >= 0; i--)
        decimalB = (decimalB + bDigits[i] * weights[i]) % MODULUS;
    cout << (decimalA - decimalB + MODULUS) % MODULUS; // 可能减出负数，先加上模数再模
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
    本题需要注意:
        1. 位权运算，转换为十进制数 这两部分运算都容易【溢出】，因此运算时需要模上1000000007

        2. 最终结果可能A<B，那么减出来是一个负数。因此结果需要先加上1000000007再模。
*/