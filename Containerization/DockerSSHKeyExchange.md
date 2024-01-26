# Docker容器间SSH公钥自动交换实现免密登录的一次尝试

## 咋想到这茬了

最近开始忙毕设的事儿了，想部署个伪分布式的Spark + Hadoop集群来进行测试。思来考去，最终咱把目光放在了Docker上。  

盘了两天，发现这玩意意外的有趣，镜像构建好后开箱即用，省去了些配置环境的成本。  

不过呢，在配置Hadoop的时候我发现了一个问题——Hadoop分布式搭建要求各节点间能通过SSH执行指令（比如启动时就需要在其他节点上执行指令以启动相应守护进程），即需要配置免密登录（Passphraseless），怎么在尽量保证安全的情况下尽快让容器之间交换SSH公钥以信任对方呢？  

<img src="https://raw.githubusercontent.com/cat-note/bottleassets/main/img/learntbefore-2024-01-25.jpeg" width="150px"/>  

首先咱当然是去Github找了个Hadoop容器化部署项目的Dockerfile看了看，发现有一种方案是在构建镜像的时候就建立SSH密钥对，然后把公钥加入到自己`authorized_keys`文件中。

```dockerfile
# Dockerfile中
RUN ssh-keygen -t rsa -f ~/.ssh/id_rsa && 
    cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys &&
    chmod 600 ~/.ssh/authorized_keys
```

理论上说，用这个镜像建立的容器之间就可以实现免密登录了（毕竟每台机器上`authorized_keys`中都有相同的公钥）。  

然鹅，我在本机WSL上装的docker中测试时发现这种方式怎么也行不通...盘了大半天，文件权限和SSH配置改来改去，SSH服务硬是没给我个好脸色，查了很久也没解决这问题。

于是我索性想了另一套方案来解决各容器间交换SSH公钥的问题。  

![losingMemories-2024-01-25](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/losingMemories-2024-01-25.jpg)  

**注**：这篇文章写到这，我恍然大悟，发现自己犯了个很蠢的错误  ↓ 展开查看  

<details>

<summary>展开查看</summary>

--------

我在生成密钥对的时候不知是脑袋哪里缺根筋，偏偏就改了密钥对的默认文件名，当时在Dockerfile里是这样写的：  

```dockerfile
RUN ssh-keygen -t rsa -f /root/.ssh/rsa_pair -N '' \
    && cat /root/.ssh/rsa_pair.pub >> /root/.ssh/authorized_keys
```  

这样一来`/root/.ssh`目录下会生成`rsa_pair`（密钥）和`res_pair.pub`（公钥）。  

这里简述一下SSH密钥对认证的过程:  

1. 客户端发送**连接请求**给服务端；
2. 服务端用**客户端的公钥**对一串随机数据`data`进行加密生成`challenge`，返回给客户端；
3. 客户端用**客户端的私钥**对`challenge`进行解密，得到`data`，然后利用哈希算法`HASH`计算`data`的摘要，将摘要用**客户端的私钥**签名为`signature`后，再发给服务端。  
4. 服务端收到数字签名`signature`后用**客户端的公钥**解密，得到摘要。此时服务端用相同的哈希算法计算出第2步中原数据`data`的摘要，然后比较摘要是否一致。
5. 如果摘要一致，签名成功验证，认证成功。

我测试的时候用的命令是`ssh root@主机名`，而这个命令默认会扫描`id_`开头的密钥文件，而我此时的密钥对文件名是`rsa_pair`，程序自然就找不到对应的密钥对了，也就无法正常进行认证。  

其实这种情况下手动指定密钥文件路径就OK啦，`ssh`和`ssh-copy-id`命令都能接受参数`-i`，指定密钥文件路径:  

```bash
ssh -i /root/.ssh/rsa_pair root@HOST
```

我这小脑袋瓜儿当时硬是没想到这点，所以说有的时候小错误往往可能浪费更多时间哇！╰（‵□′）╯    

<del>菜是原罪</del> 

------

</details>

## 利用Bash脚本实现SSH公钥自动交换的尝试

相关代码已经开源:  

https://github.com/SomeBottle/haspark  

### Dockerfile

1. 导出主机名列表
   
    ```dockerfile
    # 环境变量配置
    # 所有节点的主机名，用于SSH配置
    ENV SH_HOSTS="shmain shworker1 shworker2"
    ```

    构建镜像时导出所有节点的主机名作为环境变量`SH_HOSTS`。

2. 生成随机密码`temp.pass`并临时设为用户密码

    ```dockerfile
    # 先生成一个临时SSH密码，用于首次启动时交换ssh密钥
    RUN echo $(openssl rand -base64 32) > /root/temp.pass
    # 修改root用户的密码
    # 管道符重定向输入
    RUN echo -e "$(cat /root/temp.pass)\n$(cat /root/temp.pass)" | passwd root
    ```
    
    利用openssl生成了32字节的随机数据并用base64方式进行了编码，作为随机密码存储到`/root/temp.pass`。  

    > 其实我想把路径写成`~/temp.pass`的，但bitnami镜像竟然把`HOME`环境变量指定为了`/`...  

    然后将随机密码**设置为用户的密码**，临时用于后面的密钥交换过程。

