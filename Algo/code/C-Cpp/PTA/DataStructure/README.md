### 数据结构与算法题目集（中文）

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