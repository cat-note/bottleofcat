# 真题练习-PRACTICAL

| 题目 | 我的题解 | 值得回味 | 备注 | 真题来源 |
|:---:|:---:|:---:|:---:|:---:|
|[左移右移](https://www.lanqiao.cn/problems/2219/learning/)|[2022-final-B-Java-1.cpp](./2022-final-B-Java-1.cpp)|  | 用**链表**储存序列，并用**数组**储存`x`与链表节点的映射关系，以按照**键值**对链表进行随机访问。 | 2022蓝桥国赛B组 |
|[(填空问题)](https://www.luogu.com.cn/problem/P8771)|[2022-final-B-Cpp-filling.cpp](./2022-final-B-Cpp-filling.cpp)|  | 第一题就是简单的进制转换。第二题有歧义，洛谷这里考虑`012`和`123`的情况就能AC。 | 2022蓝桥省赛B组 |
|[刷题统计](https://www.luogu.com.cn/problem/P8780)|[2022-final-B-Cpp-C.cpp](./2022-final-B-Cpp-C.cpp)|  | 用循环来模拟做题过程即可。注意，题目中的**数据规模可能达到** $10^{18}$ | 2022蓝桥省赛B组 |
|[修剪灌木](https://www.luogu.com.cn/problem/P8781)|[2022-final-B-Cpp-D.cpp](./2022-final-B-Cpp-D.cpp)|  | 对于每棵灌木，**越久没被修剪就会长得越高**，而修剪者是**循环往复**地在修剪的，因此需要寻找每棵灌木在左端和右端中**距离哪一端更远**。 | 2022蓝桥省赛B组 |
|[X进制减法](https://www.luogu.com.cn/problem/P8782)|[2022-final-B-Cpp-E.cpp](./2022-final-B-Cpp-E.cpp)| 0.5√ | 采用贪心思想。要使得A-B尽量小，那么**各位的进制也要尽可能的小**，但要保证A和B都是有效的数且最低进制不低于二进制。<br>同时要注意，运算过程中可能有溢出的情况，因此要多次进行模运算。<br>**很奇怪的是，本题的N没有用**。 | 2022蓝桥省赛B组 |
|[统计子矩阵](https://www.luogu.com.cn/problem/P8783)|指针无回退： [2022-final-B-Cpp-F.cpp](./2022-final-B-Cpp-F.cpp) <br> 指针有回退(80%AC): [2022-final-B-Cpp-F-80.cpp](./2022-final-B-Cpp-F-80.cpp)| √ | 以**二维前缀和**的形式储存**矩阵和**以简化运算，然后用四个指针`up`/`down`/`left`/`right`来枚举子矩阵。我的做法是先确定子高度(`up`和`down`)，然后用`left`和`right`双指针去寻找**同高度下**所有满足要求的子矩阵。 | 2022蓝桥省赛B组 |

