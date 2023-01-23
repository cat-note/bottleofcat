# 【速记】C++ STL自定义排序

这篇笔记咱日后应该还会进行补充。

## 关于sort的比较函数

```cpp
void sort( RandomIt first, RandomIt last, Compare comp );
```

STL的algorithm库中的`sort`函数，可以接受一个`comp`函数作为第三个参数，用来指定排序的规则。

### 自定义sort比较函数

`comp(a,b)`函数的返回值是一个`bool`值，当返回值为`true`时**不改变元素顺序**，反之则需要调换元素。

可以把其中的`a`看作序列中**前一个位置的元素**，`b`看作**后一个位置的元素**:

* 如果`a < b`的时候`comp(a,b)=true`，那么`a`就会被放在`b`前面，排序呈升序。
* 如果`a < b`的时候`comp(a,b)=false`，那么`b`就会被放在`a`前面，排序呈降序。

> 也就是说如果`a < b`时有`comp(a,b)=true`成立，就是期待程序**把小元素放在前面**。

```cpp
#include <iostream>
#include <algorithm>

using namespace std;

bool ascending(int a, int b) // 升序排序，让小元素放在前面
{
    // 把a看作序列中前一个元素，b看作后一个元素
    return a < b; // 如果返回true就说明a<b成立，a是较小的元素
};

bool descending(int a, int b) // 降序排序，让大元素放在前面
{
    // 把a看作序列中前一个元素，b看作后一个元素
    return a > b; // 如果返回true就说明a>b成立，a是较大的元素
};

int main()
{
    int test[10] = {9, 4, 2, 5, 1, 7, 3, 6, 8, 10};
    sort(test, test + 10, ascending);
    for (int i = 0; i < 10; i++)
        cout << test[i] << " ";
    // Ouput: 1 2 3 4 5 6 7 8 9 10
    cout << "\n";
    sort(test, test + 10, descending);
    for (int i = 0; i < 10; i++)
        cout << test[i] << " ";
    // Ouput: 10 9 8 7 6 5 4 3 2 1
    return 0;
}
```

### 缺省sort比较函数

默认情况下，`sort`函数会**使用`<`运算符**作比较。(也就是默认**升序**排序)

这个时候如果要自定义排序规则，可以**重载`<`运算符**。

```cpp
#include <iostream>
#include <algorithm>

using namespace std;

struct Node
{
    int data;
    bool operator<(const Node &obj)
    {
        // a>b的时候才返回true, 期待a是较大的元素。
        // 把较大的元素放在前面，降序排序
        return data > obj.data; 
    }
};

int main()
{
    Node test[10] = { {1}, {4}, {2}, {5}, {9}, {7}, {3}, {6}, {8}, {10} };
    sort(test, test + 10);
    for (int i = 0; i < 10; i++)
        cout << test[i].data << " ";
    // Output: 10 9 8 7 6 5 4 3 2 1
    return 0;
}
```

## 自定义容器内元素排序

`priority_queue`、`map`、`set`这些元素有序的容器都可以自定义比较规则，**常用**的有以下两种途径:

1. 自定义比较器类(Comparator Class)

2. 重载运算符(缺省比较器类)

### 自定义比较器类

