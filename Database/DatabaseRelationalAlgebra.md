# 关系数据库de关系代数小记

最近学数据库概论学到了**关系数据库**的关系代数了。哎嘛，真的把我整晕了，尤其是关系代数的使用，很容易让人被蒙在鼓里。  

![letmeunderstand-2021-10-06](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/letmeunderstand-2021-10-06.webp)

**对我来说**槽点最大的莫过于书上的例子不多了，本来就挺抽象的一种**查询**语言，加上没有足够多让我懂的例子<del>(不过我本来也比较迟钝了)</del>，真的是难上加难...  
但没办法啊，这是我需要跨过的一道坎，于是我决定慢慢写出这篇小记（**主要针对数据库关系代数**）。不求挖的多深，但求能理解地透彻点😂  
目标是...希望这篇文章对大家有所帮助吧！废话不多说，开整！  o(￣ヘ￣o＃)

* **注意，因采用了动图(WebP)直观表达，流量用户请慎重浏览~**

* **关系代数是一种查询语言。**

## 0x00 关系代数演示工具  

I'm still working on it...

## 0x01 小概念  
<details>
<summary>展开阅读：行与列 , 目&度&元数 , 属性&字段 , 元组&基数&分量 , 域</summary>

--------

* ### **行与列**

    ![rowandcolumn-2021-10-06](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/rowandcolumn-2021-10-06.webp)

* ### **目** & **度** & **元数**  

    ![meshanddegree-2021-10-06](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/meshanddegree-2021-10-06.webp)  

    1.如果使用关系表进行展示的话，**属性数（字段数）**其实就是**列数**，而进一步我们将属性数称为**元数**或者**目**或者**度**。  

    2.这样看来其实**元数就是目，目也就是度**。  

* ### **属性** & **字段**  

    1.**属性**就是**字段**（至少在数据库关系表这里是这样），都表示的是**列**。  

    2.上面和下面的图中第一行 **学号,姓名,性别...** 等等是 **属性名（字段名）**。  

* ### **元组** & **基数** & **分量**

    ![tupleandcardinalityandcomponent-2021-10-06](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/tupleandcardinalityandcomponent-2021-10-06.webp)  

    1.表中的**一行**，是**一条记录**，也是**一个元组**，有n个元素的元组可以被称为**n元组**。  
    
    2.记录（元组）的数称为**基数**，一定一定注意辨别这里的基数和下面**域的基数**。  
    
    3.**分量**是元组中一个**属性对应的值**。  

* ### **域**  

    ![domain-2021-10-06](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/domain-2021-10-06.webp)  

    1.**域**就是属性的**取值范围所在**。  
    
    2.域是一个 **集合** ，域这个**集合**里的值都是**统一数据类型的**。  

    3.域也有一个**域的基数**，**域的基数**代表域能取的值的数量，比如： ```年龄属性的域={17,18,19,20,21,22,23,24,25}```，那么这个域的基数就是9  

    ![rememberfirst-2021-10-07](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/rememberfirst-2021-10-07.png)

</details>

## 0x02 传统的集合运算  

<details>
<summary>展开阅读：并/差/交的前提 , 并 , 差 , 交 , 广义笛卡尔积</summary>

--------

传统的集合运算是从 **行** 的角度来进行的，也就是说操作对象集中在**元组**上~(๑•̀ㅂ•́)و✧  

* ### **并,差,交的前提**

    ![union1-2021-10-06](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/union1-2021-10-06.webp)  

    1.运算要求两个关系**具有相同的目**(度,元数,属性数,列数)  
    2.运算要求两个关系**相应属性**取自**同一个域**，通俗来说，就是**两个关系对应的属性是相同的**  

* ### **并**  
    
    ![union2-2021-10-06](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/union2-2021-10-06.webp)

    因为是集合运算，并运算过程中**一定要记得元组去重**  

    返回的结果是 **n目(属性数)** 关系，由**属于R**或者**属于S**的**元组**组成  

