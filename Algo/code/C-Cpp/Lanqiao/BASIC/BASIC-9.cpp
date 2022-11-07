/*
试题 基础练习 特殊回文数
资源限制
	内存限制：512.0MB   C/C++时间限制：1.0s   Java时间限制：3.0s   Python时间限制：5.0s
问题描述
　　123321是一个非常特殊的数，它从左边读和从右边读是一样的。
　　输入一个正整数n， 编程求所有这样的五位和六位十进制数，满足各位数字之和等于n 。
输入格式
　　输入一行，包含一个正整数n。
输出格式
　　按从小到大的顺序输出满足条件的整数，每个整数占一行。
样例输入
	52
样例输出
	899998
	989989
	998899
数据规模和约定
　　1<=n<=54。
*/
#include <iostream>

using namespace std;

/*
	我原本是这样写的，但有一个问题，如果按题目所述的按升序输出，那就有点难办了

int main()
{
	int targetSum; // 目标总和
	cin >> targetSum;
	int tens = 0;  // 十位数
	int units = 0; // 个位数
	for (; units < 10; units++)
	{
		// 减少循环次数，利用对称性
		// 需要枚举一侧的个位和十位（比如09和90中只用枚举出09，0+9=9+0；再比如46和64中也只需要枚举出46）
		// 另外个位和十位如果不同的话，还要交替
		for (tens = 0; tens <= units; tens++)
		{
			int fourSum = (tens + units) * 2;	   // 计算其他四位的和
			int middleDigit = targetSum - fourSum; // 五位中的中间一位
			if (middleDigit >= 0)
			{
				if (middleDigit < 10)
				{ // 如果加上中间的数可以达到题目要求，就获得了所需的 5位回文数
					// (易错)如果十位和个位不同，还得交换一下，比如有98989，那么89998也行
					// 因为题目要求顺序输出，只能牺牲一下空间了，储存一下回文数
					if (units != tens)
						cout << tens << units << middleDigit << units << tens << endl;
					cout << units << tens << middleDigit << tens << units << endl;
				}
				if (middleDigit % 2 == 0 && (middleDigit /= 2) < 10)
				{
					// 如果是六位的话，middleDigit必须要能被2整除，且分成两位后每位数<10，这样才能分为中间两位
					// (易错)如果十位和个位不同，还得交换一下，比如有989989，那么899998也行
					// 因为题目要求顺序输出，只能牺牲一下空间了，储存一下回文数
					if (units != tens)
						cout << tens << units << middleDigit << middleDigit << units << tens << endl;
					cout << units << tens << middleDigit << middleDigit << tens << units << endl;
				}
			}
		}
	}
	return 0;
}
*/

// 放弃思考，投奔for循环的怀抱吧！
// 实际上这里时间规模是固定的10*10*10，其实还好。

int main()
{
	int targetSum; // 目标总和
	cin >> targetSum;
	// 先枚举五位回文数
	// 一个我非常不理解的地方，这里units要改成1才能完全AC，但是你题目给的n是>=1啊，你这样n=1不就出不了结果了？
	// 大概是因为开头不能为0吧？不行，我还是不理解
	for (int units = 1; units < 10; units++)
		for (int tens = 0; tens < 10; tens++)
			for (int middle = 0; middle < 10; middle++)
			{
				if ((units + tens) * 2 + middle == targetSum)
					cout << units << tens << middle << tens << units << endl;
			}
	// 再枚举六位回文数
	for (int units = 1; units < 10; units++)
		for (int tens = 0; tens < 10; tens++)
			for (int middle = 0; middle < 10; middle++)
			{
				if ((units + tens) * 2 + middle * 2 == targetSum)
					cout << units << tens << middle << middle << tens << units << endl;
			}
	return 0;
}