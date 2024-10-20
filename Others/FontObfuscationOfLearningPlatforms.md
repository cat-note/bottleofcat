# 【小记】探探学习平台的字体混淆

正在某学习平台做题，想着把题目复制出来~~和搜索娘~~深入探讨一下，却发现：  

![problem-2024-10-13](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/problem-2024-10-13.png)  

嗯？怎么是一坨火星文？  

实际上有好几个学习平台都引入了这种**字体混淆**机制以防止复制，**打乱了部分汉字 Unicode 码点和字形的对应关系**。这回咱就来折腾折腾，看看这是怎么个事儿。  

![itBurns-2024-10-13](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/itBurns-2024-10-13.png)

## 1. 怎么个混淆法

来到某课堂平台，打开一份作业题。

### 1.1. 找到字体

鉴于混淆机制很有可能是在字体上执行的，咱们先找一下网页载入时是不是顺带载入了一些额外的字体文件。

在浏览器开发者工具的“网络”面板中对“字体”类型的数据进行筛选：  

![findFont-2024-10-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/findFont-2024-10-19.png)  

很快就能找到这个可疑的字体文件 `exam_font_*.ttf`，下载下来盘一盘。

### 1.2. 寻找字体的异常之处

以“针”字为例，其原本的 Unicode 码点为 `0x9488`。

```javascript
'针'.codePointAt(0).toString(16)
// '9488'
```

查看网页中相应的 DOM 元素，可以发现元素文本内容的首个字符为汉字“**户**”，但是页面中渲染出来的却是“**针**”字：

![compareGlyph-2024-10-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/compareGlyph-2024-10-19.png)  

“户”字的 Unicode 码点为 `0x6237`：

```javascript
'户'.codePointAt(0).toString(16)
// 6237
```