* ### **差**  

    ![except1-2021-10-06](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/except1-2021-10-06.webp)  

    **R-S** 返回的结果是 **n目(属性数)** 关系，由**属于R**但**不属于S**的**元组**组成  

    一个例子可能有点模糊，接下来我们再来个 **S-R** 的例子：  

    ![except2-2021-10-07](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/except2-2021-10-07.webp)  

* ### **交**  

    ![intersection-2021-10-07](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/intersection-2021-10-07.webp)  

    **R∩S**返回的结果是 **n目(属性数)** 关系，由**属于R**且**属于S**的**元组**组成  

    很容易能发现：```R ∩ S = R - (R-S)```  

* ### **广义笛卡尔积**  

    ![bigiscoming-2021-10-07](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/bigiscoming-2021-10-07.png)  

    咱实话实说，关系代数这一部分第一个让我晕头的地方就是这个笛卡尔积o(≧口≦)o，接下来我尽力搞懂并用动图展示出来笛卡尔积到底做了什么。  

    为什么这里是 **广义笛卡尔积** 呢？我们先看 **笛卡尔积** 算了什么：  

    > 笛卡尔乘积是指在数学中，两个集合X和Y的笛卡尔积（Cartesian product），又称直积，表示为X×Y，第一个对象是X的成员而第二个对象是Y的所有可能有序对的其中一个成员 
    >> 摘自某百科  

    这个概念看得我脑袋嗡嗡的，什么叫 **“第二个对象是Y的所有可能有序对的其中一个成员”** ？结合例子琢磨了一会儿，我结合离散数学教材关于笛卡尔积的定义梳理了一下：  

    > 笛卡尔乘积是指在数学中，两个集合X和Y的笛卡尔积，又称直积，表示为X×Y。X中的元素作为第一个元素，Y中的元素作为第二个元素，构成有序对。而笛卡尔积就是所有这样的有序对构成的一个集合。  

    接下来我们回到这里，**域** 其实就是一个 **集合**，结合上文，**笛卡尔积是在域上面的一种\<集合\>运算** ，下面放个简单的例子：   

    ![cartesian1-2021-10-08](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/cartesian1-2021-10-08.webp)

    复习一下，一个 **域** 允许的 **不同取值的个数** 称为这个 **域的基数**。换种说法，也是一个 **集合** 中的 **元素个数**。  

    在上图的示例中，TEACHER域里有 ```张前程,赵向前``` 两位老师，这个域的基数也就是2；而MAJOR域里有 ```计算机专业,信息专业``` 两个专业，这个域的基数也是2。最后算出的域中有四个有序对，结果这个域的基数是4.  

    当 **域的基数为m和n** 的两个域进行笛卡尔积后我们得到的 **域** 的 **域的基数** 就是 **m×n** （这里×是乘号的意思哈，不要多想！ (￣△￣；)） 

    ![waitaminute-2021-10-08](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/waitaminute-2021-10-08.png)  

    仔细看一下能发现上面的例子中有个 **有序对** 的概念，不用怕，在关系这里我们后面就用不着它了！看看接下来这个例子：  

    ![cartesian2-2021-10-09](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/cartesian2-2021-10-09.webp)  

    到底其实我们算出了一个包含 **老师所带专业课程的所有可能性** 的集合（域），把每一项元素放进关系中，这不就是元组嘛！  

    -------
    ![guangyi-2021-10-09](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/guangyi-2021-10-09.png)  

    现在再说回 **广义笛卡尔积** ，为什么“广义”呢？因为它面向的是关系，通过 **操作元组** 进行运算。接下来我们快速上一个例子：  

    ![generalizedcartesian1-2021-10-10](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/generalizedcartesian1-2021-10-10.webp)

    > 两个分别为 **n目** 和 **m目** 的关系R和S的广义笛卡尔积是一个 **(n+m)列(目)** 的元组的集合(新关系)。元组的 **前n列** 是 **关系R** 的一个元组，**后m列** 是 **关系S** 的一个元组。若R有k1个元组，S有k2个元组，则关系R和关系S的广义笛卡尔积有 **k1 × k2** 个元组。  

    进行广义笛卡尔积时对两个关系的要求是比较宽松的：  

    1. **不要求** 两个关系的 **目(度,元数,属性数,列数)** 相同。  

    2. 有了上面这条，其实也应该知道了，广义笛卡尔积也 **不要求** 相应属性取自同一个域。

    在这之后我们再来个详细点的例子，把两张**不同**的表进行笛卡尔积：  

    ![generalizedcartesian2-2021-10-10](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/generalizedcartesian2-2021-10-10.webp)

    --------

    **总结**：

    1. 在数据库关系里，**笛卡尔积** 是面向 **域（集合）** ，操作元素来进行计算的；而 **广义笛卡尔积** 是面向 **关系** ，操作元组来进行计算的。  

    2. **随便两个关系** 都可以进行广义笛卡尔积运算（在保证 **结果的关系** 有意义的情况下）  

        ![doIlearnitbefore-2021-10-10](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/doIlearnitbefore-2021-10-10.png)  

