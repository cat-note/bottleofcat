# Python正则表达式细节小记
最近做Python课实验发现**正则表达式**和它在py中的的**标准库re**有很多能多琢磨一下的点，遂决定写成一篇小记，以后想复习能再来看看。  

![fuwafuwa-2021-11-02](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/fuwafuwa-2021-11-02.gif)  

## 名词  

因为不同文献书籍对正则表达式的描述有差别，我在这里列出一下本文用到的部分名词表述：  

|本小记中|其他说法|
|:---:|:---:|
|模式|表达式 / pattern|
|子模式|子表达式 / 子组 / subpattern|
|贪婪模式|贪心模式 / greedy mode|
|非贪婪模式|非贪心模式 / 懒惰模式 / lazy mode |
|非捕获组|non-capturing groups|
|向前查找|look-ahead|
|向后查找|look-behind|
|字符组|character class|

## 子模式扩展语法  

* ### look-behind语法问题  

    <details open>
    <summary>展开阅读</summary>

    ----

    这一节主要围绕<a href='#lookBehindIf'>```(?<=[pattern])```</a>和<a href='#lookBehindIfNot'>```(?<![pattern])```</a>两个子模式扩展语法展开。

    ```python
    s = 'Dr.David Jone,Ophthalmology,x2441 \
    Ms.Cindy Harriman,Registry,x6231 \
    Mr.Chester Addams,Mortuary,x6231 \
    Dr.Hawkeye Pierce,Surgery,x0986'
    pattern=re.compile(r'(?<=\s*)([A-Za-z]*)(?=,)')
    ```

    在这个例子中我原本是想寻找字符串中人名的姓氏的，但脑袋一热写了个```\s*```，跑了一下当即给我返回了错误:  

    ```re.error: look-behind requires fixed-width pattern```  

    我一会儿没反应过来，国内搜索引擎也没查到个大概。冷静下来后咱注意到了 **requires fixed-width pattern** 这一句，意思是需要**已知匹配长度**的模式（表达式），再看一眼前面的look-behind，突然咱就恍然大悟了：  

    ```python
    pattern=re.compile(r'(?<=\s)([A-Za-z]*)(?=,)')
    ```

    这样写就没问题了，我们匹配到了所有的姓氏：

    ```python
    print(pattern.findall(s))
    # ['Jone', 'Harriman', 'Addams', 'Pierce']
    ```    
    
    <a id='lookBehindIf'>问题出在哪呢？</a>  

    所谓```look-behind```其实就是```(?<=[pattern])```一类子模式扩展语法。

    * **注意分辨** ```(?<=[pattern])```和```(?=[pattern])```，**前者**是放在**待匹配正则表达式 之前**的，**后者**是放在**待匹配正则表达式 之后**的。

    * 这两个子模式扩展语法的功能是 **匹配[pattern]的内容**，但在结果中**并不会返回这个子模式**。
    
    * 我们通过表格来说明一下，功能是**如果匹配到了**即返回 **```[pattern2]``` 匹配** 的内容：  

        | 正则写法 | 正误 |
        |:---:|:----:|
        |```(?<=[pattern1])[pattern2](?=[pattern3])```|√|
        |```(?=[pattern1])[pattern2](?<=[pattern3])```|×|
        |```[pattern4](?<=[pattern1])[pattern2](?=[pattern3])```|√|
        |```[pattern4](?<=[pattern1])[pattern2](?=[pattern3])[pattern5]```|√|
        |```(?<=[pattern1])[pattern2]```|√|
        |```[pattern2](?=[pattern1])```|√|


    
    拿上面的模式（表达式）举例：  

    ```python
    (?<=\s)([A-Za-z]*)(?=,)
    ```  

    从匹配内容上来说该模式（表达式）其实就是：

    ```python
    \s([A-Za-z]*),
    ```  

    但 **如果该模式（表达式）匹配到了内容**，返回的 **部分** 是不包含```(?<=\s)```和```(?=,)```的匹配内容的：  

    ```python
    [A-Za-z]*
    ``` 

    ![sweat-2021-11-02](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/sweat-2021-11-02.jpg)  

    咳咳，有点偏了，继续讲回来。**要匹配的正则表达式**在```(?<=[pattern])```后面，所以匹配的时候是**往后看的**，所以```(?<=[pattern])```就叫```look-behind```。  

    连起来看**look-behind requires fixed-width pattern**这个错误，意思就是```(?<=[pattern])```中的待匹配子模式```[pattern]```的**宽度一定要能确定**！  

    我们之前的写法```(?<=[pattern]*)```用了一个**元字符** ```*``` ，这个元字符代表前面的```[pattern]```会重复匹配 **0次或更多次** ，所以**宽度是不确定**的，由此导致了报错。  

    -----

    <a id='lookBehindIfNot'>除此之外，</a>

    ```(?<![pattern]*)```也是look-behind子模式，所以也适用于**上面的情况**。

    * **同样注意分辨** ```(?<![pattern])```和```(?![pattern])```，**前者**是放在**待匹配正则表达式 之前**的，**后者**是放在**待匹配正则表达式 之后**的。

    * 这两个子模式扩展语法的功能是 如果**没出现[pattern]的内容**就匹配，但在结果中**并不会返回这个子模式**。


    **一句话总结**：综上，在使用```(?<=[pattern]*)```和```(?<![pattern])```时，在```[pattern]```里请不要使用 ```?``` , ```*``` , ```+``` 这些导致**宽度不确定**的元字符。  

    |元字符|功能|
    |:---:|:---:|
    |?|	匹配前面的子模式**0次或1次**，或者指定前面的子模式进行**非贪婪匹配**|
    |*|匹配前面的子模式**0次或多次**|
    |+|匹配前面的子模式**1次或多次**|

    ![speechless-2021-11-02](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/speechless-2021-11-02.jpg)  

    要好好记住哦~
    </details>

