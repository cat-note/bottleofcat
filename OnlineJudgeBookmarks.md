# 咱的OJ错题本

老实说这玩意一道题做的时候可能要错几次才能通过，这里就选一些我觉得很有意思的题目记录一下。

## PTA

网站：https://pintia.cn/problem-sets/  

### 基础编程题目集  

|编号|题目&链接|主要思路|易错点|
|:---:|:---:|:---:|:---:|
|6-7|[统计某类完全平方数](https://pintia.cn/problem-sets/14/problems/739)|见[笔记](Algo/PerfectSquareWith2SameNumbers.md)|判断完全平方数时的逻辑判断不要将浮点型和整型一起比较|
|6-9|[统计个位数字](https://pintia.cn/problem-sets/14/problems/741)|大体同6-7|对于负数和0的处理容易漏掉|
|6-10|[阶乘计算升级版](https://pintia.cn/problem-sets/14/problems/742)|如果阶乘结果大到C语言中任意一种**基本数据类型**都无法表达，不妨考虑一下能不能用**某种数据结构**来解决问题。详见[笔记](Algo/BigNumberFactorial.md)|一股脑地用基本数据类型 / 遗漏了乘法进位的一种情况|