# 【动画笔记】二分查找（折半查找）

进入大二以来一直把学数据结构和算法这事藏在心里，<del>毕竟大学破事太多了，</del>最近实在是按捺不住了。拿起《算法图解》，结合网络上的总结，准备循序渐进入门数据结构和算法...  

目前笔记里代码示例我都是用的**Python**语言（正好学校在教，趁热打铁），日后打C/C++基础后我应该会回来再增加示例。

![alright-2021-11-29](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/alright-2021-11-29.png)

## 二分查找的前提

1. 查找的**数据目标**需要是**有顺序的储存结构**，比如Python中的列表```list```。
2. 这个**数据目标**还需要**按一个顺序排列**（升序or降序）。

## 写写练练

>Don't try to understand it.Feel it.  

废话不多说了，关于二分查找，我最开始写了个**错误的玩意**：  

<a id='wrongcode1'></a>

```python
# 错误写法例子
my_list = [1, 3, 4, 5, 8, 16, 24, 56, 78]

def find(from_list, which_one):
    start = 0
    end = len(from_list)-1
    while start < end:
        middle_ind = (start+end)//2
        middle = from_list[middle_ind]
        print(start, end)
        if middle == which_one:
            return middle_ind
        elif middle > which_one:
            end = middle_ind  # 搜索范围尾部前移
        elif middle < which_one:
            start = middle_ind  # 搜索范围头部后移
    return False

found_ind = find(my_list, 78)
print(found_ind)

```

这个例子的问题体现在两个方面：  

1. 一旦寻找的是**顺序储存结构**中没有的值，会**卡在循环里**。

2. 一旦寻找的是**末尾**的边缘值，也会**卡在循环里**。  

3. 无论是奇数个数据还是偶数个数据都有这个问题。

从输出能看到原因：```start```和```end```满足不了```end == start```的条件，到最后两值会永远差```1```。  

我试着用动画展示一下这个**有问题的**执行过程：  

![whatswrong-2021-11-29](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/whatswrong-2021-11-29.gif)  

到最后死循环的时候因为我们将**中间值**采用了**向下取整**，导致当```start```指向倒数第二个索引时卡死在```start=7```。  

<a id='whatsWrong'></a>
通过观察，咱发现每次卡死的时候 **start和end永远会相差 1**。

![005-2021-11-30](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/005-2021-11-30.png)

