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
|7-10|[公路村村通](https://pintia.cn/problem-sets/15/exam/problems/718)| Prim(**AC**): [DS-7-10.cpp](./DS-7-10.cpp) <br>Kruskal(**AC**,并查集): [DS-7-10-Kruskal.cpp](DS-7-10-Kruskal.cpp) <br> Kruskal(**TLE**): [DS-7-10-Kruskal-TLE.cpp](./DS-7-10-Kruskal-TLE.cpp) <br> Dijkstra(错误思路): [DS-7-10-WA-Dijkstra.cpp](./DS-7-10-WA-Dijkstra.cpp) | 求**无向图**的**最小生成树**的**权**，有**Prim**和**Kruskal**实现 | √ |
|7-11|[关键活动](https://pintia.cn/problem-sets/15/exam/problems/719)| [DS-7-11.cpp](./DS-7-11.cpp) <br> 随机AOE网生成工具: [randomAOE.js](./randomAOE.js) | 通过**拓扑排序**序列求**AOE**网(无环有向图)的**关键活动**。本题考察的点非常全面，涵盖到了关键路径这一块的所有内容。个人感觉出的很好，很适合查漏补缺。 | √√ |
|7-12|[排序](https://pintia.cn/problem-sets/15/exam/problems/720)| [DS-7-12.cpp](./DS-7-12.cpp) | **常用的排序算法**我都尝试写了一遍，适合复习。 | √√ |
|7-13|[统计工龄](https://pintia.cn/problem-sets/15/exam/problems/721)| [DS-7-13.cpp](./DS-7-13.cpp) | 简单题，利用**直接定址**哈希表对相应工龄的员工数进行统计  |  |
|7-14|[电话聊天狂人](https://pintia.cn/problem-sets/15/exam/problems/722)| [DS-7-14.cpp](./DS-7-14.cpp) | 利用**哈希表**对大规模数据进行统计工作，采用**位与取余法**，并利用**链地址法**处理哈希碰撞。位与取余 `被除数 & (除数-1)`的公式值得记忆，不过这里的**除数必须是2的N次方**  |  |
|7-15|[QQ帐户的申请与登陆](https://pintia.cn/problem-sets/15/exam/problems/723)| [DS-7-15.cpp](./DS-7-15.cpp) | 利用**哈希表**存取账号信息，哈希表实现几乎和题**7-14**一致。  |  |
|7-16|[一元多项式求导](https://pintia.cn/problem-sets/15/exam/problems/820)| [DS-7-16.cpp](./DS-7-16.cpp) | 入门题，边输入边处理边输出，无需任何其他辅助空间  |  |
|7-17|[汉诺塔的非递归实现](https://pintia.cn/problem-sets/15/exam/problems/821)| [DS-7-17.cpp](./DS-7-17.cpp) <br> 【递归实现: [DS-7-17-recursive.cpp](./DS-7-17-recursive.cpp)】 <br> 【咱写的[汉诺塔演示工具](https://cat-note.github.io/bottleofcat/Algo/code/Web/Hanoi/index.html)】 | 经典递归问题-汉诺塔问题的**非递归实现**。借助栈和循环进行实现。适合用于回味递归思想。 | √ |
|7-18|[银行业务队列简单模拟](https://pintia.cn/problem-sets/15/exam/problems/825)| [DS-7-18.cpp](./DS-7-18.cpp) | 利用个**队列**结构模拟两个窗口的处理过程 |  |
|7-19|[求链式线性表的倒数第K项](https://pintia.cn/problem-sets/15/exam/problems/826)| [DS-7-19.cpp](./DS-7-19.cpp) | 采用**队列**储存数字序列的**倒数K个数字**，只需要**取队头数字**即可找到倒数第K项 |  |
|7-20|[表达式转换](https://pintia.cn/problem-sets/15/exam/problems/827)| [DS-7-20.cpp](./DS-7-20.cpp) | 采用**栈**将**中缀表达式**转换为**后缀表达式**。本题AC要求十分严苛，包括**输出不能有多余空格**，**负数**，**小数**等情况，考察全面，十分适合用于复习。 | √√ |
|7-21|[求前缀表达式的值](https://pintia.cn/problem-sets/15/exam/problems/836)| [DS-7-21.cpp](./DS-7-21.cpp) <br> 按字符串读入的写法: [DS-7-21-string.cpp](./DS-7-21-string.cpp) | 采用**栈**计算**前缀表达式**的值。关于**前缀表达式能否被正确计算**的判断是本题的核心坑点。 | √ |
|7-22|[堆栈模拟队列](https://pintia.cn/problem-sets/15/exam/problems/837)| [DS-7-22.cpp](./DS-7-22.cpp) | 利用**容量不同**的**两个栈**实现一个队列。两个栈分为**入队栈**和**出队栈**，队列的所有元素并不是都要储存在入队栈中，入队栈满时**借助出队栈**也可以储存一些元素。**较小的那个栈**决定了**队列容量的上限**。 |  |
|7-23|[还原二叉树](https://pintia.cn/problem-sets/15/exam/problems/838)| [DS-7-23.cpp](./DS-7-23.cpp) | 利用**先序**遍历序列和**中序**遍历序列还原二叉树并求得高度。其中**先序序列**用于**确定根结点**，而随后的**中序序列**用于**确定左子树和右子树各自包含的结点**。程序实现采用递归，操作是**求字符串的子串**。本题利于理解二叉树的遍历序列，值得回顾。 | √ |
|7-24|[树种统计](https://pintia.cn/problem-sets/15/exam/problems/839)| [DS-7-24.cpp](./DS-7-24.cpp) | 利用C++ STL中的**map容器**即可快速解决。map容器底层的红黑树是有序的，其中自动对字符串键按字典序排了序。 |  |
|7-25|[朋友圈](https://pintia.cn/problem-sets/15/exam/problems/840)| [DS-7-25.cpp](./DS-7-25.cpp) | 本题考察**并查集**的**合并操作**，解题思路是将**独立、互不相交的学生集合**根据**俱乐部的关系**合并成**朋友圈集合**。合并过程中需要**对集合元素数量进行统计**，最后扫描并查集森林中**所有根节点**，找到**计数最大**的朋友圈集合，即为结果。<br>这是我第一次遇到考察并查集的题目，很值得回顾 | √ |
|7-26|[Windows消息队列](https://pintia.cn/problem-sets/15/exam/problems/841)| [DS-7-26.cpp](./DS-7-26.cpp) | 本题比较直白地考察了**优先队列**。本题的优先队列采用**小根堆**实现，在插入/弹出元素时，利用**上滤**和**下滤**两个基本方法进行维护。 |  |
|7-27|[家谱处理](https://pintia.cn/problem-sets/15/exam/problems/842)| [DS-7-27.cpp](./DS-7-27.cpp) <br> **随机测试数据**生成工具: [genogram.js](./genogram.js) | 归纳得出本题只需要储存**各节点的父子关系**，因此采用了类似于并查集的`parents`数组。<br> 然而要注意，这里的**树根节点**的**父节点最好单独标记**，而不是设置为根结点本身，以免**判断兄弟节点时出错**。 | 0.5√ |
|7-28|[搜索树判断](https://pintia.cn/problem-sets/15/exam/problems/843)| [DS-7-28.cpp](./DS-7-28.cpp) | 把**先序序列**的元素依次**插入**BST，然后对BST(如果有必要还要对镜像BST)进行先序遍历，**比对**先序序列即可。本题中所有遍历采用**堆栈+循环**实现。 |  |
|7-29|[修理牧场](https://pintia.cn/problem-sets/15/exam/problems/856)| [DS-7-29.cpp](./DS-7-29.cpp) | 不妨**反着看**切割的过程: 每次由**两个小木块**组成**一块大木头**。采用霍夫曼贪心算法，在序列中**每次选择最小的两块木板**来组成大木板，大木板的长度就是**单次切割的花费**，选择过程类似于霍夫曼树的构造。这题一定程度上能锻炼算法思维，值得回顾 | 0.5√ |
|7-30|[目录树](https://pintia.cn/problem-sets/15/exam/problems/857)| [DS-7-30.cpp](./DS-7-30.cpp) | 本题比较基础，考察**多叉树**的**构建**与**遍历**。多叉树节点中**后继节点的指针**用**数组**来储存。题目要求按**字典序**输出，因此本题咱还用到了`algorithm`库的`sort`函数。 | |
|7-31|[笛卡尔树](https://pintia.cn/problem-sets/15/exam/problems/858)| [DS-7-31.cpp](./DS-7-31.cpp) | 本题考察对**是否是二叉搜索树**和**是否是小根堆**的判断。值得回顾的是对二叉搜索树的判断，因为BST的**中序遍历序列**是呈**升序**的，因此如果题目给的二叉树的**中序遍历序列递增**，这棵树就是二叉搜索树的特性。| 0.5√ |
|7-32|[哥尼斯堡的“七桥问题”](https://pintia.cn/problem-sets/15/exam/problems/859)| [DS-7-32.cpp](./DS-7-32.cpp) | 考察对**无向图中是否有欧拉回路**的判断。无向图中如果有欧拉回路，那么图中**所有顶点都是连通的**，且**每个顶点的度数都是偶数**。判断连通性时用到了**并查集**。 | √ |
|7-33|[地下迷宫探索](https://pintia.cn/problem-sets/15/exam/problems/860)| [DS-7-33.cpp](./DS-7-33.cpp) | 本题采用**深度优先搜索遍历**DFS即可解决，需要注意的是，本题还需要**打印出遍历中回溯的路径**。 |  |
|7-34|[任务调度的合理性](https://pintia.cn/problem-sets/15/exam/problems/861)| [DS-7-34.cpp](./DS-7-34.cpp) / <br> **栈**辅助**拓扑排序**: [DS-7-34-withStack.cpp](./DS-7-34-withStack.cpp)  | 本题需要判断一张图是不是**AOV网**，也就是要判断是不是**无环**的有向图。我采用了**卡恩拓扑排序算法**，也就是比较常用的“**拆0入度顶点**”的方法。// 值得注意的是，拓扑排序可以用**栈**辅助储存**所有0入度的顶点**以优化运行时间。 |  |
|7-35|[城市间紧急救援](https://pintia.cn/problem-sets/15/exam/problems/862)| [DS-7-35.cpp](./DS-7-35.cpp) | 和题**7-9**一样是采用**Djikstra**算法，但更进一步。不仅有**涉及两个因素**的最短路径的取舍，同时还考察了**最短路径的记录与输出**以及**对等长最短路径条数的统计**。<br>------<br> 其中最值得回味的是统计最短路径条数: 额外借助一个数组`pathNum`，用`pathNum[i]`标记**从起点到顶点`i`的最短路径条数**。当更新最短路径长时，对顶点**最短路径条数**进行赋值；而出现等长最短路径时，就得对顶点**最短路径条数**进行累加。 | √ |
|7-36|[社交网络图中结点的“重要性”计算](https://pintia.cn/problem-sets/15/exam/problems/863)| [DS-7-36.cpp](./DS-7-36.cpp) | 常规的最短路径题，可以用Dijkstra高效求解。本题给出的是一张**无权**无向图，因此我默认**所有边的权为**`1`。 | |
|7-37|[模拟EXCEL排序](https://pintia.cn/problem-sets/15/exam/problems/864)| [DS-7-37.cpp](./DS-7-37.cpp) | 选择一种排序算法(**稳定性无所谓**)来对包含多字段的元组进行排序。这里我采用了STL的`priority_queue`，辅以运算符重载实现了**堆排序**。快速排序更快(像STL的`sort`函数就有快排实现)，但我还没完全理解。稍后理解了我会在排序题**7-12**中写一下。 | |
|7-38|[寻找大富翁](https://pintia.cn/problem-sets/15/exam/problems/865)| [DS-7-38.cpp](./DS-7-38.cpp) | 从题目看是比较明显的“TopK”问题，也就是从**可能的大规模序列**中找出**前K个最大的数值**，适合用堆排序。我的解法是维护一个**存放K个最大数的小根堆**(优先队列)。具体维护方法是: 读入数值时如果遇到**比堆顶元素还大**的数就**弹出堆顶**，将新数值**加入堆中**。 | |
|7-39|[魔法优惠券](https://pintia.cn/problem-sets/15/exam/problems/866)| [DS-7-39.cpp](./DS-7-39.cpp) | 分别且**成对**计算**负值优惠券和商品**以及**正值优惠券和商品**所得总回报，**不一定要用完所有优惠券**，**回避所有“倒贴”的情况**。具体解题上可以借助**快排**或者**堆排序**进行解决。 | |
|7-40|[奥运排行榜](https://pintia.cn/problem-sets/15/exam/problems/867)| [DS-7-40.cpp](./DS-7-40.cpp) | 本质是考察排序的题，但**对于并列排名**的处理值得琢磨。我的解法是把**每个国家**的**每种计算方式**下的**排名都算出来**，并**进行并列处理**。在最后查询国家排名时，只需要从国家**每种计算方式下的排名中选出最小的排名**即可。<br> 这题是我不擅长的**重复项处理**题，之前类似的题有**多项式合并**。 | 0.5√ |
|7-41|[PAT排名汇总](https://pintia.cn/problem-sets/15/exam/problems/888)| [DS-7-41.cpp](./DS-7-41.cpp)<br>测试数据生成脚本:<br>[randomRank7-41.js](randomRank7-41.js) | 题**7-40**的升级版，仍然考察的是**排序**+**并列排名处理**。先对每个分区进行排序，算出每个分区内考生的排名，然后再对全部考生进行排序，算出总排名即可。 |  |

