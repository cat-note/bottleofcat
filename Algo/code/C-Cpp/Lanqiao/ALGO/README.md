# 算法训练-ALGO

| 题目编号 | 题目 | 我的题解 | 值得玩味 |备注|
|:---:|:---:|:---:|:---:|:---:|
|ALGO-1007|[印章](https://lx.lanqiao.cn/problem.page?gpid=T3002)|[ALGO-1007.cpp](./ALGO-1007.cpp)| √ | DP(动态规划)入门题，咱也是第一次接触。这题我用了递归写法 |  
|ALGO-1006|[拿金币](https://lx.lanqiao.cn/problem.page?gpid=T3000)|[ALGO-1006.cpp](./ALGO-1006.cpp)| √ | DP(动态规划)入门题，这题适合非递归写法 |  
|ALGO-123|[A+B problem](https://www.dotcpp.com/oj/problem1586.html)|[ALGO-123.cpp](./ALGO-123.cpp)|  | 经典入门题。 |
|ALGO-107|[链表数据求和操作](https://www.dotcpp.com/oj/problem1585.html)|[ALGO-107.cpp](./ALGO-107.cpp)|  | 链表简单操作。另外复习复数的加减。 |
|ALGO-106|[判定字符位置](https://www.dotcpp.com/oj/problem1584.html)|[ALGO-106.cpp](./ALGO-106.cpp)|  | 简单的字符查找。空间换时间也挺不错的 |
|ALGO-140|[提货单](https://www.dotcpp.com/oj/problem1597.html)|[ALGO-140.cpp](./ALGO-140.cpp)|  | 简单求和 |
|ALGO-141|[学生成绩](https://www.dotcpp.com/oj/problem1598.html)|[ALGO-141.cpp](./ALGO-141.cpp)| 0.5√ | 复习基础算法**冒泡排序**。不停地交换相邻元素以达到排序目的。 |
|ALGO-139|[s01串](https://www.dotcpp.com/oj/problem1600.html)|[ALGO-139.cpp](./ALGO-139.cpp)|  | 简单的字符串处理 |
|ALGO-27|[FBI树](https://www.dotcpp.com/oj/problem1592.html)|[ALGO-27.cpp](./ALGO-27.cpp)|  | **满二叉树**的生成与**后序遍历** |
|ALGO-83|[阶乘](https://www.dotcpp.com/oj/problem1604.html)|[ALGO-83.cpp](./ALGO-83.cpp)| √ | 找阶乘结果低位的首个非零数，不妨**让阶乘过程中低位始终不出现0**。实现方法是剥去**每个乘数**中的**因数2**和**因数5**（因为10的质因数是2和5）。当然，最后还需要**还原**阶乘结果。 |
|ALGO-46|[Hanoi问题](https://www.dotcpp.com/oj/problem1594.html)|[ALGO-46.cpp](./ALGO-46.cpp)|  | 初始有`n`个盘子的汉诺塔**需要移动的次数的计算公式**是$2^n-1$。<br> 本题可以同时移动M个盘子，那就可以按M个一组将盘子分为几组，**组数就是新的汉诺塔盘子数** |