很明显了，问题就出现在 **每次查找后对```start```和```end```的处理** 上。就[上面这个有问题的代码](#wrongcode1)而言，其实每次搜索后 **如果```中间项 < 搜索项目```**，进行```start=middle_ind+1```的操作就可以，列个表格： 

* 搜寻的值：```78``` 

|搜寻范围 (start,end)|middle_ind (中间项的索引)|start=middle_ind+1|end|循环继续|
|:---:|:---:|:---:|:---:|:---:|
|(0,8)|4|5|8|是|
|(5,8)|6 (向下取整)|7|8|是|
|(7,8)|7 (向下取整)|8|8|否|

*↑ 这样的话就能顺利达到查找循环的**终止**条件```start=end```了*

<a id='thinkThisWay'></a>

------

我的理解：**在本次搜索中**我们**已经检查了```middle_ind```对应的项目**，下一次搜索范围的开始(start)就应该从**这一项的下一位**开始，也就是```middle_ind+1```。

------


![091-2021-11-30](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/091-2021-11-30.png)  

但是吧，上面我们找的是**末尾的值```78```**，如果找的是开头的值 ```1``` 呢？  

照葫芦画瓢呗~依照[上面的思路](#thinkThisWay)，每次搜索后的**下一次搜索**就应该从**这一项的前一位**了，也就是```middle_ind-1```。（上面动画中能直观看出来**范围下限索引**```start```是趋于**增大**的，反之**范围上限索引**```end```值就是趋于**减小**的），再列个表：

* 搜寻的值：```1```  

|搜寻范围 (start,end)|middle_ind (中间项的索引)|start|end=middle_ind-1|循环继续|
|:---:|:---:|:---:|:---:|:---:|
|(0,8)|4|0|3|是|
|(0,3)|1 (向下取整)|0|0|否|

方便直观对照，我放个静态图在这里：  

![list-2021-11-30](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/list-2021-11-30.jpg)  

在寻找既不是开头也不是末尾的值时，搜索过程中往往会交替有```start```和```end```的**增**和**减**，所以在二分查找程序中关键部分就要**兼顾**上述两种处理：  

```python
if middle == which_one:
    return middle_ind
elif middle > which_one:
    end = middle_ind-1  # 搜索范围尾部前移，注意是middle_ind-1
elif middle < which_one:
    start = middle_ind+1  # 搜索范围头部后移，注意是middle_ind+1
```

（这也是为什么之前**寻找列表中没有的值时**会卡在循环里）  

经过这些处理后，咱成功弥补了[这个问题](#whatsWrong)。  

![NaNi-2021-11-30](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/NaNi-2021-11-30.png)  

等等...在搜索**列表中开头或末尾的值**时循环在```start=end```后就停止了...程序仍然**无法搜索到**开头或末尾的值，而是返回了```False```！  

通过观察发现，其实只要**再执行一次循环**，问题就完美解决了：把**循环条件**```start < end``` 换成 ```start <= end (start等于end时继续循环一次）```

最后写成的二分查找代码如下：  

```python
def find(from_list, which_one):
    start = 0  # 开始的索引
    end = len(from_list)-1  # 结尾的索引
    while start <= end:  # 当范围没有缩减至start>end时，不停二分查找（易错点：为什么用<=？因为当start=end的时候会遗漏一个处理项）
        middle_ind = (start+end)//2  # 找到二分中间项目的索引，这里向下取整(floordiv)
        middle = from_list[middle_ind]  # 获得中间项
        if middle == which_one:  # 找到了，返回对应的索引
            return middle_ind # 找到就停车跑路
        elif middle > which_one:  # 要寻找的值小于中间值
            end = middle_ind-1  # 将范围尾部索引减小到中间值索引-1（易错点）
        elif middle < which_one:  # 要寻找的值大于中间值
            start = middle_ind+1  # 将范围头部索引start增大到中间值索引+1（易错点）
    return False  # 啥都没找到
```

最后咱基于上面的写法整了几个动画：  

1. 寻找列表最开头的值： 

    ```python
    my_list = [1, 3, 4, 5, 8, 16, 24, 56, 78]
    found_ind = find(my_list, 1)
    ```
    
    ![findBeginning-remade-2021-12-01](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/findBeginning-remade-2021-12-01.gif) 

2. 寻找列表中间的一个值：  

    ```python
    my_list = [1, 3, 4, 5, 8, 16, 24, 56, 78]
    found_ind = find(my_list, 16)
    ```

    ![findMedian-remade-2021-12-01](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/findMedian-remade-2021-12-01.gif)

3. 寻找一个找不到的值：  

    ```python
    my_list = [1, 3, 4, 5, 8, 16, 24, 56, 78]
    found_ind = find(my_list, 6)
    ```

    ![cannotFind-2021-12-01](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/cannotFind-2021-12-01.gif)

## 时间复杂度  

通过大O表示法咱可以写成 ```O(f(n))``` 这样，其中：

* ```n```是操作的**数据的规模**。
* ```f(n)```是**操作的次数（程序执行的次数）**。  
* **大O**的量和```f(n)```成正比（这是不是说明看```O```其实就可以粗略地看```f(n)```？）。  

![043-2021-12-01](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/043-2021-12-01.png)

<a id="atMost"></a>

上面例子的列表中有**9**个元素，数据规模 ```n=9```，这些例子中**除了卡死循环**的情况外，我发现程序 **最多** 执行检查```4```次，而不是9次——

——因为每次执行操作后会将**搜索范围折半**，也就是每次操作后数据规模会**成半缩减**：  

![timeComplexity-2021-12-01](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/timeComplexity-2021-12-01.jpg)  

（字丑勿cue ┑(￣Д ￣)┍）

所以**二分查找**的时间复杂度表示为 ```O(log```<sub>2</sub>```n)```。

为什么说到[“最多”](#atMost)这个词呢？因为大O时间体现的是**最不理想情况下**的运行时间，也就是该算法的时间复杂度的**上界**。

## 更多的写法  

这篇笔记里的二分查找写法只是**所有写法中的一种**。我认为掌握一个算法并不是要对每种写法都了如指掌，而是**要去理解其中的原理**。

这里贴个知乎问题贴：

* [https://www.zhihu.com/question/36132386/answer/155438728](https://www.zhihu.com/question/36132386/answer/155438728)  

注意这个链接里的回答说 ```median = ( low + high ) / 2``` 写法会溢出是 **C/C++** 里的一个小坑。
