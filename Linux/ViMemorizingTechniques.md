# 想法子记忆Vi/Vim操作及指令

在Linux系统中编辑文本总是离不开一位老帮手——**Vi**。而因为其诞生的年代有些久远，有些操作在现在看来可能有点“反直觉”。  

于是我决定写这样一篇小笔记，记录一下我**记忆**Vi的这些这些**常用**操作和指令的方法（**主要靠的是英语了**）。

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

<details open>
<summary>展开阅读</summary>

------

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

* **末行模式** 

    输入 ```Shift```+```:``` 进入末行模式（事实上我们一般都是这样打出冒号来的）

    末行模式在```Vim```中被称为**命令(行)模式**。

* **可视模式**  

    可视模式是```Vim```中新引入的模式，支持在高光选择下进行指令操作。

    可视模式即 ```Visual Mode```，进入可视模式的按键则是```v``` / ```V```，

    其中 ```v``` 键能普通地进入可视模式：

    ![visualNormal-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/visualNormal-2022-04-07.gif)
    
    而 ```V``` 在进入可视模式后**会保持高光选择到光标所在行**（**可视行**模式）：

    ![visualLine-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/visualLine-2022-04-07.gif)


</details>

## 命令模式/正常模式

这是进入```Vi/Vim```后所处的默认模式，和文本处理是靠的非常近的，我决定先记一下这部分。

