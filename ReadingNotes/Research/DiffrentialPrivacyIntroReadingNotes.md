# 差分隐私友好介绍阅读笔记

原文：[A friendly, non-technical introduction to differential privacy - Ted is writing things](https://desfontain.es/blog/friendly-intro-to-differential-privacy.html)  

差分隐私（Differential Privacy）已经备受瞩目，公司和政府都开始发布利用差分隐私方法处理后的数据，统计学家们也开始逐渐习惯于用这种新方法来保护数据。
# 1. 为什么差分隐私很棒

* 原文：[Why differential privacy is awesome - Ted is writing things](https://desfontain.es/blog/differential-privacy-awesomeness.html)  

“在发布和人相关的数据时，怎么同时保护他们的隐私？” 这是一个已经被抛出了很久的问题。前人提出的很多方法都有局限性，很难实际应用且容易破坏**数据效用（可用）性**（**Utility of the data**），即数据没法在实际场景中用。  

## 1.1. 差分隐私的核心思路

假设有一个流程，以一些数据（集）作为输入，并能返回一些输出结果：  

$$
datasets \rightarrow \fbox{Some Process} \rightarrow outputs
$$
中间这个流程可以是：  

1. 计算数据的统计数据。
2. 脱敏策略。
3. **机器学习训练过程**。  
4. ...

为了让这个流程在差分的层面是隐私安全的，需要对其进行修改，比如**加上一些随机扰动、噪声**。

* 对于不同的流程，可能需要设计不同的扰动模式。  

💡 在进行处理后，如果从数据集中**移除某个个体的数据后再执行同样的流程**，且其输出结果和没有移除该个体的数据集的处理结果 ❗ **基本上一致**，那么其就满足差分隐私。（无论移除的是哪个个体，也无论是什么数据集）  

* 先把这个处理过程定义为一个神奇的过程 $Magic$ ：

$$
\begin{matrix}
\fbox{A,B,C,D,E} & \rightarrow & \fbox{Magic} & \rightarrow & Outputs \\
Datasets & & & & \uparrow \\
& & & & 基本一致 \\
Datasets' & & & & \downarrow \\
\fbox{A,B,C,\ ,E} & \rightarrow & \fbox{Magic} & \rightarrow & Outputs' 
\end{matrix}
$$
💡 所谓的“**基本一致**”指的是经过相同的流程 $Magic$ ，在两个数据集上的输出**有类似的 ❗ 概率分布**情况。  

---

应用：这种时候如果有人想通过输出判断一个个体是否在原数据中（MIA）就很难了，一个输出可能来自存在这个个体数据的数据集 $Datasets$ ，也可能来自不包含这个个体数据的数据集 $Datasets'$ ，有没有这个个体，输出的概率都是类似的，因此难以判断。    

---

❗ 差分隐私**是处理流程的一部分**，而不是输入数据集和输出的。  

## 1.2. 差分隐私咋就那么特别咧

### 1.2.1. 不再需要建立攻击模型（Attack modeling）  

以前很多隐私保护方法都需要关于攻击者能力和行为的一些假设以实行防御思路，但实际上这是很难施行的，攻击者稍微绕绕弯，防御方法就打水漂了。

而差分隐私确保：  

1. 可以保护**任何个体**的**各种信息**。无关攻击者的目的。
2. 防御**无关攻击者的先验知识**，只要是攻击者未曾掌握的数据都能被保护。  

### 1.2.2. 可以量化隐私损失（Privacy loss）

DP 有明显的隐私损失（Privacy loss），记为 $\epsilon$ ，量化了攻击者通过分析数据或查询结果，能够推断出个体信息的**可能性**。 

### 1.2.3. 可量化数据组合的风险

分别以参数 $\epsilon$ 向 Alex 和 Brinn 提供差分隐私保护的数据，即使他们**私下勾结**，合并后的数据仍然受到差分隐私的保护，只是隐私参数变为 $2\epsilon$，风险是可量化的，隐私保护作用依然在。  

而像 K-匿名性保护方式可能就会不再有效。  


### 1.2.4. 核心利用点

**操作数据的流程中的不确定性**越多就能使得**攻击者面临的不确定性越多**，攻击也就越难，隐私保护也就越好。  


# 2. 更详细地了解差分隐私

* 原文：[Differential privacy in (a bit) more detail - Ted is writing things](https://desfontain.es/blog/differential-privacy-in-more-detail.html) 

