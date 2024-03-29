# 咱洛谷上做的题

这部分咱主要按着[能力全面提升综合题单](https://www.luogu.com.cn/training/9391)来刷。

## Part.1 基础篇

这部分是比较基础的入门题，这部分我的注释写的比较简单。

| 题目编号 | 题目 | 我的题解 | 值得玩味 |备注|
|:---:|:---:|:---:|:---:|:---:|
| P1421 | [小玉买文具](https://www.luogu.com.cn/problem/P1421) | [P1421.cpp](./P1421.cpp) | | 把a看作十位，b看作个位，组成一个整数进行运算即可 |  
| P1909 | [买铅笔](https://www.luogu.com.cn/problem/P1909) | [P1909.cpp](./P1909.cpp) | | 注意**刚好能买到n支**的情况即可 |  
| P1089 | [津津的储蓄计划](https://www.luogu.com.cn/problem/P1089) | [P1089.cpp](./P1089.cpp) | | 简单的模拟题，记得**最后要加上手中的余额**。 |  
| P1085 | [不高兴的津津](https://www.luogu.com.cn/problem/P1085) | [P1085.cpp](./P1085.cpp) | | 注意题目要求的是**最不高兴的一天**。 |  
| P1035 | [级数求和](https://www.luogu.com.cn/problem/P1035) | [P1035.cpp](./P1035.cpp) | | 题目已经给出公式，直接求和+判断即可。 |  
| P1980 | [计数问题](https://www.luogu.com.cn/problem/P1980) | [P1980.cpp](./P1980.cpp) | | 注意题目要统计的是一串数字**字符**序列中**某一个0-9的数字的出现次数**，本题考察的其实是整型到字符串的转换。 |  
| P1014 | [Cantor表](https://www.luogu.com.cn/problem/P1014) | [P1014.cpp](./P1014.cpp) | | 观察枚举规律写出模拟过程即可。本题还有更快的、非模拟写法。 |  
| P1307 | [数字反转](https://www.luogu.com.cn/problem/P1307) | [P1307.cpp](./P1307.cpp) | | 以字符串的形式去处理数字，用**双指针交换字符**对字符串进行翻转并**剔除高位多余的0**。 |  
| P1046 | [陶陶摘苹果](https://www.luogu.com.cn/problem/P1046) | [P1046.cpp](./P1046.cpp) | | 简单题。扫描数组统计符合要求的值的个数即可。 |  
| P1047 | [校门外的树](https://www.luogu.com.cn/problem/P1047) | [P1047.cpp](./P1047.cpp) | | 用数组记录各棵树是否被移走即可。 |  
| P1427 | [小鱼的数字游戏](https://www.luogu.com.cn/problem/P1427) | [P1427.cpp](./P1427.cpp) | | 用数组模拟一个栈，以在输出时倒序输出。 |  
| P2141 | [珠心算测验](https://www.luogu.com.cn/problem/P2141) | [P2141.cpp](./P2141.cpp) | | 采用一个**标记数组**来标记**某个和是否出现**，也就是将序列中任两数之和作为下标对标记数组进行随机访问。最后扫描序列，统计在标记数组中标记为`true`的数字数量即可。 |  
| P5594 | [【XR-4】模拟赛](https://www.luogu.com.cn/problem/P5594) | [P5594.cpp](./P5594.cpp) | | 用二维数组来表示**某天**中**是否有人做某套模拟卷**并统计某天中**需要多少套不同的模拟卷**即可。 |  
| P5015 | [标题统计](https://www.luogu.com.cn/problem/P5015) | [P5015.cpp](./P5015.cpp) | | 逐字符读入，判断每个字符是否满足要求，进行统计即可。 |  
| P1055 | [ISBN号码](https://www.luogu.com.cn/problem/P1055) | [P1055.cpp](./P1055.cpp) | | 按格式读取ISBN号并进行计算即可。本题值得回顾的是`printf`和`scanf`的**限定字符串宽度**进行输出/输入的方法。 |  
| P1308 | [统计单词数](https://www.luogu.com.cn/problem/P1308) | [P1308.cpp](./P1308.cpp) | | 主要考察基本的字符串读取和处理以及比较。注意题目中**没有**说两个词之间只会有一个空格。 |  
| P2010 | [回文日期](https://www.luogu.com.cn/problem/P2010) | [P2010.cpp](./P2010.cpp) | | **枚举年份**，每枚举一年，就把年份倒过来作为月日，判断日期是否合法即可。 |  
| P1012 | [拼数](https://www.luogu.com.cn/problem/P1012) | [P1012.cpp](./P1012.cpp) | 0.5√ | 按照**字符串字典序降序**进行排序，值得注意的是，比较过程中**当字符串长度不等**时可能需要额外处理。 |  
| P5587 | [打字练习](https://www.luogu.com.cn/problem/P5587) | [P5587.cpp](./P5587.cpp) | √ | 先**逐行**读入并处理每行字符串中的**退格情况**，然后将**已经处理了退格的字符串**进行对比，统计相同的字符数即可。注意本题的“范文”中**也有退格键**。 |  
| P1028 | [数的计算](https://www.luogu.com.cn/problem/P1028) | 递归做法: [P1028.cpp](./P1028.cpp) <br> DP做法: [P1028-dp.cpp](./P1028-dp.cpp) <br> 改进的DP做法: [P1028-dp-optimized.cpp](P1028-dp-optimized.cpp) | 0.5√ | 寻找规律进行递推，可以用递归亦或是DP进行解决。本题的关键就是找到**递推公式**。 |  
| P1036 | [数的计算](https://www.luogu.com.cn/problem/P1036) | [P1036.cpp](./P1036.cpp) |  | 利用**限制深度**的**深度优先搜索遍历**(DFS)写法，另外注意，并不需要每层都把`n`个数全遍历一遍。 |  
| P1464 | [Function](https://www.luogu.com.cn/problem/P1464) | [P1464.cpp](./P1464.cpp) |  | **记忆化搜索**入门题。用一个三维数组**储存递归函数之前计算过的结果**，以避免重复调用，增加无谓的开销。 |  
| P5534 | [【XR-3】等差数列](https://www.luogu.com.cn/problem/P5534) | [P5534.cpp](./P5534.cpp) <br> 用求和公式:[P5534-formula.cpp](./P5534-formula.cpp) |  | 简单递推题，可以用等差数列求和公式秒杀。 |  
| P1192 | [台阶问题](https://www.luogu.com.cn/problem/P1192) | $O(n)$ :[P1192-optimized.cpp](./P1192-optimized.cpp) <br> $O(n^2)$: [P1192.cpp](./P1192.cpp) | √ | 经典动规题，递推时要注意：可能**两值相减会得到负数**，因此要在相减后加上除数再进行**模除数**运算。 |  
| P1025 | [数的划分](https://www.luogu.com.cn/problem/P1025) |[P1025.cpp](./P1025.cpp)  | √ | 用**DFS**搜索+**剪枝**进行解决，剪枝的规律通过打表来观察出来。 |  
| P4994 | [终于结束的起点](https://www.luogu.com.cn/problem/P4994) |[P4994.cpp](./P4994.cpp)  |  | 用两个变量进行斐波那契数列递推即可，整个运算只涉及到第`n`和`n+1`项。另外利用模运算性质将数据值大小限定在`[0,M)`内。 |  

## Part.2 基础算法

| 题目编号 | 题目 | 我的题解 | 值得玩味 |备注|
|:---:|:---:|:---:|:---:|:---:|
| P1003 | [铺地毯](https://www.luogu.com.cn/problem/P1003) | [P1003.cpp](./P1003.cpp) | | 简单的几何计算题 |  
| P1024 | [一元三次方程求解](https://www.luogu.com.cn/problem/P1024) | [P1024.cpp](./P1024.cpp) | √ | **小区间**内的**浮点数折半查找**，还挺新奇的，值得回顾。 |  
| P1226 | [快速幂and取余运算](https://www.luogu.com.cn/problem/P1226) | [P1226.cpp](./P1226.cpp) | √ | **快速幂**模板题，快速幂的核心其实就是**进制转换**与**递推**，这种思想很值得回顾。 |  
| P1208 | [混合牛奶 Mixing Milk](https://www.luogu.com.cn/problem/P1208) | [P1208.cpp](./P1208.cpp) |  | 基础的**贪心算法**题，每次决策的时候只选定目前最优的方案。 |  
| P1601 | [ A+B Problem（高精）](https://www.luogu.com.cn/problem/P1601) | [P1601.cpp](./P1601.cpp) |  | 实现高精度的**无符号十进制整数**加法，注意运算过程中的进位以及**运算终止条件**。 |  
| P4995 | [跳跳！](https://www.luogu.com.cn/problem/P4995) | [P4995.cpp](./P4995.cpp) |  | 典型的贪心算法题，需要借助排序来降低时间复杂度。注意最后输出的数值大小可能超过`int`类型所能表示的范围。 |  
| P1094 | [纪念品分组](https://www.luogu.com.cn/problem/P1094) | [P1094.cpp](./P1094.cpp) |  | 贪心算法+排序优化题。关键点在于**尽量让每组的纪念品价格总和接近上限**。 |  
| P1067 | [多项式输出](https://www.luogu.com.cn/problem/P1067) | [P1067.cpp](./P1067.cpp) |  | 简单粗暴的模拟题。 |  