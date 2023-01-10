# 【简记数据结构】AVL树的插入操作

AVL树又称平衡二叉**搜索(排序)树**，其最大的特点就是能维持所有节点的**左右子树高度差绝对值不大于1**。

* 一般对于每个节点都会添加一个**平衡因子(Balance Factor)字段**，平衡因子的值就是**左右子树的高度差**，程序借此判断某棵子树是否平衡。

因此，AVL树的**插入操作**要能维持住:

1. 二叉搜索树的节点大小关系。

2. 平衡二叉树中每个节点的平衡因子**绝对值不大于1**。

------

* 注:

    1. 本笔记中的平衡二叉树规定**所有左子树都小于其父节点，所有右子树都大于其父节点**。

    2. 本笔记中的平衡因子计算方法是```左子树高度 - 右子树高度```。

## 平衡树二叉的插入操作

AVL树的插入操作在二叉搜索(排序)树的操作的基础上新增了如下两个过程: 

1. 插入过程中将**沿途比较的节点**压入**栈**。

2. 插入完成后，借助**弹栈**来沿着插入时比较的各节点**回到整棵树的根节点**(回溯)，检查各节点的**平衡因子**，若出现了`平衡因子绝对值 > 1`的情况，则**对不平衡树进行调整**以保证整棵树的平衡性。

## 失衡节点

当树中某个节点的**平衡因子BF**不属于`[-1,1]`的范围时，这个节点就是**失衡节点**。

## 定义树节点

```cpp
typedef struct TreeNode *Tree;

struct TreeNode
{
    Tree left;  // 左子树
    Tree right; // 右子树
    short bf; // 平衡因子
    int val; // 节点值
};
```

## 纸上快速做题思路

这一招适合纸上解题。

首先插入新节点（**红色**的节点就是新插入的节点）:  
<img src="https://raw.githubusercontent.com/cat-note/bottleassets/main/img/paperStep1-2023-01-10.jpg" width="300px" />
此时【值为9的节点】平衡因子为2，为失衡节点。

1. 从**失衡节点**开始，沿着**插入新节点的比较路径**找，找到其中**与其最邻近的两个点**:  

    <img src="https://raw.githubusercontent.com/cat-note/bottleassets/main/img/paperStep2-2023-01-10.jpg" width="400px" />

2. 包括失衡节点在内，现在**一共有三个节点**，从中选择**值的大小在中间的节点**。

3. 将**除了中间值节点外的**两个节点按照**二叉搜索树的规则**接到【中间值节点】上，然后将【中间值节点】接到原本**失衡节点所在的位置**，作为根节点。

    <img src="https://raw.githubusercontent.com/cat-note/bottleassets/main/img/paperStep3-2023-01-10.jpg" width="400px" />

4. 将**移动了的节点的子树**按照**二叉搜索树插入规则**插入到这三个节点组成的子树中: 

    <img src="https://raw.githubusercontent.com/cat-note/bottleassets/main/img/paperStep4-2023-01-10.jpg" width="450px" />

5. 更新这三个节点中**子树发生改变的节点**的**平衡因子**:  

    <img src="https://raw.githubusercontent.com/cat-note/bottleassets/main/img/paperStep5-2023-01-10.jpg" width="300px" />

这种解题方法在纸上可以快速解决LL/LR/RL/RR这些类型的平衡调整问题，非常实用。

## LL型失衡

LL型字面展开来看就是**Left** - **Left**。意思是**新插入节点**位于**失衡节点**的**左孩子的左子树中**。

### 举例

![Unbalanced-LL-altered-2023-01-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/Unbalanced-LL-altered-2023-01-10.png)  

新节点插入在【值为2的节点】的**左子树中**，而【值为2的节点】又是【值为3的节点】的**左孩子**。  

此时【值为3的节点】的平衡因子`BF = 2-0 = 2 > 1`，是一个**失衡节点**。

* 注: 插入节点后的回溯过程当然是自下而上的，因此这里指的是自下而上**首个**失衡节点。

可以发现新节点插在【失衡节点】的**左孩子**的**左子树**中，这就是LL型失衡。

### 调整方法-右旋转

这里写的是程序实现思路，可以结合[纸上快速做题思路](#纸上快速做题思路)理解。

先不要着急想“右旋转”是什么。

![Unbalanced-LL-highlight-2023-01-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/Unbalanced-LL-highlight-2023-01-10.png)  

设**失衡节点**为A，**失衡节点的左孩子**为B，**失衡节点的左孩子的右子树**为C。

1. 回溯到**失衡节点A**时，发现失衡节点的平衡因子是`2`，且其**左孩子B**的平衡因子是`1`，这说明新插入节点插入在了**左孩子B**的**左子树**中，导致树左侧升高。

2. 将失衡节点A接到**左孩子B**的**右孩子**位置上，左孩子B原本的**右子树C**接到**节点A**的左孩子位置上。

    * 这里就是【值为3的节点】接到【值为2的节点】的右孩子上，【值为2的节点】原本的右子树NULL接到【值为3的节点】的左孩子上。(详见动图)

3. 将**节点B**接到**原本节点A**所在的位置，作为该子树的**根节点**。

* 动图演示过程:  

    ![Unbalanced-LL-animation-2023-01-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/Unbalanced-LL-animation-2023-01-10.gif)  

    通过动图演示就能很直观地看到这个“**右旋转**”的过程。

### 程序实现



## 判断失衡类型

## 插入后回溯

注意，回溯的时候如果更新某个节点平衡因子为0，就可以停止回溯了。