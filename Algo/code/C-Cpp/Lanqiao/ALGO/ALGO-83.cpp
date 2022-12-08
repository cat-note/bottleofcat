#include <iostream>

using namespace std;

int main()
{
    int N;
    cin >> N;
    int firstNonZero = N;            // 目前的【右边的第一个非零位】
    for (int i = N - 1; i >= 1; i--) // 阶乘过程
    {
        int result = firstNonZero * i;
        while (result % 10 == 0)
            result /= 10;
        firstNonZero = result % 100;
    }
    cout << firstNonZero % 10;
    return 0;
}