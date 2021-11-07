# JavaScript正则表达式replace的一个坑

经常听大家说JavaScript是魔法语言，咱却没有什么深刻体会。直到这回踩到这个坑，我终于醒悟了，JavaScript果然来自霍格沃茨！  

![trymagic_smaller-2021-11-06](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/trymagic_smaller-2021-11-06.jpg)  

## 0x00 踩到坑  

昨天咱经过一番考虑后决定将 [Python正则表达式细节小记](https://www.cnblogs.com/somebottle/p/python_notes_of_regex.html) 这篇笔记发到个人博客上。选好文章音乐，复制markdown内容...发布！  

按照惯例我检查了一下发布后的文章内容，然后就见到了一个奇怪的现象：  

![issuehappens-2021-11-06](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/issuehappens-2021-11-06.png)  

文章内容到一半的时候全被替换成 **模板里的HTML** 了...

之前调试博客的时候从没遇到过这个问题，我一时就有点摸不着头脑( >﹏<。)，但没关系，比对一下原文档就应该知道问题在哪了：  

![comparison-2021-11-06](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/comparison-2021-11-06.png)  

很明显能发现是从```$```开始被替换了，我心里咯噔一下：怕不是和正则表达式有关！不过在排查正则表达式之前我去改了一下博客后台部分的代码：  

![modifiedcode-2021-11-06](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/modifiedcode-2021-11-06.jpg)  

结果问题仍然存在，我接着还拿着这样的内容片段进行**复现**，但没能成功（```$```并没有被替换成其他内容）：  

```
test$

 $ 
test$
```

## 0x01 坑在哪  

到最后，我还是怀疑回了正则表达式，但想来想去还是摸不着头脑，正则表达式和用作替换的字符串有什么关系，```$```不是用在正则表达式里的吗？而且为什么刻意用```$```去复现又不行呢？  

![baldscratch-2021-11-07](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/baldscratch-2021-11-07.jpg)

实在不行只能去求助一下某搜索引擎了：  

![googlesearch-2021-11-07](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/googlesearch-2021-11-07.png)  

不查不知道，一查吓一跳，看到有老哥提到了replace函数接收的字符串不仅仅是字符串，我赶紧去MDN查了一下：  

![syntaxofreplace-2021-11-07](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/syntaxofreplace-2021-11-07.jpg)  

原来用作替换的字符串内能包括一些特殊的变量名（不过这个特性不止是JavaScript有，其他支持正则表达式的语言也多多少少支持，详细看[0x04-事后](#0x04-事后)）  

| 变量名 | 代表的值 |
|:---:|:---:|
| $$ | 插入一个 "$"。 |
| $& | 插入匹配的子串。 |
| $` | 插入当前匹配的子串左边的内容。 |
| $' | 插入当前匹配的子串右边的内容。 |
| $n | 假如第一个参数是 RegExp对象，并且 n 是个小于100的非负整数，那么插入第 n 个括号匹配的字符串。提示：索引是从1开始。如果不存在第 n个分组，那么将会把匹配到到内容替换为字面量。比如不存在第3个分组，就会用“$3”替换匹配到的内容。 |
| $<Name> | 这里Name 是一个分组名称。如果在正则表达式中并不存在分组（或者没有匹配），这个变量将被处理为空字符串。只有在支持命名分组捕获的浏览器中才能使用。 |

表格原地址：  https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/String/replace#%E4%BD%BF%E7%94%A8%E5%AD%97%E7%AC%A6%E4%B8%B2%E4%BD%9C%E4%B8%BA%E5%8F%82%E6%95%B0  

现在再回去看文章markdown内容，有一部分我是这样写的：  

```markdown
到了这里，我发现老师说的在```[]```中**被当作普通字符**的元字符只是一部分罢了，主要是 ```*```，```?```，```+```，```{}```，```()```，```$``` 这些元字符。
```

毫无疑问其中的``` $\` ```就被替换为了匹配**字串左边的内容**，也就是模板的前面一部分，才导致文章被处理成这样。  

可以说真的非常魔法了，万万没想到JavaScript竟然在待替换字符串这里内置了一些```$```变量名的用法。要是我没有想着把这篇小记发到个人博客上，说不定还得要好一阵子才能发现这个问题。  

## 0x02 解决方法  

解决方法其实很简单，```str.replace(regexp|substr, newSubStr|function)```的第二个参数是可以接受一个函数的，而这个函数的 **返回值** 就被直接用作匹配项替换了，而不是先寻找一遍```$```变量名。  

比如我原来是这样写的：

```javascript
str.replace(new RegExp('\\{\\[' + from + '\\]\\}','gi'), to);
```

那么我用箭头函数改写一下就行了：  

```javascript
str.replace(new RegExp('\\{\\[' + from + '\\]\\}','gi'), ()=>to);
```

其实就相当于：  

```javascript
str.replace(new RegExp('\\{\\[' + from + '\\]\\}','gi'), function(){
    return to;
});
```

关于这个函数传入的参数可以看MDN文档这里的 [指定一个函数作为参数](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/String/replace#%E6%8C%87%E5%AE%9A%E4%B8%80%E4%B8%AA%E5%87%BD%E6%95%B0%E4%BD%9C%E4%B8%BA%E5%8F%82%E6%95%B0)  

![usefuncasarg-2021-11-07](https://cdn.jsdelivr.net/gh/cat-note/bottleassets@latest/img/usefuncasarg-2021-11-07.jpg)  

## 0x03 教训  

以后写JavaScript代码的时候还是不能掉以轻心了，说不定在哪个角落还有我不太清楚的魔法。遇到不会或者不清楚的一定要多查文档，不然一旦写进项目里可能就会成为一个遗留的潜在问题。(ノへ￣、)   

## 0x04 事后  

经 [@Ajanuw](https://github.com/januwA) 老哥提醒，不止是JavaScript的正则替换中待替换字串(replacement)有这种用法，例如：  

* [Python的re.sub](https://docs.python.org/zh-cn/3/library/re.html#re.sub)  
* [PHP的preg_replace](https://www.php.net/preg_replace/)  

不过```$&``` , ``` $` ``` , ``` $' ```这种写法似乎是JavaScript独有的了，这回也是正好踩在这上面了（还是好想吐槽JavaScript的这个特殊变量配上``` ` ``` , ``` & ``` , ```'```的设计 Σ( ° △ °|||)︴  