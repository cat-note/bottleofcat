# 关系数据库de关系代数小记

最近学数据库概论学到了**关系数据库**的关系代数了。哎嘛，真的把我整晕了，尤其是关系代数的使用，很容易让人被蒙在鼓里。  

![letmeunderstand-2021-10-06](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/letmeunderstand-2021-10-06.webp)

**对我来说**槽点最大的莫过于书上的例子不多了，本来就挺抽象的一种**查询**语言，加上没有足够多让我懂的例子<del>(不过我本来也比较迟钝了)</del>，真的是难上加难...  
但没办法啊，这是我需要跨过的一道坎，于是我决定慢慢写出这篇小记（**主要针对数据库关系代数**）。不求挖的多深，但求能多少理解地透彻点😂  
目标是...希望大家看到这篇文章也能有所收获吧！废话不多说，开整！  o(￣ヘ￣o＃)

* 咱学校使用的教材是：**《数据库系统概论（第五版）》王珊主编 - 高等教育出版社**  

* **注意，因采用了动图(WebP)直观表达，流量用户请慎重浏览~**

## 0x01 小概念
* **行与列**

    ![rowandcolumn-2021-10-06](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/rowandcolumn-2021-10-06.webp)

* **目** & **度** & **元数**  

    ![meshanddegree-2021-10-06](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/meshanddegree-2021-10-06.webp)  

    1.如果使用关系表进行展示的话，**属性数（字段数）**其实就是**列数**，而进一步我们将属性数称为**元数**或者**目**或者**度**。  

    2.这样看来其实**元数就是目，目也就是度**。  

* **属性** & **字段**  

    1.**属性**就是**字段**（至少在数据库表这里是这样），都表示的是**列**。  

    2.上面和下面的图中第一行 **学号,姓名,性别...** 等等是 **属性名（字段名）**。  

* **元组** & **基数** & **分量**

    ![tupleandcardinalityandcomponent-2021-10-06](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/tupleandcardinalityandcomponent-2021-10-06.webp)  

    1.表中的**一行**，是**一条记录**，也是**一个元组**。  
    
    2.记录（元组）的数称为**基数**，一定一定注意辨别这里的基数和下面**域的基数**。  
    
    3.**分量**是元组中一个**属性的值**。  

* **域**  

    ![domain-2021-10-06](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/domain-2021-10-06.webp)  

    1.**域**就是属性的**取值范围所在**。  
    
    2.域这个**集合**里的值都是**统一数据类型的**。  

    3.域也有一个**域的基数**，**域的基数**代表域能取的值的数量，比如： ```年龄属性的域={17,18,19,20,21,22,23,24,25}```，那么这个域的基数就是9  

    ![tired-2021-10-06](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/tired-2021-10-06.webp)

## 0x02 传统的集合运算  
传统的集合运算是从 **行** 的角度来进行的，也就是说操作对象集中在**元组**上~(๑•̀ㅂ•́)و✧  

* **前提**

    ![union1-2021-10-06](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/union1-2021-10-06.webp)  

    1.运算要求两个关系**具有相同的目**(度,元数,属性数,列数)  
    2.运算要求两个关系**相应属性**取自**同一个域**，通俗来说，就是**两个关系对应的属性是相同的**  

* **并**  
    
    ![union2-2021-10-06](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/union2-2021-10-06.webp)

    因为是集合运算，并运算过程中**一定要记得元组去重**  

    返回的结果是 **n目(属性数)** 关系，由**属于R**或者**属于S**的**元组**组成  

* **差**  

    ![except1-2021-10-06](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/except1-2021-10-06.webp)  

    **R-S** 返回的结果是 **n目(属性数)** 关系，由**属于R**但**不属于S**的**元组**组成  

    一个例子可能有点模糊，接下来我们再来个 **S-R** 的例子：  

    


To be updated...


## 特别感谢
* [LemonPerfect](https://github.com/orgs/cat-note/people/LemonPrefect)  协助核对  
* [板砖猫](#) 协助核对  
* 动画番剧截图来自 [AnimeShot](https://as2.bitinn.net/)  

## References  
* [关系数据库--关系代数](https://zhuanlan.zhihu.com/p/258122592)  