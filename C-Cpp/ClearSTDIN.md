# 简记清空C语言输入流

为了在命令行程序中实现和用户的交互，我们编写的程序的运行过程中往往涉及到对标准输入/输出流的多次读写。

在C语言中接受用户输入这一块，有着一个老生常谈的问题：“怎么样及时清空输入流中的数据？”
 
这也是这篇小笔记的主题内容。

![idling-2022-06-28](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/idling-2022-06-28.png)  

先从缓冲区说起。

缓冲区是**内存中**划分出来的一部分。通常来说，缓冲区类型有三种：

* 全缓冲
* 行缓冲
* 无缓冲

## 行缓冲

在C语言中缓冲区这个概念的存在感还是挺强的，比较常用到的缓冲区类型则是**行缓冲**了，如标准输入流 `stdin` 和标准输出流 `stdout`一般（终端环境下）就是在行缓冲模式下的。  

行缓冲，顾名思义，就是针对该缓冲区的**I/O操作**是**基于行**的。  
* 在遇到**换行符**前，程序的**输入**和**输出**都会先被**暂存**到**流对应**的缓冲区中  

* 而在遇到**换行符**后（或者缓冲区满了），程序才会进行真正的**I/O操作**，将该缓冲区中的数据写到对应的**流** (stream) 中**以供后续读取**。  

就**标准输入**`stdin`而言，用户的输入首先会被存到**相应的输入缓冲区**中，每当用户按下回车键输入一个**换行符**，程序才会进行I/O操作，将缓冲区暂存的数据写入到`stdin`中，以供**输入函数**使用。

而对**标准输出**`stdout`来说，输出内容也首先会被暂存到**相应的输出缓冲区**中，每当输出数据遇到**换行符**时，程序才会将缓冲区中的数据写入`stdout`，继而打印到屏幕上。  

这也是为什么在缓冲模式下，输出的内容不会立即打印到屏幕上：
```c
#include <stdio.h>
int main()
{
	// 设置缓冲模式为行缓冲，缓冲区大小为10字节
	setvbuf(stdout, NULL, _IOLBF, 10);
	fprintf(stdout, "1234567"); // 这里先向stdout对应的缓冲区中写入了7字节
	getchar(); // 这里等待用户输入
	printf("89"); // 再向stdout对应的缓冲区中写入了2字节
	getchar(); // 接着等待用户输入
	printf("Print!"); // 再向stdout对应的缓冲区中写入了6字节
	getchar(); // 最后再等待一次用户输入
	return 0;
}
```

运行效果：

![outputBuffer_remake-2022-06-28](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/outputBuffer_remake-2022-06-28.gif)

可以看到，直到执行到第二个`getchar()`时，屏幕上没有新的输出。  

而在执行了`printf("Print!")`之后，输出缓冲区**被填满**了，输出缓冲区中现有的`10`字节的数据被写入到`stdout`中，继而才在屏幕上打印出`123456789P`。

缓冲区内容被读走后，剩余的字符串`rint!`接着被写入输出缓冲区。程序运行结束后，**输出缓冲区**中的内容会被全部打印到屏幕上，所以会在最后看到`rint!`。



## C语言中常用的输入函数

输入函数做的工作主要是从**文件流**中读取数据，亦可将**读取到的数据**储存到内存中以供后续程序使用。

### 基于字符

```c
// 从给定的文件流中读一个字符 (fgetc中的 f 的意思即"function")
int fgetc( FILE *stream ); 

// 同fgetc，但是getc的实现*可能*是基于宏的
int getc( FILE *stream ); 

// 相当于是getc(stdin)，从标准输入流读取一个字符
int getchar(void);

// 返回获取的字符的ASCII码值，如果到达文件末尾就返回EOF（即返回-1）
```

### 基于行

```c
// 从给定的文件流中读取(count-1)个字符或者读取直到遇到换行符或者EOF
char *fgets( char *restrict str, int count, FILE *restrict stream );

// 返回指向字符串的指针或者空指针NULL
```

### 格式化输入

```c
// 按照format的格式从标准输入流stdin中读取所需的数据并储存在相应的变量中
// scanf中的f代表“format”
int scanf( const char *restrict format, ... );

// 按照format的格式从文件流stream中读取所需的数据并储存在相应的变量中
// fscanf中前一个f代表“file(stream)”，后一个f代表“format”
int fscanf( FILE *restrict stream, const char *restrict format, ... );

// 按照format的格式从字符串buffer中截取所需的数据并储存在相应的变量中
int sscanf( const char *restrict buffer, const char *restrict format, ... );

// 返回一个整型数值，代表成功根据格式赋值的变量数（arguments）
```

## 最常到的输入流问题

先来个不会出问题的示例：

```c
#include <stdio.h>
int main()
{
	char test1[200];
	char test2[200];
	char testChar;
	printf("Input a Character: \n");
	testChar = getchar();
	fprintf(stdout, "Input String1: \n");
	scanf("%s", test1);
	fprintf(stdout, "Input String2: \n");
	scanf("%s", test2);
	printf("Got String1: [ %s ]\n", test1);
	printf("Got String2: [ %s ]\n", test2);
	printf("Got Char: [ %c ]\n", testChar);
	return 0;
}
```

运行效果：  

![correctExample-2022-06-28](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/correctExample-2022-06-28.png)  

------
出问题的示例：

```c
#include <stdio.h>
int main()
{
	char test[200];
	char testChar1, testChar2, testChar3;
	fprintf(stdout, "Input String: \n");
	scanf("%3s", test);
	printf("[1]Input a Character: \n");
	testChar1 = getchar();
	printf("[2]Input a Character: \n");
	testChar2 = fgetc(stdin);
	printf("[3]Input a Character: \n");
	testChar3 = getchar();
	printf("Got String: [ %s ]\n", test);
	printf("Got Char1: [ %c ]\n", testChar1);
	printf("Got Char2: [ %c ]\n", testChar2);
	printf("Got Char3: [ %c ]\n", testChar3);
	return 0;
}
```

运行效果：  

![incorrectExample-2022-06-28](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/incorrectExample-2022-06-28.png)  

因为我将格式设置为了`%3s`，所以`scanf`**最多**接收包含三个字符的字符串。

在这个示例中，我按要求输入了一条字符串`Hello`，并按下**回车**输入一个换行符，缓冲区数据`Hello\n`被写入到了`stdin`中。而`scanf`只从标准流`stdin`中读走了`Hel`这一部分字符串。  

此时，标准流`stdin`中实际上还剩3个字符：

1. `l`
2. `o`
3. `\n` (回车输入的换行符)

于是接下来三次**针对字符的**输入函数只会分别**从`stdin`中**取走这三个字符，而**不会等待用户输入**，这就没有达到我想要的效果。

在基本的命令行程序中很容易遇到这类问题，这也是为什么需要及时**清空输入流`stdin`中的数据**。  

## 清空标准流

以下内容假设`stdout`和`stdin`两个标准流都是在**行缓冲**模式下的。

### 标准输出流stdout  

虽然本文主要是写输入流，但这里我还是掠过一下标准输出流`stdout`。在有行缓冲的情况下，