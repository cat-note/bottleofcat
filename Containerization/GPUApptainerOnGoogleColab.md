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

利用 `setcap` 进行 capabilities 权限设定还是麻烦了，这里我根据 issue 中的指引，直接在一个新的**命名空间**下运行了容器:  

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

# 3. 尝试跑一个 GPU 容器

## 3.1. 构建 .sif 文件

这里拉取了 Docker Hub 上 PyTorch 官方的一个 GPU 支持镜像：  

```bash  
# 转换得到 pytorch-gpu.sif 文件
apptainer build pytorch-gpu.sif docker://pytorch/pytorch:2.5.1-cuda12.4-cudnn9-runtime
```

## 3.2. 使用 Apptainer 的标准 GPU 支持

Apptainer 默认支持 NVIDIA GPU，前提是宿主机系统中已经安装了 Nvidia 驱动以及 CUDA 相关库。

* 具体要求依旧见[官方文档](https://apptainer.org/docs/user/1.3/gpu.html#requirements)。  

在 `apptainer` 的 `run`, `shell`, `exec` 等命令后加上 `--nv` 选项即可启用 GPU 支持:  

```bash
# 以 somebottle 身份启动 GPU 容器，容器内用户为 somebottle
# 启动容器后执行 nvidia-smi 来查看 GPU 状态
sudo -u somebottle apptainer exec --nv pytorch-gpu.sif nvidia-smi
```

## 3.3. 問題、襲来  

![problem_emergence-2025-01-21](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/problem_emergence-2025-01-21.png)  

诶？动啊，NVIDIA，为什么不动？  

![mio_solemn-2025-01-22](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/mio_solemn-2025-01-22.png)  

```text
NVIDIA-SMI couldn't find libnvidia-ml.so library in your system. Please make sure that the NVIDIA Display Driver is properly installed and present in your system.
Please also try adding directory that contains libnvidia-ml.so to your system PATH.
```

定睛一看发现是在容器内找不到动态链接库 `libnvidia-ml.so`。  

## 3.4. 解决问题

提到动态链接库路径，很快能想到一个环境变量 `LD_LIBRARY_PATH`，动态链接器会在其列出的目录下搜索库。分别在宿主机和容器内输出这个环境变量看看:    

```bash
unshare -r env | grep LD_
# >> LD_LIBRARY_PATH=/usr/lib64-nvidia
unshare -r apptainer exec --nv pytorch-gpu.sif env | grep LD_
# >> LD_LIBRARY_PATH=/usr/local/nvidia/lib:/usr/local/nvidia/lib64:/.singularity.d/libs
```

可见**宿主机上 NVIDIA 动态链接库**位于 `/usr/lib64-nvidia`，而容器内列出了三个路径，其中前两个路径对应的目录是不存在的，链接器会去 `/.singularity.d/libs` 这个路径下找共享库。  

```bash
# 输出看看目录下有什么
unshare -r apptainer exec --nv pytorch-gpu.sif ls -ahl /.singularity.d/libs
```

![list_libraries-2025-01-22](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/list_libraries-2025-01-22.png)  

推测在使用 `--nv` 选项时，Apptainer 会自动将宿主机上的 NVIDIA 动态链接库绑定挂载到容器内的 `/.singularity.d/libs` 目录下。  

* [用户文档 - GPU Support](https://apptainer.org/docs/user/1.3/gpu.html#requirements) [1]  
* [管理文档 - Apptainer Configuration Files](https://apptainer.org/docs/admin/1.3/configfiles.html#nvidia-gpus-cuda) [2]  

从上面两个文档可以得知，Apptainer 有一个配置文件 `nvliblist.conf`，其中指定了可执行文件和动态链接库的**文件名**（没错，仅仅是文件名！）。  

* 注: 通过 `find / -name "nvliblist.conf"` 找到配置文件路径。  

在默认的 `nvliblist.conf` 中可以找到上面 `NVIDIA-SMI` 运行所缺失的库: 

![find_libnvidia-ml-2025-01-22](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/find_libnvidia-ml-2025-01-22.png)  

当然也可以找到 `/.singularity.d/libs` 目录下已有的库。  

---

**于是现在问题就转换为了**：为什么 `/.singularity.d/libs` 目录下没有 `libnvidia-ml.so` 和其他一些必要的库呢？  

仍然是看文档， [2] 中提到:  

> When adding new entries to `nvliblist.conf` use the bare filename of executables, and the `xxxx.so` form of libraries. Libraries are resolved via `ldconfig -p`, and exectuables are found by searching `$PATH`.   

即共享库（`.so`）路径是通过 `ldconfig -p` 来解析的，而可执行文件则是通过 `$PATH` 来搜索的。  

```bash
# ldconfig 可以管理动态链接库的缓存
# 查看 ldconfig -p 的输出（输出已缓存的库），筛出有 nvidia 字段的
ldconfig -p | grep nvidia
# 没有输出
```

哔啵~问题已定位。接下来只需要把宿主机上的 NVIDIA 共享库目录 `/usr/lib64-nvidia` 加入到缓存中即可。  

`ldconfig` 会从 `/etc/ld.so.conf.d` 目录中的配置文件读取共享库路径，先看看这个目录下有什么:  

```bash
ls -hl /etc/ld.so.conf.d
# >> total 36K
# >> -rw-r--r-- 1 root root  41 Sep  9  2023 000_cuda.conf
# >> -rw-r--r-- 1 root root  44 Sep  9  2023 988_cuda-12.conf
# >> -rw-r--r-- 1 root root  15 Jan 17 14:36 colab.conf
# >> -rw-r--r-- 1 root root  38 Mar  5  2022 fakeroot-x86_64-linux-gnu.conf
# >> -rw-r--r-- 1 root root  46 Aug 15  2023 gds-12-2.conf
# >> -rw-r--r-- 1 root root  44 Dec 16  2020 libc.conf
# >> -rw-r--r-- 1 root root  46 Nov 10  2023 nvidia.conf
# >> -rw-r--r-- 1 root root 100 Mar  4  2022 x86_64-linux-gnu.conf
# >> -rw-r--r-- 1 root root  56 May  6  2024 zz_i386-biarch-compat.conf

cat /etc/ld.so.conf.d/nvidia.conf /etc/ld.so.conf.d/*cuda*.conf
# >> /usr/local/nvidia/lib
# >> /usr/local/nvidia/lib64
# >> /usr/local/cuda/targets/x86_64-linux/lib
# >> /usr/local/cuda-12/targets/x86_64-linux/lib

ls /usr/local/nvidia/lib
# >> ls: cannot access '/usr/local/nvidia/lib': No such file or directory
```

看来大概是 Colab 官方的配置有误。  

最后，咱们**直接把路径 `/usr/lib64-nvidia` 写入到这里的一个配置文件**中:    

```bash
echo "/usr/lib64-nvidia" >> /etc/ld.so.conf.d/nvidia.conf
```

然后**刷新缓存**，让 `ldconfig` 重新读取配置文件即可:  

```bash
ldconfig
```

再次查看 `ldconfig -p` 的输出，就可以看到 `libnvidia-ml.so` 等共享库已经被缓存了。  

再次尝试运行 GPU 容器，`nvidia-smi` 就能正常打印出 GPU 信息了，冇问题啦!   

![nvidia-smi_success-2025-01-22](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/nvidia-smi_success-2025-01-22.png)  

## 3.5. 测试一下 PyTorch 是否能正常使用 GPU

咱先把如下脚本 `test.py` 放在了 Colab 会话环境的 `/content/test` 目录下:  

```python
import torch
if torch.cuda.is_available():
    device = torch.device("cuda")
    print(f"GPU 可用，设备: {torch.cuda.get_device_name(0)}")
else:
    device = torch.device("cpu")
    print("GPU 不可用，设备: CPU")
tensor = torch.tensor([1.0, 2.0, 3.0])
tensor = tensor.to(device)
print(f"张量: {tensor}")
print(f"张量所在设备: {tensor.device}")
# compute
result = tensor * 2
print(result)
```

把 Python 脚本挂载到容器内并执行:  

```bash
sudo -u somebottle apptainer exec --bind /content/test:/mnt/data --nv pytorch-gpu.sif python /mnt/data/test.py
# >> GPU 可用，设备: Tesla T4
# >> 张量: tensor([1., 2., 3.], device='cuda:0')
# >> 张量所在设备: cuda:0
# >> tensor([2., 4., 6.], device='cuda:0')
```

这样一来我应该就能愉快地在 Google Colab 等平台上使用 Apptainer 跑实验啦~  

## 3.6. 关于 sudo -u

在上面的例子中，我使用了 `sudo -u somebottle <command>` 来以 somebottle 用户的身份执行命令。  

然而 `sudo -u` 实际上可能会重置环境变量，可以打印出来看看:  

```bash
echo $PATH
# >> /opt/bin:/usr/local/nvidia/bin:/usr/local/cuda/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/tools/node/bin:/tools/google-cloud-sdk/bin
sudo -u somebottle env | grep PATH
# >> PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin
```

很明显至少环境变量 `$PATH` 被重置了。  

3.4 节中文档 [2] 曾提到:  

> ... , and exectuables are found by searching `$PATH`.  

即 Apptainer 在挂载时， `nvidia-smi` 这类可执行文件是依赖于 `$PATH` 环境变量进行搜索的。幸运的是，可执行文件 `nvidia-smi` 在 `/usr/bin` 目录下有软链，因此这里启动容器时 Apptainer 能成功找到。    

```bash
whereis nvidia-smi
# >> nvidia-smi: /usr/bin/nvidia-smi /opt/bin/nvidia-smi
ls -hl /usr/bin/nvidia-smi
# >> lrwxrwxrwx 1 root root 27 Jan 22 13:34 /usr/bin/nvidia-smi -> /opt/bin/.nvidia/nvidia-smi
```

----

假如需要保留 `sudo` 执行命令时的环境变量，可以使用 `sudo -E` 选项，但尽管如此 `$PATH` 环境变量可能仍然会被重置，因为在 `/etc/sudoers` 中可能有 `secure_path` 配置项进行了限制:  

```bash
cat /etc/sudoers | grep secure_path
# >> Defaults   secure_path="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin"
```

这种情况下要不直接修改 `/etc/sudoers` 文件，要不就手动设置 `$PATH` 环境变量:  

```bash
# env PATH=$PATH <command>，在指定环境变量后执行命令
sudo -u somebottle env PATH=$PATH printenv PATH
# >> /opt/bin:/usr/local/nvidia/bin:/usr/local/cuda/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/tools/node/bin:/tools/google-cloud-sdk/bin
```