至此，传统的集合运算这部分我们就过完了。你，学会了吗？喝杯茶休息一下，准备进入下一节吧！  

</details>

## 0x03 专门的关系运算   

<details>
<summary>展开阅读：基本运算符，选择，投影，除，连接必需的一步，θ连接，等值连接，自然连接，外连接</summary>  

-----------------

专门的关系运算中就不仅涉及到 **行（元组）** 的运算了，还会牵扯到 **列**，由此会更加复杂。

我自己**可能也有理解错误**的地方，但我会尽力依据现有资料去呈现这些知识点。Sit back and relax，我们准备开始接下来的旅途！  

![v2-e8e7460c27f012d93794ca8b62ba727b_720w-2021-10-10](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/v2-e8e7460c27f012d93794ca8b62ba727b_720w-2021-10-10.jpg)  

* ### **基本运算符**  

    | 运算符   |    | 含义   |
    |:-----:|:---:|:----:|
    | 比较运算符 | ＞  | 大于   |
    |       | ≥  | 大于等于 |
    |       | ＜  | 小于   |
    |       | ≤  | 小于等于 |
    |       | =  | 等于   |
    |       | <> | 不等于  |
    | 逻辑运算符 | ┐  | 非    |
    |       | ∧  | 与(且) |
    |       | ∨  | 或    |

    基本运算符在后面的 **条件/逻辑表达式** 中经常用到。∧和∨这两个符号我觉得是最容易搞混的，着重记忆 (*￣3￣)╭  


* ### **选择**  

    选择是对 **行（元组）** 进行操作的运算。

    例子：属性名 = 常量  

    ![selection1.1-2021-10-10](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/selection1.1-2021-10-10.webp)  

    例子：属性名 θ 属性名  

    ![selection2-2021-10-10](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/selection2-2021-10-10.webp)

    
    * 选择语句形如 **σF(关系)** ，其中**F**是 **逻辑表达式** 。当遇到让逻辑表达式为 **真** 的 **元组** 时，该语句会将其选择出来。  

    * 逻辑表达式形如 **A θ B** ，也就是 **将A表达式和B表达式进行比较**，**θ** 是比较运算符。举几个例子：```StudentAge > 17``` , ```17 < StudentAge``` , ```CourseNum ≥ StudentNum``` , ```StudentName = 'Jerry'```。  

    * 上面的例子中```StudentAge``` , ```StudentName```一类是属性名，而```17``` , ```'Jerry'``` 一类则是常量。  

    * **字符串常量** 请一定记得用 **单引号** 括起来。  

    * 所以A和B表达式可以是 **属性名** , **常量** , 书上还补充说可以是 **简单函数** ，不过在基础应用中不太能见得到。  

    * **多个** 逻辑表达式可以用 **逻辑运算符** 进行连接，举几个例子：```AθB ∧ CθD``` , ```AθB ∨ CθD``` , ```┐(AθB)```。

    大体来说，**选择** 选的是关系中符合条件的 **元组** ，得到的结果是 **元组集合（也算一个关系）**  

