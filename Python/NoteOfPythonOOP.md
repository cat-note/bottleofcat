# Python面向对象小备忘

最近学到面向对象了，感觉到Python这方面的语法也有点神奇，这里专门归纳一下Python面向对象中**我觉得比较重要**的笔记。  

* 本文目前有的内容：实例属性和类属性的访问，使用@property修饰器  

![thumbup_FpdhDFbCkK9NSiiqHnDTWhYuHHPR-2021-11-24](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/thumbup_FpdhDFbCkK9NSiiqHnDTWhYuHHPR-2021-11-24.png)  

## 实例属性和类属性的访问

<details>
<summary>
展开阅读
</summary>

-------

Python里面向对象编程的**类属性和实例属性**与普通情况下**全局变量和局部变量**还是有相似之处的：

1. 我们可以通过实例名访问**实例属性和类属性**，就像上面例子中的```new_instance.test_var```和```new_instance.pub_var```。就像局部作用域能访问局部变量和全局变量。  

2. 我们可以通过**创建赋值**让实例对象有 **与类属性同名** 的属性，比如```new_instance.pub_var = 'own property'```就会在new_instance本身创建一个属性，从而屏蔽 **通过实例名对于类属性的访问**。而在没有global关键字的情况下，局部变量在局部作用域被创建赋值后也会屏蔽同名的全局变量。

对于第2点可以试试通过实例名来删除类属性：  

```python
class Test:
    pub_var = 'Hello' # 类属性

    def __init__(self):
        pass


new_instance = Test()
print(new_instance.pub_var)  # Hello
del new_instance.pub_var  # AttributeError: pub_var
```

很明显通过实例名是无法删除类属性```pub_var```的，但如果我们给实例创建赋值一个同名属性呢？  

```python
# 紧接上面例子
new_instance = Test()
print(new_instance.pub_var)  # 此时访问了类属性，输出：Hello 
new_instance.pub_var = 'Hello World'
print(new_instance.pub_var)  # 此时访问的是实例自身属性，输出：Hello World
del new_instance.pub_var  # 删除了实例自身属性，一切正常
print(new_instance.pub_var)  # 实例在自身找不到同名属性了，就又指向了类属性，输出：Hello
del Test.pub_var # 可以通过类名删除类属性
print(new_instance.pub_var) # 在实例自身和类里都找不到pub_var属性了，返回no attribute异常
```

可以看出通过实例名可以删除实例自身的属性，当实例**在自身上**找不到属性时，就会**转而寻找类属性**。类比局部变量和全局变量，局部变量也是先在局部作用域找，如果没找到就去找同名的全局变量。

通过**类名**，可以在很多地方访问到类属性，并可以进行修改（比如在实例的方法函数里就可以直接通过类名访问。

![jiiiii-2021-11-24](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/jiiiii-2021-11-24.png)  

</details>

## 使用@property修饰器  

<details>
<summary>
展开阅读
</summary>

--------

```python
class Test:
    def __init__(self, val):
        self.__secret_value = val

    def my_value(self):
        return self.__secret_value


new_instance = Test(233)
print(new_instance.my_value())
```

上面例子中我们将类**实例化**为对象 ```new_instance``` (用类创建对象)，该对象得到了```my_value()```方法，同时Python自动调用了```__init__``` 给 ```new_instance``` 绑定了属性```__value```并进行赋值。  

当我们要获得值的时候就要调用实例对象```new_instance```的```my_value()```方法：  

```python
print(new_instance.my_value())
```

如果 **使用了@property修饰器** 呢？  

```python  
class Test:
    def __init__(self, val):
        self.__secret1value = val

    @property
    def my_value(self):
        return self.__secret1value


new_instance = Test(233)
print(new_instance.my_value) # 末尾不用再加()了，因为这不是一个可调用的方法，而是一个属性
```

**@property**的作用正如其名，将**实例的方法**转换为了**属性**，上面例子中原本的方法```my_value()```被修饰后只用访问对应的属性名```my_value```我们就能获得同样的返回值。  

