# 【记忆用】算法常用数学公式/方法 

(2022.11.7)最近开始写些算法题了，发现有很多算法中常用数学公式我给忘了，因此我决定专门开这样一篇文章来记忆我认为**比较常用**的数学公式及方法。  

随缘更新中~\_( '-' \_)⌒)\_  

## 圆周率的计算

*	编程语言中通过反正切函数：  

	$$\pi = \arctan{1}\times 4$$
	因为：  
	$$\arctan{1} = \frac{\pi}{4}$$  
	
	C++示例：
	```cpp
	#include <cstdio>
	#include <cmath>

	using namespace std;

	int main()
	{
		double PI = atan(1.0) * 4;
		printf("%.8lf", PI);
		return 0;
	}
	```

## 质数(素数)的判断

### 程序思路

> 素数指的是某个**除了1和它自身，没有其他因数**的正整数。

比如要判断正整数`number`是否是素数：

尝试用：

$$ \left [  2, \sqrt{number}\right ] $$

这个范围内的**每个数**来除`number`，**只要找到了一个**可以整除`number`的数，`number`就**不是**素数(质数)。  

------

### 原理

为什么右闭区间是 $ \sqrt{number} $ 捏？

假如有如下一式成立（`a`,`b`,`x`均为**正整数**)：

$$ a \times b = x $$

可以看到`a`和`b`是`x`的因数。  
接下来将式两边同时除以 $ b \times \sqrt{x} $ ：  

$$ \frac{a}{\sqrt{x}} =  \frac {\sqrt{x}} {b} $$  

1. 如果 $ a \le \sqrt{x} $ ,要使得等式成立则必有 $ b \ge \sqrt{x} $  

2. 如果 $ b \le \sqrt{x} $ ,要使得等式成立则必有 $ a \ge \sqrt{x} $  

可以发现，**只要`x`能被分解为两个因数**`a`×`b`的形式，那么**至少有一个因数**是 $ \le \sqrt{x} $ 的。  

因此如果一个数`x`**不是**素数，那么在 $ \left [  2, \sqrt{x}\right ] $ 范围内**肯定有**其一个因数。

### 实战

[蓝桥杯基础练习BASIC-16](https://www.dotcpp.com/oj/problem1464.html)   

* [我的题解](https://github.com/SomeBottle/bottleofcat/blob/main/Algo/code/C-Cpp/Lanqiao/BASIC/BASIC-16.cpp)  

-------

...to be updated