# 【简记数据结构】AVL树的插入操作

AVL树又称**平衡**二叉**搜索(排序)树**，其最大的特点就是能维持所有节点的**左右子树高度差绝对值不大于1**。

* 一般对于每个节点都会添加一个**平衡因子(Balance Factor)字段**，平衡因子的值就是**左右子树的高度差**，程序借此判断某棵子树是否平衡。

因此，AVL树的**插入操作**要能维持住:

1. 二叉搜索树的节点大小关系。

2. 平衡二叉树中每个节点的平衡因子**绝对值不大于1**。

------

* 注:

    1. 本笔记中的平衡二叉树规定**所有左子树都小于其父节点，所有右子树都大于其父节点**。

    2. 本笔记中的平衡因子计算方法是```左子树高度 - 右子树高度```。

------

## 简述平衡二叉树的插入操作

AVL树的插入操作在二叉搜索(排序)树的操作的基础上新增了如下两个过程: 

1. 插入过程中将**沿途比较的节点**压入**栈**。

2. 插入完成后，借助**弹栈**来沿着插入时比较的各节点**回到整棵树的根节点**(回溯)，检查各节点的**平衡因子**，若出现了`平衡因子绝对值 > 1`的情况，则**对不平衡树进行调整**以保证整棵树的平衡性。

------

## 什么是失衡节点

当树中某个节点的**平衡因子BF**值不属于`[-1,1]`的范围时，这个节点就是**失衡节点**。

------

## 纸上快速做题思路

这一招适合纸上解题。

首先插入新节点（**红色**的节点就是新插入的节点）:  
<img src="https://raw.githubusercontent.com/cat-note/bottleassets/main/img/paperStep1-2023-01-10.jpg" width="300px" />
此时【值为9的节点】平衡因子为2，为失衡节点。

1. 从**失衡节点**开始，沿着**插入新节点的比较路径**找，找到其中**与其最邻近的两个点**:  

    <img src="https://raw.githubusercontent.com/cat-note/bottleassets/main/img/paperStep2-2023-01-10.jpg" width="400px" />

2. 包括失衡节点在内，现在**一共有三个节点**，从中选择**值的大小在中间的节点**。

3. 将**除了中间值节点外的**两个节点按照**二叉搜索树的规则**接到【中间值节点】上，然后将【中间值节点】接到原本**失衡节点所在的位置**，作为这棵子树的根节点。

    <img src="https://raw.githubusercontent.com/cat-note/bottleassets/main/img/paperStep3-2023-01-10.jpg" width="400px" />

4. 将**移动了的节点的子树**按照**二叉搜索树插入规则**插入到这三个节点组成的子树中: 

    <img src="https://raw.githubusercontent.com/cat-note/bottleassets/main/img/paperStep4-2023-01-10.jpg" width="450px" />

5. 更新这三个节点中**子树发生改变的节点**的**平衡因子**:  

    <img src="https://raw.githubusercontent.com/cat-note/bottleassets/main/img/paperStep5-2023-01-10.jpg" width="300px" />

这种解题方法在纸上可以快速解决LL/LR/RL/RR这些类型的平衡调整问题，非常实用。

程序实现的话也可以靠这个思路来记忆和理解。


------

## 程序中定义树节点

这里的树节点**没有**指向父节点的指针。

```cpp
typedef struct TreeNode *Tree;

struct TreeNode
{
    Tree left;  // 左子树
    Tree right; // 右子树
    int height; // 节点所在高度，平衡因子靠这个算
    int val;    // 节点值
};
```

------


## LL型失衡

LL型字面展开来看就是**Left** - **Left**。意思是**新插入节点**位于**失衡节点**的**左孩子的左子树中**。

### 举例

![Unbalanced-LL-altered-2023-01-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/Unbalanced-LL-altered-2023-01-10.png)  

新节点插入在【值为2的节点】的**左子树中**，而【值为2的节点】又是【值为3的节点】的**左孩子**。  

此时【值为3的节点】的平衡因子`BF = 2-0 = 2 > 1`，是一个**失衡节点**。

* 注: 插入节点后的回溯过程当然是自下而上的，因此这里指的是自下而上**首个**失衡节点。

可以发现新节点插在【失衡节点】的**左孩子**的**左子树**中，这就是LL型失衡。

### 调整方法-右旋转

