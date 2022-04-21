# 想法子记忆Vi/Vim常用操作及指令

在Linux系统中编辑文本总是离不开一位老帮手——**Vi**。而因为其诞生的年代有些久远，有些操作在现在看来可能有点“反直觉”。  

于是我决定写这样一篇小笔记，记录一下我**记忆**Vi的这些这些 **常用** 操作和指令的方法（**主要靠的是英语和图示了**）。

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

    4. ```s``` / ```S``` 剪切(取代) （可以直接记删的拼音```Shan```，也可以记成```Substitute [v]取代```）
        
        <a id="cutAndInsert"></a>

        小写的```s```，会**剪切光标所在位置右边的字符**，然后进入插入模式：

        ![subWord-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/subWord-2022-04-07.gif)

        大写的```S```就很暴躁了，会**剪切光标所在行**，然后进入插入模式：

        ![subLine-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/subLine-2022-04-07.gif)  

        > 记```Substitute```的话，可以理解为这个操作是剪切原有`字符/行`后进行插入，**取代**原有的`字符/行`。

        💡 `S` 和 `cc` 指令的功能是相同的！这个在下面的[**“剪切一行”**](#cutSingleLine)这一节中也提到了。

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

    <a id="arrowMovement"></a>

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
        > **👆记忆方法**：插入模式下 ```Enter``` 起到了换行作用，换行后光标会处于行首。  
        > ```-``` 就记成“减去一行”（老实说我也不太知道这个怎么记了，JK其实也够用了），  
        > ```+``` 和 ```-``` 记在一起就行。

        -------

    **PS：** 光标移动是在```命令模式/正常模式```下的指令，当然是支持重复的啦！  

    在上面所有指令的前面加上重复的次数（数字）即可，比如我要往下翻`233`行：  ```233j```或```233↓```。

3. **行内光标移动**

    这里记录一下在**一行**文本中光标的移动指令：

    * **跳到行首**

        <a id="jumpAheadOfLine"></a> 

        ```0``` 回到行首，`列数`=`1`的地方   
        ```Home``` 同上👆   
        ```^``` 回到行首，**第一个不为空白符**的字符处（对于加了缩进的行可谓是非常有用）

        > **👆记忆方法**：`0`记为“归零”；  
        > `^`的话，**正则表达式**里是用于匹配字符串首部的，可以一起记，另外其还形如一个顶部，可以记成一行中的“顶头”部分。

    * **跳到行尾**

        ```$``` 回到行尾**最后一个字符**的地方  
        ```End``` 同上👆   
        ```g_``` 回到行尾**最后一个不为空白符**的字符处

        > **👆记忆方法**：`$`在正则表达式里是用于匹配字符串尾部的，可以和正则表达式一起记；  
        > `g_`可以记成`go _`，`_`形如一个输入光标，这样记成跳转到行尾最后一个非空白字符。

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

        > **👆记忆方法**：`w`可以记成`word next`，下一个单词；  
        > `b`则记成`behind word`，后一个单词。  
        > `W`和`B`则比较“激进”，不会考虑标点符号。

        ------

        <a id="jumpToTheEndOfWord"></a> 

        `e` 跳转到当前**光标所在词**的**词尾**，如果光标在词尾，就跳转到**下一个词的词尾** （**考虑标点符号**）。  

        `E` 同上👆，但是**只考虑空格分隔**。

        > **👆记忆方法**：`e`可以记成`end of word`，词尾。

    * **按句移动**  

        <a id="jumpAheadOfSentence"></a>

        `(` 移动到**光标所在句子的开头部分**，如果已经在开头，就移动到**上一个句子**的开头部分

        `)` 移动到**下一个句子**的开头部分

        > **👆记忆方法**：写代码的时候会用半角括号`()`把表达式括起来，可以理解为“一句”。

    * **按段落移动**  

        `{` 移动到**光标所在段落的开头部分**，如果已经在开头，就移动到**上一个段落**的开头部分

        `}` 移动到**下一个段落**的开头部分

        > **👆记忆方法**：写代码的时候会用半角大括号`{}`把代码块括起来，也就成为了一“段”代码，可以理解为一个段落。

    值得注意的是，上面这些按词、句、段落移动光标的操作，主要是**针对英文句式**的。如果操作对象是中文的话，可能并不能如愿。  
    （最明显的就是按词移动 `W`，中文里一般是不会用空格分隔每个词的，所以会直接跳转到一段的末尾！）

    --------

    * **在整个文件中跳转光标**  

        ⚠️ 下面的指令全部都会使光标**跳转到行首第一个不为空白符的字符**处。    

        `gg` 跳转到文件**最开头的一行**，相当于`1G`。  

        `G` 跳转到文件**最末尾的一行**。

        `[row]G` 跳转到文件**第[row]行**（row代表数字）。  

        ![GJump-2022-04-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/GJump-2022-04-10.gif)  

        👆 分别演示了 `gg`，`G`，`2G` 的操作。

        > 记忆方法：上面三个指令本质都和`G`有关，`G`可以直接记忆为`Goto 转到,跳转`。  

        ![nGJump-2022-04-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/nGJump-2022-04-10.webp)

6. **自定义跳转标记**

    <a id="makeMarks"></a> 

    在这一坨文本中进行移动时，我经常要使用指令反复横跳。有一个地方我**经常跳转过去**，如果一步步来实在很麻烦！此时就可以用**自定义跳转标记**来解决。  

    * **设置标记**  

        指令非常简单：  

        ```ma```  

        这样就设置了一个名为`a`的标记！  

        经过测试发现 ```m``` 后面可以接 `a-zA-Z0-9` 之中的任意字符！  

        > 也就是说`m1`, `mA`, `mO`, `mz`这些都是合法的指令！  

        > 👆记忆方法：`m` 可以直接记忆为 `Mark [v]标示，标记`。  

    * **跳转到标记处**  

        ``` `a ```   

        这样就能跳转到`a`标记处~  

        标记名是**大小写敏感的**。

        > 💡 可以和下面的`y`, `c`, `d`指令联合使用。  

    * **跳转到标记所在行的行首**

        ``` 'a ```  

        使用单引号 `'` 加标记名能跳转到**该标记所在行**的行首**第一个不为空白符的**字符处！  

        ![jump2markedLine-2022-04-12](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/jump2markedLine-2022-04-12.gif)  

        > 这样是不是就相当于标记了一个行的行首呢？ 

    * **删除标记**  

        ma可以设置名为`a`的标记，删除的话其实在前面加个del即可：  

        ```delm a``` 或 ```delmarks a``` 
        
        （ `Delete Marks` ）  

        同时支持**删除多个标记**。比如我要删除`a`,`b`,`c`三个标记，可以写成：  

        ```delm abc``` 或 ```delmarks a b c```  

        也就是说删除标记的时候**标记名之间有没有间隔是无所谓的**。
        
    > 💡 在进行了[**缩进操作**](#indenting)后，标记的位置仍然在文本的相应位置，无需担心。 

    `末行模式/命令行模式`下可以**查看标记信息**：[:marks](#LLmarks)  

7. **翻页**  

    * **后空翻**  

        ```Ctrl + b``` 往回翻**一整页**  
        ```PageUp``` 往回翻**一整页**   
        ```Ctrl + u``` 往回翻**半页**    

        > **👆记忆方法**：`b` 代表 `Backward`，也就是往回；  
        > `u`则代表`Up`，也就是往上（在界面上往回翻就如同往上翻）;   
        > `PageUp`不多赘述，在现在的大多系统中都是支持的。

    * **前空翻**

        ```Ctrl + f``` 往前翻**一整页**  
        ```PageDown``` 往前翻**一整页**   
        ```Ctrl + d``` 往前翻**半页**    

        > **👆记忆方法**：`f` 代表 `Forward`，也就是往前；  
        > `d`则代表`Down`，也就是往下（在界面上往前翻就如同往下翻）。

    `Ctrl`键代表`Control`，在这里可以理解为**控制**页面滚动。

    **PS：** 再记忆一下，`命令模式/正常模式`下的指令是可以重复的。在这里如果我要向前翻`3`整页：`3PageDown`或`3Ctrl+f`。

8. **复制**

    复制是文本的经典便捷操作之一。

    * **复制一行**

        `yy` 复制光标所在行（到寄存器中）  
        `Y` 同上

        > **👆记忆方法**：`yy` 中的 `y`代表`yank [v]猛拉；猛拽`，复制其实就是把对应的文本给“拽走”了。

    * **多行复制**

        <a id="multiLineCopy"></a>

        `2yy` 复制**光标所在行**以及**下一行**，共`2`行  
        `2Y` 同上
        `10yy` 复制**光标所在行**以及**下面9行**，共`10`行

        > 本质上其实就是`命令模式/正常模式`下的指令重复（在指令前面加数字前缀）。

    * **打一套复制组合拳！（选区复制）**  

        `y`其实可以和很多**光标移动指令**结合使用！利用好这套“组合拳”，效率倍增有木有！  

        ![yankBoxing-2022-04-10](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/yankBoxing-2022-04-10.png)  

        比如 `ye` 就能复制光标所在位置到[**单词尾部**](#jumpToTheEndOfWord)的部分，

        而 `y^` 能复制光标所在位置[**到行首**](#jumpAheadOfLine)第一个非空白符的单词部分，

        再者 `y↓` 或 `yj` 能实现同`2yy`一样的功能：**复制光标所在行**以及[**下一行**](#arrowMovement)，

        甚至 `y(` 都是可以的，复制**光标所在位置**到[**光标所在句开头**](#jumpAheadOfSentence)。  

        > 也就是说 `y{`, `y}`, `yk` ,`yl` ,`y$` ,`yg^`, ```y`a``` 等等其实都是可以的！  
        > 另外还可以复制多行，比如`3ygj`。

        ------

        🤩 **总结一下**就是：
        
        `y` 可以和很多**光标移动指令**结合使用达成**部分复制**的效果， 

        通常以**光标所在位置**为起点，直到**光标移动指令执行后的光标位置**为止作为**选区**进行复制。

        ![yankGoTest-2022-04-11](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/yankGoTest-2022-04-11.gif)  

        👆示例`ygj`的效果。光标所在位置是`语`字前面，执行`gj`后光标位于`草`字前面，所以复制选区就是：

        `语人曰：“此当有大手笔事。”俄而帝崩，哀册说议皆淘所`。

9. **剪切**  

    有了复制，就不得不提到**复制并移除**——剪切了。通过这些指令移除的文本会被存进寄存器，可用于**粘贴**。

    * **剪切一行**  
        
        <a id="cutSingleLine"></a>

        `dd` 剪切**光标所在行**  
        `cc` 剪切**光标所在行**，并且**进入插入模式**  
        `S` 同上👆，这个其实在[**模式切换**](#cutAndInsert)这一节里面写了  

        > **👆记忆方法**：`dd` 中的 `d`记为`delete [v]删除`；   
        > `cc`中的`c`记为`change[v]改变`，亦可记成`Chi 吃`；   
        > `S`可以记成`Shift [v]转移`，剪切就像是在转移文本的位置，亦可记成`Shan 删`。

    * **剪切多行**

        这一部分和复制多行真的相差无几，可以说会用[**多行复制**](#multiLineCopy)了，肯定是会剪切多行的。  

        `2dd` 剪切**光标所在行**和**下面一行**，共`2`行  
        `6dd` 剪切**光标所在行**及**下面5行**，共`6`行  
        `3cc` 剪切光标所在行及下面2行，并进入**插入模式**  
        `3S` 同上👆

    * **剪切一个字符**

        `x` 剪切**光标右边的一个字符** （相当于`dl`或`d→`）  
        `s` 剪切**光标右边的一个字符**，并且**进入插入模式**  

        > **👆记忆方法**：`x` 中的 `x`记为`画个叉叉删掉字符`；  
        > `s`则可以记为`shan 删`。

    * **行内剪切**

        和复制不同的是，大写 `D` 和 `C` 的作用**并不等同于** `dd` 和 `cc`！

        `D` 剪切从**光标所在位置**到**光标所在行的行末**部分  
        `C` 完成上述操作👆，并且进入**插入模式**  

        > `C`相对`D`仍然是多出了个**进入插入模式**的功能

    * **组合剪切（选区剪切）**

        复制`y`能打组合拳，我堂堂剪切为什么不行！

        确实，剪切 `d` 或 `c` 都是可以和**光标操作指令**进行结合的，  
        他俩不同的地方在于 `d` 只是普通的剪切，`c` 还附赠了[**切换到插入模式**](#cutAndInsert)服务!  

        ![arrogantCandD-2022-04-11](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/arrogantCandD-2022-04-11.png)  

        `dj` 或 `d↓` 的效果就等同于`2dd`  
        `cj` 或 `c↓` 的效果就等同于`2cc`，会进入插入模式   
        `5x` 能剪切光标右边的5个字符   
        `5s` 能剪切光标右边5个字符且进入插入模式  
        `3cgj` 能剪切从**当前光标**到`3`个**屏幕可见行**后的相应位置部分  
        `dw` 剪切从**当前光标位置**到**下一个单词前**的部分  
        `cw` 在进行上面的操作👆后，会进入**插入模式**  
        `d^` 剪切从**当前光标位置**到**本行头部一个不是空白符的字符**的部分   
        `c^` 在进行上面的操作👆后，会进入**插入模式** 
        ```d`a``` 剪切从**当前光标**位置到`a`[**标记位置**](#makeMarks)的部分  

        ![3dgj-2022-04-11](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/3dgj-2022-04-11.gif)  

        👆演示了一下 `3dgj` 。从光标所在位置 `语` **开始**，到执行了`3gj`之后的光标位置**为止**作为选区，进行剪切。也就是这一部分：  

        ```
                                      语人曰：“此当有大手
        笔事。”俄而帝崩，哀册说议皆淘所草。释义用来赞誉写作才
        能极高，并用来称颂着名的作家和作品、故事东晋的文士王玖
        从小才思敏捷，胆量很大，散文和诗
        ```

        ------

        🤩 **总结一下**就是：
        
        `d` / `c` 可以和很多**光标移动指令**结合使用达成**部分剪切**的效果，  

        通常以**光标所在位置**为起点，直到**光标移动指令执行后的光标位置**为止作为**选区**进行剪切。

        > 💡 应该和**复制**结合记忆！

10. **粘贴**

    Copy和Cut都记了，接下来咱要想办法粘贴(Paste)内容咯~ 

    粘贴的指令其实非常简单！( ´･ω･)ﾉ 

    * `p` （向下一行/向右）粘贴文本一次（记为 `Paste [v]粘贴`）

    * 大写 `P` （向上一行/向左）粘贴文本一次（记为 `Upper(大写，同时也有“上边”的意思) Paste`）

    -------

    当**寄存器**中的内容是**一行或多行**文本（开头有换行符`\n`）时：  
    * `p`会把文本粘贴到当前**光标所在行**的**下一行**，  

    * 大写 `P` 会把文本粘贴到当前**光标所在行**的**上一行**！

    * 👇举例：先利用`yy`复制**一行**文本，然后分别用 `p` 和 `P` 进行粘贴

        ![pasteAndUpperPaste-2022-04-11](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/pasteAndUpperPaste-2022-04-11.gif)  

        > 很明显能看到使用`p`时，该行文本会被粘贴到`line2`这一行的下方；  
        > 而使用`P`时，该行文本会被粘贴到`line2`这一行的上方。

    -------

    当**寄存器**中的内容是**不足一行的文本**（**开头没有**换行符`\n`）时：

    * `p`会把文本粘贴到当前**光标所指字符**的**右边**，  

    * 大写 `P` 会把文本粘贴到当前**光标所指字符**的**左边**！

    * 👇举例：先执行`ygj`复制**开头没有换行符**的一段文本，然后分别用 `p` 和 `P` 进行粘贴

        ![ygjPasteAndUpperPaste-2022-04-11](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/ygjPasteAndUpperPaste-2022-04-11.gif)

        > 粘贴的时候**光标指向的字符**是`n`。使用`p`粘贴时，内容粘贴在`n`的右方；而使用`P`粘贴时，内容粘贴在`n`的左方。

    -------

    🌟 `命令模式/正常模式`下指令可加数字前缀重复执行，这里也是一样的。比如 `4p` 就是把寄存器的文本在光标右边/下一行**粘贴四次**。

11. **Vim上下文编辑**

    Vim在复制`y`，剪切`c`、`d`这些操作的基础上复用了`i`和`a`指令用于对上下文进行操作：  

    > 👆记忆方法：`i` 记作 `inner 内部的`，在下面使用这个指令，操作**不会考虑周围的空格或符号**；  
    > `a` 记作 `around 周围`，这个则**会考虑周围的空格或符号**。具体视情况而定。  

    * **针对单词**（`w` / `W`）

        > ⭐ 记作`Word`

        `diw` 剪切**光标所在的单词**（不考虑标点符号）  

        `diW` 剪切**光标所在的单词**（考虑周围的标点符号，也就是说剪切的时候会把周围的标点符号一并算进去）  

        > `i` (inner) 在这里的意思是操作的部分**不包括单词周围的空格**  

        `daw` 和 `daW` 也是剪切**光标所在的单词**（`w`和`W`区别不再多赘述）  

        > `a` (around) 在这里的意思是操作的部分**包括单词周围的空格**  

        ※ 下面演示一下`diw`和`daw`的区别：  

        ![diwAnddaw-2022-04-11](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/diwAnddaw-2022-04-11.gif)  

        很明显能看到，使用`diw`时，剪切后遗留的文本是 `of ,`（单词左边的空格保留了）；  
        使用`daw`时，剪切后遗留的文本是 `of,`（单词左边的空格被一同剪切了） 。   

        > 💡 像 `y`, `c`, `d` 一类指令都是可以这样用的，比如`yaw`, `yiW`,  `ciw`, `caW` 等等... 

        ------- 

    * **针对句子**（`s`） 

        > ⭐ 记作`Sentence`。注意，这里**没有大写** `S` 的用法！  

        其上下文操作和上面单词的是一样的！比如：

        `yis` 复制**光标所在的句子**（`i`不考虑周围的空格）    
        `yas` 复制**光标所在的句子**（`a`考虑周围的空格）  
        `cas` 剪切**光标所在的句子**，并进入插入模式  
        `dis` 剪切**光标所在的句子**  
        ...and so on  

        > 💡 注意，这里只有小写`s`的用法。

        -------

    * **针对段落**（`p`）

        > ⭐ 记作`Paragraph`。注意，这里**没有大写** `P` 的用法！   

        这里的用法也和上面是一样的：

        `cap` 剪切**光标所在的段落**，并进入插入模式（`a`考虑周围的空格）   
        `dip` 剪切**光标所在的段落**（`i`不考虑周围的空格）  
        `yip` 复制**光标所在的段落**  
        ...and so on  

        > 💡 这里同样只有小写`p`的用法。

        -------

    * **针对单/双引号**（ `'` / `"` ） 

        很明显，这一部分操作是用来处理单/双引号**引起来的文本**的，和上面的`w`、`W`、`s`、`p`不同的是这里的 `i` / `a` 的含义：

        > `i` (inner) 在这里的意思是操作的部分**不包括**两侧的引号  
        > `a` (around) 在这里的意思是操作的部分**包括**两侧的引号  

        `di"` 剪切光标所在的**双引号**引起来的**同行**文本（`i`代表不包括两侧的引号） 👇  

        ![diQuoted-2022-04-12](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/diQuoted-2022-04-12.gif)  

        `da'` 剪切光标所在的**单引号**引起来的**同行**文本（`a`代表包括两侧的引号） 👇

        ![daQuoted-2022-04-12](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/daQuoted-2022-04-12.gif)  

        > 💡 `y` 和 `c` 也是一样的用法

        -------
    
    * **针对括号** （ `( )`, `< >`, `[ ]`, `{ }` ）  

        这一部分用来处理**括号括起来的文本**。

        > 📌 注意，**这里的操作是支持多行的！！**  

        > `i` (inner) 在这里的意思是操作的部分**不包括**两侧的括号  
        > `a` (around) 在这里的意思是操作的部分**包括**两侧的括号  

        `di(` 或 `di)` 剪切光标所在的**小括号`( )`括起来的**文本（`i`代表不包括两侧的括号） 👇

        ![diBracket-2022-04-12](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/diBracket-2022-04-12.gif)  

        `da{` 或 `da}` 剪切光标所在的**大括号`{ }`括起来的**文本（`a`代表包括两侧的括号） 👇  

        ![daBracket-2022-04-12](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/daBracket-2022-04-12.gif)  

        > 💡 `y` 和 `c` 也是一样的用法，适用于多种括号。

    ![quoteAndBracketComic-2022-04-12](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/quoteAndBracketComic-2022-04-12.png)
    

12. **双引号指定寄存器**

    <a id="specifyRegs"></a> 

    默认情况下使用复制粘贴似乎用的都是一个公共的寄存器（Registers），Vim实际上是支持寄存多条文本的，**只需要在指令前指定寄存器名**即可。

    * **默认的寄存器**被系统记为 `"` ，一个双引号

        当执行`yy`时，会将目标文本行存入 `"` 寄存器，操作等同于：  

        `""yy`  

        > 💡 其中第一个单引号代表**引用寄存器**，第二个单引号便是**默认寄存器的名字**。
        
        于是粘贴指令也可以改写成：`""p`，即从默认寄存器中取出文本进行粘贴。  

        ------

    * **指定寄存器名字**

        把上述第二个单引号改成 `a-z` 的**小写字母**即可！这样就相当于得到了`26`个“剪贴板”呢~  

        比如将当前行复制到`a`寄存器中：  

        ```"ayy```

        那么粘贴该内容的指令也显而易见了：  

        ```"ap```

        --------

    * **往已命名寄存器中附加内容**

        上面写到用**小写字母**对寄存器进行命名，那么**大写字母**呢？

        大写字母用于往寄存器中**附加内容**：

        ```"Ay$``` 将光标位置到行尾的文本附加到`a`寄存器中 👇

        ![AppendToRegisterA-2022-04-12](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/AppendToRegisterA-2022-04-12.gif)

    这里主要记录一下复制粘贴方面的寄存器引用方法。Vim寄存器储存的内容远不止这么一点，还包括操作历史记录等信息，就不多赘述了。

    > 👆记忆方法：`"` 就像个图钉📌，标明是哪一个寄存器。  

    `末行模式/命令行模式`下可以**查看寄存器信息**：[:reg\[isters\]](#LLregisters)  

13. **缩进**  

    <a id="indenting"></a> 

    Vi/Vim 常用于编辑代码，缩进功能当然也是比较常用的了。  

    * **增加缩进**  

        `>>` 给**光标所在行**增加缩进   
        `4>>` 给**光标所在行**以及下面`3`行增加缩进，共`4`行  

    * **减少缩进**  

        `<<` 给**光标所在行**减少缩进   
        `5<<` 给**光标所在行**以及下面`4`行减少缩进，共`5`行  

    > 👆记忆方法：`>` 可以看作在往右边“顶”文本，而 `<` 可以看作在往左边“拉”文本。

14. **撤销与重做** 

    文本编辑必不可少的便是撤销与重做功能了！  

    * **撤销** 

        `u` 撤销**上一步操作**  

        大写 `U` 撤销**光标所在行**的**所有操作**  

        > 👆记忆方法：`u`直接记成 `undo [v]撤销，取消` 即可。  
        > 一定要辨别小写`u`和大写`U`的操作区别！  

    * **重做**  

        `Ctrl + r` 或者 `Ctrl + R`  

        重做操作和小写`u`的操作是反向的！重新执行撤销掉的操作。  

        > 👆记忆方法：`r`直接记成 `redo [v]重做` 即可。  
        > 为什么多了个`Ctrl`呢？因为`r`已经被**替换模式**占用了！  

15. **数字快捷增减**

    <a id="numberAscDesc"></a> 

    要修改一个数值的时候可以进入插入模式，移动光标到数值上进行修改，这样可能略嫌麻烦了。在`命令模式/普通模式`下提供了对**数值**进行**递增**或**递减**的操作指令：  

    * `Ctrl + a` （`Ctrl + A`亦可） 
    
        对光标指针所在数值进行**递增操作**。如果光标没有指向数值，会**向右查找**数值进行递增。  

        > 💡 记忆方法： `a` 可以记成 `ascend [v]上升；升高；递增`，   
        > 亦可以记成 `accumulate [v]积累；逐渐增加`。  
        > 之所以有 `Ctrl` ,是因为`a`已经被用于切换**插入模式**了！

        ![ascendingNum-2022-04-12](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/ascendingNum-2022-04-12.gif)  

        👆 示例中可以看到，当光标右方没有数值时会屏幕闪烁提示指令无效。  

        -------

    * `Ctrl + x` （`Ctrl + X`亦可）  

        对光标指针所在数值进行**递减操作**。如果光标没有指向数值，同样会**向右查找**数值进行递减。  

        > 💡 记忆方法： `x` 可以记成 `叉掉数字，从而让数值减小`。  
        > 之所以有`Ctrl`也是因为`x`已经被用于**剪切字符**。  

        -------

    * **重复指令**  

        设想一下，如果我要增加10亿，难不成要一直按住按键不动？当然不是这样啦！  

        再记忆一遍，`命令模式/普通模式`下的指令可以加**数字前缀**进行重复，在这里只需要这样输入指令即可快速递增指定数值：  

        ```1000000000Ctrl + a```  

        ![ascendBillionTimes-2022-04-12](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/ascendBillionTimes-2022-04-12.gif)  

        ```Ctrl + x``` 同理！  

        ![rocketAscending-2022-04-12](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/rocketAscending-2022-04-12.png)

    * **进制的表示方法**

        `0x` 前缀表示**十六进制**  （记**hex**的**x**）

        `0` 前缀表示**八进制**  （记**octal**的**o**，虽然这里是**0**）

        `0b` 或 `0B` 前缀表示 **二进制**  

        ---------

    <a id="availableNumTypes"></a> 

    > 💡 Vim支持**进行增减**的类型有：二进制数（**bin**ary），八进制数（**octal**），十进制数（decimal，默认支持），十六进制数（**hex**adecimal），无符号数（**unsigned**），字母（**alpha**betical）。

    👆 具体配置见`末行模式/命令(行)模式`的[nrformats配置](#nrformats)一节。  

16. **字母大小写切换**  

    数字增减有了，字母其实也是可以“随地大小变”的！

    * `gu后接光标移动指令`

        将从光标位置开始到**执行移动指令后**光标的位置结束作为选区，  
        将选区内的所有字母转换为**小写**!  

        ![guiw-2022-04-16](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/guiw-2022-04-16.gif)  

        👆 演示了`guiw`指令。`iw`选择的是光标所在单词`FLOOD`，执行`gu`后将单词所有字母转换为**小写**：`flood`。  

    * `gU后接光标移动指令`  

        将选区内的所有字母转换为**大写**  

    > 💡 记忆方法：`gu`和`gU`中一个是小写`u`，一个是大写`U`。大写`U`可以记成`UPPERCASE 大写`，而当U小写成为`u`，对应的便是`小写`了。`g`仍然可以记成`go`，`go UPPERCASE`。  

    * `~` (波浪符)  

        波浪符将**光标**所在的字母进行**大小写交换**，比如`b`会转换为`B`；`B`则会转换为`b`。  

        这个指令可以用于转换**光标所在行**中**自光标往后**的字母**大小写**。  

        ![tilde-2022-04-16](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/tilde-2022-04-16.gif)  

        👆 `50~`，将本行中自光标往后50个字符进行大小写转换。  

    * `g~后接光标移动指令`  

        上面的`~`只能对**光标所在行**的字符进行大小写交换，如果要涉及**一个选区**呢？  

        于是，`g~`他来了！在`g~`后接光标移动指令，将**光标当前位置**开始到**执行指令后光标所在位置**为止之间的字符进行大小写交换。  

        ![gT2gj-2022-04-17](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/gT2gj-2022-04-17.gif)  

        👆 `g~2gj`，将从光标位置开始到光标向下移动**两个屏幕行**为止的部分字符大小写交换。

    > 💡 记忆方法：波浪线可以看作是一条**波**，随着波的传播，同一个地方的**波峰波谷交替出现**，在这里就记成**大写小写的交替**。

    ![caseInversion-2022-04-17](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/caseInversion-2022-04-17.gif)

17. **重复最后一次更改**  

    `.`  

    ↑ 没错，指令就是一个句点！  

    比如，之前我输入了`3dd`剪切了三行文本，之后再按一次 `.` 会再往下剪切三行；

    又比如，我先输入了`x`剪切光标所指的字符，之后输入 `3.` 就会再剪切三个字符。

    **但是！**，如果我先输入`3j`让光标往下移动3行，再按 `.` 是没有效果的，因为`j`指令仅仅是移动了光标，**没有对文本造成任何更改！**  

    > ⚠ 注意！ `.` 重复的是**最后一次更改**，而不是最后一次指令！  
    
    > 👆 记忆方法：硬背

18. 其他可能常用到的指令  

    * 大写 `J`  

         这个指令能将**光标所在行**和其下面一行**连接成一行**，之间用**一个空格间隔**：  

         ![LinesJoin-2022-04-17](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/LinesJoin-2022-04-17.gif)  

         > `4J` 的话则是将光标所在行和**下面3行**进行连接，每行之间用空格隔开。**一共4行**。  

         > 💡 记忆方法：直接记  `Join [v]加入；连接` 即可。  
        
    * `gJ`  

        同样是连接行。和上面`J`不同的是，这个指令在连接行之后**不会在之间添加空格！**。  

        > 💡 记忆方法：可以记成`gross Join`，`gross`有`总的；粗鲁的`的意思，gross Join就可以记成 “**连接成一个总体**” 或者 “**粗鲁地直接连在一起**”。  

    ...to be updated  


## 末行模式/命令(行)模式

在这个模式中涉及到了最基础的文件保存 / 退出编辑器等操作。

1. ```:w``` **写入** （记为```Write [v]写文件```）

    顾名思义，这个指令会将**自开始编辑以来**的更改写入文件中：

    ![write-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/write-2022-04-07.gif)  

    ![writeFile-2022-04-07](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/writeFile-2022-04-07.png)

    文件保存总让人想起桌面软件的“另存为”功能，当然```Vi```也是能实现的：

    ⭐ ```:w filename```  

    ![writeToAnother-2022-04-08](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/writeToAnother-2022-04-08.gif)  

    ![writeAnotherFile-2022-04-08](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/writeAnotherFile-2022-04-08.png)  

    除了上面针对单一文件的写入指令，还可以同时**写入多个文件**，下面这个指令适用于Vim编辑器中同时打开多个文件的情况：

    ⭐ ```:wa```  

    👆 将所有打开的文件进行写入保存。（可以记成`Write All 写入所有`）  

    ![WriteAll-2022-04-17](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/WriteAll-2022-04-17.png)  

    如果只想保存文本中的一部分呢？Vim还支持指定行写入文件：  

    ⭐ ```:<line1>,<line2> w filename```  

    👇 比如我想写入第`13`至第`14`行的文本（共两行）：```:13,14 w filename```  

    ![writeSpecificLines-remake-2022-04-18](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/writeSpecificLines-remake-2022-04-18.gif)  

    输出文件效果：  

    ![SpecificLines-2022-04-18](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/SpecificLines-2022-04-18.jpg)  



2. ```:q``` **退出**（记为```Quit [v]离开；关闭```）  

    文本编辑器必备操作——关闭文件。  

    ⭐ ```:q``` 指令会退出**当前编辑的文件**（如果只打开了一个文件，这个操作同时会**退出编辑器**）  

    ⭐ ```:qa``` 指令则会退出**所有打开的文件**，并且**退出编辑器**。

    > 👆 记忆方法：`Quit All 关闭所有`  

    > **注意**：上面这两个指令执行的前提是**当前文件自打开后没有任何更改**，也就是说文件缓冲区中没有任何新增内容，不然编辑器会警告文件尚未保存更改。  
    > 怎么强制退出呢？下面就将写到.....  

3. **联合使用** ```w``` 与 ```q```   

    ![wCombineWithQ-2022-04-18](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/wCombineWithQ-2022-04-18.png)  

    既然未保存没法正常关闭文件，那么保存后关闭不就行了嘛！  

    ⭐ ```:wq```  

    👆 双剑合璧，数据无忧！这个指令会将缓冲区中的编辑内容**写入文件**，并且**关闭文件**。（针对**当前正在编辑的文件**）  

    当然，也有针对**编辑器所有打开的文件**的操作：

    ⭐ ```:wqa```  

    👆 写入并关闭**所有打开的文件**，这之后会**退出编辑器**。（```Write & Quit All```）


4. 感叹号 ```!``` **强制执行指令**  

    有些情况下末行模式指令的执行可能会被拒绝，比如上面的`:q`在文件保存前不能直接执行。（这一点其实也是为了数据安全）  

    ⭐ 然而，只需要在这些指令**末尾**加上感叹号 ```!``` 就可以**强制执行**了！  

    比如**未保存强制退出编辑器**就可以用： ```:qa!```  

    ![forcedQuitAll-remake-2022-04-18](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/forcedQuitAll-remake-2022-04-18.png)  

    此外我觉得**文件强制写入**也是值得记录一下的：```:w!```  

    👆 这个指令会**尝试强制写入文件，即使文件是只读(Read-only)的**，前提是**用户得要有修改文件属性的权限**。   

    > ⚠ 强制执行指令时一定要再三考虑，防止数据丢失等问题。  

5. ```:r``` **在下一行读入文件** （记成 ```Read [v]读取```） 

    在编辑文本的时候可能需要将**另一个文件的文本**插入进来，这个时候就轮到`:r`大显身手了。  

    ⭐ ```:r``` 能够**读取一个文件**，并将文件内容**追加**到光标所在行的**后一行**。  

    ![appendFileContent-2022-04-18](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/appendFileContent-2022-04-18.gif)  

    👆 左边是待读取的文件，右边是正在编辑的文件。利用`:r`指令，成功将文件内容插入到光标后一行。  

6. ```:help [command]``` 或 ```:h [command]``` **查看帮助**  

    就像Linux中的`man`指令一样，Vi/Vim的`末行模式/命令行模式`也有个`:help`指令用来查看编辑器使用帮助。  

    ⭐ 直接使用 ```:help``` 时会打开帮助文件，并将光标移动到顶部。  

    ⭐ 指令形如```:help [command]```时，在开启文件后光标会跳转到对应的指令帮助信息。  

    > 比如要看寄存器指令的帮助信息：```:help reg``` 或 ```:help registers``` （ ```:h reg``` 当然也行 ）  

7. ```:registers``` 或 ```:reg``` **查看寄存器**  

    <a id="LLregisters"></a> 

    这一部分可以结合```命令模式/正常模式```的[双引号指定寄存器](#specifyRegs)一起记。    

    ⭐ 直接使用 ```:registers``` 或 ```:reg``` 时会展示**所有的寄存器** 👇  

    ![registers-2022-04-18](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/registers-2022-04-18.jpg)  

    也可以在后面**指定要查询的寄存器名**，这些名称可以用空格分开，也可以连在一起写：  

    ⭐ 比如 ```:reg[isters] a b c``` 或 ```:reg[isters] abc``` ，可以筛选查询`a`,`b`,`c`寄存器 👇  

    ![specifyRegisters-2022-04-18](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/specifyRegisters-2022-04-18.gif)  

    关于寄存器更多用法这里就不多赘述了~  

    ![009-2022-04-18](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/009-2022-04-18.png)  


8. ```:marks``` **查看标记信息**  

    <a id="LLmarks"></a> 

    这个和上面```:reg[isters]```的使用**很类似**，所以咱给写一起了。  

    这一部分可以结合```命令模式/正常模式```的[自定义跳转标记](#makeMarks)一起记。 

    ⭐ 直接使用 ```:marks``` 能查询到**所有现有的标记**  

    和:registers一样，在后接查询的标记名，能筛选结果：  

    ⭐ 比如 ```:marks a b c``` 或 ```:marks abc``` ，可以筛选查询`a`,`b`,`c`标记  

9. ```:set``` **设置项**  

    这个指令用于设置一些配置项，这里就记几个比较常用的（因为配置项有非常多，全写出来太累赘了）  

    * **开启/关闭行号显示**  

        ```:set number``` 或 ```:set nu``` 可以在行首显示行号  

        而 ```:set nonumber``` 或 ```:set nonu``` 则可以关闭行号显示  

        此外 ```:set nu!``` 或 ```:set number!``` 可以在**开启行号/关闭行号**之间互相切换。  

        ![lineNumbers-2022-04-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/lineNumbers-2022-04-19.gif)  

        > 👆 记忆方法：直接记 `Number [n]号码 [v]标号`  

    * **开启/关闭相对行号显示**  

        ```:set relativenumber``` 或 ```:set rnu``` 可以显示相对行号  

        而 ```:set nonumber``` 或 ```:set nonu``` 则可以关闭相对行号显示  

        此外 ```:set rnu!``` 或 ```:set relativenumber!``` 可以在**开启相对行号/关闭相对行号**之间互相切换。

        ![relativeNumber-2022-04-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/relativeNumber-2022-04-19.gif)  

        👆 可以看到，相对行号以**光标所在行**为第`0`行，向上或向下标出相对的行号。  

        这个特性可以用于**辅助涉及多行的指令**，比如 `y3j`, `3k`, `2>>` 等。  

        > 💡 记忆方法：按字面记，即 `Relative Number 相对 号码`  

    * **识别数字进制的配置**  

        <a id="nrformats"></a> 

        上面已经写过`Ctrl+x`和`Ctrl+a`能[快捷减增数字](#numberAscDesc)。不过呢，Vim怎么识别**增减的类型**呢? 这就涉及到下面这个配置项了：  

        * ```:set nrformats``` 或 ```:set nf```  

            直接输入这个指令会查询**该配置项当前的设置**：  

            ![queryNrformats-2022-04-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/queryNrformats-2022-04-19.gif)  

            上面也写过Vim[能识别的增减类型](#availableNumTypes)。这里的`octal,hex` 则代表会尝试识别**八进制**，**十六进制**以及**十进制**的数字。（十进制是默认支持的）  

        * 使用 `+=` 或 `-=` 进行配置  

            这种写法其实可以理解为一些编程语言中的**运算赋值**操作：

            ```:set nrformats+=bin```  

            ```:set nrformats+=bin,alpha```  

            (或者 `set nf+= ...` )  

            👆 多个可以用**逗号**分隔，会“附加”到配置项的末尾。  

            然而，移除的时候就要注意了，一定要按**配置中的顺序**来：

            ```:set nrformats-=bin```  

            ```:set nrformats-=bin,alpha```  

            👆 这样写的话必须保证配置项中也有**bin,alpha**这个字串！  

            ![removeNf-2022-04-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/removeNf-2022-04-19.gif)  

            ↑ 没有`bin,alpha`这个字串，无法移除。

            ![successfullySetNf-2022-04-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/successfullySetNf-2022-04-19.gif)  

            ↑ 移除`octal,alpha`的话就很成功，因为配置项中有这个字串。  

        * 直接使用 `=` 进行配置  

            同样可以按编程语言里的**赋值**来理解，会直接**改变整个配置**  

            ```:set nrformats=octal,hex```  

            👆 多个值一样是用**逗号分隔**  
        
        > 💡 记忆方法：`nrformats`可以看成`number recognizing formats`，也就是`数字识别格式`。取`n`和`f`即为`nf`，因此`nf`也可以简单记为`number format`！  

    * **执行Shell指令**  

        编辑文本到一半突然想执行一个Shell指令。我之前可能会利用`:wq`先退出编辑器，输入执行指令后再重新打开编辑器。  

        实际上在`末行模式/命令行模式`下Vi/Vim也是能快捷切换到Shell执行指令的：  

        * ```:! <command>``` **暂时离开**编辑界面，在Shell下执行指令**并打印结果**   

            ![temporaryShell-2022-04-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/temporaryShell-2022-04-19.gif)  

            👆 **临时**跳转到命令行执行`清屏`及`查询日期`指令并展示结果。  

            > 💡 `:!` 还有很多神奇的用法，比如执行指令后将返回的结果插入下一行：`:r! <command>`；  
            > 又比如将文本`21`至`25`行进行升序排序，并替换原文本：`:21,25! sort`，这里就很像Shell中`管道符`的用法了。  
            
            ![sortByShell-2022-04-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/sortByShell-2022-04-19.gif)  

            👆 演示 `:21,25! sort`  

            > 更多用法在这里就不多赘述了╮(╯3╰)╭  

        * ```:sh``` **创建一个新的Shell会话**

            这个指令就比较简单粗暴了，在执行后会创建一个新的Shell会话，我可以直接在Shell中执行指令！  

            ![createNewShell-2022-04-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/createNewShell-2022-04-19.gif)  

            这个Shell是一个**非登入**Shell，所以需要使用 `exit` 指令退出。  

            该Shell退出后会**回到编辑器界面！**  

            ![exitShell-2022-04-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/exitShell-2022-04-19.gif) 

            > 👆 这个直接记忆 `Shell` 就行   

    * **重复上一条指令**  

        在`命令模式/正常模式`下我可以使用 `.` 句点来重复上一次更改。在`末行模式/命令行模式`下也有类似的方法。  

        * **经典方法**  

            在`末行模式/命令行模式`下输入 `:` 后会进入`末行模式/命令行模式`，此时可以通过：  

            ⭐ `↑`, `↓`, `PageUp`, `PageDown` 
            
            来浏览之前的输入历史  

            ![scrollHistory-2022-04-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/scrollHistory-2022-04-19.gif)  

            💡 可以在 `:` 后面输入一些字符以**加快检索**：  

            ![scrollHistoryWithHint-2022-04-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/scrollHistoryWithHint-2022-04-19.gif)  

            👆 输入`:s`后，能快速浏览`:s`开头的历史指令记录；`:se`则能快速浏览`:se`开头的历史指令。  

        * **`@` 方法**  

            在`命令模式/正常模式`下输入：

            `@:`  

            能重新执行**上一条`末行模式/命令行模式`指令**。  

            > 👆 记忆方法：把 `:` 末行指令给**at**出来！  

            虽然我把这个方法写在末行模式这里了，但实际上其可以算是`命令模式/正常模式`的指令。  

            因此也是**可以重复执行的！**  

            ![repeatLastLine-2022-04-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/repeatLastLine-2022-04-19.gif)  

            👆 演示：先用`:d`删除一行，然后再用`3@:`重复执行`:d`三次以继续删除下面三行。  

    ![however-2022-04-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/however-2022-04-19.png)  

    有点累了...喝口水...   

## 基本搜索替换  

1. **搜索**  

    **前提**：在`命令模式/正常模式`下：  

    ```/<搜索模式>```  自光标**向后查找**  

    ```?<搜索模式>```  自光标**向前查找**  

    > 👆 其实`末行模式/命令行模式`下也不是不行，也就多了个冒号：`:/<搜索模式>`、`:?<搜索模式>`。不过接下来这两个指令就不行了 ↓
    
    ```n``` ---> 这个指令**会重复上一次搜索动作**。  
    * 比如`/....`是往**光标后**搜索，按下`n`就是**在光标后**再次搜索
    * 而`?....`是往**光标前**搜索，那么按下`n`就是**在光标前**再次搜索  

    ```N``` ---> 这个指令和 ```n``` 的操作相反  

    > 👆 记忆方法：`n`可以记成 `next [a]下一个的` ，正如o和O操作相反一样，`N`和`n`操作相反。  

    -------------

    这里的`搜索模式`实际上就是**正则表达式**，不过和普通的正则略微有些不同！  

    1. 当然是可以直接搜索字串的  

        ![searchStr-2022-04-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/searchStr-2022-04-21.gif)  

    2. **大小写不敏感**搜索  

        一般在写正则表达式时要进行大小写不敏感匹配我一般会在正则表达式末尾加上`i`标记，但是在`搜索模式`里是不行的，需要用到特殊转义标记：
        
        `\c`  

        这个标记写在`搜索模式`的中间（不要写在中括号`[]`里！）一般也是可以的，但我觉着还是**放在末尾**好辨别一些。

        ![caseInsensitivelySearch-2022-04-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/caseInsensitivelySearch-2022-04-21.gif)

        ![caseInsensitiveSearch-2022-04-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/caseInsensitiveSearch-2022-04-21.png)  

        > 👆 记忆方法：这里的 `\c` 可以记成 `case [n]大小写`。

    3. 利用**正则表达式**进行搜索  

        这里使用正则表达式和在编程语言里有些小区别，这里简单写一下：  

        - 采用 `<` 和 `>` 分别匹配**单词词首**和**单词词尾**，而不是`\b`。  

        - 部分元字符**要发挥元字符**的作用的话，需要**先转义**。这里列个表格：  

            | 元字符 | 用途 | 在搜索模式中的用法 |
            |:---:|:---:|:---:|
            | `+` | 匹配1次或多次 | `\+` |
            | `?` | 匹配0或1次（非贪婪模式在Vim中另有元字符） | `\?` |
            | `{n,m}` | 匹配n到m次 | `\{n,m}` |
            | `{n,}` | 匹配n次或更多次 | `\{n,}` |
            | `{n}` | 匹配n次 | `\{n}` |
            | `{,m}` | 匹配0到m次 | `\{,m}` |
            | `<` | 匹配单词首部 | `\<` |
            | `>` | 匹配单词尾部 | `\>` |
            | `(` | 子模式开始标志 | `\(` |
            | `)` | 子模式结束标志 | `\)` |
            | `\|` | 两项之间任意匹配一个 | `\\|` |  

            除此之外的元字符大多是可以直接使用的，下面是一些示例：  

            ![regexSearch1-2022-04-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/regexSearch1-2022-04-21.jpg) ![regexSearch2-2022-04-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/regexSearch2-2022-04-21.jpg) ![regexSearch3-2022-04-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/regexSearch3-2022-04-21.png) ![regexSearch4-2022-04-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/regexSearch4-2022-04-21.jpg) ![regexSearch5-2022-04-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/regexSearch5-2022-04-21.jpg)  

    4. 神奇的**非贪婪模式**  

        Vim这里的非贪婪模式用的就不是元字符 `?` 了，取而代之借用了一下大括号 ```{...,...} ```  

        > 官方说明可以在`末行模式/命令行模式`下输入`:help non-greedy`查看。

        当 `{` 后面**紧接**了一个连字符(Hyphen) `-` 时，就相当于采用了**非贪婪匹配**，下面举些例子：    

        `e\{2,3}` 匹配 `ee` 或 `eee`，如果有更长的`eee`就优先匹配（也就是**贪婪匹配**）  

        `e\{-2,3}` 匹配 `ee` 或 `eee`，优先匹配更短的`ee`（也就是**非贪婪匹配**）  

        ![non-greedyMatch-2022-04-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/non-greedyMatch-2022-04-21.gif)  

        > 👆 这个例子中在`{`后紧接`-`后会往少的匹配，也就是只匹配一个`paprika `。

        ------

        如果在 `-` 后面**没有指定匹配次数**，就有点类似 `*` 了。形如：  

        ```\{-}```  匹配`0-任意`次，但是非贪婪匹配，匹配次数尽量少。

        ```\{-,3}```  匹配`0-3`次，但是非贪婪匹配，匹配次数尽量少。  

        > 例：用模式 `ke\{-}` 匹配字串`keep`只匹配到了`k`，因为`\{-}`代表匹配`0-任意次`，但是**非贪婪匹配**，所以这里`e`匹配了`0`次。

    5. 搜索光标下的单词

        这一小节的操作是在`命令模式/正常模式`下的：

        * `*` **往后**搜索**光标目前指向的单词**，只匹配**一整个单词**  

        * `#` **向前**搜索**光标目前指向的单词**，只匹配**一整个单词**  

            ![searchWordUnderCursor-2022-04-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/searchWordUnderCursor-2022-04-21.gif)  

            > 从这个例子可以看到，实际上Vim是把光标指向的单词转换成了搜索语句。`*` 对应 `/\<accept\>`，`#` 对应 `?\<\accept\>`。  

        * `g*` **往后**搜索**光标目前指向的单词**，单词可作为**字串的一部分**被匹配。

        * `g#` **向前**搜索**光标目前指向的单词**，单词可作为**字串的一部分**被匹配。

            ![searchWordPartUnderCursor-2022-04-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/searchWordPartUnderCursor-2022-04-21.gif)  

            > 可以看到 `*` 对应 `/accept`，`#` 对应 `?accept`，单词可作为**字串的一部分**被匹配。 

        >💡 因为这几个指令被转换为末行搜索操作了，所以在搜索中可以用的`n`、`N`这一类指令也是可以用的。 

    6. 开启搜索高光显示

        上面的图示中搜索匹配项都会“**黄的发光**”，这种**匹配结果高光显示**是可以作为配置项使用 `:set` 进行设置的：  

        * `:set hlsearch` 或 `:set hls` 开启搜索结果**高光显示**  

        * `:set nohlsearch` 或 `:set nohls` 关闭搜索结果**高光显示**  

            ![highlightSearch-2022-04-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/highlightSearch-2022-04-21.gif)  

        虽然高亮显示一目了然，但是开了之后光标就不明显了，高光咱已经看够了，怎么**关掉目前结果的高光展示**呢？  

        * `:nohlsearch` 或 `:noh` 关闭**目前的高光显示**  

            ![nohlsearch-2022-04-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/nohlsearch-2022-04-21.gif)  

            > 可以看到输入`:noh`后会取消目前的高亮，但是这并不影响重新开始搜索时高亮展示匹配。  

        > 💡 记忆方法：`hlsearch` 即 `Highlight Search`，`Highlight [v]突出，强调]`，`Search [n]搜索`。  

        ![hlsearch-2022-04-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/hlsearch-2022-04-21.png)  

2. **替换**

## 简单多文件编辑

## 可视模式


> 帮助:help，registers/reg查看寄存器，marks查看标记，:set nrformats/nf设置递增递减数值类型支持，@:重复上一条末行操作，行号:set nu/number ，:!命令行执行
> 搜索部分还有高光显示:set hls[earch]
> 搜索部分记得在“可能用到的指令”中添加g* * g# #  
> 记得这些部分都要和上文形成关联
