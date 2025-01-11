# Effective Go 阅读笔记

* [Effective Go](https://go.dev/doc/effective_go)  

# 0. 个人前言

Effective Go 算是官方文档中很经典的一篇了，这里记录一下自己的阅读笔记。其中有些内容可能稍微显得有些过时了，咱也**额外补充了很多个人认为需要注意的点**。  

这篇笔记是在 Obsidian 中写的。  

Somebottle 2025.1

# 1. 代码格式化

`gofmt` 能帮助完成工作，包括对于注释的对齐。  

* 保持风格统一，便于他人阅读代码。  

# 2. 清晰的运算符优先级

Go 语言中用到的括号更少，运算符优先级表如下：  

![file-20250102145835835-2025-01-11](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/file-20250102145835835-2025-01-11.png)  

可以看到位运算的优先级还是比较高的，这点和 C++ 不一样：  

```go
x<<8 + y<<16
```
> `gofmt` 在这种情况会**自动添加一些空格**以方便阅读。

会被解析为：  

```go
(x<<8) + (y<<16)
```

❗ `++` 和 `--` 运算符在 Go 中形成一个语句 ⚠ **而不是表达式**，不算在运算符优先级中。而 `*` 又是优先级最高的运算符之一，因此在 Go 中：  

```go
*p++ 
```

和  

```go
(*p)++
```

是一样的。

# 3. 注释（Commentary）

❗ 一般建议是用**行级注释** `//` 。

* `package` 级别或者表达式中间的注释可以用块级注释 `/* */`，也可用于注释掉一大片代码。  

如果注释出现在顶级声明（如函数、类型、变量等）之前，并且**中间没有空行**，那么这些注释会被视为该声明的**文档注释**（类似 Python 的 docstring）。  

```go
// 初始化路由
// 2333
func InitRouter() *gin.Engine {
```
> 比如这样，注释之间没有空行，会被一起认为是 InitRouter 方法的注释。

# 4. 命名（Naming）

## 4.1. 包命名（Package name）

* **全小写字母**
* 用**一个词**精炼描述（尽量简短，没必要用下划线和驼峰式）  
* ❗ 和包**所在的目录名一致**  

* 🤔 一个**有用的注释**比长命名更有用。  

## 4.2. Getter 和 Setter 命名

* 💡 **对于 Getter 方法**，❗ 不建议在方法名中写 `Get`，而是用首字母大写的相应字段的名字作为 Getter 方法名： `student.Age()` 而不是 `student.GetAge()`
* Setter 则照旧： `student.SetAge()`  

## 4.3. 接口（Interface）命名

* 如果接口**只有一个方法**，可以在方法后面加上 `-er` 来表示这是一个执行什么操作的方法，比如 `Reader`, `Scraper`  
* ❗ 如果有方法和 Go 语言标准库中方法的**功能一致**，应当保持相同命名，比如字符串化的方法 `String()`    

## 4.4. 其他的命名

* **用小驼峰或者大驼峰即可**，而不是下划线分隔单词。

# 5. ❗ 分号（Semicolons）

Go 语言中大多数地方分号由编译器的词法分析器（Lexer）**自动插入在语句可能结束的地方，主要是在换行的时候进行判断**。  

❗ 因此在写控制逻辑和函数的时候**必须把圆括号和大括号写在同一行**，不然编译器会插入分号导致解析失败：  

```go
// 正确写法
if i < f() {
    g()
}

// 错误写法
if i < f()  // wrong!
{           // wrong!
    g()
}
```

# 6. 初始化赋值 `:=`

```go
a := 1
// 左侧至少有一个变量是新声明的
b,a := 3,4

// wrong
a := 4
```

初始化赋值声明语法 `:=` 其实**可以复用之前已经声明的变量**，比如这里的 `a` ，并不是每次一定要全部声明新的变量：  

1. 待赋值的值应当和 `a` **同类型**。
2. ❗ `:=` 左侧**至少有一个变量是新声明**的！！！（上面的例子中可以看到）  

# 7. 条件控制结构

* 循环控制语句只有 `for`
* 💡 `if` 与 `switch` 和 `for` 一样可以**有一个初始化语句**，可以用于**设立一个局部变量**。
	```go
	if err := file.Chmod(0664); err != nil {
		log.Print(err)
		return err
	}
	```
* `continue` 和 `break` 都支持一个标签（Label），来控制外层循环的继续与停止

## 7.1. range

从 `range` 接受值时可以只接受第一个：  

```go
for key := range m {
    if key.expired() {
        delete(m, key)
    }
}
```

💡 对 `string` 使用 `range` 时，会以 `UTF-8` 编码将字串按码点拆分，取得的**每个 Unicode 码点用 `rune` 类型存储**。

* ❗ 有问题的编码会被替换为 `U+FFFD`  

## 7.2. 循环中声明多个局部变量

```go
// Reverse a
for i, j := 0, len(a)-1; i < j; i, j = i+1, j-1 {
    a[i], a[j] = a[j], a[i]
}
```

❗ Go 语言中 `++` 和 `--` 是**语句而不是表达式**（没有值，每个应该放在单独一行中），也**不能用逗号分隔表达式**，因此得要像上面这样借助声明和并行赋值语句来写。  

## 7.3. switch

比 C 语言的 `switch` 要灵活很多，**可以接受值和逻辑表达式**：  

```go
func unhex(c byte) byte {
    switch {
    case '0' <= c && c <= '9':
        return c - '0'
    case 'a' <= c && c <= 'f':
        return c - 'a' + 10
    case 'A' <= c && c <= 'F':
        return c - 'A' + 10
    }
    return 0
}
```
> 这样写**甚至可以简化 `if-else` 语句**。  

`case` 后面还可以是**逗号分隔**的表达式列表：  

```go
func shouldEscape(c byte) bool {
    switch c {
    case ' ', '?', '&', '=', '#', '+', '%':
        return true
    }
    return false
}
```

❗ 注意 Go 语言匹配了某个 `case` 后**不会**继续尝试执行下一个 `case`，也就**不需要写** `break`. （可以用 `fallthrough` 来让程序顺着执行下面的 `case`。。  
* 💡 当然写 `break` 也仍然会**立即停止执行 `case` 中剩余的语句**。  

## 7.4. 类型推断 switch

switch 可以**结合类型断言**来使用，对于不同的类型可以进行不同的处理。  

```go
var t interface{} // 即 Go 1.18 以后的 any
t = functionOfSomeType()
switch t := t.(type) { // 这里就写 .(type)
default:
    fmt.Printf("unexpected type %T\n", t)     // %T prints whatever type t has
case bool:
    fmt.Printf("boolean %t\n", t)             // t has type bool
case int:
    fmt.Printf("integer %d\n", t)             // t has type int
case *bool:
    fmt.Printf("pointer to boolean %t\n", *t) // t has type *bool
case *int:
    fmt.Printf("pointer to integer %d\n", *t) // t has type *int
}
```

## 7.5. break 标签

可以用于**中断上层的**循环：  

```go
for i := 0; i < 3; i++ {
secondLoop:
	for j := 0; j < 3; j++ {
		for k := 0; k < 3; k++ {
			fmt.Printf("i=%d, j=%d, k=%d\n", i, j, k)
			if i == 1 && j == 1 && k == 1 {
				// 满足条件时，跳出第二层循环
				break secondLoop
			}
		}
	}
}
```

# 8. 函数

## 8.1. 多返回值

Go 的函数支持多返回值，声明函数时多返回值需要用括号包裹起来：  

```go
var a func(i, j int) (int, int)
```

## 8.2. ❗ 已命名的返回参数

如果返回参数命名了，在调用函数时其会被初始化为**零值变量**，在函数作用域中都可以使用。  

💡 **函数中 `return` 不需要显式返回任何内容**。

```go
func ReadFull(r Reader, buf []byte) (n int, err error) {
    for len(buf) > 0 && err == nil {
        var nr int
        nr, err = r.Read(buf)
        n += nr
        buf = buf[nr:]
    }
    return // 这样写就行
}
```

## 8.3. ❗ Defer 推迟执行

把**某个函数**推迟到**当前函数执行完成后，返回之前**执行。

* 💡 即使函数执行时发生了 `panic`，`defer` 也会执行。
* 💡 有点类似于其他语言中的 `try...finally`，常用于释放资源。 
	* ❗ 为了表达清晰，可以**放在资源获取的语句的后面**，比如先 `open` 然后紧跟一个 `defer close` 。

❗ 有多个 `defer` 时会形成一个函数推迟执行栈（LIFO），**最后的 `defer` 最先被执行**：  

```go
func deferExample() {
    defer fmt.Println("Defer 1") // 最后执行
    defer fmt.Println("Defer 2") // 第二个执行
    defer fmt.Println("Defer 3") // 最先执行

    fmt.Println("Function body")
}
// Function body
// Defer 3
// Defer 2
// Defer 1
```

❗ 延迟执行的函数**参数**是在 `defer` **执行时立即求值**的！

```go
func trace(s string) string {
    fmt.Println("entering:", s)
    return s
}

func un(s string) {
    fmt.Println("leaving:", s)
}

func a() {
    defer un(trace("a")) // 这个时候 trace 函数会被立即执行以进行求值
    fmt.Println("in a")
}

func b() {
    defer un(trace("b"))
    fmt.Println("in b")
    a()
}

func main() {
    b()
}
// entering: b
// in b
// entering: a
// in a
// leaving: a
// leaving: b
```

## 8.4. 可变参数函数

* ❗ 可变参数必须放在参数列表中的最后一个位置  
* 类型前缀加上 `...`

```go
func Printf(format string, v ...interface{}) (n int, err error)
```

这里 `v` 可以接受任意多个**任意类型**（Go 1.18 后 `interface{}` 即 `any`）的实参。

* `v` 本身也就被**当作一个 `[]any{}` 切片**进行处理。

✨ 反过来，相应数据类型的**切片**也可以利用 `...` **后缀**，让编译器将切片中的元素当作参数传入：  

```go
a := [3]any{1, 2, 3}
// 必须转换为切片，然后用 ... 后缀
fmt.Println(a[:]...)
```

## 8.5. 在内置类型上自定义方法

内置类型无法像结构体那样可以直接定义其上的方法，不过我**可以先给内置类型定义一个别名，就可以直接在这个别名类型上定义方法了**。  

```go
type MySize float64 // 别名

func (ms MySize) String() string {
	...
}
```

# 9. 数据与操作（Data）

## 9.1. ❗ new（返回指针）

💡 初始化一个**清零的内存区域**，并把指向内存的**指针**返回。  

* 这个指针 `*T` 指向 `T` 类型的一个**零值**对象。  
* ❗ 也就是 `new` 了之后**立即就可以使用**。  

```go
p := new(SyncedBuffer)  // type *SyncedBuffer
var v SyncedBuffer      // type  SyncedBuffer
// p, v 都可以直接拿来用
```

## 9.2. 构造函数（Constructors）和复合结构字面量（Composite literal）

### 9.2.1. Constructor

💡 Go 语言中构造函数其实就是**用户手动写的一个函数**，函数名习惯以 `New` 开头：  

```go
// 定义一个结构体
type Person struct {
    Name string
    Age  int
}

// 定义一个构造函数
func NewPerson(name string, age int) *Person {
    return &Person{
        Name: name,
        Age:  age,
    }
}
```

### 9.2.2. ❗ 复合结构字面量

💡 像映射、切片、结构体、数组等这些复合类型可以用**字面量**（literal）直接在声明和定义时初始化：  

```go
Type{Value1, Value2, ..., ValueN}
```

如：  

```go
type Person struct {
    Name string
    Age  int
}
p := Person{Name: "Alice", Age: 30} // 创建一个结构体实例
matrix := [2][2]int{{1, 2}, {3, 4}} // 创建一个 2x2 的二维数组
```

💡💡 对于复合类型，**可以用类似 `字段:值` 的方式来只指定部分值**，而不必全部写出来：  

```go
s := []string{2: "Hello", 1: "World"}
fmt.Println(s)
// [ World Hello]
v := []int{3: 7, 1: 4}
fmt.Println(v)
// [0 4 0 7]
m := map[string]int{
	"hey":   3444,
	"there": 5666,
}
fmt.Println(m)
// map[hey:3444 there:5666]
p := Person{name: "Somebottle"}
fmt.Println(p)
// {0 Somebottle}
```
> 这是**常用技巧**。对于切片和数组这种结构来说“字段”指的是“**下标**”。

## 9.3. ❗ 函数返回一个局部变量的指针

在 C 语言中**返回函数的局部变量的指针**（地址）是新手常犯的一个内存管理和作用域错误，函数执行完后相应的局部变量会被释放掉。  

💡 但是！在 Go 语言中是**完全可以这样写**的：  

```go
func NewFile(fd int, name string) *File {
    if fd < 0 {
        return nil
    }
    f := File{fd, name, nil, 0}
    return &f
    // 写得更简洁：return &File{fd, name, nil, 0}
}
```

* 💡 因为 Go 语言编译器有“**逃逸分析**”，如果变量的生命周期逃逸到了函数外部，编译器**会自动把这个变量分配到堆上**，由 GC 管理。  

## 9.4. 用 make 进行分配（返回初始化后的数据结构）

❗ `make` **仅用于**新分配切片（slice）、映射（map）、通道（channel）  

* 返回一个**初始化的**对象 `T` （而**不是指针**）  
	* ❗ 注意，和 `new` 不同，`make` 是**初始化了对象而不是内存清零**！！
	* 💡 因为这三种类型是引用类型，**其底层的数据结构必须先初始化后才能使用**。比如 `slice` 底层可以看作一个结构体，有指针指向数组，并有变量表示了长度和容量。  
	* ❗ **没有初始化的引用类型默认就是** `nil`

```go
make([]int, 10, 100)
```
> 分配一个 100 个 `int` 数据的数组，并让一个切片指向其首 10 个元素，返回这个切片。

```go
new([]int)
```
> 分配一个零化的空切片结构（`nil`），并返回其**指针**。

官方例子：  
```go
var p *[]int = new([]int)       // 分配一个零切片结构; *p == nil; 几乎不怎么这样写
var v  []int = make([]int, 100) // 切片 v 指向至少有 100 个 int 的数组

// 哥你这样写是这给自己找麻烦呢: 
var p *[]int = new([]int) 
*p = make([]int, 100, 100)

// 常见的写法，地道！
v := make([]int, 100)
```

## 9.5. 数组（Arrays）

* ❗ Go 语言中数组是**值类型**，赋值（比如函数传参）时**会拷贝所有元素**。  
* ❗ 数组的长度是**数据类型的一部分**，比如 `[10]int` 和 `[20]int` 是不同的类型。

💡 定义时可以用 `[...]T` **自动推断数组**长度：

```go
// 相当于 [3]int 类型的数组
arr := [...]int{2,3,3}
```

## 9.6. 切片（Slices）

切片包装了数组，操作起来更方便。  

* 切片是**引用类型**，赋值后底层仍然是指向相同的数组。
* 内置函数 `len` 和 `cap` 可以获得切片长度(length)以及底层数组实际容量(capacity)，💡 **对于 `nil` 切片，均为 0**.  


## 9.7. 二维切片（Two-dimensional slices）

主要是官方给出了一个神奇的例子：  

```go
// Allocate the top-level slice, the same as before.
picture := make([][]uint8, YSize) // One row per unit of y.
// Allocate one large slice to hold all the pixels.
pixels := make([]uint8, XSize*YSize) // Has type []uint8 even though picture is [][]uint8.
// Loop over the rows, slicing each row from the front of the remaining pixels slice.
for i := range picture {
    picture[i], pixels = pixels[:XSize], pixels[XSize:]
}
```

可以看到定义时 `pixels` 和 `picture` 是分开的，但通过不断切片和赋值，使得 `picture` 的第二维和 `pixels` 关联了起来。  


## 9.8. 映射（Maps）

存储映射关系键值对。  

* ❗ 键可以是**任何支持等号（equality operator）比较**的类型。（比如切片类型没有定义等号，因此不能作为键）  
* `map` 也是**引用类型**。

上面已经展示过，`map` 可以通过复合字面量（composite literal）来初始化。

通过 `[key]` 可以获取到映射中 `key` 对应的值：  

```go
offset := myMap["test"]
```

* 💡 对于不存在的键，会取出**值类型对应的零值**。  

💡 当然还可以接受第二个返回值，这是一个**代表键是否存在**的布尔值，通常将其赋给一个 `ok` 变量：  

```go
_,ok := myMap["test"]
```

从映射 `map` 中移除键可以用内置的 `delete` 方法，从官方文档可以看到这个方法**专门用于映射类型**：  

```go
delete(myMap, "test") // 从 myMap 中移除键 'test' 对应的键值对
```
> 如果本来就没这个键，那么这就是个**空操作**。

## 9.9. 输出数据（Printing）

### 9.9.1. Println 一类

* 如 `Fprintln`, `Println`, `Sprintln`  

这一类输出会在每个参数值之间**加空格**，最后加上换行符进行输出。  

```go
fmt.Println("A", "B", 23, 46, "C", 67, 78, 89)
// A B 23 46 C 67 78 89\n
```

### 9.9.2. ❗ Print 一类

* 如 `Fprint`, `Print`, `Sprint`  

❗ 这一类输出仅在**相邻两个参数值都不是字串的地方**加空格：  

```go
fmt.Print("A", "B", 23, 46, "C", 67, 78, []int{89}, "STR")
// AB23 46C67 78 [89]STR
```

### 9.9.3. Printf 一类

* 💡 注：对于无序的 `map` 类型，`Printf` 一类默认**会按键进行字典升序排列**再输出。

`Printf` 即类似 C 的格式化输出字串，但**格式动词**与 C 就有些不同了：  

#### 9.9.3.1. ❗ 自动推断数值规模和符号

数字格式动词（format verbs），如 `%d`，不接受一些**是否有符号**、**数值规模**等限制的标志。（比如 C 语言中有 `%d`, `%u`, `%ld`，但是 Go 语言会自动推断这些）。  

```go
var x uint64 = 1<<64 - 1
// %d 输出十进制，%x 输出十六进制
fmt.Printf("%d %x; %d %x\n", x, x, int64(x), int64(x))
// 18446744073709551615 ffffffffffffffff; -1 -1
```

#### 9.9.3.2. 默认输出格式

💡 `%v` （记：value）可以**按默认输出方式输出 Go 中任意类型**（就像 `Println` 和 `Print` 输出的那样）。  

✨ 在输出结构体 `struct` 时，如果使用 `%+v` 可以**顺带输出结构体字段名**，而如果使用 `%#v` 会**按 Go 语言的语法**输出这个类型：  

```go
type T struct {
    a int
    b float64
    c string
}
t := &T{ 7, -2.35, "abc\tdef" }
fmt.Printf("%v\n", t) // &{7 -2.35 abc   def}
fmt.Printf("%+v\n", t) // &{a:7 b:-2.35 c:abc     def}
fmt.Printf("%#v\n", t) // &main.T{a:7, b:-2.35, c:"abc\tdef"}
fmt.Printf("%#v\n", timeZone) // map[string]int{"CST":-21600, "EST":-18000, "MST":-25200, "PST":-28800, "UTC":0}
```

✨ 对于自定义结构，比如一个 `struct`，可以为其写一个 `String() string` 方法来**自定义其默认输出格式**：  

```go
func (t *T) String() string {
    return fmt.Sprintf("%d/%g/%q", t.a, t.b, t.c)
}
fmt.Printf("%v\n", t) // 7/-2.35/"abc\tdef"\n
```

#### 9.9.3.3. 输出带引号字串

`%q` （记：quoted）可以输出带引号的字串（双引号）或者字符（单引号），`\t`,`\n` 这类特殊字符**也会被转义**：  

```go
// 字符串
fmt.Printf("%q\n", "Hello, World!") // 输出: "Hello, World!"
fmt.Printf("%q\n", "Hello\nWorld!") // 输出: "Hello\nWorld!"
// 字符
fmt.Printf("%q\n", 'H')  // 输出: 'H'
fmt.Printf("%q\n", '\n') // 输出: '\n'
// 非字符串和字符类型
fmt.Printf("%q\n", 65)              // 输出: 'A'
fmt.Printf("%q\n", []byte("Hello")) // 输出: "Hello"
// 用反引号(backquote)
fmt.Printf("%#q\n", []byte("Hello")) // 输出: `Hello`
```

#### 9.9.3.4. 将字串输出为字节串

✨ `%x` 还可以将字串中**每个字节转换为两个十六进制字符**，`% x` 会在输出时**用空格分隔**开。  

```go
fmt.Printf("%x\n", "Hello你好")          // 输出: 48656c6c6fe4bda0e5a5bd
fmt.Printf("%x\n", []byte("Hello你好"))  // 输出: 48656c6c6fe4bda0e5a5bd
fmt.Printf("% x\n", []byte("Hello你好")) // 输出: 48 65 6c 6c 6f e4 bd a0 e5 a5 bd
```


#### 9.9.3.5. 输出数据类型  

✨ `%T` 可以输出一个数据的类型。  

```go
fmt.Printf("%T\n", myMap) // map[string]int\n
```


### 9.9.4. Fprint 一类

这一类会将内容输出到指定**文件流**中，第一个参数接受**任意实现了 `io.Writer` 接口**的结构，常见的有标准输出 `os.Stdout` 和 `os.Stderr`。  


### 9.9.5. ❗ 写成死循环

通过重写类型上的 `String() string` 方法可以自定义默认输出格式，可是如果在这种方法中又尝试打印自身，就会造成死循环：  

```go
type MyString string
func (m MyString) String() string {
    return fmt.Sprintf("MyString=%s", m) // Error: will recur forever.
}
```

✨ 一个解决方法就是让 `m` 脱离类型 `MyString` 再进行输出，即转换为 `string`：  

```go
type MyString string
func (m MyString) String() string {
    return fmt.Sprintf("MyString=%s", string(m)) // OK: note conversion.
}
```

❗ 注意这种死循环是因为使用 `%s` （或者 `%v`）这类输出字串的格式动词时，`fmt.Sprintf` 会**尝试调用** `m` 的 `String()` 方法。

✨ 如果用其他格式动词，就不会有这个问题了：  

```go
type MyFloat float64

func (f MyFloat) String() string {
	// 这里用的是 %f
    return fmt.Sprintf("%.3f", f) 
}

// main 函数内
var num MyFloat = 1.41421
fmt.Println(num) // 1.414
```

## 9.10. 附加到切片（Append）  

```go
func append(slice []Type, elems ...Type) []Type
```

之前提到，切片实际上是**对底层数组的一个引用**，因此调用 `append` 向切片中添加元素时其实就是在**修改底层数组**。

* `append` 会返回**修改后的切片** 

💡 从下面这个例子可以看到 `append` 的逻辑：  

```go
arr := [...]int{1, 2, 3, 4, 5, 6}
s := arr[1:3]
s = append(s, 8, 9)
fmt.Println(s)   // [2 3 8 9]
fmt.Println(arr) // [1 2 3 8 9 6]
// re-allocated
s = append(s, 21, 22, 23, 24, 25)
fmt.Println(s)   // [2 3 8 9 21 22 23 24 25]
fmt.Println(arr) // [1 2 3 8 9 6]
```

可以看到，底层数组容量足够时，元素附加到切片之后其实就修改了底层数组中**切片最后一个位置的后一个元素**。  

❗ 但是底层**元素容量不足时就会自动分配新的底层数组**，比如上面我第二次一次附加了 5 个元素，但是原本的底层数组没有进一步被更改，程序中自动分配了新的内存。

还记得上面让编译器把切片元素当作函数参数的语法吗（`...` 后缀）：  

```go
s = append(s, []int{1,2,3,4}...)
```

# 10. 初始化（Initialization）  

## 10.1. 常量（Constants）

💡 无论是全局（**包级别**）还是函数内的局部，常量都是在**编译时**（compile time）创建的。

**常量只能是这些类型**：  

1. 数值（`int`, `float`, `int64`, `float64` 等）
2. 字符串（`string`）
3. Unicode 码点（`rune`，✨ **本质上是** `int32`）（这个其实可以和数值记在一起）
4. 字节（`byte`，✨ **本质上是** `uint8`）（这个其实可以和数值记在一起）
5. 布尔值（`bool`）

❗ 定义常量的表达式**必须是常量表达式，能在编译时求值**。比如 `1<<3` 就可以，但是像 `math.Sin(math.Pi/4)` 这种需要在运行时求值的就不行。

### 10.1.1. 按行递增量 iota

在 Go By Example 的笔记中我已经写过这部分要注意的地方：  

* `iota` 是**按行**递增的。
* 下一行如果没有指定表达式，**默认会继承上一行的表达式**。  

```go
const (
    _           = iota // ignore first value by assigning to blank identifier
    KB ByteSize = 1 << (10 * iota)
    MB
    GB
    TB
    PB
    EB
    ZB
    YB
)
```

## 10.2. 变量（Variables）

变量定义时可以写**需要在运行时求值**的表达式：  

```go
var (
    home   = os.Getenv("HOME")
    user   = os.Getenv("USER")
    gopath = os.Getenv("GOPATH")
)
```

## 10.3. ❗ 初始化函数（`init`）

主要用于在程序启动时执行**一些初始化**操作。

* 每个**程序源文件中**都可以定义 `init()` 
* ✨`init()` **可以定义多个**，会**按定义的顺序依次**执行。  
* ✨ 在当前的 `init()` 执行前，所有导入的包的 `init()` 会先执行（**先初始化被导入包，再初始化当前包**）
* ✨ 在 `init()` 执行前，所有**包级别**（看上去像全局变量）的变量都会被初始化完成（常量就更不用说了）

```go
package main

import "fmt"

func main() {
	var (
		A = 1
		B = 2
		C = 3
	)
	fmt.Println(A, B, C)
}

func init() {
	fmt.Println("Init 1")
}

func init() {
	fmt.Println(SomeBottle)
}

// Package-level variables
var SomeBottle int = 114514

/*
输出为：  
Init 1
114514
1 2 3
*/
```

> 除了一些难以用声明形式表达的初始化之外，`init` 还经常用于在程序实际执行开始之前验证或修复程序状态（比如检查必要的文件目录，修复缺失项）

# 11. 方法（Methods）

## 11.1. 指针与值接收器（Receiver）  

❗ **指针类型**（pointer）和**接口类型**（interface）上**不可定义方法**！！  

```go
type MyPointer *int

// ⚠ 编译错误: invalid receiver type MyPointer (pointer or interface type)
func (m MyPointer) test() {
	fmt.Println(*m)
}
```

----

其他**自定义命名类型**都可以用作 receiver，即可以在其上定义方法。

比如可以给 `[]byte` 类型取别名后在其上定义方法：  

```go
type ByteSlice []byte

func (slice ByteSlice) Append(data []byte) []byte {
    return append(slice, data...)
}
```

💡 如果要能**原地修改**，就需要用指针类型作为 receiver：

```go
func (p *ByteSlice) Append(data []byte) {
    *p = append(*p, data...)
}
```
> `ByteSlice` 本身就是引用类型，指向底层的一个数组；`*ByteSlice` 则相当于是引用类型的指针变量，指向一个 `ByteSlice` 实例。通过修改指针变量的值，指向 `append` 返回的实例，以完成原地修改。  

## 11.2. ❗ 自动取地址、解引用

### 11.2.1. 什么是解引用

```go
var p *ByteSlice = &ByteSlice{}

*p // 这就是解引用
```

💡 `*p` 其实就是**解引用**了指针变量 `p`，获取其所指向的值。  

### 11.2.2. ❗ 自动转换机制

* ⚠ 注：为了代码可读性，尽量**还是用 receiver 对应的类型调用**，隐形转换用多了容易混淆。

无论是用了 `ByteSlice` 还是 `*ByteSlice` 作为 receiver 定义方法，都可以这样调用：  

```go
s := ByteSlice{}
var p *ByteSlice = &s

// 无论 receiver 是用 *ByteSlice 还是 ByteSlice，都可以调用
s.Method()
p.Method()
```

----
如果是这样定义：  

```go
func (o ByteSlice) Method()
```

💡 在通过 `p.Method()` 调用时其实会**自动解引用**，相当于这样：`(*p).Method()`  

----
如果是这样定义：

```go
func (p *ByteSlice) Method()
```

💡 在通过 `s.Method()` 调用时会**自动取地址**，相当于这样：`(&s).Method()`

---
### 11.2.3. ❗ 最多只能自动解一级指针

若定义了：  

```go
func (m MyType) Method()
```

且：  

```go
var m MyType
var p **MyType = &&m
```

❗ 是**不可以**通过 `p.Method()`  调用方法的，Go 语言**最多只会自动解一级指针的引用**。

即使像这样也不行：  

```go
type ByteSlice []byte

func (s *ByteSlice) Method() {
	fmt.Println("Can be called on p2")
}

func main() {
	s := ByteSlice{}
	var p1 *ByteSlice = &s
	var p2 **ByteSlice = &p1
	s.Method()
	// 编译出错: p2.Method undefined (type **ByteSlice has no field or method Method)
	p2.Method()
}
```
> `p2` 是多级指针，编译器**不会自动解引用**。

## 11.3. 接口实现

💡 定义了某个接口中的所有方法后就**相当于实现了这个接口**。

比如 `io.Writer` 接口：  

```go
type Writer interface {
	Write(p []byte) (n int, err error)
}
```

可以在上面的 `ByteSlice` 上实现一下：  

```go
func (p *ByteSlice) Write(bs []byte) (n int, err error) {
	*p = append(*p, bs...)
	n, err = len(bs), nil
	return
}
```
> 可以看到这里用了命名的函数返回值，这种语法便于代码阅读。

可以看到 `fmt.Fprintf` 的签名，其接受一个实现 `io.Writer` 的实例作为首个参数：  

```go
func Fprintf(w io.Writer, format string, a ...any) (n int, err error)
```

因此可以这样写：  

```go
s := ByteSlice{}
// 注意这里 Write 方法定义在 *ByteSlice 上，所以是 &s
var w io.Writer = &s
fmt.Fprintf(w, "Hello, %s!", "Somebottle")
fmt.Println(string(s)) // Hello, Somebottle! 
```
> ❗ 注意 `func (p *ByteSlice) Write(bs []byte) (n int, err error)` 是**定义在 `*ByteSlice` 上的**，`ByteSlice` 上并没有 `Write`，因此只有 `*ByteSlice` 实现了接口 `io.Writer`。

### 11.3.1. ❗ 实现接口时接受者（receiver）类型的坑

* ⚠ 注：这里**非常容易和上面调用方法时的自动转换机制搞混**，要区分开！  

1. 如果在 `ByteSlice` 上定义了接口方法，`*ByteSlice` 也会继承，因此 `*ByteSlice` 也实现了接口。

	```go
	type ByteSlice []byte
	
	// Stringer 接口
	type Stringer interface {
	    String() string
	}
	
	// 在 ByteSlice 上定义 String 方法
	func (b ByteSlice) String() string {
	    return string(b)
	}
	
	func main() {
	    var s Stringer
	
	    b := ByteSlice{'H', 'e', 'l', 'l', 'o'}
	    s = b  // ByteSlice 实现了 Stringer 接口
	    fmt.Println(s.String())
	
	    pb := &b
	    s = pb  // *ByteSlice 也实现了 Stringer 接口
	    fmt.Println(s.String())
	}
	```

2. 如果在 `*ByteSlice` 上定义了方法，那么就**只有 `*ByteSlice` 实现了接口**，而 `ByteSlice` 没有实现接口。

	```go
	// 在 *ByteSlice 上定义 String 方法
	func (b *ByteSlice) String() string {
	    return string(*b)
	}
	
	func main() {
	    var s Stringer
	
	    b := ByteSlice{'H', 'e', 'l', 'l', 'o'}
	    // s = b  // 这行会报错，因为 ByteSlice 没有实现 Stringer 接口
	
	    pb := &b
	    s = pb  // *ByteSlice 实现了 Stringer 接口
	    fmt.Println(s.String())
	}
	```


# 12. 接口和其他类型

## 12.1. 接口（Interfaces）

