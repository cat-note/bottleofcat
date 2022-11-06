/*
 * 试题 基础练习 十进制转十六进制
    问题描述
    　　十六进制数是在程序设计时经常要使用到的一种整数的表示方式。它有0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F共16个符号，分别表示十进制数的0至15。十六进制的计数方法是满16进1，所以十进制数16在十六进制中是10，而十进制的17在十六进制中是11，以此类推，十进制的30在十六进制中是1E。
    　　给出一个非负整数，将它表示成十六进制的形式。
    资源限制
        内存限制：512.0MB   C/C++时间限制：1.0s   Java时间限制：3.0s   Python时间限制：5.0s
    输入格式
　　      输入包含一个非负整数a，表示要转换的数。0<=a<=2147483647
    输出格式
　　      输出这个整数的16进制表示
    样例输入
        30
    样例输出
        1E
 */
#include <iostream>
#include <stack>

using namespace std;

int main(){
	int decNum;
	cin>>decNum;
	stack<int> hexDigits;
	int quotient=decNum;
	int remainder;
	do{
		remainder=quotient%16;
		quotient/=16;
		hexDigits.push(remainder); // 用栈是因为这里是 除以基数倒着取余数
	}while(quotient>0);
	while(hexDigits.size()>0){ // 把每位十六进制数字打印出来
		int digit=hexDigits.top();
		hexDigits.pop();
		char character; // 要打印出来的字符
		if(digit>=10){
			character='A'+(digit-10); // 转换为字符A-F
		}else{
			character='0'+digit; // 0-9
		}
		cout<<character;
	}
	cout<<endl;
	return 0;
}