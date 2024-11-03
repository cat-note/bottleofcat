# 【题解笔记】LeetCode 134. Gas Station

题目地址：https://leetcode.cn/problems/gas-station/

## 题目描述

<details>
<summary>展开查看详情</summary>

<p>在一条环路上有 <code>n</code>&nbsp;个加油站，其中第 <code>i</code>&nbsp;个加油站有汽油&nbsp;<code>gas[i]</code><em>&nbsp;</em>升。</p>

<p>你有一辆油箱容量无限的的汽车，从第<em> </em><code>i</code><em> </em>个加油站开往第<em> </em><code>i+1</code><em>&nbsp;</em>个加油站需要消耗汽油&nbsp;<code>cost[i]</code><em>&nbsp;</em>升。你从其中的一个加油站出发，开始时油箱为空。</p>

<p>给定两个整数数组 <code>gas</code> 和 <code>cost</code> ，如果你可以按顺序绕环路行驶一周，则返回出发时加油站的编号，否则返回 <code>-1</code> 。如果存在解，则 <strong>保证</strong> 它是 <strong>唯一</strong> 的。</p>

<p>&nbsp;</p>

<p><strong>示例&nbsp;1:</strong></p>

<pre>
<strong>输入:</strong> gas = [1,2,3,4,5], cost = [3,4,5,1,2]
<strong>输出:</strong> 3
<strong>解释:
</strong>从 3 号加油站(索引为 3 处)出发，可获得 4 升汽油。此时油箱有 = 0 + 4 = 4 升汽油
开往 4 号加油站，此时油箱有 4 - 1 + 5 = 8 升汽油
开往 0 号加油站，此时油箱有 8 - 2 + 1 = 7 升汽油
开往 1 号加油站，此时油箱有 7 - 3 + 2 = 6 升汽油
开往 2 号加油站，此时油箱有 6 - 4 + 3 = 5 升汽油
开往 3 号加油站，你需要消耗 5 升汽油，正好足够你返回到 3 号加油站。
因此，3 可为起始索引。</pre>

<p><strong>示例 2:</strong></p>

<pre>
<strong>输入:</strong> gas = [2,3,4], cost = [3,4,3]
<strong>输出:</strong> -1
<strong>解释:
</strong>你不能从 0 号或 1 号加油站出发，因为没有足够的汽油可以让你行驶到下一个加油站。
我们从 2 号加油站出发，可以获得 4 升汽油。 此时油箱有 = 0 + 4 = 4 升汽油
开往 0 号加油站，此时油箱有 4 - 3 + 2 = 3 升汽油
开往 1 号加油站，此时油箱有 3 - 3 + 3 = 3 升汽油
你无法返回 2 号加油站，因为返程需要消耗 4 升汽油，但是你的油箱只有 3 升汽油。
因此，无论怎样，你都不可能绕环路行驶一周。</pre>

<p>&nbsp;</p>

<p><strong>提示:</strong></p>

<ul>
	<li><code>gas.length == n</code></li>
	<li><code>cost.length == n</code></li>
	<li><code>1 &lt;= n &lt;= 10<sup>5</sup></code></li>
	<li><code>0 &lt;= gas[i], cost[i] &lt;= 10<sup>4</sup></code></li>
</ul>


</details>

## 思路

首先注意这是**环路**，为了方便起见，我可以将数组遍历两遍，如果能回到起点，遍历两遍肯定能找到。  

设每次从 $i$ 走到 $i+1$ 的位置，**剩余的油**为： 

$$
diff_i=gas[i]-cost[i]
$$

如果我从 $a$ 出发走到 $b$ 这个位置，则在 $b$ 处**剩余的油**为：  

$$
\texttt{sum\_until}(b)=\sum_{i=start}^b diff_i
$$

* 此处 $start=a$

很明显，剩余的油为**负数**时，**我肯定是走不下去了**。  

从 $a$ 到 $b$，我一直在求和，其中有 $diff_i \lt 0$，也有 $diff_i \ge 0$（$i \in [a,b]$）：

