# 【小记】图片托管从又拍云迁移到腾讯云 EO + COS（兼容图片处理参数）

2026 年 9 月 4 日，我正写着半年总结的博文呢，突然发现托管在又拍云上的图片无法访问了，去 itdog 测了一下发现真的是又拍云的节点炸完了，之前从未遇到这种情况。

去又拍云联盟的用户群里问了一下，正好认识的博主 Ghost_chu 在那，他提醒我尽量把又拍云额度用完，企查查上又拍云已经是“限制高消费”状态。去 [L 站看了一下](https://linux.do/t/topic/2817601)，发现也有佬友反馈了又拍云问题无人处理的情况，看起来就像是要跑路的样子...明明年初我申请又拍云联盟还有人处理呢...十年前如日中天的又拍云，如今落得如此地步，真的令人唏嘘啊。  

![sob](./MigrateFromUpyunToEO/images/sob.gif)  

话说回来，我博客图片还得迁移一下。正好腾讯云有免费的 EdgeOne 套餐，且也给了老用户长期免费的对象存储 (COS) 资源包，我决定迁移到腾讯云这边了。这篇小记就记录一下迁移过程中的小巧思吧。

## 1. 发现问题

虽然无法通过 URL 访问图片了，但好在 FTP 还是能登录上去的（支持 S3 协议的客户端应该也还能用），我火速把所有的图片都下载下来了，总共有 500+ MiB （这样看写博客这十年其实也没有用多少空间）。  

迁移到腾讯云对象存储桶是很简单的事。然鹅，我很快就发现了一个问题，我在博文中**大量使用了又拍云的图片处理功能**，即 URL 末尾有类似 `!/scale/50` 这样的参数，而迁移到腾讯云这边是没有办法直接使用的。我希望能保持所有现存图片 URL 不变的情况下进行迁移。

## 2. 分析问题

为了确认该问题的影响面积，我拉取了博客仓库并利用了 Pi Agent 编写脚本对我博客的所有博文进行扫描:  

```text
目前你正处在我的博客仓库下，博文都写在 .html 文件中。  

我在博文中大量使用了又拍云的图片处理功能，即图片 URL 末尾有类似 `!/scale/50` 这样的参数，同时我也可能不止使用了 `scale`，还有 `rotate`, `flip` 等处理，可能也有 `!/scale/50/rotate/90` 这样的组合。  

请你编写 Python 脚本对所有博文进行扫描，统计这种参数出现的次数，影响的博文面积。同时也统计 `scale`、`rotate` 等参数各自的出现次数。 

**不要修改任何仓库内容**，统计后输出结果即可。  
```

<details>

<summary>Agent 排查结果</summary>

### 总览

| 指标 | 数值 |
|---|---|
| HTML 文件总数 | 310 |
| 其中博文文件（`post-*.html` / `bottle-*.html`） | 287 篇 |
| 含参数 URL（`!` 链） | 400 条 |
| 操作实例总数 | 406 处 |
| 受影响 HTML 文件 | 31 个 / 310 个（10.0%） |
| 受影响博文 | 30 篇 / 287 篇（10.5%） |
| 受影响图片 URL | 400 次引用，去重后 400 个 |

### 各操作出现次数与取值分布

| 操作 | 次数 | 取值分布 |
|---|---|---|
| `scale` | 397 | 30×172, 40×120, 20×36, 60×26, 50×20, 10×6, 70×6, 35×5, 80×3, 41×1, 25×1, 15×1 |
| `rotate` | 9 | 270×4, 90×4, auto×1 |
| 其它（flip 等） | 0 | — |

### 链形态分布（共 19 种）

| 形态 | 条数 |
|---|---|
| `!/scale/30` | 170 |
| `!/scale/40` | 119 |
| `!/scale/20` | 34 |
| `!/scale/60` | 26 |
| `!/scale/50` | 19 |
| `!/scale/10` | 6 |
| `!/scale/70` | 6 |
| `!/scale/35` | 5 |
| `!/scale/80` | 3 |
| `!/scale/20/rotate/270` | 2 |
| `!/rotate/90` | 2 |
| `!/scale/41` | 1 |
| `!/scale/25` | 1 |
| `!/rotate/auto` | 1 |
| `!/rotate/90/scale/40` | 1 |
| `!/rotate/90/scale/50` | 1 |
| `!/rotate/270/scale/30` | 1 |
| `!/scale/30/rotate/270` | 1 |
| `!/scale/15` | 1 |

即 400 条链中：391 条只缩放，3 条只旋转，6 条为组合写法（`rotate→scale` 与 `scale→rotate` 各 3 条）。

</details>

---

腾讯云这边对象存储集成了数据万象 (CI) 服务，是[支持通过 ImageMogr2](https://cloud.tencent.com/document/product/460/36540) 来处理图片的，好消息是它支持类似又拍云的组合式图片变换，但坏消息是它的 URL 参数模式与又拍云的不一样。不过咱发现 EdgeOne 这边也有边缘函数的免费额度，我完全可以**利用边缘函数做一个兼容层**，把又拍云的 URL 参数转换为腾讯云 COS 数据万象支持的 ImageMogr2 参数，这样看上去就可以完美解决了。  

但是！事情并没有这么简单，从 [EO 套餐的说明文档](https://cloud.tencent.com/document/product/1552/94165)可以看到边缘请求数和 CPU 耗时都有限制:  

![EO 套餐限制](./MigrateFromUpyunToEO/images/limit_of_edge_funcs.png)  

更糟的是，根据 EO 的请求处理顺序说明（见文档：[请求处理顺序](https://cloud.tencent.com/document/product/1552/84772)、[边缘函数概述](https://cloud.tencent.com/document/product/1552/81344)），边缘函数执行时是**在查询节点缓存之前**的。也就是说，如果我直接用边缘函数去做转换，那么**每次访问带处理参数的图片 URL 时都会耗费掉边缘函数执行额度**，这一部分无法利用 EdgeOne 缓存！  

![朴素方案](./MigrateFromUpyunToEO/images/naive_edgeone.drawio.png) 
> **朴素**的 EdgeOne 方案 

从上图中可以看到有四部分可能产生费用或被限制:  

1. 边缘函数（每次请求都会执行，消耗免费额度，消耗完了就无法处理图片了）；
2. COS 请求费用（缓存未命中时。老用户有每个月的免费额度）；
3. 数据万象图像处理费用（缓存未命中时。按原图文件体积计费，每个月免费 10 TiB）；
4. 数据万象 CDN 回源流量费用，按处理后图像体积计费。没错，这个是要额外算的，可以见文档：[数据万象流量费用](https://cloud.tencent.com/document/product/460/58122/)。  

其中 1 是固定开销，300 万次请求后我们就完全没有边缘函数额度了，带又拍云风格参数的请求会原样透传到 COS，响应 404，这样就会导致所有带处理参数的图片无法访问。

**能不能进一步缓解边缘函数的开销呢**？开通小脑筋后我想到了一个神奇的方案: **套娃**，**在 EdgeOne 服务的基础上再套一层 EdgeOne 服务**，更高层的 EdgeOne 服务就可以根据 URL 缓存经过边缘函数处理后的响应了。方案示意图如下:  

![套娃方案](./MigrateFromUpyunToEO/images/nested_edgeone.drawio.png)  
> **套娃方案**，这样甚至有些多级缓存架构的意思了  


<!-- 示意图 -->

## 3. 解决问题

### 3.1. 配置内层 EdgeOne 服务

首先先在数据万象 (CI) [控制台](https://console.cloud.tencent.com/ci)开通服务，然后绑定博客图像资源的存储桶。如果之前没有开通过 CI，可能暂时没有免费资源包，开始使用后会自动发放。  

![绑定存储桶](./MigrateFromUpyunToEO/images/ci_bind_bucket.png)  
> 绑定存储桶  

在 EdgeOne 控制台给站点新增一个域名，源站设定为存储桶，准备作为内层 EdgeOne 服务。  

![新增域名](./MigrateFromUpyunToEO/images/eo_add_inner_domain.png)  
> 新增域名

在规则引擎新增针对这个内层服务域名的规则。为了防止其他人直接通过内层域名访问服务，我添加了规则来**要求请求必须通过 `X-Upstream-Token` 头部传递一个预设的 token**，否则返回 403。保险起见这里我还显式指明不缓存 403 响应，虽然[默认情况下 403 是不缓存](https://cloud.tencent.com/document/product/1552/87651)的。  

* 在官方*请求处理顺序*文档中能看到，规则引擎处理在边缘函数执行之前，因此规则若直接应答 403 是不会触发边缘函数的。

![设置内层规则](./MigrateFromUpyunToEO/images/eo_set_inner_rule.png)  



接下来编写边缘函数并设定触发条件。边缘函数编写可以直接用腾讯云自带的边缘函数 AI 助手，也可以用其他 Coding 能力强的模型和 Agent 来编写。  

![编写边缘函数](./MigrateFromUpyunToEO/images/eo_write_edge_func.png)  

个人编写的提示词如下:  

```markdown
编写一个边缘函数来转换 URL，将又拍云的图片处理参数转换为腾讯云 COS 数据万象支持的 ImageMogr2 参数，主要针对缩放和旋转操作，以及二者的组合。  

scale 限制在 100 以内，离散化，只能有 5, 10, 15, ... 这样 5 的倍数，中间值自动 round 取整到最近的 5 的倍数；rotate 则只能是 45 的倍数或者 10 的倍数，或者 auto。

组合时，最多只能组合一个 scale 加一个 rotate。

* 如请求中的 URL 为 `https://example.com/path/to/image.jpg!/scale/50/rotate/90`，则边缘函数应将其转换为 `https://example.com/path/to/image.jpg?imageMogr2/thumbnail/!50p/rotate/90`，**保持处理顺序**。

* 又如请求中的 URL 为 `https://example.com/path/to/image.jpg!/scale/30`，则边缘函数应将其转换为 `https://example.com/path/to/image.jpg?imageMogr2/thumbnail/!30p`。

* 也有自适应旋转的情况，如请求中的 URL 为 `https://example.com/path/to/image.jpg!/rotate/auto`，则边缘函数应将其转换为 `https://example.com/path/to/image.jpg?imageMogr2/auto-orient`。

URL 转换后再继续后续的缓存命中和回源等流程。  
```

<details>

<summary>ChatGPT 调整后的边缘函数代码</summary>  

```javascript
/**
 * Tencent Cloud EdgeOne Edge Function
 *
 * 功能：
 * 将又拍云风格的图片处理参数转换为腾讯云 COS 数据万象 ImageMogr2 参数。
 *
 * 当前支持：
 *
 *   !/scale/50
 *     ->
 *   ?imageMogr2/thumbnail/!50p
 *
 *   !/rotate/90
 *     ->
 *   ?imageMogr2/rotate/90
 *
 *   !/rotate/auto
 *     ->
 *   ?imageMogr2/auto-orient
 *
 *   !/scale/52
 *     ->
 *   ?imageMogr2/thumbnail/!50p
 *
 *   !/scale/53/rotate/90
 *     ->
 *   ?imageMogr2/thumbnail/!55p/rotate/90
 *
 * 设计原则：
 * - 仅允许 GET / HEAD
 * - 不接受客户端 Query String
 * - 仅支持 scale 和 rotate
 * - scale 输入范围：1 - 100
 * - scale 自动 round 到最近的 5 的倍数
 * - scale 最终结果只会是：5, 10, 15, ... 100
 * - rotate 允许 auto
 * - rotate 数字范围：1 - 360
 * - rotate 数字必须为 45 的倍数或 10 的倍数
 * - 最多只允许一个 scale 和一个 rotate
 * - 不支持的参数直接返回 404
 * - 不重定向客户端，只在边缘节点内部改写请求
 */

addEventListener('fetch', event => {
  event.respondWith(handleRequest(event.request));
});


async function handleRequest(request) {
  try {

    // ------------------------------------------------------------
    // 1. 仅允许 GET / HEAD
    // ------------------------------------------------------------

    if (
      request.method !== 'GET' &&
      request.method !== 'HEAD'
    ) {
      return new Response(
        'Method Not Allowed',
        {
          status: 405,
          headers: {
            'Allow': 'GET, HEAD',
            'Content-Type':
              'text/plain; charset=utf-8',
            'Cache-Control':
              'no-store'
          }
        }
      );
    }


    // ------------------------------------------------------------
    // 2. 解析请求 URL
    // ------------------------------------------------------------

    const url = new URL(request.url);

    const marker = '!/';


    // ------------------------------------------------------------
    // 3. 不允许客户端携带任何 Query String
    //
    // 例如：
    //
    // /image.jpg?imageMogr2/...
    // /image.jpg?ci-process=...
    // /image.jpg?foo=bar
    //
    // 全部直接拒绝。
    //
    // 这一层建议同时在 EdgeOne 规则引擎中配置，
    // 这里再做一次 defense in depth。
    // ------------------------------------------------------------

    if (url.search) {
      return new Response(
        'Forbidden',
        {
          status: 403,
          headers: {
            'Content-Type':
              'text/plain; charset=utf-8',
            'Cache-Control':
              'no-store'
          }
        }
      );
    }


    // ------------------------------------------------------------
    // 4. 查找又拍云风格处理标记
    // ------------------------------------------------------------

    const markerIndex =
      url.pathname.indexOf(marker);


    // 没有图片处理参数，普通图片直接放行
    if (markerIndex === -1) {
      return fetch(request);
    }


    // ------------------------------------------------------------
    // 5. 防止出现多个 !/
    //
    // 例如：
    //
    // /image.jpg!/scale/50!/rotate/90
    //
    // 直接视为非法请求。
    // ------------------------------------------------------------

    if (
      url.pathname.indexOf(
        marker,
        markerIndex + marker.length
      ) !== -1
    ) {
      return rejectInvalidProcessing();
    }


    // ------------------------------------------------------------
    // 6. 分离原始图片路径和处理参数
    //
    // 输入：
    //
    // /path/image.jpg!/scale/50/rotate/90
    //
    // imagePath:
    //
    // /path/image.jpg
    //
    // processingString:
    //
    // scale/50/rotate/90
    // ------------------------------------------------------------

    const imagePath =
      url.pathname.slice(
        0,
        markerIndex
      );

    const processingString =
      url.pathname.slice(
        markerIndex + marker.length
      );


    if (
      !imagePath ||
      !processingString
    ) {
      return rejectInvalidProcessing();
    }


    // ------------------------------------------------------------
    // 7. 严格拆分参数
    //
    // 不使用 filter(Boolean)。
    //
    // 防止：
    //
    // scale//50
    //
    // 被错误理解为：
    //
    // scale/50
    // ------------------------------------------------------------

    const params =
      processingString.split('/');


    if (
      params.length === 0 ||
      params.some(
        param => param.length === 0
      )
    ) {
      return rejectInvalidProcessing();
    }


    // ------------------------------------------------------------
    // 8. 转换成 ImageMogr2 参数
    // ------------------------------------------------------------

    const convertedParams =
      convertToImageMogr2(params);


    if (convertedParams === null) {
      return rejectInvalidProcessing();
    }


    // ------------------------------------------------------------
    // 9. 构造 COS ImageMogr2 参数
    // ------------------------------------------------------------

    const imageMogr2 =
      `imageMogr2/${convertedParams.join('/')}`;


    // ------------------------------------------------------------
    // 10. 构造内部 URL
    //
    // 原：
    //
    // /image.jpg!/scale/53/rotate/90
    //
    // 内部：
    //
    // /image.jpg?imageMogr2/thumbnail/!55p/rotate/90
    // ------------------------------------------------------------

    const newUrl =
      new URL(request.url);

    newUrl.pathname =
      imagePath;

    newUrl.search =
      `?${imageMogr2}`;


    // ------------------------------------------------------------
    // 11. 构造内部请求
    //
    // Host 保持不变。
    //
    // 同 Host fetch：
    //
    // Edge Function
    //      ↓
    // EdgeOne Cache
    //      ↓ MISS
    // COS Origin
    // ------------------------------------------------------------

    const headers =
      new Headers(
        request.headers
      );


    const upstreamRequest =
      new Request(
        newUrl.toString(),
        {
          method:
            request.method,

          headers,

          redirect:
            'manual'
        }
      );


    // ------------------------------------------------------------
    // 12. 请求转换后的 URL
    // ------------------------------------------------------------

    return await fetch(
      upstreamRequest
    );

  } catch (error) {

    // ------------------------------------------------------------
    // 不向客户端返回内部错误信息。
    // ------------------------------------------------------------

    console.error(
      'Upyun -> COS ImageMogr2 conversion failed:',
      error
    );


    return new Response(
      'Internal Server Error',
      {
        status: 500,
        headers: {
          'Content-Type':
            'text/plain; charset=utf-8',

          'Cache-Control':
            'no-store'
        }
      }
    );
  }
}


/**
 * 将又拍云参数转换为 COS ImageMogr2 参数。
 *
 * 允许：
 *
 * !/scale/50
 *
 * !/rotate/90
 *
 * !/rotate/auto
 *
 * !/scale/50/rotate/90
 *
 * !/rotate/90/scale/50
 *
 *
 * 不允许：
 *
 * !/scale/50/scale/60
 *
 * !/rotate/90/rotate/180
 *
 * !/scale/50/rotate/90/scale/60
 *
 * !/crop/100
 *
 * !/scale/50/crop/100
 */
function convertToImageMogr2(params) {

  // ------------------------------------------------------------
  // 只接受：
  //
  // [operation, value]
  //
  // 或：
  //
  // [operation1, value1, operation2, value2]
  // ------------------------------------------------------------

  if (
    params.length !== 2 &&
    params.length !== 4
  ) {
    return null;
  }


  const operations = [];

  const seenOperations =
    new Set();


  // ------------------------------------------------------------
  // 解析 operation / value 对
  // ------------------------------------------------------------

  for (
    let i = 0;
    i < params.length;
    i += 2
  ) {

    const operation =
      params[i];

    const value =
      params[i + 1];


    // 只支持 scale / rotate

    if (
      operation !== 'scale' &&
      operation !== 'rotate'
    ) {
      return null;
    }


    // ----------------------------------------------------------
    // 最多只能有：
    //
    // 一个 scale
    // +
    // 一个 rotate
    //
    // 防止：
    //
    // scale/50/scale/60
    //
    // rotate/90/rotate/180
    // ----------------------------------------------------------

    if (
      seenOperations.has(
        operation
      )
    ) {
      return null;
    }


    seenOperations.add(
      operation
    );


    operations.push({
      operation,
      value
    });
  }


  // ------------------------------------------------------------
  // 转换
  // ------------------------------------------------------------

  const result = [];


  for (
    const item of operations
  ) {

    // ==========================================================
    // SCALE
    // ==========================================================

    if (
      item.operation === 'scale'
    ) {

      const scale =
        parseAndNormalizeScale(
          item.value
        );


      if (scale === null) {
        return null;
      }


      result.push(
        `thumbnail/!${scale}p`
      );


      continue;
    }


    // ==========================================================
    // ROTATE
    // ==========================================================

    if (
      item.operation === 'rotate'
    ) {

      const rotate =
        parseAndNormalizeRotate(
          item.value
        );


      if (rotate === null) {
        return null;
      }


      if (
        rotate === 'auto'
      ) {

        result.push(
          'auto-orient'
        );

      } else {

        result.push(
          `rotate/${rotate}`
        );
      }


      continue;
    }


    return null;
  }


  // 理论上的额外保险

  if (
    result.length === 0 ||
    result.length > 2
  ) {
    return null;
  }


  return result;
}


/**
 * 解析并规范化 scale。
 *
 * 规则：
 *
 * 1. 必须是纯数字整数
 *
 * 2. 输入范围：
 *
 *    1 - 100
 *
 * 3. 自动 round 到最近的 5 的倍数
 *
 * 4. 最终结果：
 *
 *    5, 10, 15, 20, ... 100
 *
 *
 * 示例：
 *
 * "1"   -> "5"
 *
 * "2"   -> "5"
 *
 * "3"   -> "5"
 *
 * "5"   -> "5"
 *
 * "7"   -> "5"
 *
 * "8"   -> "10"
 *
 * "12"  -> "10"
 *
 * "13"  -> "15"
 *
 * "52"  -> "50"
 *
 * "53"  -> "55"
 *
 * "97"  -> "95"
 *
 * "98"  -> "100"
 *
 * "99"  -> "100"
 *
 * "100" -> "100"
 */
function parseAndNormalizeScale(
  value
) {

  const number =
    parseStrictInteger(
      value
    );


  if (number === null) {
    return null;
  }


  // 输入值严格限制在 1 - 100

  if (
    number < 1 ||
    number > 100
  ) {
    return null;
  }


  // ------------------------------------------------------------
  // Round 到最近的 5 的倍数
  //
  // 例如：
  //
  // 52 / 5 = 10.4
  // Math.round = 10
  // 10 * 5 = 50
  //
  // 53 / 5 = 10.6
  // Math.round = 11
  // 11 * 5 = 55
  // ------------------------------------------------------------

  let rounded =
    Math.round(
      number / 5
    ) * 5;


  // 下限保护

  if (rounded < 5) {
    rounded = 5;
  }


  // 上限保护

  if (rounded > 100) {
    rounded = 100;
  }


  return String(
    rounded
  );
}


/**
 * 解析 rotate。
 *
 * 规则：
 *
 * 1. "auto" 合法
 *
 * 2. 数字必须为：
 *
 *    1 - 360
 *
 * 3. 且满足以下至少一个条件：
 *
 *    - 45 的倍数
 *
 *    或
 *
 *    - 10 的倍数
 *
 *
 * 合法示例：
 *
 * auto
 *
 * 10
 * 20
 * 30
 * 40
 * 45
 * 50
 * 60
 * 70
 * 80
 * 90
 * 100
 * 110
 * 120
 * 130
 * 135
 * 140
 * ...
 * 180
 * ...
 * 270
 * ...
 * 360
 *
 *
 * 非法示例：
 *
 * 1
 * 5
 * 11
 * 44
 * 46
 * 91
 * 359
 */
function parseAndNormalizeRotate(
  value
) {

  // 自动 EXIF 方向纠正

  if (
    value === 'auto'
  ) {
    return 'auto';
  }


  const number =
    parseStrictInteger(
      value
    );


  if (number === null) {
    return null;
  }


  // 限制角度范围

  if (
    number < 1 ||
    number > 360
  ) {
    return null;
  }


  // ------------------------------------------------------------
  // 必须满足：
  //
  // 45 的倍数
  //
  // 或
  //
  // 10 的倍数
  // ------------------------------------------------------------

  if (
    number % 45 !== 0 &&
    number % 10 !== 0
  ) {
    return null;
  }


  return String(
    number
  );
}


/**
 * 严格解析整数。
 *
 * 只接受：
 *
 * 0-9
 *
 * 不接受：
 *
 * +50
 * -50
 * 50.0
 * 5e1
 *  50
 * 50%
 *
 *
 * 前导 0 会被规范化：
 *
 * "050" -> 50
 */
function parseStrictInteger(
  value
) {

  if (
    !/^\d+$/.test(
      value
    )
  ) {
    return null;
  }


  const number =
    Number(value);


  if (
    !Number.isSafeInteger(
      number
    )
  ) {
    return null;
  }


  return number;
}


/**
 * 非法图片处理参数统一在边缘返回 404。
 *
 * 不再将非法 !/ 请求回源，
 * 避免攻击者构造大量垃圾 URL 打 COS。
 */
function rejectInvalidProcessing() {

  return new Response(
    'Not Found',
    {
      status: 404,
      headers: {
        'Content-Type':
          'text/plain; charset=utf-8',

        'Cache-Control':
          'no-store'
      }
    }
  );
}
```

</details>

部署函数后还得写写触发规则，这里可以写得比较简单，URL full 正则匹配 `^.*!/.+$` 即触发:  

![函数触发规则](./MigrateFromUpyunToEO/images/eo_inner_trigger_condition.png)  

至此内层服务的核心部分就配置完成了，其他还有节点缓存 TTL 之类的可以按个人喜好配置。

### 3.2. 配置外层 EdgeOne 服务

首先依旧是新增域名，注意这里源站和回源 HOST 都**指向内层 EdgeOne 服务的域名**。回源协议其实 HTTP 和 HTTPS 都可以，前者更快（握手次数少），后者更安全:      

![新增外层域名](./MigrateFromUpyunToEO/images/eo_add_outer_domain.png)  

接着同样是在规则引擎，我们需要新增回源请求头，**新增内层服务所需的 `X-Upstream-Token` 头并设置相应的值**以确保能正常回源:  

![设置外层规则](./MigrateFromUpyunToEO/images/eo_set_outer_rule.png)  

注意，如果请求参数能透传到底层 COS 和数据万象服务，就意味着恶意攻击者能构造大量不同的处理参数来通过数据万象服务耗尽免费额度甚至消耗账户余额。因此咱觉得可以在自定义 Cache Key 规则这里**忽略查询字符串**，也可以新增一个子条件判断（正则匹配 `^https?://[^?]*\?.*$`）来**阻止查询字符串**（毕竟我们只用得上 `!/` 风格的处理参数）:  

![规则阻止查询字符串](./MigrateFromUpyunToEO/images/eo_set_outer_rule_block_query.png)  

其余节点缓存 TTL、浏览器缓存、请求头判断等规则亦可按喜好进行配置。  

至此，由内外两层套娃而成、且兼容又拍云风格处理参数的图片资源服务就迁移和部署完成了 ٩(ˊᗜˋ*)و。  

## 4. 那么，代价是什么呢

显而易见地，在缓存未命中的情况下，我们这个套娃方案下客户端与 COS 源站间的请求 / 响应路径都会多经过一层 EdgeOne，无论有没有使用图像处理参数。

为了量化此方案对于一般资源（不使用图像处理参数的资源）造成的影响，咱围绕 TTFB 等延迟指标展开了实验。

### 4.1. 实验设置

主要测试**缓存未命中**的情况。首先咱把 `potato.jpg` 这个文件复制了 200 份，重命名为 `potato1.jpg`, `potato2.jpg`, ..., `potato200.jpg`，存放到了存储桶的 `test_latency/` 目录下。  

![potato](./MigrateFromUpyunToEO/images/potato.jpg)  
> 没错，就是这个 potato  

将这些文件分为两堆，每个文件对应一条请求，第一堆 `potato1...potato100`, 第二堆则是剩下的 `potato101...potato200`。其中第一堆的请求基于 `外层服务域名/test_latency/` 进行，第二堆则基于 `内层服务域名/test_latency/` 进行。

* 基于内层服务域名请求时会带上 `X-Upstream-Token` 头

两种请求随机打乱进行**配对**，一个外层的请求配对一个内层的请求，共 100 对。如**外层服务域名**下 `potato1` 的请求可能和**内层服务域名**下 `potato114` 的请求配对。后续会根据配对计算一些统计量。

每个请求**均独立发出**，URL **不包含任何图像处理参数**，且 200 个资源对象**没有被任何 EdgeOne 节点缓存**。每条请求的处理包含有 DNS 解析、TCP 连接建立、TLS 握手、HTTP 请求发送、CDN 回源处理等流程。  

具体实现基于 Python 的 `pycurl` 库进行，启用 curl 选项: `FRESH_CONNECT=1`, `FORBID_REUSE=1`, `SSL_VERIFYPEER=1`, `SSL_VERIFYHOST=2`。各阶段耗时情况依据 curl 的 `NAMELOOKUP_TIME`, `CONNECT_TIME`, `APPCONNECT_TIME` 等信息进行计算。实验在一台阿里云广州地域服务器上进行。  

### 4.2. 评估指标

* **TTFB** (Time To First Byte) - 从请求开始到收到第一个响应字节的耗时（包含 DNS 解析、TCP 和 TLS 握手）
* **PTTFB** (Processing Time To First Byte) - 从**请求准备就绪**到收到第一个响应字节的耗时（不包含 DNS, TCP, TLS 环节）  
* **CDT** (Content Download Time) - 首字节后数据传输耗时，从收到第一个响应字节开始直至收到整个文件的传输耗时
* **总请求耗时** - 从请求开始到数据接收完毕的耗时

会计算这些统计量:  

* **中位数** (Median) - 反馈比较典型的延迟，而不是被异常值拉高或拉低  
* **截尾平均值** (Trimmed Mean) - 删除最快 10 个和最慢 10 个数据后求平均，缓解极端值影响
* **P95** (95th Percentile) - 第 95 百分位数，指只有 5% 的请求比这个值还慢，观察尾部延迟，即较差情况下的延迟如何
* **配对差** - **外层请求与配对的内层请求在某个指标上的差**
  * **中位数**
  * **截尾平均**
  * **中位数的 95% Bootstrap 置信区间**（设最终计算得到 N 个配对差。每次有放回抽样 N 个计算一个中位数，重复这个有放回抽样和中位数计算 10000 次，得到 10000 个中位数。排序中位数后去掉最低 2.5% 和最高 2.5%，剩下 95% 部分的左右边界形成置信区间）  

### 4.3. 实验结果

* 请求入口: 外层服务（套娃方案）标记为 `Outer`，内层服务（朴素方案）标记为 `Inner`；
* 中位数标记为 **M**；
* 截尾平均值标记为 **TMean**
* 中位数的 95% Bootstrap 置信区间标记为 **M95PBCI**

#### 4.3.1. TTFB (ms)

| ↓ 请求入口 \ 统计量 → | M | TMean | P95 | 
| --- | --- | --- | --- | 
| Outer | 248.515 | 266.344 | 467.433 | 
| Inner | 196.954 | 204.718 | 344.536 | 

| 配对差 M | 配对差 TMean | 配对差 M95PBCI |
| --- | --- | --- | 
| +49.533 | +59.387 | `[+35.660, +68.251]` |

从配对差中位数可以看到从请求开始到首字节响应，典型情况下套娃方案比朴素方案在延迟上要慢 **49.5ms** 左右。  

#### 4.3.2. PTTFB (ms)

| ↓ 请求入口 \ 统计量 → | M | TMean | P95 | 
| --- | --- | --- | --- | 
| Outer | 216.463 | 231.166 | 376.445 | 
| Inner | 157.808 | 167.695 | 302.892 | 

| 配对差 M | 配对差 TMean | 配对差 M95PBCI |
| --- | --- | --- | 
| +56.636 | +60.742 | `[+44.359, +70.675]` |

相比 TTFB，PTTFB 在配对差统计量上显然要更大一些，说明延迟差别主要体现在连接建立后的 HTTP 传输和 CDN 回源上。  

#### 4.3.3. CDT (ms)

| ↓ 请求入口 \ 统计量 → | M | TMean | P95 | 
| --- | --- | --- | --- | 
| Outer | 1.265 | 2.942 | 21.982 | 
| Inner | 0.742 | 1.428 | 26.888 | 

| 配对差 M | 配对差 TMean | 配对差 M95PBCI |
| --- | --- | --- | 
| +0.239 | +1.281 | `[+0.079, +0.505]` |

可以看到首字节之后下载阶段的传输延迟的差别是非常小的，套娃方案并不会显著增大下载耗时。  

#### 4.3.4. 总请求耗时

| ↓ 请求入口 \ 统计量 → | M | TMean | P95 | 
| --- | --- | --- | --- | 
| Outer | 249.486 | 272.477 | 468.084 | 
| Inner | 198.711 | 208.689 | 351.649 | 

| 配对差 M | 配对差 TMean | 配对差 M95PBCI |
| --- | --- | --- | 
| +49.862 | +61.635 | `[+35.846, +71.348]` |

总请求耗时反映的情况很接近 TTFB 的情况。  

### 4.4. 实验分析

显然，缓存未命中的情况下，从配对差置信区间均为正数可以看到，套娃方案普遍比朴素方案要慢（延迟更大）。综合各个指标来看，咱的套娃方案新产生的代价主要体现在 PTTFB 指标上，即 DNS / TCP / TLS 等连接建立阶段完成后的传输、处理和响应过程。总的来说大概率情况下套娃方案只会新引入 **44-70ms** 左右的延迟，用户难以察觉，是完全可以接受的。

~~先暂且这样吧，其他的没有很多想分析的~~  

## 5. 总结

这次折腾了一下图片托管服务从又拍云到腾讯云 EdgeOne + COS 的无感迁移，整了个神奇的套娃方案来缓解成本，且引入的延迟是完全可以接受的。于是咱又没能按捺住自己的双手，写了这篇小记，希望多多少少能帮助到大家吧。  

个人学艺不精，也可能有些理解错的地方，请各位多指教。  

咱们下一篇文章再会~ (づ￣ ³￣)づ  

