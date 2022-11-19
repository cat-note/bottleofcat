# 基础练习-BASIC

| 题目编号 | 题目 | 我的题解 | 值得玩味 |备注|
|:---:|:---:|:---:|:---:|:---:|
|BASIC-12|[十六进制转八进制](http://lx.lanqiao.cn/problem.page?gpid=T51)|[BASIC-12.cpp](./BASIC-12.cpp)|√|利用二进制数进行过渡，输出过程用了栈结构|
|BASIC-11|[十六进制转十进制](http://lx.lanqiao.cn/problem.page?gpid=T50)|[BASIC-11.cpp](./BASIC-11.cpp)|√|乘以位权并相加|
|BASIC-10|[十进制转十六进制](http://lx.lanqiao.cn/problem.page?gpid=T49)|[BASIC-10.cpp](./BASIC-10.cpp)|√|除以基数倒取余，倒取余用栈实现|
|BASIC-9|[特殊回文数](http://lx.lanqiao.cn/problem.page?gpid=T48)|[BASIC-9.cpp](./BASIC-9.cpp)| |如果要符合题目输出要求，暴力解法来的最快|
|BASIC-8|[回文数](http://lx.lanqiao.cn/problem.page?gpid=T47)|[BASIC-8.cpp](./BASIC-8.cpp)| |枚举个位和十位即可|
|BASIC-7|[特殊的数字](http://lx.lanqiao.cn/problem.page?gpid=T46)|[BASIC-7.cpp](./BASIC-7.cpp)| |枚举即可|
|BASIC-6|[杨辉三角形](http://lx.lanqiao.cn/problem.page?gpid=T10)|[BASIC-6.cpp](./BASIC-6.cpp)|√|利用性质：杨辉三角中任意一数等于其两肩上的数之和|
|BASIC-5|[查找整数](http://lx.lanqiao.cn/problem.page?gpid=T9)|[BASIC-5.cpp](./BASIC-5.cpp)| |简单的查找|
|BASIC-4|[数列特征](http://lx.lanqiao.cn/problem.page?gpid=T8)|[BASIC-4.cpp](./BASIC-4.cpp)| |简单的统计|
|BASIC-3|[字母图形](http://lx.lanqiao.cn/problem.page?gpid=T7)|[BASIC-3.cpp](./BASIC-3.cpp)|√|利用字符ASCII码和一个偏移量解决|
|BASIC-2|[01字串](http://lx.lanqiao.cn/problem.page?gpid=T6)|[BASIC-2.cpp](./BASIC-2.cpp)|√|模拟二进制数的递增，也可以直接for循环枚举|
|BASIC-1|[闰年判断](http://lx.lanqiao.cn/problem.page?gpid=T5)|[BASIC-1.cpp](./BASIC-1.cpp)| |闰年：被100整除时必须要被400整除，其余情况被4整除的年份|
|BASIC-04|[Fibonacci数列](http://lx.lanqiao.cn/problem.page?gpid=T4)|[BASIC-04.cpp](./BASIC-04.cpp)| √ |写的是斐波那契数列，实际上考察的是对取模的理解|
|BASIC-03|[圆的面积](http://lx.lanqiao.cn/problem.page?gpid=T3)|[BASIC-03.cpp](./BASIC-03.cpp)|  |一种圆周率的算法:`π≈arctan(1)*4`|  
|BASIC-02|[序列求和](http://lx.lanqiao.cn/problem.page?gpid=T2)|[BASIC-02.cpp](./BASIC-02.cpp)|  |求和公式（等差数列）来处理大规模数值序列|  
|BASIC-30|[阶乘计算](https://www.dotcpp.com/oj/problem1474.html)|[BASIC-30.cpp](./BASIC-30.cpp)|√|用数组存放大规模的结果，手动实现乘法和进位操作|
|BASIC-29|[高精度加法](https://www.dotcpp.com/oj/problem1475.html)|[BASIC-29.cpp](./BASIC-29.cpp)| |类似`BASIC-30`，用数组存放结果，手动实现进位操作，比较有趣的是接受a,b数输入的方法|
|BASIC-28|[Huffuman树](https://www.dotcpp.com/oj/problem1462.html)|[BASIC-28.cpp](./BASIC-28.cpp)|√|选择出数列中最小两个数的方法值得品味一下|
|BASIC-27|[2n皇后问题](https://www.dotcpp.com/oj/problem1460.html)|[BASIC-27.cpp](./BASIC-27.cpp)|√√|采用`DFS`的思想枚举所有可行情况, 另外对斜线方向上的冲突判断值得玩味 |
|BASIC-26|[报时助手](https://www.dotcpp.com/oj/problem1468.html)|[BASIC-26.cpp](./BASIC-26.cpp)| | 基本的转换和条件判断 |  
|BASIC-25|[回形取数](https://www.dotcpp.com/oj/problem1465.html)|[BASIC-25.cpp](./BASIC-25.cpp)| | 从外环向内环，除最内环外，每环上的遍历要经过四个方向(四个`for`循环) |  
|BASIC-24|[龟兔赛跑预测](https://www.dotcpp.com/oj/problem1476.html)|[BASIC-24.cpp](./BASIC-24.cpp)| | 数值规模不大，直接用一个循环模拟整个比赛过程即可 |  
|BASIC-23|[芯片测试](https://www.dotcpp.com/oj/problem1473.html)|[BASIC-23.cpp](./BASIC-23.cpp)| | 抓住题目关键条件，利用数组统计结果 |   
|BASIC-22|[FJ的字符串](https://www.dotcpp.com/oj/problem1461.html)|[BASIC-22.cpp](./BASIC-22.cpp)| | 观察规律，拼接字符串 |  
|BASIC-21|[Sine之舞](https://www.dotcpp.com/oj/problem1463.html)|[BASIC-21.cpp](./BASIC-21.cpp)| | 观察规律，拼接字符串，同`BASIC-22`一样可以用非递归写法 |  