1. [**模式切换**](#模式切换)

2. **基本光标移动（上下左右）**

    在交互式字符界面下没有能动的鼠标，只有闪烁的光标来标注位置。要编辑文本，真得先掌握移动光标的指令。  

    **PS:** 下方涉及的光标**上下**移动都是**针对换行符`\n`分隔的行**的，光标会从一行移动到上方或下方的行：

    <details>
    <summary>展开查看示例</summary>

    -------
    
    ![jkLine-2022-04-08](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/jkLine-2022-04-08.gif)  

    👆 ```j```、```k```、```↓```、```↑```将光标从一行的指定位置移动到另一行的**指定位置**。

    ![enterDash-re-2022-04-08](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/enterDash-re-2022-04-08.gif)  

    👆 ```Enter```、```-```，将光标移动到下一行或上一行的**行首**！


    </details>

    * **“经典”款**

        ```h``` 向左移动光标  
        ```l``` 向右移动光标  
        ```k``` 向上移动光标  
        ```j``` 向下移动光标

        之所以这样设计，因为在早期**终端机**上，键盘是这个样的：

        ![hjklOnTerminal-2022-04-08](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/hjklOnTerminal-2022-04-08.jpg)  

        **记忆方法**：现代键盘上```↓```、```↑```键在中间，而```←```、```→```键在两侧，而HJKL也正是两侧的```K```、```L```代表横向移动，中间的```J```、```K```代表纵向移动（HJKL四个键连在一起的设计一直保留到了今天）。

        ![hjkl-remarked-2022-04-08](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/hjkl-remarked-2022-04-08.gif)

    * **现代款**

        ```←``` 向左移动光标  
        ```→``` 向右移动光标  
        ```↑``` 向上移动光标  
        ```↓``` 向下移动光标 

    * **“隐藏”款**

        ```Backspace(退格键)``` 向左移动光标  
        ```Space(空格键)``` 向右移动光标  
        > **👆记忆方法**：在插入模式下，`Backspace`和`Space`是分别用于删去字符和增加空格的，与之对应的光标分别是**往左移动**和**往右移动**的。 

        -------

        ```Enter``` 把光标移动到**下一行行首**  
        ```+``` 把光标移动到**下一行行首**  
        ```-``` 把光标移动到**上一行行首**  
        > **👆记忆方法**：插入模式下```Enter```起到了换行作用，换行后光标会处于行首。```-```就记成“减去一行”（老实说我也不太知道这个怎么记了，JK其实也够用了），```+```和```-```记在一起就行。

        -------

    **PS：** 光标移动是在```命令模式/正常模式```下的指令，当然是支持重复的啦！  

    在上面所有指令的前面加上重复的次数（数字）即可，比如我要往下翻`233`行：  ```233j```或```233↓```。

3. **行内光标移动**

    这里记录一下在**一行**文本中光标的移动指令：

    * **跳到行首**

        ```0``` 回到行首，`列数`=`1`的地方   
        ```Home``` 同上👆   
        ```^``` 回到行首，**第一个不为空白符**的字符处（对于加了缩进的行可谓是非常有用）

        > **👆记忆方法**：`0`记为“归零”；`^`的话，**正则表达式**里是用于匹配字符串首部的，可以一起记，另外其还形如一个顶部，可以记成一行中的“顶头”部分。

    * **跳到行尾**

        ```$``` 回到行尾**最后一个字符**的地方  
        ```End``` 同上👆   
        ```g_``` 回到行尾**最后一个不为空白符**的字符处

        > **👆记忆方法**：`$`在正则表达式里是用于匹配字符串尾部的，可以和正则表达式一起记；`g_`可以记成`go _`，`_`形如一个输入光标，这样记成跳转到行尾最后一个非空白字符。

4. **屏幕可见行中移动光标**

    上面所说的“行”取决于**换行符`\n`**的位置，但这里的**屏幕可见行**是取决于窗口大小的：

    ![screenLine-2022-04-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/screenLine-2022-04-10.jpg)

    （图中标注的即为**一个屏幕可见行**）

    针对这样的一个行，也是有指令可用于行内移动的：

    * ```g^``` 跳转到行首第一个不是空白符的字符处

        > **👆记忆方法**：`g^`可以记成`go ^`，去往当前的顶头部分。

    * ```g0``` 跳转到行首字符处

    * ```gm``` 跳转到**行中间**的字符处

        > **👆记忆方法**：`gm`可以记成`go midst`，去往当前的中间部分。

    * ```g$``` 跳转到行尾字符处

    完全是可以结合前面的`0`，`^`，`$`一起记的。当然，`g + Home`，`g + End`也是可以的。

    -----

    再展开想想，既然`g`开头的指令**在这里看的是屏幕可见行**，我是不是可以用`gj`（或`g↓`）指令从一个屏幕中的行移动到另一个行呢？

    ![JLOperation-2022-04-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/JLOperation-2022-04-10.gif)

    👆这个主要是用`j`（`↓`）指令进行的光标移动  

    ![gjOperation-2022-04-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/gjOperation-2022-04-10.gif)  

    👆而这个主要是用`gj`（`g↓`）指令进行的光标移动。  

    > 利用`g`+`移动指令`，可以实现在屏幕中的行之间移动。当然，`3gj`这种移动多行的指令也是支持的哦~  

    -------

    针对屏幕可见行，还有几个指令：

    * 大写 `H` 用于将光标移动到**屏幕可见**的**最上面一行**（这个判定有点怪，可能不太准确！）  
    
        > **👆记忆方法**：`H`可以记成`Head`，去往可见区的头部。  

    * 大写 `M` 用于将光标移动到**屏幕可见**的**最中间行**（同样判定很怪）  

        > **👆记忆方法**：`M`可以记成`Middle`，去往可见区的中间部分。

    * 大写 `L` 用于将光标移动到**屏幕可见**的**最下面一行** 

        > **👆记忆方法**：`L`可以记成`Last`，去往可见区的最后一行。

5. **文件内光标移动**  

    除了上述几种移动外，还有几个常用指令能按词、按句、按段等移动光标：

    * **按词移动**  

        `w` 跳转到**下一个词**的开头部分（**考虑标点符号**），演示如下👇

        ![wordNext-2022-04-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/wordNext-2022-04-10.gif)

        `W` 跳转到**下一个词**的开头部分（只考虑**空格分隔**） ，演示如下👇

        ![wordNext-CAPS-2022-04-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/wordNext-CAPS-2022-04-10.gif) 

        `b` 跳转到**上一个词**的开头部分（**考虑标点符号**）。

        `B` 跳转到**上一个词**的开头部分（只考虑**空格分隔**）。

        这里的`b`、`B`和`w`、`W`的操作就是**反着来的**。

        > **👆记忆方法**：`w`可以记成`word next`，下一个单词；`b`则记成`behind word`，后一个单词。`W`和`B`则比较“激进”，不会考虑标点符号。

        ------

        `e` 跳转到当前**光标所在词**的**词尾**，如果光标在词尾，就跳转到**下一个词的词尾** （**考虑标点符号**）。  

        `E` 同上👆，但是**只考虑空格分隔**。

        > **👆记忆方法**：`e`可以记成`end of word`，词尾。

    * **按句移动**  

        `(` 移动到**上一个句子**的开头部分

        `)` 移动到**下一个句子**的开头部分

        > **👆记忆方法**：写代码的时候会用半角括号`()`把表达式括起来，可以理解为“一句”。

    * **按段落移动**  

        `{` 移动到**上一个段落**的开头部分

        `}` 移动到**下一个段落**的开头部分

        > **👆记忆方法**：写代码的时候会用半角大括号`{}`把代码块括起来，也就成为了一“段”代码，可以理解为一个段落。

    值得注意的是，上面这些按词、句、段落移动光标的操作，主要是**针对英文句式**的。如果操作对象是中文的话，可能并不能如愿。  
    （最明显的就是按词移动 `W`，中文里一般是不会用空格分隔每个词的，所以会直接跳转到一段的末尾！）

    --------

    * **在整个文件中跳转光标**  

        下面的指令全部都会使**跳转到行首第一个不为空白符的字符**处。  

        `gg` 跳转到文件**最开头的一行**，相当于`1G`。  

        `G` 跳转到文件**最末尾的一行**。

        `[row]G` 跳转到文件**第[row]行**（row代表数字）。  

        ![GJump-2022-04-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/GJump-2022-04-10.gif)  

        👆 分别演示了 `gg`，`G`，`2G` 的操作。

        > 记忆方法：上面三个指令本质都和`G`有关，`G`可以直接记忆为`Goto 转到,跳转`。  

        ![nGJump-2022-04-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/nGJump-2022-04-10.webp)

6. **翻页**  

    * **后空翻**  

        ```Ctrl + b``` 往回翻**一整页**  
        ```PageUp``` 往回翻**一整页**   
        ```Ctrl + u``` 往回翻**半页**    

        > **👆记忆方法**：`b` 代表 `Backward`，也就是往回；`u`则代表`Up`，也就是往上（在界面上往回翻就如同往上翻）; `PageUp`不多赘述，在现在的大多系统中都是支持的。

    * **前空翻**

        ```Ctrl + f``` 往前翻**一整页**  
        ```PageDown``` 往前翻**一整页**   
        ```Ctrl + d``` 往前翻**半页**    

        > **👆记忆方法**：`f` 代表 `Forward`，也就是往前；`d`则代表`Down`，也就是往下（在界面上往前翻就如同往下翻）。

    `Ctrl`键代表`Control`，在这里可以理解为**控制**页面滚动。

    **PS：** 再记忆一下，`命令模式/正常模式`下的指令是可以重复的。在这里如果我要向前翻`3`整页：`3PageDown`或`3Ctrl+f`。

7. **复制**

    复制是文本的经典便捷操作之一。

    * **复制一行**

        `yy` 复制光标所在行  
        `Y` 同上

        > **👆记忆方法**：`yy` 中的 `y`代表`yank [v]猛拉；猛拽`，复制其实就是把对应的文本给“拽走”了。

    * **多行复制**

        `2yy` 复制**光标所在行**以及**下一行**，共`2`行  
        `2Y` 同上
        `10yy` 复制**光标所在行**以及**下面9行**，共`10`行

        > 本质上其实就是`命令模式/正常模式`下的指令重复（在指令前面加数字前缀）。

    * **打一套复制组合拳！**  

        `y`其实可以和很多**光标移动指令**结合使用！利用好这套“组合拳”，效率倍增有木有！  

        ![yankBoxing-2022-04-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/yankBoxing-2022-04-10.png)  

        待更新....



## 末行模式/命令(行)模式

在这个模式中涉及到了最基础的文件保存 / 退出编辑器等操作。

1. ```w``` 写入 （记为```Write [v]写文件```）

    顾名思义，这个指令会将**自开始编辑以来**的更改写入文件中：

    ![write-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/write-2022-04-07.gif)  

    ![writeFile-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/writeFile-2022-04-07.png)

    文件保存总让人想起桌面软件的“另存为”功能，当然```Vi```也是能实现的：

    ```:w filename```  

    ![writeToAnother-2022-04-08](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/writeToAnother-2022-04-08.gif)  

    ![writeAnotherFile-2022-04-08](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/writeAnotherFile-2022-04-08.png)


