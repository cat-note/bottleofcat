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

阅读了作者的[博客文章](https://mariozechner.at/posts/2025-11-30-pi-coding-agent/)后，个人理解，Pi 的哲学简单来说就是 “*Bash and Documents are All You Need*”：只要有 Bash，Agent 就能在当前用户权限的范围下执行任何操作，复用现有的命令行程序，**甚至再启动一个它自己的实例**；而只要有 Documents (通常是 Markdown 这种结构化文档)，Agent 就能将上下文中的信息持久化到硬盘上并能够**按需**读取和更新，也能读取文档，按照文档中的指示执行操作，而且这些文档是**可以跨工具、跨 Agent，甚至是可以在 Agent 和人之间共用**的。

二者组合起来有多强大呢？不妨看看 Pi 和开箱即用 Agent 的不同之处，以及符合上述哲学的解决方案：  

1. **没有 MCP**：我们可以写文档说明如何在 Bash 中调用各种工具，需要使用时**按需**让模型载入文档即可；
2. **没有 Sub-Agents**：Agent 可以通过 Bash 再启动另一个 Agent 实例，并且可以通过文档来共享必要的信息；
3. **没有 Plan / Todo 模式**：完全可以让 Agent 把计划写入本地文档（如 PLAN.md），然后新开会话载入文档来执行计划。而 TODO 也可也写成文档，让 Agent 在执行计划的同时顺手去修改 TODO.md 来勾选完成的内容。  

追究到底，Pi 主打复用现有的命令行工具生态，不引入过多的复杂度，实在满足不了需求再自己加。个人认为，之所以可以如此极简（只要有 Bash 和文档读写就能完成大部分操作），还得是多亏了模型本身经过强化学习后训练所掌握的**越来越强大的任务规划和指令遵循能力**。模型越强，以往框架中繁多的约束就越冗余，我们能简化的部分就越多。  

* 比如最近 GPT 5.6 推出后，Superpowers 这类规范工作流程的 Skills 反而会降低模型的编码效率（尤其是在中小项目上），浪费更多 Token，而产出也没比模型自行规划好多少。  

但！是！纵使模型再强大，其也终究是按概率来行事，有些缰绳是我们不能完全放松的，比如安全边界的硬性控制。

总的来说，在 Pi 这个毛坯房上定制自己的 Harness 时，比较重要的应该是搞清楚模型现在还不擅长什么，以及如何安装或极简封装我们比较常用的功能。接下来就写写咱是怎么配置自己的 Pi 的。    

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


<!-- TODO: 备用本地 web fetch-->

### 0.2. 安装和魔改 Matt Pocock 的 Skills

随着模型能力越来越强，咱认为对于中小项目越来越不需要 OpenSpec, Superpowers 这类比较沉重的工作流编排 skills，繁重的约束虽然能最大程度对项目进行规范，但毫无疑问**相当耗费 Token 且效率极低**。  

有时候一些**简单而高效**的 skills 用起来反而更顺心，比较有名的就是 [Matt Pocock](https://github.com/mattpocock/skills) 的这一套。打开仓库可以发现有数十个 skills，但实际上我并不需要这么多，暂且先安装 engineering (工程化) 和 productivity (生产力) 两方面的:  

```bash
# https://github.com/mattpocock/skills - commit hash: 9603c1cc8118d08bc1b3bf34cf714f62178dea3b
npx skills@latest add https://github.com/mattpocock/skills/tree/main/skills/engineering --skill '*' --agent pi -y --copy
npx skills@latest add https://github.com/mattpocock/skills/tree/main/skills/productivity --skill '*' --agent pi -y --copy
```

Matt Pocock 的这批 skills 在设计的时候是为了维护 GitHub 这类代码托管平台的项目的，因此他引入了 Issue Tracker (Issue 跟踪器，比如 GitHub Issues) 这个概念，多个 skills 都是围绕着 Issue 跟踪器来进行的，用 Issue 编排了需求的开发工作流（我寻思这应该算是一种 **Issue 驱动开发**的思想）。但我还是更习惯于**文档驱动**的思路，其能减少开发工作对某个平台的依赖（毕竟 Issue 跟踪器是依托于托管平台的），只要有文件系统就能进行存放，因此我想去掉一些 Issue 跟踪器相关的 skill，且对其余 skill 进行魔改简化。  

进入 `./.pi/skills`，移除下列这些我暂时不需要的:  

* `teach` - 用户学习用，可以生成一些课件、问答之类的。
* `triage` - 需要结合 GitHub Issues / Pull Requests，主要用来管理 Issue 的分类和状态，本地开发通常用不着。
* `setup-matt-pocock-skills` - 初始化项目，主要初始化 triage 技能相关内容。
* `resolving-merge-conflicts` - 冲突解决一般是手动来的，不太用得上。
* `ask-matt` - Matt Pocock 的 skill 路由，但其实用得多了就不太需要了，主打一个自由组合。
* `wayfinder` - 针对大型项目，拆解模糊的大目标，调查找到解决问题的路线，围绕 Issue 跟踪器展开。个人而言，开发大型项目就通常不会用 Matt Pocock 这一套流程了。

移除了上面这些 skill 后，还有下列这些 skills 有 Issues 跟踪器相关残留，我们需要进行魔改：  

* `to-spec` - (原本叫 `to-prd`) 把对话内容转换为规范和需求文档，并作为大 Issue 发布到 Issues。  
* `to-tickets` - (原本叫 `to-issues`) 根据 Spec 垂直切片拆分任务 (有点 TODO 文档的意思)，即把规范文档大 Issue 拆分成多个小 Issue。  
* `code-review` - 代码审查，可能会去查找 commit 消息中涉及的 Issues。  
* `implement` - 可以根据规范文档或者拆分得到的 Issue 来执行一个 TDD 开发、测试、审查和提交流程。

其实 `setup-matt-pocock-skills` skill 是可以直接指定本地 Issue 跟踪器的（本质上是通过往 `.scratch` 目录下写 Markdown 文档来实现），但我觉得既然都要围绕文档工作了，其实没必要再抽象出一个 Issue 跟踪器这样的模块了（况且每次开发时还得在一开始使用一次 `setup-matt-pocock-skills` 来进行设置）。如果直接在涉及 Issue 跟踪器的每个 skill 中直接指定文档写入的路径，就免去了这一个设置的流程，用起来是更加方便的。  

这里可以直接用 `grill-me` 来让模型拷问我们，对齐魔改需求后执行修改：  

```text
/skill:grill-me 我移除了 teach, triage, setup-matt-pocock-skills, resolving-merge-conflicts, ask-matt, wayfinder 这几个 Matt Pocock 的 skills，抛弃了围绕 Issues Tracker 的开发方式，转而我希望能围绕本地文档来进行。现在应该需要对 to-spec, to-tickets, implement, code-review 这几个 skills 进行修改，我们讨论一下该怎么改。
```

* Matt 这一套方法论建立于两个关键的抽象上—— Spec 和 Tickets，对应需求规范和细分的执行工作单元。无论是围绕 Issue 还是围绕文档，实际上都是这两个抽象的不同实现方式，因此就算进行了魔改，用 Spec 来明确需求意图、垂直拆分 Ticket 后执行开发的核心模型还是保留了下来。  

我和 DeepSeek V4 Pro (预览版) 对齐后产生的决策文档在这里：[MATT_SKILLS_MODIFY_PLAN.md](https://github.com/SomeBottle/pi-config/blob/0a02df04472269860f0a9364d787030314bde0e7/.pi/skills/MATT_SKILLS_MODIFY_PLAN.md), 魔改后的 Matt Skills 在这里: [.pi/skills](https://github.com/SomeBottle/pi-config/tree/0a02df04472269860f0a9364d787030314bde0e7/.pi/skills) 。  

### 0.3. 实现自己的 Subagents 功能

Pi 没有自带子 Agent 功能。  

从 Pi 的[命令行文档](https://pi.dev/docs/latest/usage#cli-reference)可以看到 Pi 在命令行参数上支持非交互模式（`-p`, `--print` 选项，输出后退出），可以指定模型和供应商、可用的工具、模型思考等级以及系统提示词等。也就是说我完全可以让 Pi **通过 Bash 启动另一个非交互式的 Pi 实例来执行任务**，任务执行完成后其会把结果输出到标准输出以传递给主 Agent。  

令人欣喜的是，Pi 的工具调用是默认并行的（见[文档](https://pi.dev/docs/latest/extensions#custom-tools)），也就是说可以同时派发多个 Pi 实例，这下看上去真的就有点像派发一批子 Agent 了。  



<!-- TODO: 其他的一些原则：尽量少用 pi 扩展或者第三方包，而是多依靠 skill，pi 本身还在经常更新，代码层面 api 可能有破坏性改动-->

## 1. 一些实践经验

### 1.0. 优化输入，明确需求

想象一下，当甲方抛给你一坨不清不白的需求且不告诉你更多细节时，你是不是只能自行发挥？但是抓耳搔腮发挥完后给甲方，甲方又给你甩脸色说没按他的来，你又只得憋住红温的脸继续改来改去 (╥﹏╥)。  

最近实习的时候我也遇到过需求对齐上的问题，指导人让我写一个测试工具来测试系统中的某个模块，但是写完了才告诉我还需要考虑效率，也就是制造测试数据时需要足够快（可能要制造亿级的数据）

<!--消除不确定性-->

<a id="progressive-disclosure">

### 1.1. 在上下文中渐进式披露工具和知识

Prompt Template > Skills 

只保留必要的索引。

保持上下文清晰简洁。


### 1.2. Subagents 的使用




模型自身能力

约束框架的能力

输入侧：

初始需求描述越清晰越好，不要吝啬自己的言语，减少给模型妄想的空间
输入时最好不仅说你要做什么，还要说明模型不要做什么（不要做什么这里可以加上 few shot），对发散性比较强的模型（比如 DeepSeek V4 预览版），保险起见，一些比较细节的约束也可以再重申（比如分表时的规则）

不要一口气吃成胖子，每次执行的任务粒度不要太大，慢就是快（合理拆分任务再开始）

上下文尽量干净，越复杂的上下文越扰乱模型发挥（慎用 MCP，工具自身就占用了很多上下文，尽量用渐进式披露的 Skills 或者 Prompt Templates，亦或者可以显式让模型阅读文档，更省 Token（有的 Agent 支持 @ 引入文件））。  
https://mariozechner.at/posts/2025-11-02-what-if-you-dont-need-mcp/#toc_0
* 无论是 SubAgents 还是 MCP，这个作者主打 bash is all you need
* 还有就是多写文档，用文档来写模型指示、驱动工具（比如类似 skills 的工具调用方式），用文档（artifact）来传递上下文（PLAN, AGENTS, TODO, 以及 SubAgents 的提示词）；让模型按需来读取和组合。

上面这两点对所有 Coding Agent 都是通用的，因为 Pi 这种基础的毛坯房就全部都支持。

* 为什么显式让模型阅读文档 / Prompt Templates 更省 token，因为他们和 Skills 的不同之处在于，Prompt Templates 和显式让模型阅读文档是在加载前不会占用一点上下文，而 Coding Agent 虽然实现不同，但至少会把 Skills 的名称、描述这两个元数据塞入上下文。

AI 擅长什么，不擅长什么

大模型不值得相信，尤其是编辑文档时必须要人为审阅，尤其是更新需求文档的时候，小心一不小心给你全覆盖了。

人都无法脱离不确定性，大模型更难以脱离不确定性，所以 Harness 肯定在很长一段时间内都是有用的，

多交叉检查，左脚踩右脚上天。



模型自身后训练效果有强有弱，但真正决定能发挥出模型百分之多少功力的，


SubAgents 适合什么任务？（完全独立的任务（比如代码审查，论文数据检查）最适合，而不是并行做本质上相关联的事，如多个 sub agents 并行写一个项目的不同模块）

<!-- 文章可以用有说服力的数据，比如 token 耗费这一块，缓存命中这一块 -->

可以定制适合自己的 Coding Agent