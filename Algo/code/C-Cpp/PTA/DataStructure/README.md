# 数据结构与算法题目集（中文）

## 函数题

💡 这一部分最值得回味的是题**6-12**，是关于**二叉搜索/排序树**的。

|编号|题目链接|我的题解|主要思路|易错点|
|:---:|:---:|:---:|:---:|:---:|
|6-1|[链表逆转](https://pintia.cn/problem-sets/15/exam/problems/724)|[DS-6-1.c](./DS-6-1.c)| 顺序遍历链表节点。每遍历到一个节点，就将当前节点接到前一个节点的前面 | 注意输入**链表为空或者只有一个链表节点**的情况！ |
|6-2|[顺序表操作集](https://pintia.cn/problem-sets/15/exam/problems/725)|[DS-6-2.c](./DS-6-2.c)| 数据结构顺序表的基本四类操作 | 进行删除和插入操作时注意**操作位置P**是否合法。在进行删除、插入操作后不要忘了改变指向最后元素下标的变量Last |
|6-3|[求链式表的表长](https://pintia.cn/problem-sets/15/exam/problems/726)|[DS-6-3.c](./DS-6-3.c)| 遍历链表进行计数 | 无 |
|6-4|[链式表的按序号查找](https://pintia.cn/problem-sets/15/exam/problems/727)|[DS-6-4.c](./DS-6-4.c)| 遍历链表找到对应序号的元素 | 无 |
|6-5|[链式表操作集](https://pintia.cn/problem-sets/15/exam/problems/728)|[DS-6-5.c](./DS-6-5.c)| 针对链表的增删查操作 | 注意**表头指针**的变动，也不要忘了操作位置的前一个节点 |
|6-6|[带头结点的链式表操作集](https://pintia.cn/problem-sets/15/exam/problems/729)|[DS-6-6.c](./DS-6-6.c)| 针对链表的增删查操作(带头节点) | 注意循环终止条件的写法。 |
|6-7|[在一个数组中实现两个堆栈](https://pintia.cn/problem-sets/15/exam/problems/730)|[DS-6-7.c](./DS-6-7.c)| 两个栈的栈顶下标移动方向相反 | PTA的裁判程序要求**栈顶下标**必须是对应**栈顶元素**的。 |
|6-8|[求二叉树高度](https://pintia.cn/problem-sets/15/exam/problems/731)|[DS-6-8.c](./DS-6-8.c)| 递归遍历二叉树，采用贪心策略，每次都找**最高的子树**的高度进行累积。 | 无 |
|6-9|[二叉树的遍历](https://pintia.cn/problem-sets/15/exam/problems/732)|[DS-6-9.c](./DS-6-9.c)| 二叉树的几种基本遍历方法，前中后序遍历是**DFS**，而层序遍历是**BFS** | 无 |
|6-10|[二分查找](https://pintia.cn/problem-sets/15/exam/problems/923)|[DS-6-10.c](./DS-6-10.c)| 二分查找顺序表(数组) | 注意本题元素是从下标1开始储存的 |
|6-11|[先序输出叶结点](https://pintia.cn/problem-sets/15/exam/problems/925)|[DS-6-11.c](./DS-6-11.c)| 先序遍历二叉树，输出所有叶节点 | 无 |
|6-12|[二叉搜索树的操作集](https://pintia.cn/problem-sets/15/exam/problems/927)|[DS-6-12.c](./DS-6-12.c)| 每一棵树的左子树放比根节点小的数据，右子树放比根节点大的数据，以此构造二叉树 | 一定要注意**删除**结点的写法，细节点很多。 |

## 编程题

|编号|题目链接|我的题解|关键思路|值得玩味|
|:---:|:---:|:---:|:---:|:---:|
|7-1|[最大子列和问题](https://pintia.cn/problem-sets/15/exam/problems/709)|[DS-7-1.cpp](./DS-7-1.cpp)| 枚举子序列长度，多次求和运算中找出拥有最大和的子序列。利用数组**缓存求出的连续子序列和**，用于后序子序列求和运算，用空间换时间。 |  |
|7-1|[最大子列和问题](https://pintia.cn/problem-sets/15/exam/problems/709)|[DS-7-1-optimal.cpp](./DS-7-1-optimal.cpp)| (最优解)只关心序列中**能求出的最大和**是多少，一遍循环找出部分序列累加的最大和即可。 | √ |
|7-2|[一元多项式的乘法与加法运算](https://pintia.cn/problem-sets/15/exam/problems/710)|[DS-7-2.cpp](./DS-7-2.cpp)| 多项式乘法的结果中的项要按指数降序排列。加法得益于输入项是按指数降序排列的，可以用两个指针来寻找同类项。 | 0.5√ |
|7-3|[树的同构](https://pintia.cn/problem-sets/15/exam/problems/711)|[DS-7-3.cpp](./DS-7-3.cpp)| 采用**DFS**思想，同时遍历两棵树，比对每个子树根节点下的左孩子和右孩子，必要时**交换**后再比对 |  |
|7-4|[是否同一棵二叉搜索树](https://pintia.cn/problem-sets/15/exam/problems/712)|AC: [DS-7-4.cpp](./DS-7-4.cpp) <br> MLE: [DS-7-4-MLE.cpp](./DS-7-4-MLE.cpp)| 树的每个结点中**新增一个标记位**，通过标记位实现两棵树的比较。在每次比较过后，所有标记位要**归位**为0 | √ |
|7-5|[堆中的路径](https://pintia.cn/problem-sets/15/exam/problems/713)|AC: [DS-7-5.cpp](./DS-7-5.cpp) <br> 自下而上建堆: [DS-7-5-SiftDown.cpp](./DS-7-5-SiftDown.cpp)| 要AC的话得采用**自顶向下**建堆法，具体实现是对每个**插入尾部**的节点进行**上滤**操作 |  |
|7-6|[列出连通集](https://pintia.cn/problem-sets/15/exam/problems/714)| [DS-7-6.cpp](./DS-7-6.cpp)| 考察**无向图的储存**，以及图的**DFS**,**BFS**遍历 |  |
|7-7|[六度空间](https://pintia.cn/problem-sets/15/exam/problems/715)| [DS-7-7.cpp](./DS-7-7.cpp)| 采用**无向图**的**BFS**遍历，值得玩味的地方在于**记录每个顶点所在层数** | 0.5√ |
|7-8|[哈利·波特的考试](https://pintia.cn/problem-sets/15/exam/problems/716)| [DS-7-8.cpp](./DS-7-8.cpp) <br> 完全图生成工具: [completeGraph.js](./completeGraph.js) | 求**无向图**的**单源最短路径**，**Dijkstra算法**的写法很值得回味(本题我采用的是暴力Dijkstra写法) | √ |
|7-9|[旅游规划](https://pintia.cn/problem-sets/15/exam/problems/717)| [DS-7-9.cpp](./DS-7-9.cpp) | 求**无向图**的**两点间最短路径**，采用**优先队列辅助的Dijkstra算法**(本题我**手写实现了基于小根堆的优先队列**) | √ |
|7-10|[公路村村通](https://pintia.cn/problem-sets/15/exam/problems/718)| Prim(**AC**): [DS-7-10.cpp](./DS-7-10.cpp) <br> Kruskal(**TLE**): [DS-7-10-Kruskal.cpp](./DS-7-10-Kruskal.cpp) <br> Dijkstra(错误思路): [DS-7-10-WA-Dijkstra.cpp](./DS-7-10-WA-Dijkstra.cpp) | 求**无向图**的**最小生成树**的**权**，有Prim和Kruskal实现 | √ |
|7-11|[关键活动](https://pintia.cn/problem-sets/15/exam/problems/719)| [DS-7-11.cpp](./DS-7-11.cpp) <br> 随机AOE网生成工具: [randomAOE.js](./randomAOE.js) | 通过**拓扑排序**序列求**AOE**网(无环有向图)的**关键活动**。本题考察的点非常全面，涵盖到了关键路径这一块的所有内容。个人感觉出的很好，很适合查漏补缺。 | √√ |
|7-13|[统计工龄](https://pintia.cn/problem-sets/15/exam/problems/721)| [DS-7-13.cpp](./DS-7-13.cpp) | 简单题，利用**直接定址**哈希表对相应工龄的员工数进行统计  |  |
|7-14|[电话聊天狂人](https://pintia.cn/problem-sets/15/exam/problems/722)| [DS-7-14.cpp](./DS-7-14.cpp) | 利用**哈希表**对大规模数据进行统计工作，采用**位与取余法**，并利用**链地址法**处理哈希碰撞。位与取余 `被除数 & (除数-1)`的公式值得记忆，不过这里的**除数必须是2的N次方**  |  |
|7-15|[QQ帐户的申请与登陆](https://pintia.cn/problem-sets/15/exam/problems/723)| [DS-7-15.cpp](./DS-7-15.cpp) | 利用**哈希表**存取账号信息，哈希表实现几乎和题**7-14**一致。  |  |
|7-16|[一元多项式求导](https://pintia.cn/problem-sets/15/exam/problems/820)| [DS-7-16.cpp](./DS-7-16.cpp) | 入门题，边输入边处理边输出，无需任何其他辅助空间  |  |
|7-17|[汉诺塔的非递归实现](https://pintia.cn/problem-sets/15/exam/problems/821)| [DS-7-17.cpp](./DS-7-17.cpp) <br> 递归实现: [DS-7-17-recursive.cpp](./DS-7-17-recursive.cpp) | 经典递归问题-汉诺塔问题的**非递归实现**。借助栈和循环进行实现。适合用于回味递归思想。 | √ |
|7-18|[银行业务队列简单模拟](https://pintia.cn/problem-sets/15/exam/problems/825)| [DS-7-18.cpp](./DS-7-18.cpp) | 利用个**队列**结构模拟两个窗口的处理过程 |  |