* ### 非捕获组和look-ahead,look-behind的区别  

    <details>
    <summary>展开阅读</summary>

    ------

    在子模式扩展语法中非捕获组(non-capturing group)写作```(?:[pattern])```，look-ahead是向前查找，look-behind是向后查找，我们列张表：

    |英文术语|中文术语|模式|
    |:---:|:---:|:---:|
    |正向向后查找|positive look-behind|```(?<=)```|
    |正向向前查找|positive look-ahead|```(?=)```|
    |负向向后查找|negative look-behind|```(?<!)```|
    |负向向前查找|negative look-ahead|```(?!)```|

    **正向**和**负向**指的分别是 ```出现则匹配``` 和 ```不出现则匹配```。

    在上面一节里我们已经谈了一下```look-ahead```和```look-behind```，现在又出现个非捕获组。  

    非捕获组```(?:[pattern])```的功能是匹配```[pattern]```，但不会记录这个组，整个例子看看：  

    ```python
    import re
    s = 'Cake is better than potato'
    pattern = re.compile(r'(?:is\s)better(\sthan)')
    print(pattern.search(s).group(0))
    # is better than
    print(pattern.search(s).group(1))
    # than
    ```
    
    ```Match对象```的```group(num/name)```方法返回的是对应组的内容，子模式序号从**1**开始。```group(0)```返回的是**整个模式**的匹配内容（is better than），而```group(1)```返回的是**第1个子模式**的内容（than）。  

    这里可以发现第1个子模式对应的是```(\sthan)```而不是```(?:is\s)```，也就是说```(?:is\s)```这个组**未被捕获**（没有被记录）  

    问题来了，positive look-ahead（正向向前查找）```(?=[pattern])``` 和 positive look-behind（正向向后查找）```(?<=[pattern])``` 是 **出现[pattern]则匹配，但并不返回该子模式匹配的内容**，它们和```(?:[pattern])```有什么区别呢？  

    拿下面这段代码的执行结果来列表：  

    ```python
    import re
    s = 'Cake is better than potato'
    pattern = re.compile(r'(?:is\s)better(\sthan)')
    pattern2 = re.compile(r'(?<=is\s)better(\sthan)')
    ```

    |子模式扩展语法|pattern.group(0)|pattern.group(1)|
    |:---:|:---:|:---:|
    |(?:[subpattern])|is better than| 空格than|
    |(?<=[subpattern])|better than| 空格than|

    ![idontunderstand-2021-11-03](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/idontunderstand-2021-11-03.jpg)

    根据上面的结果总结一下：  

    1. ```(?<=[pattern])```和```(?=[pattern])```是匹配到了[pattern]**不会返回、亦不会记录（捕获）[pattern]子模式**，所以在上面例子中整个模式的匹配结果中没有 ```is空格```。

    2. ```(?:[pattern])```是匹配到了[pattern]**会返回，但不会记录（捕获）[pattern]子模式**，所以在上面例子中整个的匹配结果中有 ```is空格```。

    3. ```(?:[pattern])```，```(?<=[pattern])```，```(?=[pattern])``` 的共同点是 **都不会记录[pattern]子模式（子组）**，所以上面例子中```group(1)```找到的**第1个组**的内容是```(\sthan)```匹配到的```空格than```。

    </details>