3. 建立标记文件目录`exchange_flags`  

    ```dockerfile
    # 若.ssh目录不存在则建立
    RUN [ -d /root/.ssh ] || mkdir -p /root/.ssh
    # 建立标记目录
    RUN mkdir -p /root/.ssh/exchange_flags
    ```  

    这个目录是为了配合后面的脚本而存在的。

4. 复制用户SSH配置并调整权限

    ```dockerfile
    # 拷贝本地写好的配置文件
    COPY configs/ssh_config /root/.ssh/config  
    # 调整.ssh目录下文件权限
    RUN chmod 600 /root/.ssh/config \
        && chmod 700 /root/.ssh
    ```  

    这个配置文件中主要配置了`StrictHostKeyChecking`策略，使得初次连接时跳过指纹确认，不然SSH连接建立过程可能卡在这里。

    > 因为我使用了sshpass这个工具来实现自动向ssh命令行传入密码实现登录，而默认情况下首次登录远程主机会列出对方指纹并询问用户是否信任对方，这一步会让sshpass失效。  

    同时`.ssh`目录有严格的权限要求，必须是`700`即`rwx------`（仅当前用户读/写/执行）。  

5. 最后给予脚本可执行权限，并设置`ENTRYPOINT`

    增加执行权限用`chown +x`即可，这里不多赘述。

    ```dockerfile
    # 容器启动待执行的脚本
    ENTRYPOINT [ "/opt/entry.sh" ]
    ```

    这里将`/opt/entry.sh`脚本作为`ENTRYPOINT`，即容器启动时会执行的脚本。  

我的方案是把SSH密钥生成与交换放到了容器初次启动的时候。

### SSH密钥生成与交换脚本

利用上述Dockerfile构建的镜像创建多个容器作为伪分布式节点并启动后：

在每个容器中，首先会执行`/opt/entry.sh`，这个脚本首先会启动SSH服务，然后转后台执行`ssh_key_exchange.sh`脚本，以下主要介绍这个脚本的思路。

脚本地址：https://github.com/SomeBottle/haspark/blob/main/scripts/ssh_key_exchange.sh  

1. 生成密钥对  

    脚本首先会为本容器生成一个密钥对`id_rsa.pub`和`id_rsa`，然后把公钥`id_rsa.pub`写入到本容器的`authorized_keys`文件中。  

    同时把本机的主机名作为文件名，建立一个新文件放在`/root/.ssh/exchange_flags`目录下，作为标记（在下图中以黑色文件名表示）。

    > 因为Hadoop进程启动时也会通过SSH连接到本机，自己到自己也要能免密登录。

2. 将本容器A的公钥分发到`SH_HOSTS`环境变量中指定的其他每台容器上

    - 先通过`ssh-copy-id`结合`sshpass`，通过`temp.pass`临时密码登录到另一个容器B并将本容器A的公钥拷贝到其`authorized_keys`文件中。  
    - 通过`ssh`连接，在这一个容器B上的`/root/.ssh/exchange_flags`下建立一个以本容器A的主机名为文件名的文件作为标记。  

3. 轮询标记文件，直到`SH_HOSTS`中**所有主机名**都存在于`exchange_flags`目录下的文件名中，则认为本容器已经取得其他所有容器的公钥，进入下一步。

4. 删除本容器的临时密码文件`temp.pass`，修改SSH配置文件，**禁止通过密码登录**，并重启SSH服务。

当所有容器都完成上述过程后，每个容器都掌握有对方的公钥，即实现了免密登录，且公钥互不相同。
    
如果有三个节点，主机名分别为`master`, `worker1`, `worker2`，三台机器交换公钥的图示如下：  

![ssh_key_exchange.drawio-2024-01-26](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/ssh_key_exchange.drawio-2024-01-26.png)  

不同颜色的箭头表示公钥传送的方向，以及`exchange_flags`目录中的标记文件的创建者。

![IMG_202401267251_142x158-2024-01-26](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/IMG_202401267251_142x158-2024-01-26.jpg)  

大概思路就是这样，写这篇文章主要是做个记录，说不定以后还能把这一套利用起来。

## 附：Git的一个坑  

每次涉及到Windows和Linux之间文本文件交换的时候总是很容易遇到行尾序列（换行字符）问题，最坑的是Windows平台上，Git工具默认会在把仓库克隆下来后自动转换为CRLF（`\r\n`）。  

![autoCRLF-2024-01-26](https://raw.githubusercontent.com/cat-note/bottleassets/main/img/autoCRLF-2024-01-26.png)  

而一旦Shell脚本和一些配置文件末尾出现了多余的`\r`字符，就很容易导致一些让人摸不着脑袋的问题。（比如Hadoop进程启动时会提示主机名中有无效字符，就是因为workers文件中有多余的`\r`字符）  

因此要保证行尾序列是原模原样的LF（`\n`），就需要对Git进行一些配置:  

1. 全局关闭自动CRLF转换

    ```bash
    git config --global core.autocrlf false
    ``` 

2. 和`.gitignore`一样编写项目的`.gitattributes`文件，比如：
   
    ```bash
    # 憋给我整成CRLF了嗷，不然保准没你好果汁吃嗷
    *.sh text eol=lf
    *.md text eol=lf
    ```

    指定`.sh`和`.md`为后缀的文件以LF为行尾序列。