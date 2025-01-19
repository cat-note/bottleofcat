# 【小记】在 Google Colab 上基于 Apptainer 运行 GPU 容器  

最近想到了可能的创新点，准备开始做实验了。在前期我准备先薅一波提供免费 GPU 运算资源的平台的羊毛，但这些平台提供的免费时长并不多，可能这家配额跑完了就要换下家，如果每次都要重新搭建环境多少有些不方便。  

![subaru_nerd-2025-01-18](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/subaru_nerd-2025-01-18.png)  

那咱用容器化技术不就行啦！直接把环境打包成镜像，哪个平台都能跑。  

比起 Docker，这回咱决定采用**更为轻量的** Apptainer（前身为 Singularity）：  

* Apptainer 默认以普通用户的身份运行容器，无需类似于 root 用户的特权，不像 Docker Daemon 那样必须要运行在特权用户下。（因而更安全，也更容易安装部署，不会有什么权限问题）  
* Apptainer 针对高性能计算（HPC）这种并行场景进行了优化（虽然我还不太用得上）。
* Apptainer 支持 Docker 镜像，体验上近乎无缝（这个是最爽滴）。

这篇笔记主要记录一下咱在 Google Colab 上基于 Apptainer 运行 GPU 容器时的踩坑和爬出坑的过程。  

## 1. 安装 Apptainer  

这里不多赘述，图快的话咱主要推荐以下两种安装方式:

### 1.1. 利用官方脚本安装

* [官方文档](https://apptainer.org/docs/admin/1.3/installation.html#install-unprivileged-from-pre-built-binaries) 

注：文档中提到在执行脚本前可能需要安装 `curl`, `cpio`, `rpm2cpio` 等必要的工具：  

```bash
sudo apt-get update
# rpm2cpio 是 rpm 包的工具
sudo apt-get install -y curl cpio rpm
```


### 1.2. 利用包管理器安装

* [官方文档](https://apptainer.org/docs/admin/1.3/installation.html#install-ubuntu-packages)  

Colab 默认是 Ubuntu 系统，copy 官方文档中的命令就能顺利安装了~  

## 2. 尝试跑一下 hello-world 镜像

### 2.1. 构建 .sif 文件

Apptainer 可以直接拉取 Docker Hub 上的镜像并构建成 `.sif` 单文件：  

```bash
apptainer build hello.sif docker://hello-world
```

![build_hello-2025-01-18](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/build_hello-2025-01-18.png)  

### 2.2. 建立普通用户

Colab 默认在 root 用户下执行命令，这里建立一个普通用户 `somebottle` ：

```bash
adduser --home /home/somebottle --gecos "" --shell /bin/bash --disabled-password somebottle
```

* root 用户当然是可以运行 Apptainer 容器的，但为了安全起见，咱接下来会尝试用普通用户来运行容器。  

### 2.3. 启动容器

Colab **对 root 用户的权限有所限制**，如果直接尝试启动容器会出现挂载问题：  

```bash
apptainer run hello.sif
# >> FATAL: container creation failed: mount hook function failure: mount overlay->/var/lib/apptainer/mnt/session/final error: while mounting overlay: can't mount overlay filesystem to /var/lib/apptainer/mnt/session/final: while setting effective capabilities: CAP_DAC_READ_SEARCH is not in the permitted capability set
```

* 相关 issue：https://github.com/apptainer/apptainer/issues/1041  

利用 `setcap` 进行权限设定还是麻烦了，这里我根据 issue 中的指引，直接在一个新的**命名空间**下运行了容器:  

```bash
# 在 root 用户下执行这条命令，容器内用户为 root
unshare -r apptainer run hello.sif
```
> `unshare -r` 建立一个新的**用户命名空间**，并把当前用户映射为命名空间内的 root 用户，这个命名空间内的进程将会默认拥有完整的 capabilities 权限集。  

* 💡 注：如果在 root 用户下执行上面这条命令，其实仍然相当于在特权用户下启动了容器。

---

✨ 或者**以普通用户的身份**运行容器:  

```bash
# 以 somebottle 身份启动容器，容器内用户为 somebottle
sudo -u somebottle apptainer run hello.sif
```

> Apptainer 默认借助用户命名空间来运行容器，系统应支持以非特权方式建立用户命名空间，经测试 Colab 已经满足了这点。具体要求可查看[文档](https://apptainer.org/docs/admin/1.3/user_namespace.html)。    

---

✨ 为了能**让普通用户在容器内以 root 用户的身份运行**，可以加上 `--fakeroot` 选项：  

```bash
# 以 somebottle 身份启动容器，容器内用户为 root
sudo -u somebottle apptainer run --fakeroot hello.sif
```

具体行为可见[官方文档](https://apptainer.org/docs/user/1.3/fakeroot.html)说明。




TODO: sudo -E -u somebottle <command> ，这里 -E 的作用是否需要阐释。注：新的用户命名空间建立时默认会**继承**当前进程的环境变量。  