## 基本语法相关  

* ### 非贪婪模式  

    <details>
    <summary>展开阅读</summary>

    ------

    要实现找出**字符串中人名姓氏和对应的电话分机码**，我会这样写：  

    ```python
    import re
    s = 'Dr.David Jone,Ophthalmology,x2441 \
    Ms.Cindy Harriman,Registry,x6231 \
    Mr.Chester Addams,Mortuary,x6231 \
    Dr.Hawkeye Pierce,Surgery,x0986'
    pattern = re.compile(r'(?<=\s)([A-Za-z]*)(?=,).*?(?<=x)(\d{4})')
    print(pattern.findall(s))
    # [('Jone', '2441'), ('Harriman', '6231'), ('Addams', '6231'), ('Pierce', '0986')]
    ```

    主要思路是**前面的模式**根据空格和逗号先匹配到姓，**后面的模式**通过x开头和```\d{4}```匹配到四位电话分机码。

    **前面和后面**的模式之间我最开始写的是```.*```，```*```元字符会将```.```的匹配重复0次或多次，然后我们就得到了这样的匹配结果：```[('Jone', '0986')]```（直接一步到位了喂！(#`O′)  

    元字符表我好歹还是看了几次的，能制止这种贪婪匹配的符号就是```?```了，但因为我记得```?```非贪婪的表现是```匹配尽可能短的字符串```，再想了一下```*```元字符重复匹配次数最少不是0次嘛！那这问号可不能加在```.*```后面了！  

    然后我就试了下面几种：

    ```
    (?<=\s)([A-Za-z]*)(?=,).*(?<=x)(\d{4})?
    (?<=\s)([A-Za-z]*)(?=,).*(?<=x)?(\d{4})?
    (?<=\s)([A-Za-z]*)(?=,).*(?<=x)?(\d{4})
    (?<=\s)([A-Za-z]*)(?=,).*(?<=x)(\d{4})\s
    (?<=\s)([A-Za-z]*)(?=,).*(?<=x)(\d{4})?\s
    ```

    当然这些模式匹配的结果都没能如我愿，实在忍不住了，我还是把中间部分改成了```.*?```，然后就成了！  

    ![wahhhhh-2021-11-03](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/wahhhhh-2021-11-03.jpg)  

    ```
    (?<=\s)([A-Za-z]*)(?=,).*?(?<=x)(\d{4})
    ```

    想了一下，原来所谓的 **匹配尽可能短**的字符串 **并不是**从元字符的功能角度上去说的。  

    就```2between1and3```这个字符串来说：

    * 如果我单独写一个```.*?```进行匹配，就会**匹配个寂寞**，  

    * 但如果我在两边加上限定：```\d+.*?\d+```（```.*?```匹配的内容必须在数字包夹之中），
    
    * 若为```.*```贪婪模式，匹配结果会是```between1and```，但正因为是```.*?非贪婪模式```，匹配的是 **结果字符串宽度更小** 的部分 ```between```。

    综上，非贪婪指的是在 **符合当前模式的情况下** 使得最终匹配结果 **尽可能地短**。 
    
    在使用非贪婪模式```?```符号时要考虑 **语境** ，结合上下文去设计功能。

    </details>

* ### 中括号中的元字符  

    <details>
    <summary>展开阅读</summary>

    ------

    写这一节是因为Python课老师说中括号[]里的元字符都只是被当作普通字符来看待了，然鹅，在做实验的时候我发现并不是这样。(・ε・｀)    

    看看这个**匹配单个Python标识符**的正则表达式：  

    ```python
    ^\D[\w]*
    # Python标识符开头不能是数字
    ```

    这个模式能顺利匹配```hello_world2```，```_hey_there```这一类字符串。等等，这样的话不就代表```\w```这种元字符可以在```[]```中用了嘛！  

    我们再试试这些：
    ```python
    ^\D[z\wza]* # 仍然可以匹配标识符，\w真的起了作用
    ^\D[z\dza]* # 可以匹配 hz2333a，\d也起了作用
    ^\D[z\nza]* # 可以匹配到带换行符的 hz\naaa，\n也起了作用
    ```

    很容易能发现```\w```，```\s```，```\n```，```\v```，```\t```，```\r```一类元字符其实都是可以在中括号```[]```中正常发挥 **元字符的作用** 的，其他还有```\b```等元字符。在**中括号中**使用他们无非是 **有没有意义** 的问题，Python并不会报错。  

    ![thinking-2021-11-03](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/thinking-2021-11-03.jpg)

    那么再试试这些吧：  

    ```python
    ^\D[\w+]* # 能匹配到 hello+world  
    ^\D[\w+*]* # 能匹配到 hello+world*2
    ^\D[\w+*?]* # 能匹配到 hello+wo?rld*2
    ^\D[(\w+*)]* # 能匹配到 hello+(world)*2
    ^\D[(\w{1,3}+*)]* # 能匹配到 hello+(world)*2,{1,3}  
    ^\D[\w$]* # 能匹配到 hello$world
    ^\D[\(\w\*\?\\)\$]* # 能匹配到hello$wor\ld*?  
    ```

    到了这里，我发现老师说的在```[]```中**被当作普通字符**的元字符只是一部分罢了，主要是 ```*```，```?```，```+```，```{}```，```()```，```$``` 这些元字符。  

    从上面的例子可以看出来，中括号里这些元字符相当于： ```\*```，```\?```，```\+```，```\{\}```，```\(\)```，```\$``` 

    **适用于**中括号```[]```的元字符主要有两个：```^``` 逆向符，```-``` 范围指定符，比如：

    ```
    [^a-z]
    ```  

    匹配的就是a-z小写字母集**之外**的随意一个字符。

    **总结**一下：

    1. ```\w```，```\s```，```\n```，```\v```，```\t```，```\r```，... **一类**元字符与其相反意义（例如```\w```对```\W```）的元字符是完全可以使用在```[]```中的，无非是有没有意义的问题。  

    2.  ```*```，```?```，```+```，```{}```，```()```，```$``` ，... **一类**其他符号元字符也可以使用在```[]```中，全被当作 **普通字符** 对待。  

    3. 中括号里用上述的元字符Python都**不会报错**，请放心~₍₍٩( ᐛ )۶₎₎  

    </details>

* ### 子模式引用方法\num  

    <details>
    <summary>展开阅读</summary>

    -------

    教材上列子模式功能时提了一下```\num```这个用法，但真的只是提了一下：

    > 此处的num是指一个表示子模式序号的正整数。例如，"(.)\1"匹配两个连续的相同字符  

    ![whattheheck-2021-11-03](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/whattheheck-2021-11-03.jpg)  

    刚开始我是真没懂这是啥意思，以为是重复引用前面的子模式：  

    ```python
    (\d)[A-Za-z_]+\1
    ```

    我试过用这个模式去对```12hello3```这个字符串进行匹配，然后返回了个寂寞...  

    什么gui，这里的```\1```难道不是重复```(\d)```再匹配个数字吗？  

    随后我改了一下待匹配字符串，就有结果了：  

    | 待匹配Str | 匹配结果 |
    |:---:|:---:|
    |12hello3| None|
    |12hello1|12hello1|
    |12hello2|2hello2|

    好家伙，原来```\num```引用的 **不是子模式本身**，而是 **已知子模式的匹配结果**  

    上面的例子中```(\d)```是**第1个**子模式，匹配结果如果是 **2**，那么后面```\1```的地方也一定要是 **2** 才会进行匹配，我们再来几个例子：  

    ```python
    (\d)(\d)[A-Za-z_]+\2\1 # 能匹配到 34hello43
    (\d)(\d)[A-Za-z_]+\1world\2 # 能匹配到 34hello3world4
    (\d)(\d)[A-Za-z_]+\1*world\2 # 能匹配到 34hello33333world4  
    ```  

    简单总结：

    1. ```\num``` 引用的是**对应的子模式匹配的结果**，注意这里只能是子模式的序号。  

    2. 子模式的序号 **从1开始**。

    3. 如果你需要**引用子模式**，可以用子模式扩展语法```(?<子模式名>)```和```(?=子模式名)```，例如：   

        ```python
        import re
        s = '34hello33333world4'
        pattern = re.compile(r'(?P<f>\d)(\d)[A-Za-z_]+(?P=f)*world\2')
        print(pattern.match(s).group(0))
        # 能匹配到 34hello33333world4
        ```

    4. 在中括号```[]```中```\num```是没有效果的（和上一节来一波联动）。  

    </details>

## re模块修饰符

* ### 如何同时使用多个flags    

    <details>
    <summary>展开阅读</summary>

    -------

    像```re.compile```，```re.search```，```re.match```，```re.findall```这几个函数都允许修饰符flags作为参数，我们拿```re.compile```举例：  

    ```python
    import re
    s='''Hello line1
    hello line2
    hello line3
    '''
    pattern=re.compile('^hElLo',re.I)
    print(pattern.findall(s))
    ```

    这不得劲啊！我想进行```多行匹配```又想保证```忽略大小写```怎么办？(￣▽￣)"  

    彳亍，那就这样写！  

    ```python
    pattern=re.compile('^hElLo',re.I | re.M)
    ```

    这里的 ```|``` 可以称作一个 ```管道符(似乎是Shell里的叫法)```。名字啥的倒无所谓了，使用了这个符号我们就能使用多个标志啦！（虽然通常情况下不会使用超过两个）  

    我口味刁钻，我偏不用 ```|``` 符，哼！(￢︿￢)

    好啊，没问题啊！那我们先去子模式买点**扩展语法**!  

    ![noproblemo-2021-11-03](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/noproblemo-2021-11-03.jpg)  

    在Python里还有个子模式扩展语法可以给**整个模块应用**多个修饰符(flags)，它就是 ```(?修饰符们)```：  

    ```python
    pattern=re.compile('(?im)^hElLo') # i->忽略大小写，m->多行匹配
    pattern=re.compile('(?sm)^hElLo') # s->换行符识别，m->多行匹配
    ```

    值得注意的是这个子模式扩展语法请最好放在 **整个模式的最前面**，不然Python会报“不建议”警告：```DeprecationWarning: Flags not at the start of the expression```.  

    </details>

* ### 常用的几个修饰符  

    <details>
    <summary>展开阅读</summary>

    -------

    |修饰符|功能|
    |:---:|:---:|
    |re.S|让元字符```.```支持换行符```\n```|
    |re.M|对多行进行匹配，对元字符```^```和```$```有影响|
    |re.I|匹配时忽略大小写|
    |re.X|允许模式中有空格和多行，方便阅读|

    注：Python3里面**没有re.U**。

    在举例之前先来个记忆方法：

    * ```re.S```和元字符```.```有关，可以背```.S```，扩写成单词背成```DOT SEARCH```，代表这个匹配和点元字符有关。  

    * ```re.I```是忽略大小写，直接字面意思背成```IGNORE CASE```即可。

    * ```re.M```是多行匹配，也可以直接字面意思背成```MULTILINE```。

    * ```re.X```嘛...想不到了，就死背吧(ノへ￣、)  

    先从```re.I```开始，这一个其实就是让模式**忽略大小写去进行匹配**：  

    ```python
    import re
    s='''Hello line1
    hello line2
    hello line3
    '''
    pattern=re.compile('hElLo')
    print(pattern.findall(s)) # []
    pattern2=re.compile('hElLo',re.I)
    print(pattern2.findall(s)) # ['Hello', 'hello', 'hello']  
    ```

    -----

    ```re.M```的话主要影响了两个元字符的匹配：```^```开头匹配和```$```尾部匹配  

    普通情况下，```^```匹配整个字符串的开头，而```$```匹配的是 **单行字符串的末尾** 或者 **多行字符串中最后一行的结尾**。  

    但使用了```re.M```后，对于**多行字符串**来说，```^```不仅匹配了字符串的开头，还 **匹配了每一行的开头**；而```$```也匹配了 **每一行的结尾和字符串的结尾**，接下来举几个例子：  

    ![makeexample-2021-11-03](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/makeexample-2021-11-03.jpg)  

    ```python
    import re
    s='''Hello line1
    hello line2
    hello line3
    '''
    print( re.findall('^hElLo\slINe\d',s,re.I) )
    # ['Hello line1']
    print( re.findall('hElLo\slINe\d$',s,re.I) )
    # ['hello line3']
    print( re.findall('^hElLo\slINe\d$',s,re.I) )
    # []

    print( re.findall('^hElLo\slINe\d',s,re.I | re.M) )
    # ['Hello line1', 'hello line2', 'hello line3']
    print( re.findall('hElLo\slINe\d$',s,re.I | re.M) )
    # ['Hello line1', 'hello line2', 'hello line3']
    print( re.findall('^hElLo\slINe\d$',s,re.I | re.M) )
    # ['Hello line1', 'hello line2', 'hello line3']
    ```

    ------

    **默认情况**下元字符 ```.``` 只能匹配**除换行符\n以外**的任意字符。

    而```re.S```让元字符 ```.``` 能匹配包括换行符```\n```在内的 **所有字符**！

    例子：  

    ```python
    import re
    s='''Hello line1
    hello line2
    hello line3
    '''
    print( re.findall('line(.*)hello',s) )
    # []
    print( re.findall('line(.*)hello',s,re.S) )
    # ['1\nhello line2\n']
    print( re.findall('line(.*?)hello',s,re.S) )
    # ['1\n', '2\n']
    ```

    ------

    ```re.X```是一个**能增加正则表达式可读性**的修饰符，让写正则变得更优雅~ ヽ(✿ﾟ▽ﾟ)ノ  

    ![elegant-2021-11-03](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/elegant-2021-11-03.jpg)  

    我们先直接上例子：  

    ```python
    import re
    s = 'Dr.David Jone,Ophthalmology,x2441 \
    Ms.Cindy Harriman,Registry,x6231 \
    Mr.Chester Addams,Mortuary,x6231 \
    Dr.Hawkeye Pierce,Surgery,x0986'
    pattern = re.compile(r'(?<=\s)([A-Za-z]*)(?=,).*?(?<=x)(\d{4})')
    print(pattern.findall(s))
    ```  

    正则越复杂，在单行里的可读性就越差，这不彳亍，我们要优雅！(￣_,￣ )，于是可以这样写：  

    ```python
    pattern = re.compile(r'''
    (?<=\s) # 根据空格匹配姓氏大概位置  
    ([A-Za-z]*) # 姓氏是由英文字母组成的
    (?=,) # 姓氏后面有个逗号  
    .*? # 匹配姓氏和电话分机号之间的内容
    (?<=x) # 找到电话分机号共同前缀x
    (\d{4}) # 电话分机号一律是4位
    ''', re.X)
    ```

    就差一个红酒杯🍷了有木有，优雅多了！可读性大幅增加o(*≧▽≦)ツ  

    由上面的例子可以看出，```re.X```忽略了多行模式中的**空白、换行和#等字符**。

    这里放一段官方文档对于```re.X```的描述：  

    >  Whitespace within the pattern is ignored, except when in a character class, or when preceded by an unescaped backslash, or within tokens like *?, (?: or (?P<...>. When a line contains a # that is not in a character class and is not preceded by an unescaped backslash, all characters from the leftmost such # through the end of the line are ignored.  

    也就是说空格的忽略也有**例外**：

    1. 当空格在字符组(character class)，也就是中括号```[]```里的时候，**不会**被忽略。  

        ```python
        import re
        s = '''Dr.David Jone,Ophthalmology,x2441 
        Ms.Cindy Harriman,Registry,x6231 
        Mr.Chester Addams,Mortuary,x6231 
        Dr.Hawkeye Pierce,Surgery,x0986'''
        # 我们用 不会忽略中括号内的空格 这个特性来匹配上面字符串中的人名，如Dr.David Jone

        print(re.findall(r'''
        ^[a-zA-Z.]*?
        [\w]* # 中括号里没有空格
        (?=,) 
        ''', s, re.X | re.M))
        # 一个都匹配不上

        print(re.findall(r'''
        ^[a-zA-Z.]*?
        [ \w]* # 中括号里有空格
        (?=,) 
        ''', s, re.X | re.M))
        # 能够匹配上:['Dr.David Jone', 'Ms.Cindy Harriman', 'Mr.Chester Addams', 'Dr.Hawkeye Pierce']
        ```

    2. 当模式中的**空格**前面有**转义斜杠```\```**，这个空格**不会**被忽略。

        ```python
        import re
        s = '''Dr.David Jone,Ophthalmology,x2441 
        Ms.Cindy Harriman,Registry,x6231 
        Mr.Chester Addams,Mortuary,x6231 
        Dr.Hawkeye Pierce,Surgery,x0986'''
        # 我们用 不会忽略中括号内的空格 这个特性来匹配上面字符串中的人名，如Dr.David Jone
        print(re.findall(r'''
        ^[a-zA-Z.]*?
        # 这儿只有个普通的空格
        [\w]* 
        (?=,) 
        ''', s, re.X | re.M))
        # 一个都匹配不上

        print(re.findall(r'''
        ^[a-zA-Z.]*?
        \ # 这儿有个被转义的空格
        [\w]* 
        (?=,) 
        ''', s, re.X | re.M))
        # 匹配上了：['Dr.David Jone', 'Ms.Cindy Harriman', 'Mr.Chester Addams', 'Dr.Hawkeye Pierce']
        ```

    3. 当空格在```*?```，```(?:```，```(?P<...>```这种语法里时，**不会**被忽略。经过测试，我觉得这一条和上一条转义不会被忽略其实是一个道理（官方文档也没写的很详细）。测试中，这样写不会被忽略：  

        ```
        \ *?
        (?:\ )
        (?P<...>\ )
        ```

        很明显能发现实际上还是 **空格转义**，当然也有可能是我理解错了。
        
    不管怎样，这样匹配空格的方法在实际操作中肯定是 **非常少用** 的，别人读这样的正则表达式时一眼望去还**真难发现哪个角落有没有个空格** (#`O′)  

    对于```#```注释符而言情况就要简单多了，在**模式中**只有两种情况```#```**不会**被忽略：  
    1. ```#```存在于字符组(character class)，也就是中括号```[]```里的时候。

    2. ```#```被反斜杠```\```转义。  

    </details>

## After All

正则表达式并不是什么时候都用得上的，尤其是很多时候**正则的效率相对于字符串处理**还真不算好。

但是，在字符串处理写起来非常繁琐的情况下，正则的确也帮我们节省了不少时间，提升了工作效率。  

在咱看来，**正则表达式**和**SQL语句**有一个共性，就是其在其所处的体系中是通式般的存在：正则表达式几乎可以在所有编程语言中进行使用，而SQL语句也可以在标准化关系数据库管理系统中进行使用。  

![gaitubao_Fjmtb33KUZe-9H0u05lo8I35ilZT-2021-11-03](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/gaitubao_Fjmtb33KUZe-9H0u05lo8I35ilZT-2021-11-03.jpg)

本人文笔不佳，写的可能有点粗糙。希望这篇小记对大家掌握正则表达式有一定的帮助，感谢各位的耐心阅读。(　ﾟ∀ﾟ) ﾉ♡

如果后面我在学习**Python正则表达式**的时候有了新的可记录的点，我会继续更新在这篇文章中。

To be continued...