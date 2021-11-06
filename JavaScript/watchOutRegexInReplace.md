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