这里我结合[纸上快速做题思路](#纸上快速做题思路)来写一下。

![Unbalanced-LL-highlight-2023-01-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/Unbalanced-LL-highlight-2023-01-10.png)  

【值为3的节点】是失衡节点。

1. 找到失衡节点**沿着插入路径上的**最邻近的两个节点，一共有三个节点。  
    这里可以看成是**以失衡节点为根结点的子树**。

    * 就是图中框出来的几个节点。

2. 找到三个节点中【**值在中间的节点**】，接下来的“右旋转”过程以它为**轴**。

    * 上图中找出的就是【值为2的节点】。其实就是失衡节点的左孩子。

3. 将**失衡节点**以【值在中间的节点】为轴进行**右旋转**(顺时针)，让【值在中间的节点】变成这棵子树的**新的根结点**。

    * 上图中的【值为3的节点】围绕【值为2的节点】进行右旋转，变成【值为2的节点】的右子树。

    * 【值为2的节点】**原本的右子树**变成【值为3的节点】的**左子树**。

    * 【值为2的节点】成为新的根结点。(详见动图)


* **动图演示过程**:  

    ![Unbalanced-LL-animation-2023-01-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/Unbalanced-LL-animation-2023-01-10.gif)  

    通过动图演示就能很直观地看到这个“**右旋转**”的过程。

    > 可以发现**旋转**节点围绕的“旋转轴”就是【三个节点中**中间值的节点**】

    > 图中我特意标出了空子树`NULL`，程序实现的时候**一定要把子树考虑在内**哦。

### 程序实现

```cpp
// 失衡节点右旋操作，node是失衡结点
void rotateRight(Tree node)
{
    Tree nodeLeft = node->left;         // 失衡节点左子树
    Tree nodeRight = node->right;       // 失衡节点右子树
    Tree lChildLeft = nodeLeft->left;   // 失衡节点的左孩子的左子树
    Tree lChildRight = nodeLeft->right; // 失衡节点左孩子的右子树
    // 这里【没有指向父节点】的指针，我们直接修改结点的值来模拟移动结点即可
    int nodeVal = node->val;   // 失衡节点的值
    node->val = nodeLeft->val; // 交换失衡节点和左孩子的值
    nodeLeft->val = nodeVal;
    // 这里已经不是左孩子了，而是“旋转”下来的失衡节点
    nodeLeft->left = lChildRight; // 修改结点的左右子树
    nodeLeft->right = node->right;
    node->left = lChildLeft; // 和子树的根结点接上
    node->right = nodeLeft;
    // 此时node是子树根结点，lChildLeft是左子树，nodeLeft是右子树
    updateHeight(nodeLeft); // 更新有变动的结点的高度，先更新子树再更新根
    updateHeight(node);
}
```

------

## RR型失衡

RR型字面展开来看就是**Right** - **Right**。意思是**新插入节点**位于**失衡节点**的**右孩子的右子树中**。

### 举例

![Unbalanced-RR-2023-02-03](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/Unbalanced-RR-2023-02-03.png)  

新节点插入在【值为8的节点】的**右子树中**，而【值为8的节点】又是【值为7的节点】的**右孩子**。  

此时【值为7的节点】的平衡因子`BF = 0-2 = -2 < -1`，是一个**失衡节点**。

可以发现新节点插在【失衡节点】的**右孩子**的**右子树**中，这就是RR型失衡。

### 调整方法-左旋转

【值为7的节点】是失衡节点。

1. 找到失衡节点**沿着插入路径上的**最邻近的两个节点，一共有三个节点。  
    这里可以看成是**以失衡节点为根结点的子树**。

    * 就是图中框出来的几个节点。

2. 找到三个节点中【**值在中间的节点**】，接下来的“左旋转”过程以它为**轴**。

    * 上图中找出的就是【值为8的节点】。其实就是失衡节点的右孩子。

3. 将**失衡节点**以【值在中间的节点】为轴进行**左旋转**(逆时针)，让【值在中间的节点】变成这棵子树的**新的根结点**。

    * 上图中的【值为7的节点】围绕【值为8的节点】进行左旋转，变成【值为8的节点】的左子树。
    
    * 【值为8的节点】**原本的左子树**变成【值为7的节点】的**右子树**。

    * 【值为8的节点】成为新的根结点。(详见动图)


* **动图演示过程**:  

    ![Unbalanced-RR-animation-2023-02-08](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/Unbalanced-RR-animation-2023-02-08.gif)  

    RR型和LL型的调节过程是**对称**的。

### 程序实现

```cpp
// 失衡节点左旋操作，node是失衡节点
void rotateLeft(Tree node)
{
    Tree nodeLeft = node->left;          // 失衡节点左子树
    Tree nodeRight = node->right;        // 失衡节点右子树
    Tree rChildLeft = nodeRight->left;   // 失衡节点的右孩子的左子树
    Tree rChildRight = nodeRight->right; // 失衡节点的右孩子的右子树
    // 这里【没有指向父节点】的指针，我们直接修改结点的值来模拟移动结点
    int nodeVal = node->val;
    node->val = nodeRight->val; // 交换失衡节点和右孩子的值
    nodeRight->val = nodeVal;
    // 这里的nodeRight就是“旋转”下来的节点
    nodeRight->right = rChildLeft;
    nodeRight->left = node->left;
    node->left = nodeRight;
    node->right = rChildRight;
    // 此时node是子树根结点，nodeRight是左子树，rChildRight是右子树
    updateHeight(nodeRight); // 更新有变动的结点的高度，先更新子树再更新根
    updateHeight(node);
}
```

------

## 程序中判断失衡类型

依赖于平衡因子

------

## 程序中的插入操作

注意，回溯的时候如果更新某个节点平衡因子为0，就可以停止回溯了。

## 相关题目

