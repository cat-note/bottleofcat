## JS小记：闭...闭包？  
博客链接：https://bottle.moe/post-274.html   

--------------
最近在重打JavaScript基础...看廖雪峰博客的基础教程时看到了闭包这个字眼，但有点意思的是文章并没有更进一步去探讨闭包到底是什么，而是列出代码让大家去体会。  

但既然是打基础，我觉着有疑惑的概念还是得摸清楚为好，遂决定写这样一篇小笔记。有可能会有理解错误的地方，各位请多指教。  

![figureitout-2021-11-07](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/figureitout-2021-11-07.jpeg)  

## 0x01 我觉得闭包是...  

闭包(Closure)的话其实一个简单的代码示例就能展示出来：  

```javascript
function test() {
    var testvar = 1;
    function innerTest() {
        console.log(testvar);
    }
    innerTest();
}
test();
```

**testvar**是**test()**函数作用域里的一个**局部变量**，而**innerTest()**这个函数可以访问到testvar这个**test()作用域内的局部变量**。  

也就是说**testvar**这个变量对于**innerTest()**来说是外部的变量，而innerTest这个函数内部却又能**访问到**这个变量，由此，**innerTest**这个函数和其所访问的这个**testvar**变量便**形成**了一个闭包，innerTest就是一个**闭包函数**。   

说起**形成**，我觉得闭包其实有点偏向一个**动词**，指的是**一个函数访问另外一个函数的_函数作用域_中的局部变量的过程**。 


之所以中文翻译里有个**“闭”**，我认为是因为这样把变量给“闭”进去了，除非调用**能访问到这个变量的函数**，没有其他办法能改变这个变量，比如下面这个例子：    

```javascript
function test() {
    var testvar = 1;
    function innerTest() {
        console.log(testvar);
        testvar++;
    }
    return innerTest;
}
var inner = test();
inner();  //1
inner();  //2
inner();  //3
inner();  //4
```

除了调用test函数返回的作用域中的函数innerTest外，无法对testvar这个局部变量进行修改。  

关于这个例子我整了个小动图（做的比较仓促）：  

![closure-2021-11-07](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/closure-2021-11-07.gif)

了解到这些我再回头看了一下，发现本博客程序-O-[初始化源码](https://github.com/SomeBottle/-O-/blob/master/jcs/i.js#L106)中也是有用到闭包的：  

![blogcode-2021-11-07](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/blogcode-2021-11-07.jpg)

也就是说其实在很多不经意的时候我已经在写JavaScript代码时使用了所谓的“闭包”。为什么会这样呢？其实更进一步想，在看这些函数和变量的过程中我们脑中总是会模拟**变量作用的范围**。没错，更为重要的是**JS的作用域**相关的知识！  

网上搜索了一下，闭包从来不是某个语言独有的概念，不如说是一种现象的统称。既然我在重摸JavaScript基础，其实打好作用域的知识便也足够了~  

## 0x02 “脑洞大开”一下  

廖雪峰的这篇[闭包相关的JavaScript教程](https://www.liaoxuefeng.com/wiki/1022910821149312/1023021250770016)还提到了个有趣的脑洞：只使用函数来完成简单的计算。  

第一眼看上去心生疑惑，这算出来是什么单位呢...看到后面，原来是函数的嵌套重复次数。我把参数名改的详细了点，试试看能不能增加点可读性：  

```javascript
// 定义数字0:
var zero = function (func) {
    return function (x) {
        return x;
    }
};

// 定义数字1:
var one = function (func) {
    return function (x) {
        return func(x);
    }
};

// 定义加法:
function add(para1, para2) {
    return function (func) {
        return function (x) {
            return para1(func)(para2(func)(x));
        }
    }
}
```  

这三个函数里后两个都非常典型地形成了闭包，因为**zero函数**内部的函数没有调用局部变量（参数func），所以zero内未形成闭包，由此通过参数传入的函数不会被执行。  

暂且当这里的x参数是为了增加可读性的<del>（不过我是真的给这个x看花眼了）</del>，再改一下：  

```javascript
var zero = function (func) {
    return function () {
        return '';
    }
};

// 定义数字1:
var one = function (func) {
    return function () {
        return func();
    }
};

// 定义加法:
function add(para1, para2) {
    return function (func) {
        return function () {
            return para1(func)(para2(func)());
        }
    }
}
```

很明显能发现，add函数内部嵌套的第一层匿名函数往后**function(func)**其实就是zero,one这类函数的构造模式。接下来咱试着算一下**"1"+"1"**  

```javascript
var two = add(one, one);
```

实际上就等价于：  

```javascript
var two = function (func) {
    var exec_one = one(func);
    return function () {
        return exec_one(exec_one());
    }
};
var exec_two = two(function () {
    console.log('print 2 times');
});
exec_two(); //输出两次  
```

所谓的输出两次其实就是把one函数再次执行嵌套了一次。可想而知如果再算一下 **"1"+"2"** ，得出的three函数核心代码将会是```exec_one(exec_one(exec_one()));```，<del>俗称套娃</del>    

如果把x给还原进去，就相当于从零开始不停加1：  

```javascript
var exec_two = two(function (val) {
    return val + 1;
});
console.log(exec_two(0));  //输出2
```

虽然但是，读清楚这段代码真的耗了我好长时间...看来用了太多return和闭包的结果就是代码可读性会降低，所以凡事要适度嘛...  

![yeah-2021-11-07](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/yeah-2021-11-07.jpeg)  

最后要感叹一下，我竟然把这篇博文写出来了，也是第一次尝试写这种小笔记~今后随着学习的感悟变多，我有可能会持续更新下去~加油吧！不要停滞不前！    

橘佬对此还有一点补充： https://github.com/cat-note/bottleofcat/issues/1  
