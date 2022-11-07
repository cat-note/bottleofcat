/*
试题 基础练习 回文数
资源限制
	内存限制：512.0MB   C/C++时间限制：1.0s   Java时间限制：3.0s   Python时间限制：5.0s
问题描述
　　1221是一个非常特殊的数，它从左边读和从右边读是一样的，编程求所有这样的四位十进制数。
输出格式
　　按从小到大的顺序输出满足条件的四位十进制数。
*/
#include <iostream>

using namespace std;

int main()
{
	int targetSum;
	cin >> targetSum;
	for (int units = 1; units < 10; units++)
	{
		for (int tens = 0; tens < 10; tens++)
		{
			cout << units << tens << tens << units << endl;
		}
	}
	return 0;
}