在cppreference页面可以看到，这类元素有序的容器都有个默认的`Compare`比较器类。比如[`priority_queue`](https://zh.cppreference.com/w/cpp/container/priority_queue)的声明:

```cpp
template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class priority_queue;
```

多观察几个容器的声明，能发现默认的`Compare`比较器类都是`std::less`，定义大概是这样:

```cpp
template<class T> struct less
{
    // 这里其实是重载了()运算符，因此其对象可以像函数一样被调用
    bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs < rhs;
    }
};
```

标准库中还有另一个比较器类`std::greater`，定义如下:

```cpp
template<class T> struct greater
{
    // 这里其实是重载了()运算符，因此其对象可以像函数一样被调用
    bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs > rhs;
    }
};
```

-------------

从**数组排序**的角度看，`lhs`就是序列中**前一个位置**的元素，`rhs`就是**后一个位置**的元素: 

1. `std::less`中，`lhs < rhs`成立的时候返回`true`，期待`lhs`是较小的元素，也就是**前一个元素是较小**的，因此是**升序**排序。

2. `std::greater`中，`lhs > rhs`成立的时候才返回`true`，期待`lhs`是较大的元素，也就是**前一个元素是较大**的，因此是**降序**排序。

很显然，和`sort`函数的默认比较函数一样，有序容器都是**默认升序排序**(`std:less`)的。

因为重载了运算符`()`，我实际上可以**把【比较器类】的对象**作为**比较函数**传入`sort`方法:  

```cpp
#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
    int test[10] = {4, 1, 3, 7, 5, 8, 2, 9, 6, 10};
    // 注意这里创建了greater对象，sort函数调用greater对象的()运算符重载方法
    sort(test, test + 10, greater<int>()); 
    for (int i = 0; i < 10; i++)
        cout << test[i] << " ";
    // Output: 10 9 8 7 6 5 4 3 2 1
    return 0;
}
```

💡 值得注意的是，这里首先调用的是`greater`类的**默认构造方法**，返回一个**对象**并传递给`sort`函数。`sort`函数内部调用`对象(a,b)`时调用的是对象的**运算符`()`重载方法**来进行比较。

> 这里重载了`()`运算符其实是构造了一个“**伪函数**”，也就是可以把类的对象作为**函数**来使用。

模仿`greater`和`less`模板类的定义，我们也可以**自己定义比较器类**:

```cpp
#include <iostream>
#include <algorithm>

using namespace std;

struct Node
{
    int data;
};

struct MyGreater // 自定义比较器类
{
    // a是序列中前一个元素，b则是后一个元素
    bool operator()(const Node &a, const Node &b) const
    {
        return a.data > b.data; // 前一个元素 > 后一个元素时才返回true
    }
};

int main()
{
    Node test[10] = { {4}, {1}, {3}, {7}, {5}, {8}, {2}, {9}, {6}, {10} };
    sort(test, test + 10, MyGreater());
    for (int i = 0; i < 10; i++)
        cout << test[i].data << " ";
    // Output: 10 9 8 7 6 5 4 3 2 1
    return 0;
}
```
-----------------

### 优先队列的比较器类

在这几种STL容器中，优先队列`priority_queue`的元素比较规则是略显“另类”的:

1. 默认情况下(`std::less`类)，优先队列中的元素出队后是呈**降序**排列的，即**大的元素在队头**，是一个**大根堆**。

2. 如果使用`std::greater`类，则优先队列中的元素出队后是呈**升序**排列的，即**小的元素在队头**，是一个**小根堆**。

这里和之前的排序不同的地方就在于：**比较方法中形参的意义不同**。

拿`std::greater`举例: 
```cpp
template<class T> struct greater
{
    // 这里其实是重载了()运算符，因此在使用的时候可以像函数一样调用
    bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs > rhs;
    }
};
```
- 在`sort`函数、`map`、`set`容器中，`lhs`代表**序列中前一个元素**，`rhs`代表**序列中后一个元素**。

- 而在`priority_queue`中，`lhs`代表**新插入的节点**，`rhs`代表**这个节点的父节点**。

    > `lhs`>`rhs`时就是期望**子节点大于父节点**，即构成**小根堆**，因此堆顶元素总是堆中最小的，所以从优先队列中取出的元素是**从小到大**的，即**升序**排列。

    > - 注：往堆中插入新节点时是插入在**最后一个叶子节点**的位置的。

### 重载运算符

`sort`函数中可以**缺省**排序函数。在**创建容器对象**时，我们也可以缺省比较器类。

在缺省比较器类的情况下，STL容器默认采用`std::less`模板类来进行比较：

1. 默认**升序**排列。

2. 对于**优先队列**来说，默认出队元素呈**降序**排列。

`std::less`类的重载方法中一样也是调用了对象的`<`运算符进行比较，因此我们也可以**重载**`<`运算符来实现自定义的比较规则。

```cpp
#include <iostream>
#include <algorithm>

using namespace std;

struct Node
{
    int data;
    // std::less中调用了这里的<运算符重载方法
    bool operator<(const Node &b) const
    {
        return data > b.data; // a>b时返回true，期待前一个元素更大，即降序排列
    }
};

int main()
{
    Node test[10] = { {4}, {1}, {3}, {7}, {5}, {8}, {2}, {9}, {6}, {10} };
    sort(test, test + 10);
    for (int i = 0; i < 10; i++)
        cout << test[i].data << " ";
    // Output: 10 9 8 7 6 5 4 3 2 1
    return 0;
}
```

## 总结

0. 把比较函数的**形参**`(a,b)`中的`a`看作**序列中前一个位置的元素**，`b`看作**序列中后一个位置的元素**，方便理解。

1. 无论是`sort`函数的比较函数`comp(a,b)`还是比较器类的重载方法`operator()(a,b)`: 

    - 返回`true`时，**不会**改变**a和b的顺序**；
    - 而返回`false`时，会改变`a`和`b`的顺序。

    > 比如在**默认**情况下实现**升序**排列，a在序列中的位置小于b且满足条件`a<b`时，返回`true`，不会改变a和b的顺序；而当a在序列中的位置大于b时则不满足条件，会改变a和b的顺序。

2. 在缺省比较函数/比较器类的时候，可以活用**待比较对象**的**运算符重载方法**。具体重载哪个运算符需要根据**具体的实现**来确定。

    > 比如容器默认采用比较器类`std::less`，其内部调用**待比较对象**的`<`运算符。

3. 优先队列容器`priority_queue`的比较方法的形参含义是不同的，重载方法`operator()(a,b)`中前一个元素`a`指的是**新插入的元素**，而后一个元素`b`指的是这个元素的**父节点**。

    * 这里仍然是比较方法返回`true`时不会改变元素顺序。

    > 比如在**默认**情况下，维持的是**大根堆**的堆序性。若新插入的元素`a`的值小于父节点`b`的值，满足条件`a<b`，则返回`true`，不会改变a和b的位置；而当新插入的元素`a`的值大于父节点`b`的值，不满足条件，会改变a和b的位置。

## 参考文章

https://blog.csdn.net/sandalphon4869/article/details/105419706