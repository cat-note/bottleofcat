# 【动画笔记】辗转相除法——求最大公约数和最小公倍数  

最近咱摸起了C语言，尝试着结合最近学的运筹学写个计算工具，途中遇到了一个需求：分数的约分。  

* 分数约分怎样一步到位呢？答案便是找分母和分子的**最大公约数**。  

* 那么怎么尽快算出最大公约数呢？网上查了一查，发现了一个算法：**辗转相除法**。  

这篇笔记就简单而直观地记录一下这个算法。  

![ehe-2022-05-01](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/ehe-2022-05-01.png)  

## 最大公约数  

这个词非常贴近分数中**约分**的方法，所谓最大公约数即是**多个整数共有的约数**中**最大**的一个，在约分的时候分子和分母**同时除以最大公约数**，能得到**最简分数**。  

因为上面说的过程中进行的都是**整除运算**，所以最大公约数也称为**最大公因数**。  

不妨说得更直接一点，**公因数就是公约数**。  

> 注：公因数/公约数是针对**整数**而言的。

## 手算 

现实中在求多个整数的最大公约数时，可以把这些数的因数都列举出来找共有的因数，亦或可以使用短除法：  

![shortDivision-2022-05-01](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/shortDivision-2022-05-01.jpg)  

我觉得这些方法其实多少有些依赖我们以往的经验，比如看到`36,405,72`中有`6,5,2`，可能很快能想到公因数`3`。（感觉我大脑里此时枚举了所有可能的因数）  

这个过程如果抽象成编程语言中的算法，具体以代码实现，大概就是`一个循环`+`判断所有数除以因数是否余数都为0`了，时间复杂度是`O(n)`，随着运算数值的增大，代码的执行次数也会线性增加。  

为了优化时间复杂度，这个时候就到了这篇笔记的主角——**辗转相除法**了。  

## 辗转相除法  

上面提到了求多个整数的最大公约数，在这之前得先看看针对**两个整数**的二元算法👇 

### 公式

辗转相除法也被称为**欧几里得算法**(Euclidean Algorithm)，怎么“辗转”的呢？先上公式看看：  

