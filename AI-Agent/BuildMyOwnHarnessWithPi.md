# 【小记】上手 Pi，记录一下我的 AI 编码实践

多个月以后，面对满屏的代码无从下手时，些瓶将会回想起，他第一次接触 Coding Agent 的那个遥远的下午。  

自从今年年初开始尝试 AI 辅助编码后，咱是真的一发不可收拾了，这体验简直和一直以来的 Tab 补全完全不同。在本地 Agent 程序的约束和辅助下，指令遵循较强的大语言模型 (LLMs, Large Language Models) 变得可以按照指定的循环流程来在一个项目的范围下来执行编码任务，而不像以前只能生成、补全代码片段。

但是吧，用着用着就发现，LLM 作为一大坨概率模型，在执行任务的时候如果没有明确的约束，很容易就会加入自己的小巧思（就像我们平时做饭一样 (๑>؂•̀๑)），小巧思越多，模型执笔的代码可能就愈发混乱，甚至会逐渐偏离我们原始的需求，即使有 Agent 框架的约束也可能如此。  

因此，如何约束模型行为，制定条条框框去规范模型的工具调用和决策流程就成了一门学问，大佬们将其凝炼成了一个词 “Harness”，字面意思就是 “控制并利用”，非常直白。同一个模型，不同的人能用出不同的花样，很大程度上就是取决于 Harness Engineering。  

[图片]

