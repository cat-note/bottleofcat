# Python正则表达式使用小记
最近做Python课实验发现**正则表达式**和它在py中的的**标准库re**有很多能多琢磨一下的点，遂决定写成一篇小记，以后想复习能再来看看。  

![fuwafuwa-2021-11-02](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/fuwafuwa-2021-11-02.gif)  

## 名词  

因为不同文献书籍对正则表达式的描述有差别，我在这里列出一下我已知的名词表述：  

|本小记中|其他说法|
|:---:|:---:|
|模式|表达式 / pattern|
|子模式|子表达式 / subpattern|

## 子模式扩展语法  

* ### look-behind语法问题  

    ```python
    s = 'Dr.David Jone,Ophthalmology,x2441 \
    Ms.Cindy Harriman,Registry,x6231 \
    Mr.Chester Addams,Mortuary,x6231 \
    Dr.Hawkeye Pierce,Surgery,x0986'
    pattern=re.compile(r'(?<=\s*)([A-Za-z]*)(?=,)')
    ```

    在这个例子中我原本是想寻找字符串中人名的姓氏的，但脑袋一热写了个```\s*```，跑了一下当即给我返回了错误:  

    ```re.error: look-behind requires fixed-width pattern```  

    我一会儿没反应过来，国内搜索引擎也没查到个大概。冷静下来后咱注意到了 **requires fixed-width pattern** 这一句，意思是需要固定长度的模式，再看一眼前面的look-behind，突然咱就恍然大悟了：  

    ```python
    pattern=re.compile(r'(?<=\s)([A-Za-z]*)(?=,)')
    ```

    这样写就没问题了，我们匹配到了所有的姓氏：

    ```python
    print(pattern.findall(s))
    # ['Jone', 'Harriman', 'Addams', 'Pierce']
    ```    
    
    问题出在哪呢？  

    所谓```look-behind```其实就是```(?<=[pattern])```一类子模式扩展。

    * **注意分辨** ```(?<=[pattern])```和```(?=[pattern])```，**前者**是放在**待匹配正则表达式 之前**的，**后者**是放在**待匹配正则表达式 之后**的。

    * 这两个子模式扩展语法的功能是 **匹配[pattern]的内容，但在结果中并不会返回这个子模式**。
    
    * 我们通过表格来说明一下，**如果匹配到了**即返回 **```[pattern2]``` 匹配** 的内容：  

        | 正则写法 | 正误 |
        |:---:|:----:|
        |```(?<=[pattern1])[pattern2](?=[pattern3])```|√|
        |```(?=[pattern1])[pattern2](?<=[pattern3])```|×|
        |```[pattern4](?<=[pattern1])[pattern2](?=[pattern3])```|√|
        |```[pattern4](?<=[pattern1])[pattern2](?=[pattern3])[pattern5]```|√|
        |```(?<=[pattern1])[pattern2]```|√|
        |```[pattern2](?=[pattern1])```|√|


    
    拿上面的代码举例：  

    ```python
    (?<=\s)([A-Za-z]*)(?=,)
    ```  

    匹配的内容其实就是：

    ```python
    \s([A-Za-z]*),
    ```  

    但 **如果匹配到了** 返回的 **部分** 是：  

    ```python
    [A-Za-z]*
    ``` 

    ![sweat-2021-11-02](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/sweat-2021-11-02.jpg)  

    咳咳，有点偏了，继续讲回来。**要匹配的正则表达式**在```(?<=[pattern])```后面，所以匹配的时候是**往后看的**，所以```(?<=[pattern])```就叫```look-behind```。  

    连起来看**look-behind requires fixed-width pattern**这个错误，意思就是```(?<=[pattern])```中的待匹配子模式```[pattern]```的**宽度一定要能确定**！  

    我们之前的写法```(?<=[pattern]*)```用了一个**元字符** ```*``` ，这个元字符代表前面的```[pattern]```会重复匹配 **0次或更多次** ，所以**宽度是不确定**的，由此导致了报错。  

    综上，在使用```(?<=[pattern]*)```时，请不要使用 ```?``` , ```*``` , ```+``` 这些导致**宽度不确定**的元字符。  

    |元字符|功能|
    |:---:|:---:|
    |?|	匹配前面的子模式**0次或1次**，或者指定前面的子模式进行**非贪婪匹配**|
    |*|匹配前面的子模式**0次或多次**|
    |+|匹配前面的子模式**1次或多次**|