* ### **投影**  

    投影可以说是四种专门关系运算中最简单的一种了，我们可以简单走一遍~  

    投影是对 **列** 和少部分 **行** 进行操作的一种运算，我们先直接上例子：  

    ![projection-2021-10-11](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/projection-2021-10-11.webp)  

    * 投影语句形如 **ΠA(关系)** 其中 **A** 是属性名。  

    * 投影选出了 **特定属性名** 对应的 **列**。  

    * 投影选择出 **一列或多列** 属性列后，需要 **去除重复元组** ， 这也是为什么说投影涉及少部分行上的操作。  

* ### **除**  

    除运算是同时从 **行** 和 **列** 的方向进行运算的。

    这里的除运算理解起来可能有点“绕”，尤其是书上还引用了一个 **“象集”** 的概念来定义除法。那好吧 o(*￣3￣)o，我们先看看 **象集做了啥子** ：  

    象集这个概念以我的水平实在是难以用文字表达清楚，这里就先上直观示例了：  

    ![imageset1-2021-10-11](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/imageset1-2021-10-11.webp)  

    ![imageset2-2021-10-12](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/imageset2-2021-10-12.webp)  

    上面的例子R(X,Z)中X指代了 **单个属性列B** ，接下来我们来几张静态图展示一下 **X代表一个属性组的情况** （其实是这里懒得做动图了_(´ཀ`」 ∠)_）

    <details>
    <summary><b>【展开查看例子】</b>X代表包含B,C属性列的属性组</summary>

    --------

    ![imageset3-2021-10-12](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/imageset3-2021-10-12.webp)  

    ![imageset3-2-2021-10-12](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/imageset3-2-2021-10-12.webp)  

    ![imageset3-3-2021-10-12](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/imageset3-3-2021-10-12.webp)

    ![imageset3-4-2021-10-12](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/imageset3-4-2021-10-12.webp)

    ![imageset3-5-2021-10-12](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/imageset3-5-2021-10-12.webp)

    ![imageset3-6-2021-10-12](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/imageset3-6-2021-10-12.webp)

    ![imageset3-7-2021-10-12](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/imageset3-7-2021-10-12.webp)


    </details>  

    (๑•̀ㅂ•́)و✧

    经过上面的例子，大家应该对 **象集做了什么** 大概有了个了解，这里做个总结：  

    1. 我们把目标 **关系** 表示为 **R(X,Z)**，其中X和Z **可以代表单个属性列** ，亦可以代表 **多个属性列组成的属性组**。  

    2. X和Z是互补的，如果X **代表其中一部分属性列** ，那么Z **一定会代表剩余的属性列**。  

    3. 象集做的事无非是 **一次选择** 和 **一次投影** 。比如我想找R关系中 **x的象集**，那么先选择的是 **X的分量** 等于 x 的**元组（可能有多个，组成集合）** ，然后将这些元组（集）在 **Z属性组** 上进行投影，得到x的象集。  

    4. 象集的英文是Image**Set**，没错，它也是 **集合** ！如果最后得到的结果中有重复项请记得 **去掉重复项**。   

    ------

    ![wehavemetimageset-2021-10-12](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/wehavemetimageset-2021-10-12.png)

    接下来继续看 **除运算** ，书上之所以用了象集的概念来定义，我觉得是因为除运算实际上是 **求象集的 “反向操作” ，从结果找源头** ，看接下来这个例子：  

    ![division1.1-2021-10-13](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/division1.1-2021-10-13.webp)  

    上面这个例子中结果得到的是 **单属性列A**，下面我们再来个相除得到 **属性组** 的例子：  

    ![division2-2021-10-13](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/division2-2021-10-13.webp)  

    总结一下就是：  
    
    1. 这里的 **除运算** 可以看作是 **求象集的反向操作** 。比如我算 **R ÷ S**，其实就是找 **S关系和R关系相同的部分** 是 **R中谁的象集** 。  

    2. 通过上面两个例子可以发现，除号**前面**关系的**元组数** 一定 **≥**  除号**后面**关系的**元组数**。例如我算 ```R÷S``` ，如果R的元组数＜S的元组数，是肯定**除了个寂寞的**。  

    3. 另外，进行除运算的两个关系必须要有 **共同的属性列** ， 不然运算是没有任何效果的。  

------

接下来要谈到的是另一个重头戏—— **连接** 了...  

![scaredmio-2021-10-13](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/scaredmio-2021-10-13.webp)  

连接的符号是 **⋈** ，一般连接是对 **行** 进行操作，一部分情况（**自然连接**）下连接**还要**对 **列** 进行(去重)操作。  

连接因为条件不同，被分为了 **θ连接** ， **等值连接** ， **自然连接** 几种，咱也准备分开记这几个了~  

* ### **连接必需的一步**  

    ![basicofthejoin1.1-2021-10-13](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/basicofthejoin1.1-2021-10-13.webp) 

    连接其实等同于从两个关系的 **广义笛卡尔积** 中按 **条件** 进行 **选择**，运算式中⋈符号下面的表达式其实和选择里的F一样是 **逻辑表达式**，但**要注意**，这里逻辑表达式θ两端用于比较的属性组是有限制的，**详细看下面的θ连接** 。  

    例子中得到的R和S两个关系的广义笛卡尔积：  

    <details>
    <summary>展开查看</summary>

    | ID       | NAME | R.COURSEID | S.COURSEID | TEACHER |
    |:--------:|:----:|:----------:|:----------:|:-------:|
    | 20230102 | 新一   | 1          | 1          | 赵向前     |
    | 20230102 | 新一   | 1          | 2          | 李先生     |
    | 20230102 | 新一   | 1          | 3          | 张前程     |
    | 20230102 | 新一   | 1          | 3          | 麦当劳     |
    | 20230102 | 新一   | 1          | 5          | 龙井茶     |
    | 20230103 | 高二   | 2          | 1          | 赵向前     |
    | 20230103 | 高二   | 2          | 2          | 李先生     |
    | 20230103 | 高二   | 2          | 3          | 张前程     |
    | 20230103 | 高二   | 2          | 3          | 麦当劳     |
    | 20230103 | 高二   | 2          | 5          | 龙井茶     |
    | 20230104 | 张三   | 3          | 1          | 赵向前     |
    | 20230104 | 张三   | 3          | 2          | 李先生     |
    | 20230104 | 张三   | 3          | 3          | 张前程     |
    | 20230104 | 张三   | 3          | 3          | 麦当劳     |
    | 20230104 | 张三   | 3          | 5          | 龙井茶     |
    | 20230105 | 李四   | 4          | 1          | 赵向前     |
    | 20230105 | 李四   | 4          | 2          | 李先生     |
    | 20230105 | 李四   | 4          | 3          | 张前程     |
    | 20230105 | 李四   | 4          | 3          | 麦当劳     |
    | 20230105 | 李四   | 4          | 5          | 龙井茶     |
    </details>

    其中属性列 ```ID``` , ```NAME``` , ```R.COURSEID``` 是原关系R的属性列，而 ```S.COURSEID``` ， ```TEACHER``` 是原关系S的属性列

    在接下来的示例中，咱就用这张表来做示例了嗷~(/▽＼)  


* ### **θ连接**  

    θ连接其实简称就是连接，很有存在感的便是 **代表比较运算符的 θ 了**（￣□￣；）  
    废话不多说，我们先上直观例子：  

    ![thetajoin1-2021-10-13](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/thetajoin1-2021-10-13.webp)

    ![thetajoin2-2021-10-13](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/thetajoin2-2021-10-13.webp)  

    ![thetajoin3-2021-10-13](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/thetajoin3-2021-10-13.webp)  

    主要操作就是从两个关系的广义笛卡尔积中 **按条件进行选择** 了，由此很大程度上可以参考上面的 **选择** 部分。尽管如此，还是要**注意**这些限定：  

    1. 拿 **R ⋈ S(逻辑表达式：A θ B)** 进行举例，**A属性组** 只能在 **R** 这部分选，而 **B属性组** 只能在 **S** 这部分选。  

    2. **A**和**B**是属性组，那么A和B的 **度数（列数）** 一定要**相同**。  

    上面举的例子可能还有点不清不白，其实咱还可以皮点嘛(￣^￣)ゞ，比如说这样写：

    ![thetajoin4.gif-2021-10-13](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/thetajoin4.gif-2021-10-13.webp)  

    在逻辑表达式中我们将学号 **ID** 和课程号 **S.COURSEID** 相比。可想而知，因为**在广义笛卡尔积结果中**学号是**恒大于**课程号的，所以结果自然是：  

    ![thetajoin5-2021-10-13](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/thetajoin5-2021-10-13.webp)  

* ### **等值连接**  

    等值连接是基于 **θ连接** 的，不同的是等值连接规定 **θ是等号=**，话不多说，直接上例子：  

    ![equivalentjoin-2021-10-13](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/equivalentjoin-2021-10-13.webp)  

* ### **自然连接**  

    关系R与关系S自然连接表示为：**R ⋈ S**。  

    自然连接是在 **等值连接** 上的扩充，不过自然连接有了新的要求：

    1. 逻辑表达式中进行比较的两个属性组 **A** 和 **B** 必须是**相同的属性组**。  

    2. 自然连接结果中要 **去除重复的属性列**。  

    3. 结合上面两点，自然连接 **过程中一定** 会有 **重复列** 出现。  

    就 **第1点** 来说，比如上面的例子中，```R.COURSEID``` 是R关系中的**COURSEID**属性列，而 ```S.COURSEID``` 是S关系中的**COURSEID**属性列，他们都是**COURSEID**属性，都是**课程号**，所以可以进行自然连接。  

    如果我拿R关系中的 ```ID``` 和S关系中的 ```COURSEID``` 属性列作为比较对象，因为二者并不是同一个属性名，**无法进行自然连接**。  

    **第2点** 的话咱还是整个例子（紧接上一个等值连接的例子，因为等值连接举的例子中R关系的COURSEID和S关系的COURSEID是同一个属性）：  

    ![naturaljoin-2021-10-13](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/naturaljoin-2021-10-13.webp)  

* ### **外连接**  

    外连接则又是在 **自然连接** 上的扩展（一环套一环啊喂(°ー°〃) ），它与自然连接不同的地方是 **保留了悬浮元组**。  

    什么是 **悬浮元组** ？  

    ![danglingtuple-2021-10-17](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/danglingtuple-2021-10-17.webp)  

    在进行自然连接后，原 **R关系** 和 **S关系** 中因为**不满足条件**，可能有 **元组** **没有在**连接结果中出现，也就是在连接过程中被**抛弃了**，这些被抛弃的元组就是 **悬浮元组**。（顺便学个新的英语单词：```dangle[verb.]悬挂，悬垂```）    

    上面的例子中的悬浮元组是：  

    | ID       | NAME | COURSEID |
    |:--------:|:----:|:--------:|
    | 20230105 | 李四   | 4        |

    | COURSEID | TEACHER |
    |:--------:|:-------:|
    | 5        | 龙井茶     |  

    那么什么是 **外连接** ？  

    外连接其实就是在**自然连接**结果关系中保留 **所有的悬浮元组**，结果中属性的**未知值**就填上**NULL**  

    我们在上面自然连接的结果中示例外连接：  

    ![outerjoin-2021-10-17](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/outerjoin-2021-10-17.webp)  

    外连接保留了**R**和**S**关系中**所有的**悬浮元组，这些悬浮元组在最终结果中会缺值，我们将这些缺值全部记为**NULL**。  

    在外连接之中，还有 **左外连接（左连接）** ：  

    ![leftouterjoin-2021-10-17](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/leftouterjoin-2021-10-17.webp)  

    左外连接则只保留 **连接符号左边** 的关系的 **悬浮元组**，同样缺值记为**NULL**。例子中保留的是R中的悬浮元组。  

    那么，**右外连接（右连接）** 做的事就显而易见了：  

    ![rightouterjoin-2021-10-17](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/rightouterjoin-2021-10-17.webp)  

    右外连接则只保留 **连接符号右边** 的关系的 **悬浮元组**，同样缺值记为**NULL**。例子中保留的是S中的悬浮元组。  

    结合上面的外连接，左外连接，右外连接，我们发现：  

    ![relationsbetweenouterjoins-2021-10-17](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/relationsbetweenouterjoins-2021-10-17.webp)  

    也就是 ```外连接 = 左外连接 ∪ 右外连接```。  

    感谢你看到这里，至此这篇文章的主要内容就结束啦！放工啦放工！╰（￣▽￣）╭    

    ![Imtired-2021-10-17](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/Imtired-2021-10-17.webp)    

</details>

## 0x04 After all  

![pleasedtomeetyouguys-2021-10-17](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/pleasedtomeetyouguys-2021-10-17.webp)

这篇文章写出来一方面是当作我自己学习的笔记，将来如果我忘记了（这个将来可能离现在不远了，我记性很差TAT）能随时捡起来，所以**在我的角度**上尽可能地表达详细了；另一方面也是希望**能多多少少帮助**到大家，毕竟现在关于关系代数的知识在网上还是有点分散的。  

因为这篇文章写了两周，中间可能因为时间间断导致我突然想不起来之前**想写什么**了，可能有遗漏或者写错的地方，也请各位多加指教。  

写完后咱觉得动图播放速度可能还是有点快了，于是有了写个**关系代数演示小工具**的想法，稍后将作为上方的**0x00环节**放出，目标是能让看到该文章的各位（包括我自己）在动手中加深理解。  


## 附表  

<details>
<summary>展开查看</summary>

* **0x01 小概念** 使用的表格：

    | 学号 | 姓名 | 性别 | 年龄 | 所在系 |
    |:--------:|:---:|:---:|:---:|:---:|
    | 20230102 | 新一 | 男  | 18 | CS  |
    | 20230103 | 高二 | 女  | 20 | CS  |
    | 20230104 | 张三 | 男  | 19 | MA  |
    | 20230105 | 李四 | 男  | 18 | IS  |
    | 20230106 | 王五 | 男  | 19 | CS  |
    | 20230107 | 赵六 | 女  | 21 | IS  |

* **0x02 传统的集合运算** 使用的表格  

    1. 前提,并,差  

        | 学号       | 姓名 | 性别 |   
        |:--------:|:---:|:---:|
        | 20230102 | 新一 | 男  |
        | 20230103 | 高二 | 女  |
        | 20230104 | 张三 | 男  |  

        | 学号       | 姓名 | 性别 |
        |:--------:|:---:|:---:|
        | 20230104 | 张三 | 男  |
        | 20230105 | 张三 | 女  |
        | 20230106 | 李四 | 男  |
        | 20230107 | 王五 | 男  |
        | 20230108 | 赵六 | 女  |

    2. 交  

        | 学号       | 姓名 | 性别 |
        |:--------:|:---:|:---:|
        | 20230102 | 新一 | 男  |
        | 20230103 | 高二 | 女  |
        | 20230104 | 张三 | 男  |

        | 学号       | 姓名 | 性别 |
        |:--------:|:---:|:---:|
        | 20230106 | 李四 | 男  |
        | 20230107 | 王五 | 男  |
        | 20230103 | 高二 | 女  |
        | 20230104 | 张三 | 男  |
        | 20230108 | 赵六 | 女  |

    3. 笛卡尔积  

        | 老师  | 课程所在专业 | 课程 |
        |:---:|:------:|:---:|
        | 张前程 | 计算机专业  | 高数 |
        | 张前程 | 计算机专业  | 离散 |
        | 张前程 | 计算机专业  | 线代 |
        | 张前程 | 信息专业   | 高数 |
        | 张前程 | 信息专业   | 离散 |
        | 张前程 | 信息专业   | 线代 |
        | 赵向前 | 计算机专业  | 高数 |
        | 赵向前 | 计算机专业  | 离散 |
        | 赵向前 | 计算机专业  | 线代 |
        | 赵向前 | 信息专业   | 高数 |
        | 赵向前 | 信息专业   | 离散 |
        | 赵向前 | 信息专业   | 线代 |

    4. 广义笛卡尔积  

        | A  | B  | C  |
        |:---:|:---:|:---:|
        | a1 | b1 | c1 |
        | a1 | b2 | c2 |

        | B  | C  | D  |
        |:---:|:---:|:---:|
        | b2 | c2 | d1 |
        | b3 | c2 | d3 |

        | 学号       | 姓名 | 性别 |
        |:--------:|:---:|:---:|
        | 20230102 | 新一 | 男  |
        | 20230103 | 高二 | 女  |

        | 课程名 | 课程号 |
        |:---:|:---:|
        | 高数  | 233 |
        | 离散  | 450 |
        | 线代  | 777 |

* **0x03 专门的关系运算** 使用的表格  

    1. 选择  

        示例一同 **0x01 小概念** 使用的表格，示例二如下：  

        | ID       | NAME | SEX | CREDIT | TARGET |
        |:--------:|:----:|:---:|:------:|:------:|
        | 20230102 | 新一   | 男   | 52     | 54     |
        | 20230103 | 高二   | 女   | 48     | 44     |
        | 20230104 | 张三   | 男   | 36     | 34     |
        | 20230105 | 李四   | 男   | 48     | 50     |
        | 20230106 | 王五   | 男   | 56     | 60     |
        | 20230107 | 赵六   | 女   | 42     | 60     |

    2. 投影  

        同 **选择** 的示例二表格

    3. 除  

        | A  | B  | C  |
        |:---:|:---:|:---:|
        | a1 | b1 | c2 |
        | a2 | b3 | c7 |
        | a3 | b4 | c6 |
        | a1 | b2 | c3 |
        | a4 | b6 | c6 |
        | a2 | b2 | c3 |
        | a1 | b2 | c1 |  

        | B  | C  | D  |
        |:---:|:---:|:---:|
        | b1 | c2 | d1 |
        | b2 | c1 | d1 |
        | b2 | c3 | d2 |

        | A  | B  | C  | D  |
        |:---:|:---:|:---:|:---:|
        | a1 | b1 | c2 | d1 |
        | a2 | b3 | c1 | d1 |
        | a3 | b4 | c6 | d2 |
        | a1 | b2 | c3 | d2 |
        | a4 | b3 | c6 | d1 |
        | a2 | b3 | c3 | d2 |
        | a1 | b2 | c1 | d1 |


        | C  | D  | F |
        |:---:|:---:|:---:|
        | c1 | d1 | f1 |
        | c3 | d2 | f2 |

    4. 连接  

        | ID       | NAME | COURSEID |
        |:--------:|:----:|:--------:|
        | 20230102 | 新一   | 1        |
        | 20230103 | 高二   | 2        |
        | 20230104 | 张三   | 3        |
        | 20230105 | 李四   | 4        |

        | COURSEID | TEACHER |
        |:--------:|:-------:|
        | 1        | 赵向前     |
        | 2        | 李先生     |
        | 3        | 张前程     |
        | 3        | 麦当劳     |
        | 5        | 龙井茶     |





</details>


## 特别感谢
* [LemonPerfect](https://github.com/orgs/cat-note/people/LemonPrefect)  协助核对  
* [板砖猫](#) 协助核对  
* 动画番剧截图来自 [AnimeShot](https://as2.bitinn.net/)  

## References  
* 《数据库系统概论（第五版）》王珊主编 - 高等教育出版社  
* 《离散数学》屈婉玲,耿素云,张立昂 - 高等教育出版社
* [关系代数 - 百度百科](https://baike.baidu.com/item/%E5%85%B3%E7%B3%BB%E4%BB%A3%E6%95%B0)  
* [关系代数(数据库) - Wikipedia](https://zh.wikipedia.org/wiki/%E5%85%B3%E7%B3%BB%E4%BB%A3%E6%95%B0_(%E6%95%B0%E6%8D%AE%E5%BA%93))  
* [笛卡尔乘积 - 百度百科](https://baike.baidu.com/item/%E7%AC%9B%E5%8D%A1%E5%B0%94%E4%B9%98%E7%A7%AF)  
* [广义笛卡尔积 - 百度百科](https://baike.baidu.com/item/%E5%B9%BF%E4%B9%89%E7%AC%9B%E5%8D%A1%E5%B0%94%E7%A7%AF/23160124)  
* [关系数据库--关系代数 - 知乎](https://zhuanlan.zhihu.com/p/258122592)  
* [关系代数的基本运算 - keelii](https://keelii.com/2017/02/19/basic-operations-of-relation-algebra/)  