# 简记清空C语言输入缓冲区

实际上，清空缓冲区已经是老生常谈的问题了。为了在命令行程序中实现和用户的交互，我们往往需要编写程序对输入/输出缓冲区进行多次读写。

然而，在C语言中接受用户输入这一块，总有着围绕着输入缓冲区的一个问题：“怎么样及时清空输入缓冲区？” 这也是这篇小笔记的主题内容。

通常来说，缓冲区类型有三种：

* 全缓冲
* 行缓冲
* 无缓冲

## 行缓冲

在C语言中缓冲区这个概念的存在感还是挺强的，比较常用到的缓冲区类型则是**行缓冲**了，如标准输入流 `stdin` 和标准输出流 `stdout`。  

行缓冲，顾名思义，就是针对该缓冲区的**I/O操作**是**基于行**的。在遇到**换行符**前，用户的所有**输入/输出**都会被暂存到该缓冲区中；而在遇到**换行符**后，程序则会进行I/O操作，从缓冲区中读取**所需的数据**。 

就**标准输入**`stdin`而言，每当用户按下回车键输入一个**换行符**，程序才会进行I/O操作，从缓冲区将暂存的数据读走。

而对**标准输出**`stdout`来说，每当输出内容遇到**换行符**时，程序才会将缓冲区中的内容打印到屏幕上。  
这也是为什么用格式化打印函数`printf`时**可能**不会立即输出到屏幕上：
```c
printf("Hello ");
getchar();
printf("World \n");
```

## C语言中常用的输入函数

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