正好最近咱尝试上手了 [Pi](https://pi.dev/)，也看到了 Pi Coding Agent 作者的[一篇博客文章](https://mariozechner.at/posts/2025-11-30-pi-coding-agent/)，感觉其中的一些观点很值得借鉴，于是准备在折腾 Pi 的同时开这样一篇笔记，记录在 Pi 配置上的一些想法的同时，也沉淀一下自己在运用 AI 编码过程中的一些实践经验。  

个人能力有限，可能也有一些理解错误的地方，也请各位多指正！这篇笔记文章可能会持续更新，希望最终能成为一份有一定参考价值的实践总结。

## 0. 打造自己的 Pi

Pi 的安装咱就不啰嗦了，直接从配置入手。和 Claude Code、Codex 等开箱即用 Agent 不同，Pi 仅提供了最基本的功能，俗称毛坯房，需要什么就自己加上什么（~~搞半天还要自己拼~~）。

阅读了作者的[博客文章](https://mariozechner.at/posts/2025-11-30-pi-coding-agent/)后，个人理解，Pi 的哲学简单来说就是 “*Bash and Documents are All You Need*”：只要有 Bash，Agent 就能在当前用户权限的范围下执行任何操作，复用和组合现有的命令行程序，**甚至再启动一个它自己的实例**；而只要有 Documents (通常是 Markdown 这种结构化文档)，Agent 就能将上下文中的信息持久化到硬盘上并能够**按需**读取和更新，也能读取文档，按照文档中的指示执行操作，而且这些文档是**可以跨工具、跨 Agent，甚至是可以在 Agent 和人之间共用**的。

二者组合起来有多强大呢？不妨看看 Pi 和开箱即用 Agent 的不同之处，以及符合上述哲学的解决方案：  

1. **没有 MCP**：我们可以写文档说明如何在 Bash 中调用各种工具，需要使用时**按需**让模型载入文档即可；
2. **没有 Sub-Agents**：Agent 可以通过 Bash 再启动另一个 Agent 实例，并且可以通过文档来共享必要的信息；
3. **没有 Plan / Todo 模式**：完全可以让 Agent 把计划写入本地文档（如 PLAN.md），然后新开会话载入文档来执行计划。而 TODO 也可也写成文档，让 Agent 在执行计划的同时顺手去修改 TODO.md 来勾选完成的内容。  

追究到底，Pi 主打复用现有的命令行工具生态，不引入过多的复杂度，实在满足不了需求再自己加。个人认为，之所以可以如此极简（只要有 Bash 和文档读写就能完成大部分操作），还得是多亏了模型本身经过强化学习后训练所掌握的**越来越强大的任务规划和指令遵循能力**。模型越强，以往框架中繁多的约束就越冗余，我们能简化的部分就越多。  

* 比如最近 GPT 5.6 推出后，Superpowers 这类规范工作流程的 Skills 反而会降低模型的编码效率（尤其是在中小项目上），浪费更多 Token，而产出也没比模型自行规划好多少。  

但！是！纵使模型再强大，其也终究是按概率来行事，有些缰绳是我们不能完全放松的，比如安全边界的硬性控制。总的来说，在 Pi 这个毛坯房上定制自己的 Harness 时，比较重要的应该是搞清楚模型现在还不擅长什么，以及如何安装或极简封装我们比较常用的功能。

接下来就写写咱是怎么配置自己的 Pi 的，主要是权限控制、网络搜索、skills、sub-agent 以及提示词模板这五块我最常用的功能。  

### 0.0. 权限控制

权限控制是我们首当其冲要配置的，自 Coding Agent 大火以来其一直是令人头疼的问题。再怎么对齐训练，模型总会有灵机一动越过安全边界的时刻，无论模型能力有多强，**权限控制必须至少交由 Agent 框架的确定性代码逻辑来控制**，而不是指望概率上不会犯错。  

但交由确定性的逻辑来约束不代表不会出事了，就算用商业公司设计和维护的 Agent，也难免有数据被误删的血泪史（硬盘分区文件被清空的事屡见不鲜，比较常见的是模型在 Windows 上用不明白 PowerShell 或者用错了斜杠和反斜杠）。对此，Pi 作者的解决方案是——直接不加权限控制，默认 YOLO（完全放权），但把 Agent 整个放到 Docker 容器这种隔离化环境中。  

个人感觉这样整还是太复杂了：我需要在容器镜像中重复安装和配置宿主机已经有的各种环境依赖，每次开发时还得把工作目录映射到容器中。提高系统安全性总得付出些代价，但为了更方便进行开发，我的权衡是放弃高隔离性，仅通过**安装扩展包**给 Pi 加上类似于其他成熟 Agent 的权限控制机制：    

```bash
# -l 选项会将这个包安装到项目级别，这里是为了方便配置备份
pi install -l npm:@gotgenes/pi-permission-system
```

* 官方仓库: https://github.com/gotgenes/pi-packages/tree/main/packages/pi-permission-system  
* `gotgenes/pi-permission-system` 是 Pi 生态中被广泛使用的权限控制包，其在 Agent 框架程序的代码层面上拦截越权的操作

`pi-permission-system` 自带有文档，完全是可以让 Pi 自己去阅读文档写配置的。直接告诉 Pi 你的需求，例如：  

```text
帮我写一下 pi-permission-system 这个插件的配置，我希望日常开发还是尽量能轻松点，少一些确认的步骤。主要管控 git reset, rm -rf 等常见危险命令以及常见敏感文件（如 .env 环境变量配置文件）和目录的访问，同时在访问工作目录以外的路径时必须进行询问（临时目录除外）。
```

Agent 为我生成的配置在这里: [config.json](https://github.com/SomeBottle/pi-config/blob/0a02df04472269860f0a9364d787030314bde0e7/.pi/extensions/pi-permission-system/config.json), 我自己只小改了一下，去掉了部分我不需要的条目。可以看到配置主要覆盖了 `path` (保护敏感文件被任何路径相关的工具访问), `external_directory` (工作目录外路径访问约束) 以及 `bash` (约束敏感命令的执行)，还是比较全面的。后续根据个人需求还可以灵活变动。  

* 注: pi 在一个工作目录中启动时会先询问用户是否信任目录，这个机制主要是提醒用户要**防范提示词注入攻击**（目录下的文档、skills、pi 扩展等都有可能暗藏玄机）。即便引入上述扩展后有权限审批环节，但仍然不排除有看走眼的情况，况且开发中我们难免要用到网络工具（如 curl），而有网络就扩大了攻击面，不可掉以轻心。  

### 0.1. 网络搜索

网络信息日新月异，而模型训练的周期至少也是按月来算的，为了让模型能检索到最新的数据，网络搜索相关工具是必须得配备的。作为编码日常使用，我的选择是引入 [Context7](https://context7.com/docs/clients/cli) 和 [Tavily](https://docs.tavily.com/documentation/agent-skills) 的相关 Skills（而不是 MCP，为什么不用 MCP 可以看下面的 [1.1 节](#progressive-disclosure)），前者提供最新文档的查询，后者则作为补充，提供网页搜索和页面提取功能：  

```bash
# 在当前项目下安装 Context7 和 Tavily Skills
# --copy 保证会在当前目录下生成 SKILL.md 而不是创建软链
# --agent pi 会将 Skills 安装到 ./.pi/skills
# 如果要安装在全局而不是当前工作目录下，可以用 -g 选项
npx skills add tavily-ai/skills --skill tavily-search tavily-extract --agent pi -y --copy
npx skills add upstash/context7 --skill find-docs --agent pi -y --copy

# 安装 Tavily CLI (参考 https://docs.tavily.com/documentation/agent-skills#installation), Skill 需要
curl -fsSL https://cli.tavily.com/install.sh | bash
```

安装 Skills 时也主打一个**按需引入**，比如 Tavily 实际上提供了 6 个 skill，但我最常用的主要是 `tavily-search` 和 `tavily-extract`，因此只引入这两个。Skills 的元数据依旧会占用少数上下文，skill 数量越多（尤其是语义相近的 skill 越多），上下文也会越混乱，干扰模型做出决策。  

[图片]

### 0.2. 安装 Matt Pocock 的 Skills

随着模型能力越来越强，咱认为对于中小项目越来越不需要 OpenSpec, Superpowers 这类比较沉重的工作流编排 skills，繁重的约束虽然能最大程度对项目进行规范，但毫无疑问**相当耗费 Token 且效率极低**。  

有时候一些**简单而高效**的 skills 用起来反而更顺心，比较有名的就是 [Matt Pocock](https://github.com/mattpocock/skills) 的这一套。打开仓库可以发现有数十个 skills，但实际上我并不需要这么多，暂且先安装 engineering (工程化) 和 productivity (生产力) 两方面的:  

```bash
# https://github.com/mattpocock/skills - v1.2.3
npx skills@latest add https://github.com/mattpocock/skills/tree/main/skills/engineering --skill '*' --agent pi -y --copy
npx skills@latest add https://github.com/mattpocock/skills/tree/main/skills/productivity --skill '*' --agent pi -y --copy
```

Matt Pocock 的这批 skills 在设计的时候是为了维护 GitHub 这类代码托管平台的项目的，因此他引入了 Issue Tracker (Issue 跟踪器，比如 GitHub Issues) 这个概念，多个 skills 都是围绕着 Issue 跟踪器来进行的，用 Issue 编排了需求的开发工作流（我寻思这应该算是一种 **Issue 驱动开发**的思想）。但我还是更习惯于**文档驱动**的思路，其能减少开发工作对某个平台的依赖（毕竟 Issue 跟踪器是依托于托管平台的），只要有文件系统就能进行存放。好在 `setup-matt-pocock-skills` 在初始化时可以直接指定本地 Issue 跟踪器（本质上是通过往 `.scratch` 目录下写 Markdown 文档来实现）。  

进入 `./.pi/skills`，移除下列这些我暂时不需要的:  

* `teach` - 用户学习用，可以生成一些课件、问答之类的。
* `wait-what` - 让模型用更清晰的语言复述内容，主要是为英文定制的，完全可以用提示词模板代替。
* `resolving-merge-conflicts` - 冲突解决一般是手动来的，不太用得上。
* `ask-matt` - Matt Pocock 的 skill 路由，但其实用得多了就不太需要了，主打一个自由组合。
* `writing-for-agents` (原本叫 `writing-great-skills`) - 编写 Agent Skill 的指导，并不常用。
* `wizard` - 生成交互式的 Bash 脚本，作为向导程序指导用户一步步完成操作，利好 HITL (Human In The Loop, 人为介入)。
* `to-questionnaire` - 生成问题清单让另一个负责人去回答，可以和 `grill` 相关 skill 一起用。

移除了上面这些 skill 后，个人理解，其余比较常用的 skills 如下:  

* `grilling` (被 `grill-me` 和 `grill-with-docs` 复用) - 非常著名的 skill，优化输入，通过拷问用户各种细节点来对齐需求、消除不确定性。最近 Matt 已经对这个 skill 进行了更新，现在会形成决策树，每次问一批问题了。
* `wayfinder` - 针对大型项目，拆解模糊的大目标，调查找到解决问题的路线，围绕 Issue 跟踪器展开（主要是为了解决目标庞大，要确定的东西太多，导致 `grilling` 的过程中会撑爆上下文的问题，如触发上下文压缩而丢失信息）。
* `to-spec` - (原本叫 `to-prd`) 把对话内容转换为规范和需求文档，并作为大 Issue 发布到 Issue Tracker。  
* `to-tickets` - (原本叫 `to-issues`) 根据 Spec **垂直切片**拆分任务 (有点 TODO 文档的意思)，即把规范文档大 Issue 拆分成多个小 Issue。垂直切片指的是拆分成**较小但端到端完整**的实现任务，也就是每个小任务对应一个完整的小功能实现（比如用户消息系统中的“已读”功能）。
* `implement` - 可以根据规范文档或者拆分得到的 Issue 来执行一个 TDD 开发、测试、审查和提交流程（咱觉着 AI 时代 TDD 是非常有效的开发范式）。
* `code-review` - **提交后**进行代码审查，会利用 git diff 找到代码变更，使用时可以给出要比对的 commit hashes。  

### 0.3. 实现一个简单的 Web Fetch Skill

之前我已经安装了 `tavily-extract` skill 用于提取网页内容，其支持 JavaScript 脚本执行、页面渲染后的实际内容提取。然而有时并不需要这么复杂，对于 SEO 支持好的页面（这些页面通常会把主要内容放在 HTML 中而不是动态渲染），我们完全可以用 curl / wget 这种命令行工具来抓取页面原始 HTML 内容，这样还可以省些 Tavily 额度。还有一种情况就是我需要抓取内网站点，那就只能用本地工具了。  

但是，curl / wget 也并不是最优解，虽然比较符合 pi 的复用命令行工具的哲学，但 curl 抓取到的 HTML 可能存在**大量冗余信息**（如页头、页脚以及大量的 HTML 标签），会**显著占用模型上下文，浪费 token**，当我只想要正文内容时这并不是一个很好的实践。因此我觉着还是要写个命令行工具来提取出去冗余的 Markdown 或纯文本，搭配 skill 来供 Agent 使用。  

能用上 Pi，环境中肯定有 Node.js，这个 Web Fetch 命令行工具自然而然也比较适合用 JS 来写了。正好 OpenCode 这里还有[开源的 Web Fetch 实现](https://github.com/anomalyco/opencode/blob/0a601cf334b9a83cc2854108a2b860f25e6e7e8e/packages/opencode/src/tool/webfetch.ts)，拿来吧你！

可以结合 `grill-me` 来编写脚本:  

```text
/skill:grill-me 我需要实现一个简单的 Web Fetch 命令行工具，接受下列三个入参:  

1. URL 
2. format (默认 markdown, 可选 markdown, text, html)
3. timeout (默认 30s, 抓取超时，单位秒)

写成单个 JavaScript 脚本文件 web-fetch.js，放到 .pi/skills/web-fetch 目录下，脚本需要有简洁明了的 -h --help 帮助信息。

你可以参考 OpenCode 官方的实现来写: https://github.com/anomalyco/opencode/blob/0a601cf334b9a83cc2854108a2b860f25e6e7e8e/packages/opencode/src/tool/webfetch.ts , 完成后请在顶部注释这个参考 URL，说明 OpenCode 的协议: https://github.com/anomalyco/opencode/blob/dev/LICENSE。

本次暂且只编写脚本，不编写 skill。  
```

测试过程中发现 OpenCode 的实现还保留了页面中的 JS 和 CSS，如果我只想要正文就显得很臃肿。因此我又和 Pi 进行了一轮对话，用 `@mozilla/readability` 和 `jsdom` 包来去除不需要的部分来返回更精简的页面内容，同时也考虑到可能确实有抓取原页面的需求，加上了 `--gross` 命令行选项来支持返回原始内容。注意，为了让 Agent **能总是收到语义化的反馈**，在状态码不为 2xx 时也要返回响应体内容。

接下来需要编写 `SKILL.md` 了，在 description (描述) 中我们需要**写清楚基本功能以及边界**：什么时候用，什么时候不适合用。元数据部分我们可以这样写:  

```markdown
---
name: web-fetch
description: Supports fetching web content by sending requests directly from the local environment. Given a URL, it can return the page content as Markdown, plain text, or HTML. Prefer this skill when content needs to be retrieved directly from a URL, it is not suitable for pages that require JavaScript rendering.
---
```

随后的正文部分我们可以写得比较简洁，主要是给模型提供 **few-shot** (少数示例)，大纲如下：

1. 先定义一个占位符，让这个占位符代表 `SKILL.md` 所在目录；
2. 如何调用工具抓取页面并返回精简内容（可以给出组合不同命令行参数的示例）；
3. 如何调用工具抓取页面并返回原始内容；
4. 告诉模型如何查看帮助信息；
5. 说明限制。

这个 skill 的完成体在这里: [SKILL.md](https://github.com/SomeBottle/pi-config/blob/e431f475fe3fa2b8e8cd86032921d5dec47fdd0c/.pi/skills/web-fetch/SKILL.md)。  

[图片: 调用 skill 示例]

### 0.4. 实现简单的 Sub-agents 功能

Pi 没有自带子 Agent (Sub-agents) 功能，但完全可以让 Pi **通过 Bash 启动另一个 Pi 实例来执行任务**。作者在[博客文章](https://mariozechner.at/posts/2025-11-30-pi-coding-agent/#toc_18)中给出了两种方案：  

1. 让 Pi 通过 `pi --print` 来启动另一个 Pi 进程，等待其输出结果，但这样无法观察到中间过程（只关注结果，看不到子 Agent 执行情况，之前安装的 `pi-permission-system` 也会直接拒绝没有显式允许的操作）；
2. 让 Pi 通过 `tmux` 在新会话 / 窗口中启动 Pi TUI，这样用户也可以直接和子 Agent 进行交互。但是这样一来子 Agent 最终的输出就没法直接通过 STDOUT （标准输出）回传给主 Agent 了。  

能不能又能和子 Agent 交互、又能让主 Agent 拿到输出结果呢？为此我尝试写了一个简单的 [subagent-loop skill](https://github.com/SomeBottle/pi-config/blob/976f6faeb1a30beef1756e94ca52ed414b3b25fe/.pi/skills/subagent-loop/SKILL.md) (中文版: [SKILL.zh-CN.md](https://github.com/SomeBottle/pi-config/blob/976f6faeb1a30beef1756e94ca52ed414b3b25fe/.pi/skills/subagent-loop/SKILL.zh-CN.md)) 来制定一套简洁、可观测且可介入的子 Agent 工作流程与生命周期管理机制。  

* Skill 中调用的是一个 shell 脚本 [`loop.sh`](https://github.com/SomeBottle/pi-config/blob/976f6faeb1a30beef1756e94ca52ed414b3b25fe/.pi/skills/subagent-loop/scripts/loop.sh)，大部分逻辑由脚本实现，模型每次只需要输出一行 bash 命令来执行脚本，能**显著节省输出 token 数**。反例可见 [SKILL.md](https://github.com/SomeBottle/pi-config/blob/6fb2b9319ff5268514030546d8caed51ca419303/.pi/skills/subagent-loop/SKILL.md)，其让模型每步都输出大量脚本，虽然能用，但是多了很多不必要的开销。

#### 0.4.0. 可观测和可介入的实现

“可观测”指*主 Agent 和用户都能观测到子 Agent 的运行状态*。

我们可以通过 `pi --session xxx.jsonl` 来让子 Agent 把新增的消息及时**附加**到这个会话 `jsonl` 文件中，每行一条消息 JSON，这样主 Agent 就可以随时通过 `tail` 命令来查看子 Agent 运行状态。为了让主 Agent 能读取到最终子 Agent 的报告，我利用 Pi 的 `--append-system-prompt` 来在系统提示词中告知子 Agent 产出报告文档：`You must write final report to {OUTPUT_PATH}.`，当主 Agent 发现 `{OUTPUT_PATH}` 文件存在时也正好说明子 Agent 执行完成了。  

我原本的设计是让主 Agent 每隔几十秒就去读取一次 `jsonl` 会话文件的末几行来检查子 Agent 的运行状态，但是每行的 JSON 数据量可能很大（比如说 `read` 工具调用读取文档，读出的内容都会塞进一行），会严重污染主 Agent 上下文，且造成明显的缓存命中抖动。因此我将这个设计简化且内化到了 shell 脚本中，Agent 只需要调用脚本，脚本逻辑发现子 Agent 消息最后一行**多轮未变动时**就会反馈给主 Agent：“子 Agent 可能卡住了”，这个时候让用户介入即可。  

也就是说**观测更多让用户来做**，主 Agent 只需要关注子 Agent 执行有没有阻塞。

“可介入”指*用户能介入到子 Agent 中，实时观察子 Agent 并进行纠偏* (steering)。要实现这点，可以借鉴 Pi 作者的思想，用 `tmux new-session -d` 来在后台启动一个 shell 来执行 Pi，用户就可以随时通过 `tmux attach` 切换到子 Agent 了（如果主 Agent 已经运行在 tmux 会话中，可以直接用 `Ctrl+b` 配合 `(`、`)` 来实现在 Agent 间来回切换）。  

#### 0.4.1. 减少 Agent 执行阻碍

`pi-permission-system` 扩展需要用户介入批准一些有风险的命令执行，但对于子 Agent 这显然是个阻碍。因此我启动子 Agent 时用上了 Pi 的 `--approve` 和 `--no-extensions` 选项，前者让 Pi 默认信任当前目录（主 Agent 其实已经信任了，那子 Agent 也顺理成章）；后者则指明不载入扩展（我只有权限控制这一个扩展）。这样一来就可以尽量保证子 Agent 能在无人干预的情况下从头跑到尾了。  

* 注意，子 agent 通常还是用于校对这种**读取占大头的独立任务**，尽量不要把大量的修改任务交给子 agent，不然这样放权有风险。

#### 0.4.2. 生命周期管理

脚本 `loop.sh` 为主 Agent 提供了管理子 Agent 的逻辑，子 Agent 生命周期可以简洁概括为三个阶段：启动、运行、终止，分别对应了脚本的 `init`, `poll`, `end` 三个子命令。主 Agent 如何调用脚本，如何在三个阶段间进行状态流转，则是 skill 文档主要负责编排的内容了：  

1. **启动** (`init`)：组合 `tmux` (或者 GNU Screen) 和 `pi` 命令启动子 agent，然后进入运行阶段；若没有 `tmux` / `screen` 则直接用 `pi --print` 来兜底，阻塞运行完成后进入终止阶段；
2. **运行** (`poll`)：休眠一小段时间后查询子 Agent 的状态，可能是 仍在运行 / 运行完毕（有报告了） / 超时 / 阻塞。若仍在运行则重复此步轮询；若运行完毕或超时则进入终止阶段；若阻塞则提醒用户；
3. **终止** (`end`)：取出子 Agent 报告，销毁 `tmux` / `screen` 的会话，清理临时文件。  

#### 0.4.3. 进阶：支持并发分派多个子 Agent

这个 skill 写出来虽然很简洁 (100 行不到)，但最明显的不足之处就是——**不支持并发多个子 Agent 同时工作**。像是在进行探索 (explore) 任务的时候，我们可能还是有并发分派多个子 Agent 去探索不同模块的需求的。  

为了实现这一功能，首先得考虑如何重新设计脚本和 skill，让主 Agent 能管理多个子 Agent。我的思路如下:  

1. 引入**组** (group) 这个组织单位，每一次分派的多个子 Agent 都归属于同一个组中；
2. 为了降低复杂度，主 Agent 每一次**只维护一个组的生命周期**。用户如果尝试启动其他组，必须结束当前组的子 Agent 并进行清理再继续；
3. 每次轮询 poll 时列出所有子 Agent 的运行状态，根据一组内所有子 Agent 的状态来进行状态转移。  

另一个点就是主 Agent 怎么去收集子 Agent 的结果，目前主流的实现有两种方案:  

1. “**总-分-总**”式：主 Agent 分派子 Agent -> 子 Agent 并发进行 -> 等待**所有**子 Agent 执行完成后主 Agent 获取结果进行汇总（有点像 Map-Reduce / Fork-Join 模式，先分治再收集结果）；
2. **事件驱动**式：主 Agent 分派子 Agent 后可以继续进行其他**不依赖于子 Agent 结果**的任务，每一个子 Agent 执行完成时会产生事件通知主 Agent，主 Agent 就可以抽空来收集结果。  

第一种方式可靠但会直接阻塞住主 Agent，阻塞时间取决于最慢的子 Agent；而第二种实现上比较复杂，费这么大力不如直接写个 Pi 扩展了。我决定采用一个投机式的折中方案：主 Agent 进行分派后可以继续执行其余无关任务，执行过程中会进行一些工具调用，这时就可以**穿插**子 Agent 状态的轮询，对于已经完成的子 Agent，主 Agent 能自主决定什么时候获取子 Agent 的结果报告。  

按照如上思路，我移除了 `loop.sh` 脚本的 `end` 子命令，转而新增了 `get` 和 `clean` 命令分别用于获取单个子 Agent 报告，清理进程和文件。Skill 这边我主要修改了 STEP-1 和 STEP-2 两节，显式说明了子 Agent 状态对应的处理方式以及主 Agent 如何从 STEP-1 进行转移。详见: [SKILL.md](https://github.com/SomeBottle/pi-config/blob/ad03c3e047827f84f60d6a85b97ecf925590b5d8/.pi/skills/subagent-loop/SKILL.md), [loop.sh](https://github.com/SomeBottle/pi-config/blob/ad03c3e047827f84f60d6a85b97ecf925590b5d8/.pi/skills/subagent-loop/scripts/loop.sh) 。   

[图片: sub-agent 使用示例]

#### 0.4.4. 折腾的尽头是...

写这 0.4 节咱主要是为了尝试能不能用脚本结合 skill 来在 Pi 作者的方向上去实现 sub-agents 功能，结果还真给整出来了，且对于 Agent 能力稳定的模型（比如 DeepSeek V4 Flash GA）真的完全够用了。  

但终究有些特性是难以实现的，比如在主 Agent 审批子 Agent 的权限，而不是默认放权。如果有这种需求的话还是别花时间造轮子了，老老实实用已经有人维护的 sub-agents 扩展吧：  

```bash
# gotgenes/pi-subagents 和 gotgenes/pi-permission-system 是同一个维护者，
# pi-permission-system 对 pi-subagents 还设计了一些权限项
# -l 选项会将这个包安装到项目级别，这里是为了方便配置备份
pi install -l npm:@gotgenes/pi-subagents
```

### 0.5. 编写提示词模板

Pi 支持提示词模板，可以通过斜杠命令 (slash command) 来展开，且支持类似 shell 脚本的位置参数（如 `/explore instruction`，`instruction` 就会替换掉模板中的 `$1`）。

我写了两个比较常用的提示词模板: [explore.md](https://github.com/SomeBottle/pi-config/blob/34d494614ee8f42843230c9a31ba568834cbd6af/.pi/prompts/explore.md), [make-plan.md](https://github.com/SomeBottle/pi-config/blob/c841803abcef9783a0994b643df955f10ed55ec9/.pi/prompts/make-plan.md)。其中 `/explore` 用于复用 subagent-loop skill 来启动 sub-agent 来执行代码仓库探索任务；`/make-plan` 则是根据上下文产出带 TODO 列表的任务计划文档。  

实际使用中我还能根据需求的变化新增一些提示词模板，如论文审阅 `/paper-review`。和 skills 不同的是，提示词模板在**用户使用前是完全没有任何部分进入上下文**的，只要命名不冲突，写多少个都可以，怎么方便怎么来。    


<!-- TODO: 其他的一些原则：尽量少用 pi 扩展或者第三方包，而是多依靠 skill，pi 本身还在经常更新，代码层面 api 可能有破坏性改动-->

## 1. 一些实践经验

接下来写写我使用了半年 Coding Agent 所体会到的一些经验。

### 1.0. 明确需求，优化输入

#### 1.0.0. 消除输入的不确定性

想象一下，当甲方抛给你一坨不清不白的需求且不告诉你更多细节时，你是不是只能自行发挥？但是抓耳搔腮发挥完后给甲方，甲方又给你甩脸色说没按他的来，你又只得憋住红温的脸继续改来改去 (╥﹏╥)。最近实习的时候我就遇到过需求对齐上的问题，指导人让我写一个测试工具来测试系统中的某个模块，但是我第一版写完后他才告诉我还需要考虑效率，也就是制造和移除测试数据时需要足够快（可能要制造亿级的数据），我只得返工进行大改。

模型也是一样，面对一个需求的时候不一定会考虑得那么周到，我们不能假设模型会像我们一样去理解需求。为什么不能？因为我们掌握的信息和模型所掌握的信息存在偏差，**我们所认为理所当然的领域知识（如特定业务场景下的）对模型来说可能是未知、不确定的**。而作为概率模型，对于不确定的点它不一定会提出疑问，而可能会进行猜测，结果就会导致最终构建出来的成果看上去像个样，但具体到细节反而和我们所理解的大相径庭。  

因此关键点就是：**在输入阶段就尽量和模型对齐需求，越充分、结构化，越好**。否则就算中途通过对话去纠偏 (steering) ，模型多少还是会受前面残留的上下文的影响。关于“越充分、结构化，越好”：  

1. 不吝啬言语，描述上可以写清楚任务需求，补充一些已知的环境约束（比如依赖在 WSL 中，那就需要在输入中提到）、出入参信息（可以给出少数样本, few-shot）和任务最终的目标（产出什么，产出到哪里）等。
2. 除了“要做什么”，还可以补充“**不要做什么**”，明确任务的边界（不然可能遇到模型执行任务时“顺手”帮忙改些别的东西的情况）。
3. 对于比较危险的修改操作，最好还要**指示模型对数据进行备份**，不然其灵机一动覆盖或移除了真的很难找回来（建议善用 Git 或者其他版本管理工具）。
4. 尽量结构化，不要挤在一坨，对于较繁杂的需求提示词可以写成 Markdown 文档再让 Agent 读取。

除此之外还可以让模型主动“暴露”不清晰的地方，一个神器就是 Matt Pocock 的 **grill-me** skill，主打**尽量消除不确定性**，让模型来主动~~拷打~~询问它觉着不清楚的要点。纵使需求给得再详细，总会有些我们认为理应如此的信息是没有提供的，grill-me 很大程度上弥补了这一点缺陷。待需求对齐后再开始执行任务，能让最终成果能更加接近我们的验收标准。  

> 说来 grill-me 其实每次只问一个问题，可能最终会拷问用户上百个问题，一方面会给用户整红温，另一方面则是效率不够高。好在 Matt Pocock 已经在 8 月更新了 [grilling skill](https://github.com/mattpocock/skills/blob/cdef59f6e311b6c5918d7c49630b6e3ca5593ca2/skills/productivity/grilling/SKILL.md) (grill-me 是 grilling 的一个别名)，现在其会指导模型形成一棵问题决策树，按轮来进行提问了。  

#### 1.0.1. 不要一口气吃成胖子

不要把提示词写成“任务大杂烩”，比如：  

```text
帮我排查一下订单服务最近偶发的数据库死锁问题，顺便把后台管理系统的登录页重新设计得现代一点，最后再给 xxx 编写一份 AGENTS.md
```

这句提示词一次性包含了 3 个相对比较独立的任务，虽然对于现在 Agent 能力强的模型要在一个会话中完成这些任务是完全可行的，但就像让人去同时进行多项任务一样，难免会“分散注意力”。况且每执行完一个任务，就会多堆积一个任务的上下文，越往后残留的冗余信息就会越多（熵增，有效信息的信噪比同时下降），耗费 token 数增加不说，模型还有可能误把前一个任务的约束套到后一个任务上。  

因此有多个任务要执行时，应拆到多个会话中独立执行（可以 `/new` 创建新的会话，也可以看看下方关于 sub-agents 的使用），让模型专注于每一个任务及其目标。

#### 1.0.2. 大而模糊的需求：细化粒度，分而治之

```text
帮忙编写一个低延迟的牧濑红莉栖 Live2D 语音助手 (Amadeus)。
```

遇到这样宽泛的输入时，按上面“消除输入不确定性”来的话，会深挖 “‘低延迟’到底指什么？”、“助手是否应该拥有长期记忆？”、“Live2D 要做到哪一级同步？” 等这些实现细节。但对于这类大型任务，这样消除不确定性也是不一定够的：**任务粒度很大**，其中大量约束、技术取舍和实现问题并不是一开始就能穷举出来的，而只能在任务执行过程中才能逐步暴露。如果在一个任务在达到可验证阶段之前，需要连续做出很多相互依赖的决策，那么早期的一处偏差仍可能在任务执行过程中被不断放大，最终可能有一大坨需要我们返工。

因此我们面对大粒度任务输入要做的是先细分成多个小粒度的问题输入，对于每个较小粒度的输入再确定决策点、消除不确定性。Matt Pocock 的 `wayfinder` skill 其实就在做这件事，其会将一个宽泛的输入划分为多个更细粒度的决策 ticket (工单)，可以结合 grilling 这种 skill 来深究细节，完成 ticket 后就能得到某一个小点中具体的决策。比如上述输入可能会被拆分为:  

```text
Ticket 1：定义实时语音交互链路
决定 VAD、ASR、LLM、TTS 如何串联，以及哪些阶段需要流式处理。

Ticket 2：定义 Live2D 驱动方式
决定口型、表情、动作分别由哪些信号触发，以及需要达到什么同步精度。

Ticket 3：定义角色记忆机制
决定红莉栖需要记住哪些信息、记忆保存多久，以及何时召回。

...
```

在得到具体决策后才会开始写需求和规范文档（对应 `to-spec`），随后则可以根据规范文档垂直切分（对应 `to-tickets`）为具体的端到端开发任务（可独立开发、验证和交付的完整小功能），最后分而治之，执行每个开发任务并完成实现和验收。例如上面的示例 Ticket 1 完成决策并形成规范后:  

```text
垂直切分任务 1：实现一次最小可用的流式对话
麦克风输入 → VAD 判断说完 → 流式 ASR → LLM → 流式 TTS → 播放首段语音。
验收：用户说完后能在目标延迟内听到回答。

垂直切分任务 2：实现可打断对话
用户在助手说话时重新开口 → 停止当前 TTS → 识别新输入 → 生成并播放新回答。
验收：能够稳定打断，不出现旧音频继续播放。

垂直切分任务 3：接入 Live2D 口型
TTS 播放 → 输出音量/音素信号 → 驱动 Live2D 嘴型 → 播放结束后闭嘴。
验收：完成一轮对话时，语音与角色口型同步。

...
```

* 注：分而治之不代表这些开发任务是彼此孤立的。**垂直拆分后的任务间很可能是有依赖关系**的（这些开发任务组织到一起会形成一张 DAG，有向无环图），需要先完成前置开发任务才能继续后继的。比如上面的“垂直切分任务 3”就需要至少等待 Live2D 基础渲染能力接入以及“垂直切分任务 1” 完成后才能进行。

总的来说就是要先把大而模糊的问题转换为小而明确的问题并作出决策，接着根据决策编写规范，划分为细粒度的开发任务，确保开发过程中模型能收到更多反馈，以避免错误积累过多，最终以更低的成本来完成这类复杂的任务。


<!-- 对于粒度较大的单个任务，在消除不确定性后，可以进一步拆分成较小的可执行、可验证的小粒度任务 -->

<a id="progressive-disclosure">

### 1.1. 在上下文中渐进式披露信息，也聊聊 MCP

直到最近，还可能会遇到这样的贴子：  

“谁说 DeepSeek API 省钱了，我发了一句‘你好’就花了几毛钱！”  

这其实很可能是因为 Coding Agent 的**初始上下文开销就不菲**。点名批评 MCP (Model Context Protocol)，虽然协议本身没有明确指定工具何时在上下文披露，但很多 Agent 客户端的实现都会把发现的 MCP 工具的描述和出入参结构定义一股脑塞进上下文，即使我们一次会话可能最多只用到了其中一两个工具。

为了简化初始上下文，**渐进式披露** (Progressive Disclosure) 的思想非常重要：*最初只提供最必要的信息，仅在需要的时候才在上下文中载入额外信息*（如工具定义）。Agent Skills 就是典型的符合这种思想的规范，最开始仅有必要的索引（如名字和描述）信息会被加入上下文，仅当用户显式指定或者模型认为要使用时才把相应 skill 文档载入。  

然而 skills 多了的话，依旧会占用一些上下文空间，有没有**在引入前完全不占用上下文**的呢？可以借鉴 Pi 作者的文档 + Bash 思想，我们能直接把任务指示和调用工具的方法写成简短文档，需要用的时候让模型去读取即可。这里“让模型去读取”的实现也是比较多样的：很多 Agent 客户端支持通过 `@FILE` 语法把文件载入上下文；也有像 Pi 这样支持提示词模板，可以直接把文档转换为斜杠命令的；最朴素的方法当然是直接通过对话让模型去读取某个文档了。  

无论是 skill 还是提示词模板，本质上都是文档 (Documents)，都在告诉模型如何调用工具 (Bash)、怎样在状态间进行流转，归根到底还是应了那句话: *Bash and Documents are All You Need*!  

再看回 MCP，近来一些 Agent 客户端其实已经支持了渐进式的 MCP 工具发现 (Progressive Discovery)，同时支持更细粒度的工具分组，每次只用载入某一组工具的详细信息，但是 skill 这类靠文档定义的方式依旧优势明显:  

1. 不像 MCP 那样要被协议的条条框框所约束，可自由定制和组合，文档方式定义的粒度也很自由，可以灵活变更，**充分复用现有的命令行工具生态**；
2. 使用 MCP 必须要 Agent 客户端支持，而文档 + Bash 只要环境依赖支持，几乎可以直接贯通于整个 AI Agent 生态。

当然 MCP 依旧还有使用场景，比如可以提供开箱即用的工具接口，不需要像本地执行那样还要安装依赖，对于比较重量级的应用来说如果在每台调用端都准备一整套依赖也是不现实的...blablabla...但话又说回来了，我为什么不能为这种应用搭配一个 CLI 工具呢，就像 tavily 的 `tvly` 一样，不需要多少依赖，一样可以用文档描述用法让模型进行远程调用（实际上也确实有人写了把 MCP 封装成命令行 CLI 的工具: [openclaw/mcporter](https://github.com/openclaw/mcporter)）。**我所能想到的，MCP 最大的优势**，大概在于“能及时把最新的接口告知 Agent”：如果远程接口发生了破坏性变更，而用户 CLI 工具还没有更新，就无法成功调用了；而 MCP 服务每次可以把最新的接口模式定义递交给 Agent（说得不通俗一点就是有“标准化的能力发现和同步机制”）。另外就是 **CLI 不可用的场景**下，也就只能使用 MCP 来调用外部能力了。

总的来说日常编码绝大多数场景其实 Bash + 文档就足够了，且符合渐进式披露的思想，能尽量保持上下文简洁，开销通常更低。就 token 使用效率 (Token Efficiency，可以理解成每个 token 对 Agent 任务执行成功概率的边际贡献，即新加入上下文的 token 是否能尽量提升任务成功的概率而不是降低) 来说显然是 **文档 / 提示词模板 > Skills >> MCP** 的。


### 1.2. Sub-agents 的使用

Sub-agents（子 Agent）是典型的上下文管理手段。

个人认为，上下文工程 (Context Engineering) 中很重要的一点就是尽量减少不必要的干扰，让**模型上下文大部分信息和当前执行的任务相关**，保持简洁。比如在代码仓库诊断 bug 时通常要先扫描仓库代码来理清项目结构和模块依赖关系，这个过程如果在主上下文中进行，会有巨量冗余的内容进入上下文，且在后续修复任务中可能只会修改某几行代码，其余的根本没用上，产生了极大的浪费；更糟的是，过分繁杂的上下文可能会干扰模型对指令的遵循效果，亦或者是很容易触发上下文压缩导致信息丢失，最终致使任务执行结果变差。  

前段时间和一位同学交流了一下，得知他编码完成后比较习惯新开一个会话来执行独立任务（如进行代码审查），而确实若下一个任务和当前主 Agent 任务没有直接依赖关系，新开一个会话来进行是不错的实践。Sub-agents 基本原理其实也就是新开一个独立的、隔离于主 Agent 的上下文，在其中执行独立的任务，最终**只把报告回传给主 Agent**，这样主 Agent 的上下文中就不会受到子任务的干扰。  

个人认为，使用 sub-agents 的时机有三种：主 Agent 执行任务前、执行任务中以及执行任务后（Agent 的执行单位是任务），以编码任务为例：  

1. **执行任务前**: 通常是进行探索性的**独立**任务。比如上述的代码仓库扫描，可以生成报告用作主任务的**补充**信息。
2. **执行任务中**：可以并发执行相互**独立**的编码任务。比如主 Agent 写主要代码，子 Agent 来补充单元测试（但如果用 TDD 开发其实没必要这样做）。
3. **执行任务后**：可以作为第三方视角来执行**独立**的代码审查任务，进行交叉检查。

尤其值得一提的是“执行任务中”的情况，我觉着还是应该尽量把编码任务放到主 Agent 执行：子 Agent 执行任务的效果**取决于主 Agent 是怎么构造子 Agent 的上下文的**，也就是主 Agent 决定了子 Agent 的局部视野（能看到什么信息），可能经过这一层转述，和原本的指令间发生了偏移，产出的内容不一定是符合验收标准的。且就算要用，也尽量用于编写完全不相关（边界明确）、独立于其他 Agent 任务的模块，否则这种局部视野极有可能因模型对模块边界划分不清，导致多个 Agent 的修改互相冲突，最终堆出一坨💩山。  

因此，Sub-agents 还是更加适合**在主 Agent 执行任务前、后**使用，适合子 Agent 的这些任务都有类似的特性:  

1. 我们**更关注其结论**，中间过程大量信息是冗余、主 Agent 用不到（和主任务无关）的；
2. 不依赖于主任务（耦合度低），独立性强且边界清晰。

最后再提一嘴个人觉得最糟糕的 sub-agents 实践：让并发的子 Agent 之间相互通信。要解决共享带来的并发竞态问题不说，子 Agent 相互通信往往意味着要让他们再次转述其认为重要的信息，也就会产生局部视野的局部视野，进一步加剧了边界的撕裂以及与主 Agent 的偏差，也显然打破了子 Agent 执行任务的隔离性。不会真有人这样做吧？！


<!-- ----------------------------------------------------------------------   -->



模型自身能力

约束框架的能力

输入侧：

不要一口气吃成胖子，每次执行的任务粒度不要太大，慢就是快（合理拆分任务再开始）

AI 擅长什么，不擅长什么

大模型不值得相信，尤其是编辑文档时必须要人为审阅，尤其是更新需求文档的时候，小心一不小心给你全覆盖了。

人都无法脱离不确定性，大模型更难以脱离不确定性，所以 Harness 肯定在很长一段时间内都是有用的，

多交叉检查，左脚踩右脚上天。



模型自身后训练效果有强有弱，但真正决定能发挥出模型百分之多少功力的，


SubAgents 适合什么任务？（完全独立的任务（比如代码审查，论文数据检查）最适合，而不是并行做本质上相关联的事，如多个 sub agents 并行写一个项目的不同模块）

<!-- 文章可以用有说服力的数据，比如 token 耗费这一块，缓存命中这一块 -->

可以定制适合自己的 Coding Agent

项目级 AGENTS.md

<!-- 在 Windows 上尽量不要用 PowerShell -->