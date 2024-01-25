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

然鹅，我在本机WSL上装的docker中测试时发现这种方式怎么也行不通...盘了大半天，文件权限和SSH配置改来改去，SSH服务硬是没给我个好脸色，每次登录多少得报个`Permission denied`，查了很久也没解决这问题。

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

我测试的时候用的是`ssh root@主机名`，而这个命令默认会扫描`id_`开头的密钥文件，而我此时的密钥对文件名是`rsa_pair`，程序自然就找不到对应的密钥对了，也就无法正常进行认证。  

其实这种情况下手动指定密钥文件路径就OK啦，`ssh`和`ssh-copy-id`命令都能接受参数`-i`，指定密钥文件路径:  

```bash
ssh -i /root/.ssh/rsa_pair root@HOST
```

我这小脑袋瓜儿当时硬是没想到这点，所以说有的时候小错误往往可能浪费更多时间哇！╰（‵□′）╯    

吃一堑长一智！  

------

</details>

## 利用Bash脚本实现SSH公钥自动交换

### Dockerfile