这个修饰器本质上其实**仍然是对方法的调用**，咱改一下上面的例子：  

```python
class Test:
    def __init__(self, val):
        self.__value = val

    @property
    def my_value(self):
        print('Here I am.') # 调用方法的时候输出'Here I am.' 
        return self.__value
new_instance = Test(233) # 实例化的时候没有任何输出
print(new_instance.my_value) # 访问这个属性时实际上内部调用了my_value()的方法，因为输出了 'Here I am.' 和 233 
```  

再进一步想想，```new_instance.my_value```这个属性取的其实就是原本```my_value()```方法的```return```返回值。  

接着再钻一下，原本```my_value()```这个方法 **只是读取了属性```__value```并返回** ，并没有进行修改。没错，这也意味着：  

**被@property修饰后产生的属性是只读的**  

![umm-2021-11-24](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/umm-2021-11-24.png)  

可以试试修改这个属性：

```python
new_instance.my_value = 450
# AttributeError: can't set attribute
```

很明显，```my_value```现在对于```new_instance```而言是**只读**属性。由此，在用户**不知道**原方法```my_value()```操作的私有属性时能起**一定的保护作用**。  

------
* 作为实例对象的一个**属性**，其和方法有一定的区别，我们调用实例对象的**方法**时候是**可以传参**的，但属性不行，这意味着```@property```修饰的方法只能有```self```一个参数（否则访问属性的时候会报参数缺少的异常）。

* 另外一个**实例对象**是**有其他属性**的，@property等修饰器修饰的方法也好，普通的实例方法也好，一定**不要和已有的属性重名**。举个例子：  

    ```python
    class Test:
        def __init__(self, val):
            self.__secret1value = val
            self.my_value = 'pre'

        @property
        def my_value(self):
            print('Here I am.')
            return self.__secret1value


    new_instance = Test(233)
    # self.my_value='pre' -> AttributeError: can't set attribute
    # 其实从这里还能看出来，@property修饰先于实例初始化进行，导致抛出的异常是无法修改属性值
    ```

------
上面我们尝试修改了@property修饰而成的属性，但返回了```can't set attribute```。其实是因为咱**没有定义这个属性的写入(setter)方法**.  

需要修改这个@property属性的话，我们就需要请出**附赠的修饰器**```@已被修饰的方法名.setter```了：

```python
class Test:
    def __init__(self, val):
        self.__secret1value = val

    @property
    def my_value(self):
        return self.__secret1value

    @my_value.setter # [被@property修饰的方法名].setter
    def my_value(self, val2set): # 这里的方法仍然是my_value
        self.__secret1value = val2set


new_instance = Test(233)
print(new_instance.my_value) # 233
new_instance.my_value = 450 # 此时这个属性有修改(setter)的方法了，我们可以修改它
print(new_instance.my_value) # 450
```

和```@property```修饰的方法不同，```@已被修饰的方法名.setter```修饰的方法除了```self```外**还可以接受第二个**参数，接收的是**修改的值**。在上面例子中我将这个形参命名为了```val2set```。  

![ugh-2021-11-24](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/ugh-2021-11-24.png)  

有了读和写，还差什么呢——**删**！  

和setter类似，@property修饰器还赠有```@已被修饰的方法名.deleter```修饰器，其修饰的方法和@property修饰的一样都**只接受一个参数```self```**：

```python
class Test:
    def __init__(self, val):
        self.__secret1value = val

    @property
    def my_value(self):
        return self.__secret1value

    @my_value.deleter # [被@property修饰的方法名].deleter
    def my_value(self):  # 注意这里只接受一个self参数
        del self.__secret1value


new_instance = Test(233)
print(new_instance.my_value)  # 233
try:
    new_instance.my_value = 450
except:
    print('Set failed.')  # Set failed.
del new_instance.my_value
print(new_instance.my_value)
# AttributeError: 'Test' object has no attribute '_Test__secret1value'
```

这个例子中咱没有定义```my_value```属性的```setter```方法，所以其无法被修改。但因为定义了```deleter```方法，在用```del```对属性进行移除的时候会通过deleter**调用原方法**，原方法中用del去删掉实例对象自己的私有属性，达成删除的目的。  

