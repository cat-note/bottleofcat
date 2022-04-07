# 想法子记忆Vi/Vim操作及指令

在Linux系统中编辑文本总是离不开一位老帮手——**Vi**。而因为其诞生的年代有些久远，有些操作在现在看来可能有点“反直觉”。  

于是我决定写这样一篇小笔记，记录一下我**记忆**Vi的这些这些操作和指令的方法（**主要靠的是英语了**）。

当然，正如“好记性不如烂笔头”所言，多实践才是熟练掌握技能的王道。

![002-2022-04-02](https://assets.xbottle.top/img/002-2022-04-02.png)  

## 关于Vi/Vim名词本身

* ```Vi```代表的可能是```Visual Instrument(可视工具)```，```Visual Interface(可视界面)```亦或就是```Visual(可视化的)```单词本身。

* ```Vim```倒是很明显，代表的是```Vi IMproved(Vi改进版)```。

## 编辑器模式

对```Vi```而言：

1. 命令模式 (```Command``` mode) ，刚进入Vi编辑器时的默认模式。在此模式下能输入Vi的合法指令（通常是一些字符）来进行操作。

2. 插入(输入)模式 (```Insert``` mode) ，顾名思义，是直截了当地编辑文本的模式。

3. 末行(底线)模式 (```Last-line``` mode) ，也是接受指令的一种模式，更偏向于文件的管理。  
之所以叫 “末行” 是因为指令的输入在窗口最下方：

    ![lastLineMode-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/lastLineMode-2022-04-07.jpg)


```Vim```是```Vi```的增强版，因此```Vim```完全可以兼容```Vi```的操作，不过它的模式名可能略有差别：

1. 正常模式 (```Normal``` mode) ，是启动```Vim```后的默认模式。

2. 插入模式 (```Insert``` mode) ，这个和```Vi```的一致。

3. 命令(行)模式 (```Command-line``` mode)，类似```Vi```的末行模式。

4. 可视模式 (```Visual``` mode) ，和普通模式类似，不同的是可以**高光选择文字的一部分进行命令操作**。

    包括上述几种模式，Vim总共有```12```种模式(这个说法来自Wikipedia)。但**最常用**的还是上面这四种，一般情况下够用了。

    ![015-2022-04-06](https://assets.xbottle.top/img/015-2022-04-06.png?233)

## 模式切换

就```Vi```的三种模式而言，要在其间互相切换，需要借默认模式——**命令模式**作为一个中转（对于```Vim```来说就是**正常模式**）。

* **命令模式**

    作为进入```Vi```的默认模式，我们可以把这儿当作一个小家，  
    无论在哪个模式下**按```Esc```**（可以记成```Escape``` [v.]逃离。逃回家了）都可以回到命令模式。（家是永远的港湾啊）

    ![EscGoHome-2022-04-06](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/EscGoHome-2022-04-06.png)

    再提一嘴，在```Vim```里这个模式称为**正常模式**。

* **插入模式**  

    通过以下按键指令能进入插入模式。这些按键通常是**大小写成对**的，**小写**对应的指令略显 **“温和”** ，**大写**对应的就比较 **“走极端”** 了。

    ![InsertModeUpperAndLower-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/InsertModeUpperAndLower-2022-04-07.png)

    1. ```i``` / ```I``` 插入 （记作```Insert [v]插入；嵌入；（在文章中）添加``` ） 

        小写的```i```，就是在**光标所在位置**进入插入模式：

        ![lowerInsert-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/lowerInsert-2022-04-07.gif)  

        大写的```I```则有点“极端”，会**跳转到当前光标所在行的开头**进入插入模式：

        ![upperInsert-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/upperInsert-2022-04-07.gif)

    2. ```a``` / ```A``` 附加 （记作```Append [v]增补，追加```）

        小写的```a```，就是在**光标所在位置之后一位**进入插入模式，即所谓的“附加”：

        ![lowerAppend-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/lowerAppend-2022-04-07.gif)  

        显而易见，大写的```A```就会**跳转到当前光标所在行的末尾**进入插入模式（在尾部附加）：

        ![upperAppend-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/upperAppend-2022-04-07.gif)

    3. ```o``` / ```O``` 新增一行 （暂且记作```Open``` new line）

        都是新增行，差别无非是往下新增还是往上新增了。

        小写的```o```，就是在光标所在行的**之后新建一行**，然后进入插入模式：

        ![newLineAfter-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/newLineAfter-2022-04-07.gif)  

        大写的```O```呢，就比较蛮横，会把光标所在行及其以下的行整体往下顶，在原位置新建一行（相对来说就是**在光标所在行上面新建一行**），然后进入插入模式：

        ![newLineBefore-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/newLineBefore-2022-04-07.gif)

    4. ```s``` / ```S``` 删除(取代) （可以直接记删的拼音```Shan```，也可以记成```Substitute [v]取代```）

        小写的```s```，会**删除光标所在位置右边的字符**，然后进入插入模式：

        ![subWord-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/subWord-2022-04-07.gif)

        大写的```S```就很暴躁了，会**删除光标所在行**，然后进入插入模式：

        ![subLine-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/subLine-2022-04-07.gif)  

        记```Substitute```的话，可以理解为这个操作是删除原有`字符/行`后进行插入，**取代**原有的`字符/行`。

    上面这些按键指令都是在```命令模式/正常模式```下输入的，通过**在指令前附加数字**，能重复这个指令的操作：

    * ```3s``` -> 删除光标后```3```个字符

        ![repeatSubstitute-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/repeatSubstitute-2022-04-07.gif) 

    * ```4a``` -> 在光标下一位附加内容，**操作结束后**(Esc)把这个内容重复```4```次

        ![appendFourTimes-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/appendFourTimes-2022-04-07.gif)  

* **替换模式** 

    这个模式和插入模式一样是用于编辑文本的，不同的是替换模式下**输入所至之处会覆盖一切。**  

    记作 ```Replace [v]取代；代替。```  

    输入大写```R```进入替换模式：

    ![replaceMode-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/replaceMode-2022-04-07.gif)  

    有了大写R，那么小写```r```呢？小写r的功能是**将光标后面的字符替换为指定字符**。比如我们先输入```r```，再输入```a```:  

    ![replaceChr-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/replaceChr-2022-04-07.gif)  

    当然，这些操作一样是可以重复进行的：

    ![repeatReplace-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/repeatReplace-2022-04-07.gif)

    (指令分别是`3rp`，`3R`->`hello`->`Esc`)