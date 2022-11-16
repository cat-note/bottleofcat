# 【小记】二八十十六，进制团团转

这篇笔记咱主要写一下二进制、八进制、十进制、十六进制之间的互相转换。

## Contents

- [十进制与其他进制的互相转换](#十进制与其他进制的互相转换)
  - [十进制 → 其他进制](#十进制到其他进制)  
    - [整数部分](#整数部分)  
        - [<方法> 除以基数倒取余](#int-dec-others-method)  
    - [小数部分](#小数部分)  
        - [<方法> 乘以基数顺取整](#frac-dec-others-method)  
  - [其他进制 → 十进制](#其他进制到十进制)  
    - [<方法> 乘以位权后求和](#others-dec-method)    
- [二进制与其他进制的互相转换](#二进制与其他进制的互相转换)  
  - [二进制 → 其他进制](#二进制到其他进制)  
    - [<方法> 几位一读，不足补零](#bin-others-method)    
  - [其他进制 → 二进制](#其他进制到二进制)  
    - [<方法> 展开一位为多位](#others-bin-method)    
- [十六进制与八进制的互相转换](#十六进制与八进制的互相转换)  
  - [<方法> 间接转换](#hex-oct-method)    
  - [【例】八进制 → 十六进制](#八进制转十六进制的例子)  
  - [【例】十六进制 → 八进制](#十六进制转八进制的例子)  

## 前言

通过观察，我觉得**二、八、十、十六**进制数的互相转换可以总结为三大块：  

1. **十进制与其他进制**互相转换
2. **二进制与其他进制**互相转换
3. 十六进制与八进制的互相转换

话不多说，下面就按这三大块来归纳一下。  

## 十进制与其他进制的互相转换

### 十进制到其他进制

#### 整数部分

<a id="int-dec-others-method"></a>

💡 **方法**：**除以基数倒取余**  

> 比如16进制，除以的基数就是16  

1. “除以基数”指的是将十进制数**不断除以**目标进制对应的**基数**，直到**商**为`0`为止。  
2. “倒取余”指的是将**每次相除**得到的**余数倒序排列**，即转换为了目标进制的数。

    ------

- [例] **10进制转换为8进制**  

    ![decOct-2022-10-29](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/decOct-2022-10-29.png)  

    > 图中的红色箭头展示了“倒取余”的过程  

    图中将**十进制**数`1145`转换为了**八进制**数`2171`

- [例] **10进制转换为16进制**  

    ![decHex-2022-10-29](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/decHex-2022-10-29.png)  

    > 图中的红色箭头展示了“倒取余”的过程  

    图中将**十进制**数`14191`转换为了**十六进制**数`376F`

    
#### 小数部分

> ⚠ **不是所有的小数**都能转换为其他进制。比如现代计算机采用二进制计数，有很多小数都**没法准确地转换**为二进制（只能取近似值），这也是为什么说现在计算机无法完全准确地表示小数。

<a id="frac-dec-others-method"></a>

💡 **方法**：**乘以基数顺取整**  

> 比如2进制，对应的基数就是2  

1. “乘以基数”指的是**不断将小数部分乘以**目标进制对应的**基数**，直到**小数部分**为`0`为止。  
2. “顺取整”指的是将**每次相乘**得到的数的**整数部分顺序排列**，即转换为了目标进制的数。

    ------

- [例] **2进制转换为10进制**  

    ![decBinFractional-2022-10-30](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/decBinFractional-2022-10-30.png)  

    > 图中的红色箭头展示了“顺取整”的过程  

    图中将**十进制**数`0.375`转换为了**二进制**数`0.011`


### 其他进制到十进制

<a id="others-dec-method"></a>

💡 **方法**：**每一位数字乘上`基数^对应指数`(位权)，然后相加求和**  

![digitWeight-2022-10-30](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/digitWeight-2022-10-30.png)  

> 本图中的数值`12C.1415926`，是十六进制数。

上图中，**基数**是16，“**对应指数**”就是**橙色**标出的数。以**小数点**为分界，小数点**左侧第一位**的对应指数为`0`，而小数点**右侧第一位**的对应指数为`-1`。  

从低位向高位（图中从右向左），这个指数不断增大。  

> 十六进制的基数是16，那么八进制的基数就是8，以此类推。 

- [例] **16进制**转换为**10进制**  

    ![hexDec-2022-10-30](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/hexDec-2022-10-30.png)  

    图中将**十六进制**数`12C.14`转换为了**十进制**数`300.078125`  

## 二进制与其他进制的互相转换  

二进制和其他进制的互相转换就要方便多了。

### 二进制到其他进制

<a id="bin-others-method"></a>

💡 **方法**：**以小数点为界**，向左右两侧，**每几位**读为一个数，不足的就**补零**。  

- 关于这里的“每几位”，需要看要转换为哪个进制的数。比如转换为**十六进制**，其一位能表示`0-F`这**16**种状态，需要用**4**个二进制位来表示，那么就是**每4位**一读。  
    > 再比如八进制一位能表示`0-7`这**8**种状态，需要用**3**个二进制位来表示，那么就是**每3位**一读。  

- 关于补零，无论是从小数点左侧第一位开始往左**每几位**一读，还是从小数点右侧第一位开始往右**每几位**一读，都难免遇到“位数不够读”的情况，这个时候就需要补零。 

    ------

- [例] **二进制**转换为**十六进制**

    ![binHex-2022-10-30](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/binHex-2022-10-30.png)  

    图中将**二进制**数`1001001.011`转换为了**十六进制**数`49.6`  

    - **红色箭头**展示了以小数点为分界，分别往左右进行读取。左侧从**低位向高位**，而右侧从**高位向低位**。  
    - **图中括号**展示了补零的过程。`100`和`011`都不足4位，按照**读取方向**，分别在其**左方**和**右方**补零。
    - 最后，**每4位**一读，用十六进制表示出来，就得到了最后的结果。  

- [例] **二进制**转换为**八进制**

    ![binOct-2022-10-30](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/binOct-2022-10-30.png)  

    图中将**二进制**数`1001101.1011`转换为了**八进制**数`115.54`  

    - 具体做法和上面一个例子一致，这里不多赘述。

### 其他进制到二进制

<a id="others-bin-method"></a>

💡 **方法**：每位读为**几位二进制数**。  

- 关于这里的“每位读为几位二进制数”，需要看**转换前**是哪个进制的数。比如原本是**十六进制**，其一位能表示`0-15`这**16**种状态，需要用**4**个二进制位来表示，那么就是**每位读为4位二进制数**。  
- 附一个对照表：  
    | 八进制位 | ←对应的二进制 | 十六进制位 | ←对应的二进制 |
    | :------: | :-----------: | :--------: | :-----------: |
    |   `0`    |     `000`     |    `0`     |    `0000`     |
    |   `1`    |     `001`     |    `1`     |    `0001`     |
    |   `2`    |     `010`     |    `2`     |    `0010`     |
    |   `3`    |     `011`     |    `3`     |    `0011`     |
    |   `4`    |     `100`     |    `4`     |    `0100`     |
    |   `5`    |     `101`     |    `5`     |    `0101`     |
    |   `6`    |     `110`     |    `6`     |    `0110`     |
    |   `7`    |     `111`     |    `7`     |    `0111`     |
    |    -     |       -       |    `8`     |    `1000`     |
    |    -     |       -       |    `9`     |    `1001`     |
    |    -     |       -       |    `A`     |    `1010`     |
    |    -     |       -       |    `B`     |    `1011`     |
    |    -     |       -       |    `C`     |    `1100`     |
    |    -     |       -       |    `D`     |    `1101`     |
    |    -     |       -       |    `E`     |    `1110`     |
    |    -     |       -       |    `F`     |    `1111`     |

    实际应用中，可以根据**8421码**来进行计算，不用死记硬背。  

    > 比如三位二进制`101`, 对应`421`，转换为十进制就是`4+1=5`。  
    > 再比如四位二进制`1010`, 对应`8421`，转换为十进制就是`8+2=10`。  

    ------

- [例] **八进制**转换为**二进制**

    ![octBin-fixed-2022-10-31](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/octBin-fixed-2022-10-31.png)  

    图中将**八进制**数`67.23`转换为了**二进制**数`110111.010011`
  
    - 每一位八进制位读成**3位二进制数**（一位八进制位能表达8种状态，需要3位二进制位来表示）  

    ------

    不想用8421码其实也没问题，八进制和十六进制的**每一位数**其实可以看作是**十进制整数**，使用**除2倒取余**的方法能将其**逐位展开为二进制数**。   
    (本质上还是按位读取，每位读为**几位二进制数**)  

    ![decBinForEachDigit-2022-10-31](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/decBinForEachDigit-2022-10-31.png)

    > 比如上图中，原八进制数中的`6`与`3`分别可以用除2倒取余的方法展开为`110`与`011`，和8421码得出的结果一致。  

## 十六进制与八进制的互相转换

<a id="hex-oct-method"></a>

十六进制和八进制之间无法直接转换：

* 如果要把**十六进制数转换为八进制数**，就需要**先将十六进制转换为十进制或二进制**，进而再转换为八进制。  
* 把**八进制转为十六进制**也需要用十进制或二进制“过渡一下”  

-------  


### 八进制转十六进制的例子

* **通过二进制进行间接转换**  

    ![octHexViaBin-2022-11-02](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/octHexViaBin-2022-11-02.png)  

    上图中我将八进制数`12.450`先转换为了二进制数`001010.100101000`，再转换为了十六进制数`A.94`。

    > 二进制和其他进制互相转换的方法在上面已经介绍过了，这里就不多赘述。  

* **通过十进制进行间接转换**  

    ![octHexViaDec-2022-11-03](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/octHexViaDec-2022-11-03.png)  

    上图中我将八进制数`12.450`先转换为了十进制数`10.578125`，再转换为了十六进制数`A.94`。
     
    - 这里再提一嘴：十进制转为其他进制时，需将**整数**和**小数**分而治之。  

    > 十进制和其他进制互相转换的方法在上面也已经介绍过了，这里就不多赘述。 

### 十六进制转八进制的例子

* **通过二进制进行间接转换**  

    ![hexOctViaBin_fixed-2022-11-03](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/hexOctViaBin_fixed-2022-11-03.png)

    上图中我将十六进制数`C.98`先转换为了二进制数`1100.10011000`, 再转换为了八进制数`14.46`。  

    > 可以看到，十六进制转八进制和八进制转十六进制的过程是十分类似的。

* **通过十进制进行间接转换**  

    ![hexOctViaDec-2022-11-03](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/hexOctViaDec-2022-11-03.png)  

    上图中我将十六进制数`C.98`先转换为了十进制数`12.59375`, 再转换为了八进制数`14.46`。  

## 冷笑话

![HalloweenMas-2022-11-04](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/HalloweenMas-2022-11-04.jpeg)  

这位老伙计为什么要在万圣节前夜穿上圣诞节的服装呢？  

答：因为 `Oct 31 = Dec 25`。  

> October(十月) 31 是万圣节前夜，December(十二月) 25 是圣诞节。  
> 但是在这也可以理解成Octal(八进制) `31` = Decimal(十进制) `25` (\*\^_\^\*)  