------

总结一下```修饰器@property```相关的着重点：

1. ```@property```让实例方法作为**属性**被访问。

2. 这一类修饰器能在**一定程度上**保护实例的**私有属性**不被随意修改（之所以是说一定程度上，是因为一旦用户知道了私有属性名就可以用```_类名__私有属性名```进行访问，Python，很神奇吧 (￣ε(#￣)☆╰╮o(￣皿￣///)） 。 

3. 实例的方法名不要和**自身其他方法或属性**重名。  

4. ```@property```和```@已被修饰的方法名.deleter```修饰的**方法**只能接受```self```一个参数；而```@已被修饰的方法名.setter```修饰的方法除了```self```外可以接受第二个参数作为被修改的值。  

-------

![surprised-2021-11-24](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/surprised-2021-11-24.png)  

除了```@property```这种修饰器写法外，Python还提供了内置方法 ```property(getter,setter,deleter,doc)``` 来达成相同的效果：  

<a id='accessPropertyThroughClassName'></a>

```python  
class Test:
    pub_var = 'Hello'

    def __init__(self, val):
        self.__secret1value = val
        self.test_val = 'World'

    def __getter(self):
        return self.__secret1value

    def __deleter(self):
        del self.__secret1value

    my_value = property(__getter, None, __deleter)


new_instance = Test(233)
print(new_instance.test_var) # World (通过实例名访问实例属性)
print(Test.pub_var)  # Hello (尝试通过类名访问类属性)
print(new_instance.pub_var)  # Hello (尝试通过实例访问类属性)
print(Test.my_value) # <property object at 0x0000025990BC5770> （这个其实也是类属性，通过类名能访问到）
print(new_instance.my_value)  # 233 （通过实例名访问类属性，间接调用了__getter，绑定上了self
```

```property(getter,setter,deleter,doc)```接受的四个参数分别为```读方法```，```写方法```，```删方法```和```描述信息```，这四个参数都是可以留空的，当**getter也留空**时访问这个属性会提示```unreadable attribute```。

![baldscratch-2021-11-24](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/baldscratch-2021-11-24.jpg)

通过上面的例子可以看出，```property```方法返回的是```类属性```，而**实例对象是可以访问到类属性**的，所以当我们访问```new_instance.my_value```的时候就是在绑定实例的基础上访问getter方法，其他的写、删操作原理一致。  

------------

再回去看[实例属性和类属性的访问](#%E5%AE%9E%E4%BE%8B%E5%B1%9E%E6%80%A7%E5%92%8C%E7%B1%BB%E5%B1%9E%E6%80%A7%E7%9A%84%E8%AE%BF%E9%97%AE)，加上这个内置方法```property()```，于是就有了奇妙的骚操作：  

```python
class Test:
    def __init__(self, val):
        Test.test_var = property(lambda self: val) # 闭包写法


new_instance = Test(233)
print(new_instance.test_var) # 233
```

1. 这个操作中首先利用了一个匿名函数**充当getter方法**，传入```property```第一个参数，然后property会返回一个类属性。

2. 因为在实例方法里我们也能访问到类名，于是我们将这个property类属性赋值给```Test.test_var```，```test_var```便是一个名副其实的类属性了。

3. 通过实例名```new_instance```能访问到类属性```test_var```。

4. 从[之前的这个例子](#accessPropertyThroughClassName)可以看出，当我们通过类名访问property属性时**只会返回一个property object**，但是通过**已创建的实例对象**来访问就能**间接调用getter**方法。  

5. 在上面过程中，始终没有```new_instance```的**自身属性出现**，取而代之我们利用**闭包**机制保护了创建实例时传入的值，我们**完全无法**通过实例名**修改**或者**删除**```test_var```这个属性，真正将其保护起来了。  

![oh..no-2021-11-24](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/oh..no-2021-11-24.png)  

当然，别让用户知道了**类名**，不然一句```Test.test_var = xxx```直接破防(,,#ﾟДﾟ)。

</details>


To be updated......