1. 很明显我不能把 $diff_i \lt 0$ 对应的位置 $i$ 作为起点，刚起步油都不够用。
2. 如果我不把 $diff_i \ge 0$ 对应的位置 $i$ 作为起点，相当于我在求和的时候**去除了一个非负项**，最终到达 $b$ 的时候，求和得到的 $\texttt{sum\_until}(b)$ **只会保持不变或者负的更多**。

-----

怎么理解上面的第 2 点？

按照**第 1 点**可以知道，只要 $[a,b]$ 这个区间长度超过 1（$|[a,b]|>1$），那么必然有头部几个 $i$ 的 $diff_i \ge 0$，即：  

$$
\begin{matrix}
\exist i \in [a,b] \\
\forall j \in [a,i],\ diff_j \ge 0
\end{matrix}
$$

而如果 $\texttt{sum\_until}(b) \lt 0$，说明我从 $a$ 这个起点到 $b$ 这个地方的时候油不够用了，那么我接下来就至少要从 $a+1$ 这个起点开始。  

如果 $|[a,b]|>1$，那么必然有 $diff_a \ge 0$，因此我从 $a+1$ 开始就相当于抛弃了非负项 $diff_a$，最终  $\texttt{sum\_until}(b)$ **只会更小**。  

-------

因此， $start=a,\ \texttt{sum\_until}(b) \lt 0$ 时我**不能把 $i \in [a,b]$ 的所有位置作为起点**，这也是本题贪心的核心思路。  

目标是找到能返回起点，且最终剩余油量 $\ge 0$ 的方案。  

## 代码实现

### C++ 代码

```cpp
class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        // 注意是环路
        // 最开始肯定从一个 gas[i]-cost[i] >=0 的地方出发
        // 维护一个状态，存储油箱剩余的油量
        // 每从 i 到 i+1，剩余的油为 diff = gas[i]-cost[i]
        // 如果我走过多个地方，总共累积的油就是  sum(diff)
        // 但是如果到 j 这个地方和为负数，说明不能从 i 到 j 这个区间开始
        // i 到 j 区间肯定有 diff<0 的位置，也有 diff>0 的位置
        // 我明显不能从 diff<0 的位置开始，但如果我不从 diff>0 的地方开始
        // 最终的 sum(diff) 只会负的更多
        int gasSum = 0;
        int start = 0; // 记录开始的下标
        int n = cost.size();
        for (int i = 0; i < (n << 1); i++) {
            // 因为要转一轮，这里把数组多遍历一遍
            int pos = i % n;
            gasSum += (gas[pos] - cost[pos]);
            if (gasSum < 0) {
                // 汽油不够用了
                if (i + 1 >= n) {
                    // 超过原数组下标范围了，从哪个下标开始都不行
                    return -1;
                }
                start = i + 1; // 尝试从下一个位置开始
                gasSum = 0;
            } else if (start == pos + 1) {
                // 如果重叠上了，说明能回到起点
                break;
            }
        }
        return start;
    }
};
```

### Golang 代码

```go
func canCompleteCircuit(gas []int, cost []int) int {
	var gasSum, start int
	n := len(cost)
	for i := 0; i < (n << 1); i++ {
		// 因为要转一轮，这里把数组多遍历一遍
		pos := i % n
		gasSum += (gas[pos] - cost[pos])
		if gasSum < 0 {
			// 汽油不够用了
			if i+1 >= n {
				// 超过原数组下标范围了，从哪个下标开始都不行
				return -1
			}
			// 尝试从下一个位置重新开始
			start = i + 1
			gasSum = 0
		} else if start == pos+1 {
			// 下一步能回到起点
			break
		}
	}
	return start
}
```

## 复杂度分析

* 时间复杂度: $O(n)$  

* 空间复杂度: $O(1)$

## 提交结果截图 

![submitResult-2024-11-03](https://assets.xbottle.top/img/submitResult-2024-11-03.png?233)    