这里采用一个**开源的字体编辑器** (https://github.com/ecomfe/fonteditor/) ，来查看刚才下载的字体 `exam_font_*.ttf` 的情况。  

1. 搜索码点 `$9488` 对应的字形：  
   
    ![obfuscatedGlyphCorrespondingTo9488-2024-10-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/obfuscatedGlyphCorrespondingTo9488-2024-10-19.png)  

    找到的字形并不是“针”。

2. 再搜索码点 `$6237` 对应的字形：  

    ![obfuscatedGlyphCorrespondingTo6237-2024-10-19](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/obfuscatedGlyphCorrespondingTo6237-2024-10-19.png)  

    虽然用的是“户”字的码点，但找到的字形却是“针”的。  

另外对比了几对码点和字形，发现都是这种情况：

* “母” (`0x6bcd`) - 对应 `exam_font_*.ttf` 中的字形为 “对” 

    ![obfuscatedGlyphCorrespondingTo6bcd-2024-10-20](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/obfuscatedGlyphCorrespondingTo6bcd-2024-10-20.png)  

* “自” (`0x81ea`) - 对应 `exam_font_*.ttf` 中的字形为 “统” 

    ![obfuscatedGlyphCorrespondingTo81ea-2024-10-20](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/obfuscatedGlyphCorrespondingTo81ea-2024-10-20.png)  

### 1.3. 可能的混淆实现方式

到这里，大致就可以**猜出**混淆的基本原理了：

* **定义**原字形为 $glp_s$，其码点为 $uni_s$
* **定义**混淆后码点为 $uni_t$，$uni_t$ 原本对应的字形为 $glp_t$

对于某个字符，**混淆的操作就是**把字体文件中 $uni_t$ 对应的字形（$glp_t$）篡改为 $glp_s$，且在网页中用码点 $uni_t$ 取代 $uni_s$。

这样一来虽然文本内容上是 $uni_t$ 对应的字符，但是网页中渲染出来的字形却是 $glp_s$。  

----

那么问题来了，这之中有什么规律吗？

咱首先猜测 $uni_s$ 和 $uni_t$ 之间存在一个固定的偏移，即 $|uni_s-uni_t|=offset$，以上面几对码点和字形为例来检验一下：

* “户” (`0x6237`) - “针” (`0x9488`)，计算得到 $offset=\texttt{0x3251}$ 
* “母” (`0x6bcd`) - “对” (`0x5bf9`)，计算得到 $offset=\texttt{0x0fd4}$ 
* “自” (`0x81ea`) - “统” (`0x7edf`)，计算得到 $offset=\texttt{0x030b}$ 

看上去混淆并不只是给 $uni_s$ 加上一个偏移...果然这玩意没有这么容易折腾哇！

## 2. 反混淆的突破口

![strenuous-2024-10-20](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/strenuous-2024-10-20.png)  

在混淆前和混淆后有啥没变呢？没错，就是**字形**，即文字看上去的样子。如果我能根据字形找到其原本对应的码点 $uni_s$，问题不就迎刃而解了！  

要找到字形和原码点，那么就必须要**找到原字体**。

### 2.1. 找到原字体

从 `exam_font_*.ttf` 这个被混淆的 **TrueType 字体**入手，TrueType 字体在结构上主要由几张表组成，其中就包含有一张 `name` 表，记录有字体的版权声明、字体标识名等元数据。在这张表里咱们应该能找到些有用的信息。

* 相关文档：[Font Names Table - TrueType Reference Manual - Apple Developer](https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6name.html)  

采用 [`opentype.js`](https://github.com/opentypejs/opentype.js) 库来读取字体的 `name` 表：  

```javascript
const opentype = require('opentype.js');
const { readFileSync } = require('fs');

// 载入字体
let ttfBuffer = readFileSync('./exam_font_4bdf2fbaceee4b098e767a8149a1b21b.ttf');
const font = opentype.parse(
    // Buffer 转换为 ArrayBuffer
    ttfBuffer.buffer.slice(ttfBuffer.byteOffset, ttfBuffer.byteOffset + ttfBuffer.byteLength)
);

console.log(font.names)
```

![outputOfTTFNameTable-2024-10-20](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/outputOfTTFNameTable-2024-10-20.png)  

幸运的是，字体的几个重要元数据都被被保留下来了。能看到字体名是 `Source Han Sans SC VF`，即**思源黑体**，版本 `2.004`。

> 实际上 Source Han Sans 字体是基于 [SIL 开源字体许可](https://openfontlicense.org/)的，其也要求保留版权声明。

这字体可太出名了。很快啊！嗖的一下我就找到了其发布页，最新版本正好是 `2.004`：  

* https://github.com/adobe-fonts/source-han-sans/releases  

![findVFVersion-2024-10-20](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/findVFVersion-2024-10-20.png)  

下载 VF (Variable Font, 可变字体) 版本，解压后正好能找到 `SourceHanSansSC-VF.ttf`

### 2.2. 检查字形是否一致

字形是某个字符的**混淆后码点** $uni_t$ 和**原码点** $uni_s$ 的桥梁，只有 `exam_font_*.ttf` 中 $uni_t$ 对应的字形和 `SourceHanSansSC-VF.ttf` 中 $uni_s$ 对应的字形（$glp_s$）一致，咱们才有办法把 $uni_t$ 还原到 $uni_s$。

以“针”这个字形为例，其在 `exam_font_*.ttf` 中对应 $uni_t=\texttt{0x6237}$，在 `SourceHanSansSC-VF.ttf` 中对应 $uni_s=\texttt{0x9488}$，通过 `opentype.js` 分别读取两个码点在两个文件中各自对应的字形：  

```javascript
// 此处略去两个字体 examFont, originalFont 的载入代码
// path.commands 决定了字形的绘制方式，因此可以通过对比 path.commands 是否一致来对比字形是否一致
let examCommands = examFont.charToGlyph(String.fromCodePoint(0x6237)).path.commands;
let originalCommands = originalFont.charToGlyph(String.fromCodePoint(0x9488)).path.commands;

// examCommands 和 originalCommands 都是 Object，作为无序结构，没法保证相同的对象能被序列化为相同的 JSON 字串，因此这是不严谨的比较。
// 但是，如果这种情况下输出 true，说明两个对象肯定是相同的
console.log(JSON.stringify(examCommands) === JSON.stringify(originalCommands))
// true
```

除了上面这个例子外，我还对多对码点 $(uni_t,uni_s)$ 进行了验证，此处不多赘述。经过比较，可以发现每一对码点在两个字体文件中对应的两个字形都是**相同**的，也就是说混淆过程并没有在字形上动手脚。

![gotcha-2024-10-20](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/gotcha-2024-10-20.jpg)  

这下算是验证了[上面的猜测了](#13-可能的混淆实现方式)，可让我找着突破口了。

## 3. 一些挑战

咱希望能在浏览器页面中就地对这些文字进行反混淆处理，但这样一来肯定会遇到下面的几个问题。

### 3.1. 每次刷新页面，采用的字体文件都不同 

如果混淆字体 `exam_font_*.ttf` 只有一份，我可以直接在本地处理出一个 Unicode 映射表，在网页中直接把被混淆的字符码点按映射表全部映射回原本的 Unicode 码点即可。  

但平台在这点上也挺鸡贼，每次刷新页面后，都会**随机取出一个** `exam_font_*.ttf` ，对应混淆后的字符码点 $uni_t$ 也**都不同**。  

因此我必须要能在页面中截获字体文件 `exam_font_*.ttf` 的 URL 并就地将字体读取出来进行处理。

### 3.2. 如何唯一地标识字形

上面提到，反混淆的突破口是字形。  

在拿到 `exam_font_*.ttf` 后，我应当能用其中的每一个字形在原字体 `SourceHanSansSC-VF.ttf` 中找到字符的原码点 $uni_s$，为此要建立字形到原码点的映射。而要建立这种映射，**我需要能唯一地标识每个字形**。  

## 4. 字符大恢复术

### 4.1. 获取混淆字体的 URL

被混淆部分的字符只有通过基于特定字体的渲染后才能变成咱们所看到的样子，因此在这些被混淆部分的 DOM 元素上肯定能找到些幺蛾子：  

![findFontFamily-2024-10-20](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/findFontFamily-2024-10-20.png)  

可以看到这些文本被混淆的元素都有一个特定的 `class`，顺着 `class` 对应的样式能找到一个很显眼的字体族 `exam-data-decrypt-font`，这个肯定就是这个平台自定义的一个字体族了。

字体族中的值要么是本机的字体族名，要么是在 CSS 中使用 `@font-face` 指定的自定义字体。这里明显是后者，采用 `@font-face` 载入了字体 `exam_font_*.ttf`。

回到开发者工具-网络面板，搜索 `"exam_font"`，看看这个字体是怎么被载入的：  

![findHowItWasLoaded.p-2024-10-20](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/findHowItWasLoaded.p-2024-10-20.png)  

可以发现这个学习平台的页面中的**被混淆段落是动态载入的**，响应的 JSON 中还包含 `font` 这个字段，其中正好就是 `exam_font_*.ttf` 的 URL。在拿到了字体 URL 后才有办法建立基于 `@font-face` 的样式，因此**字体族相关的样式表肯定也是动态载入页面**的，也就是说我应该能在 DOM 中找到相应的元素。  

在开发者工具-元素面板中搜索字体族名 `"exam-data-decrypt-font"`：  

![findOutHowItWasInserted.p-2024-10-20](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/findOutHowItWasInserted.p-2024-10-20.png)  

嘿，老伙计，瞧瞧咱们发现了什么！果然相关的样式是通过内部样式表 `<style></style>` 来动态插入的，这样一来我就可以在 JavaScript 中定位到这个元素，并获取到字体 URL 了：  

```javascript
const styleElems = document.querySelectorAll('style');
// 和混淆字体相关的样式
let fontStyle = '';
for (let s of styleElems) {
    // 正则表达式，小子！
    if (/font-family:\s*\"exam-data-decrypt-font\"/.test(s.innerHTML)) {
        // 先找到和 exam-data-decrypt-font 相关的 font-face 样式
        fontStyle = s.innerHTML;
    }
}
let matches = fontStyle.match(/url\("(.+?)"\)/);
// 被混淆字体的 URL
let obfuscatedFontUrl = matches[1];
```

### 4.2. 唯一地标识每个字形



...