![Formula](https://render.githubusercontent.com/render/math?math=\LARGE\color{Gray}%20\gcd(A,B)%20=%20\gcd%20(B,A%20\bmod%20B))  

> `A` 代表**被除数**(Dividend)；  
> `B` 代表**除数**(Divisor)；  
> `A mod B` 代表 A **取模** B，也就是`A除以B后的余数`，在C语言里的表达式可以写成`A % B`。

这里的`gcd`代表的是`Greatest Common Divisor`，也就是`最大公约数`。此公式的含义是：  

* `[被除数] 和 [除数] 的最大公约数` = `[除数] 和 [余数] 的最大公约数`  

这便是整个“**辗转**”过程的核心，具体证明这里就不多赘述，可以看看文末的[相关文章](#相关文章)。  

-----

### 辗转

下面演示一下这个“**辗转**”过程，计算一下`36`和`405`的最大公因数：  

![iteration-2022-05-02](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/iteration-2022-05-02.gif)  


> 所谓的**辗转**实际上指的是一个**迭代运算**的过程，迭代运算在编程语言里的体现其实就是重复执行一段代码，在每一次执行过程中**以上一次执行运算**的结果值为基础，运算出新值后**代入下一次运算**，不断用**旧值推算新值**，直至**达到终止条件为止**。  

辗转相除法过程的抽象描述如下：  

1. 初始化：**两个整数**之中的`较大值A`作为**被除数**，`较小值B`作为**除数**。

2. 运算：对 `A` 和 `B` 进行 **取模（取余数）运算**，得到`余数C`。  

3. 迭代：将原本的除数`B`作为**被除数**，余数`C`作为**除数**，重复第2步的运算。

4. 迭代终止：当某次运算中`余数`为0时，无法继续进行运算，算法结束，最后一次运算的**除数**就是最初两个整数的**最大公约数**。  

### 直观化理解  

刚开始我想了好一会儿也没想到这种辗转相除怎么理解，直到查了资料才了解到了古人的智慧方法——几何化表达。  

可以这样想，两个整数其实可以看作**两个维度**，可以在一个二维空间中用一个图形表达出来。显而易见，最适合的图形便是**长方形**了，长方形对边相等，拥有**长和宽**两个参数，正好对应了两个整数。  

那么除的过程呢？实际上在**长方形**的长和宽都是**整数**的情况下（长宽比为有理数），是肯定能被**有限个正方形**填充满的。除的过程实际上就是**往长方形中塞入尽可能大的正方形**。

------

下面直观展示一下辗转相除法求`100`和`245`的最大公约数： 

![demonstration1-2022-05-02](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/demonstration1-2022-05-02.gif)  

👆 首次运算，以短边100为长构造正方形填入（尽可能大的正方形），可见**剩余下来的长方形**长宽比为`100:45`，正好对应了下一次运算的**被除数**和**除数**。  

![demonstration2-2022-05-02](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/demonstration2-2022-05-02.gif)  

👆 第一次迭代，以短边45为边长构造正方形填入，剩余下来长方形区域为`45:10`，对应下一次迭代的**被除数**和**除数**。  

![demonstration3-2022-05-02](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/demonstration3-2022-05-02.gif)  

👆 第二次迭代，以短边10为边长构造正方形填入，剩余下来长方形区域为`10:5`。  

![demonstration4-2022-05-02](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/demonstration4-2022-05-02.gif)  

👆 第三次迭代，以短边5为边长构造正方形填入，**正好填满了剩余的长方形区域**，算法结束，最初两整数的最大公约数为`5`。  

* 即一定数量的`5×5`的正方形**正好能填充满**`100×245`的长方形（正方形的边`5`正好能整除`100`和`245`）。

由此可见，整个算法的过程直观体现出来就是每一次都在**剩余的长方形空间**中塞入**尽可能大的正方形**，直至**正方形正好能填充满剩余的长方形空间**。  
（这也是为什么首次运算要用较大的值除以较小的值）  

### 具体实现  

这里用C语言实现辗转相除算法：  

```c
long int GCD(long int num1, long int num2) {
    // 寻找两数最大公约数(欧几里得算法)
    // 公式 GCD(被除数,除数)=GCD(除数,余数)
    long int dividend; // 被除数
    long int divisor; // 除数
    long int remainder; // 余数

    if (labs(num1) > labs(num2)) { // 绝对值大的作为被除数
        dividend = num1;
        divisor = num2;
    } else {
        dividend = num2;
        divisor = num1;
    }

    do {
        remainder = dividend % divisor;
        dividend = divisor; // 把被除数换成除数
        if (remainder) { // 余数不为0，就把除数换成余数
            divisor = remainder;
        }
    } while (remainder != 0);

    return divisor;
}
```

### 时间复杂度

观察发现，该算法的核心操作是“**取模**”(Modulo)。整个算法可能依赖于多次迭代，每次迭代都会进行取模操作，而每次取模操作后留给下次运算的数据量相对来说都会减少**超过一半**。  

------

为什么是减少超过一半呢？因为每次除法运算中**余数**的绝对值一定**小于被除数绝对值的一半**：```|余数| < 0.5 |被除数|```  

下面用文字解释了一下（这张图中所有数都是**正整数**）：  

![remainderLTHalfOfDividend-2022-05-02](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/remainderLTHalfOfDividend-2022-05-02.jpg)  

对于有负数参与的除法运算也是可以同理推证的。  

-------

话说回来，提到**数据减半**，我不由得就想到了二分查找。二分查找每一次迭代只用处理上一次运算中的**一半**数据，这样下来能推算出其时间复杂度是`O(log2(n))`级别（我之前尝试着[推算](https://www.cnblogs.com/somebottle/p/algo_binary_search.html#%E6%97%B6%E9%97%B4%E5%A4%8D%E6%9D%82%E5%BA%A6)过这个）。  

回到**辗转相除法**:

![](https://render.githubusercontent.com/render/math?math={\color{Gray}%20\large%20A%20\bmod%20B%20%3C%20\frac{A}{2}%20%20})  

![](https://render.githubusercontent.com/render/math?math={\color{Gray}%20\large%20B%20\bmod%20(A%20\bmod%20B)%20%3C%20\frac{B}{2}%20%20})  

> (A是被除数，B是除数)  

**两次**迭代**中**，A和B的值分别**减少了一半**，也就是![](https://render.githubusercontent.com/render/math?math={\color{Gray}%20\large%20A%20%2B%20B%20\to%20\frac{A}{2}%20%2B%20\frac{B}{2}%20%20})，  

对于除数`B`来说，每次**减少一半**的时候经过的操作次数：`1<操作次数<2`；因此迭代次数**至多**为`2log(min{A,B})`次（这里除数是B，即`2log(B)`）。  

> 之所以是`min{A,B}`(取A和B中较小的值)，是因为算法的**终止条件**是**余数为0**，也可以理解成**除数为0**，而算法最开始会选择**较小**的值作为**除数**。

所以辗转相除法的时间复杂度是`O(logn)`级别的。

-------

对于两**正整数**的**辗转相除法**来说：

* **最好的情况**

    首次运算**余数**即为0，一步完成，时间复杂度为 ![](https://render.githubusercontent.com/render/math?math={\color{Gray}%20\large%20O(1)})  

* **最糟糕的情况**  

    一直迭代到**除数**为 `1` （1是所有整数的公因数），**余数为0**，时间复杂度级别为![](https://render.githubusercontent.com/render/math?math={\color{Gray}%20\large%20O(\log_{}{n})%20%20%20%20})  


## 求多个整数的最大公约数  

辗转相除法是针对**两个整数**的二元算法。  

而如果要求多个整数的最大公约数，只需要化成**多次二元辗转相除运算**即可。  

### C语言实现  

```c
#include <stdio.h>
#include <math.h>  

long int GCD(long int num1, long int num2) {
    // 寻找两数最大公约数(欧几里得算法)
    // 公式 GCD(被除数,除数)=GCD(除数,余数)
    long int dividend; // 被除数
    long int divisor; // 除数
    long int remainder; // 余数
    if (labs(num1) > labs(num2)) {
        dividend = num1;
        divisor = num2;
    } else {
        dividend = num2;
        divisor = num1;
    }
    do {
        remainder = dividend % divisor;
        dividend = divisor; // 把被除数换成除数
        if (remainder) { // 余数不为0，就把除数换成余数
            divisor = remainder;
        }
    } while (remainder != 0);
    return divisor;
}

long int ArrGCD(long int *arr, int arrLen) {
    long int temp = arr[0];
    int i;
    for (i = 1; i < arrLen; i++)
        temp = GCD(temp, arr[i]); // 用前一次的最大公约数和当前的数进行辗转相除
    return temp;
}

int main() {
    long int testArr[] = {405, 45,180,210};
    int arrLen = sizeof(testArr) / sizeof(int);
    printf("%ld\n", ArrGCD(testArr, arrLen));
    return 0;
}
```

核心方法：用**前一对数值的最大公约数和当前数进行辗转相除**，遍历数组进行迭代运算。  

### 时间复杂度  

前面针对两个整数的辗转相除的时间复杂度级别是![](https://render.githubusercontent.com/render/math?math={\color{Gray}%20\large%20O(\log_{}{n})%20%20%20%20})。而在这里需要遍历一次数组元素，且**每遍历一个元素需要进行一次辗转相除算法**。  

一层循环加上对数阶复杂度，很容易能得出这个求多整数最大公约数算法的时间复杂度是：  

![](https://render.githubusercontent.com/render/math?math={\color{Gray}%20\LARGE%20O(n\log_{}{n})%20%20%20%20})  

## 求最小公倍数

![thinking-2022-05-02](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/thinking-2022-05-02.jpg)  

前面的求最大公约数我是由**约分**的概念引出的。在分数运算中除了约分，还有一个很重要的运算技巧便是**通分**。  

通分要找的是**最小公倍数**。

### 关于最小公倍数  

**公倍数**是两个整数`A`,`B`共有的倍数，也就是**公倍数**可以被`A`和`B`整除。  

两个整数的**公倍数**有无限多个，而这些公倍数中**除0外**最小的一个便是**最小公倍数**（`Least Common Multiple`）。  

### 用最大公约数算最小公倍数

在求了最大公约数后，求最小公倍数可谓是小菜一碟了~因为**最小公倍数**和**最大公约数**有一个性质：  

![](https://render.githubusercontent.com/render/math?math={\color{Gray}%20\LARGE%20lcm(A,B)\cdot%20gcd(A,B)%20=%20A%20\times%20B%20})  

即 `最小公倍数 × 最大公约数 = 两整数之积`。  

在已经知道了最大公约数的情况下，利用`两整数之积 / 最大公约数`即可算出`最小公倍数`。  

> PS：关于这个公式的证明就不多说了...

### C语言实现  

这里的实现很简单，调用一次上面的求最大公约数函数即可。

但是程序中有一点需要注意，最好不要写成`两整数之积 / 最大公约数`的形式。如果两个整数的**数值都很大**，在运算过程中很容易发生**数据溢出**问题。  

为了尽可能避免溢出，可以写成 `A / 最大公约数 * B` 的形式。  

```c
long int LCM(long int num1, long int num2) {
    long int divisor = GCD(num1, num2); // GCD 即上面提到的求最大公约数的函数
    return (num1 / divisor) * num2; // A/GCD * B
}
```

### 时间复杂度  

因为上面的写法实际上是**套用了辗转相除算法**，所以时间复杂度也是一致的：  

![](https://render.githubusercontent.com/render/math?math={\color{Gray}%20\large%20O(\log_{}{n})%20%20%20%20}) 

## 求多个整数的最小公倍数  

和[求多个整数的最大公约数](#求多个整数的最大公约数)一样，可以利用多次二元迭代运算来实现。  

### C语言实现  

```c
long int ArrLCM(long int *arr, int arrLen) {
    long int temp = arr[0];
    int i;
    for (i = 1; i < arrLen; i++) // 从第二个元素开始遍历
        temp = LCM(temp, arr[i]); // LCM是上面提到的求最小公倍数的函数
    return temp;
}
```

### 时间复杂度

同上面求多个整数最大公约数的一致：  

![](https://render.githubusercontent.com/render/math?math={\color{Gray}%20\LARGE%20O(n\log_{}{n})%20%20%20%20})  

## 总结  

![ei-2022-05-02](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/ei-2022-05-02.png)  

脑袋一热写了这样一篇笔记。写的过程中咱深刻体会到了我在证明公式方面能力的缺乏...要继续加油了！

另外可能我对辗转相除法时间复杂度的理解有些许问题，希望大家能予以指正。  


## 相关文章  

* [辗转相除法介绍](https://zhuanlan.zhihu.com/p/324578532) - 林择枝  
* [辗转相除法](https://zh.wikipedia.org/wiki/%E8%BC%BE%E8%BD%89%E7%9B%B8%E9%99%A4%E6%B3%95) - Wikipedia  

