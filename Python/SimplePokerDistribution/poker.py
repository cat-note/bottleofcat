#!/usr/bin/python
# -*- coding: UTF-8 -*-
import random
import copy
import timeit

'''
题目：两副扑克牌（去掉大小王）随机分发给4位玩家，并显示输出4位玩家手中的牌
要求：用列表和元组实现
'''
# 使用Python版本：3.8.5
'''
要使用元组，元组是不能变的。我们分发扑克牌这个过程中什么不能变呢？那当然是扑克牌号不能变啦
那么问题又来了，我们把扑克牌丢进了元组里，怎么选出来呢？
我最开始觉着其实没必要用到元组，直接把保存扑克牌的元组转换成列表然后shuffle一下，再按步分给四个人就行了
但有求也要有应嘛，既然要用到元组，这元组可得有点存在感哈~
废话不多说，我们先生成一组扑克牌嗷
'''
'''
扑克牌啊扑克牌，一副有54张，大王小王今天有点事来不了，我们先撇去他们的存在感，那就是一副52张正牌，其中13张牌一组
13张牌一组嘛，我们先丢到一个元组里哈~
'''
pokerGroup = ('A', '2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K')

# 来看看啊来瞧瞧，实打实的元组哈！
print(type(pokerGroup), 'id:', id(pokerGroup))

# 最开始这里直接用了pokerSet *= 4，我忘了牌的花色了(囧)
pokerSet = ()
print(type(pokerSet), 'id:', id(pokerSet))
# 梅花(Club)，方块(Diamond)，红心(Hearts)，黑桃(Spade)
for style in ['spade_', 'hearts_', 'club_', 'diamond_']:
    pokerSet += tuple(map(lambda x: style + x, pokerGroup))

# What's up，并不是原地操作！ 这也不出意料，毕竟元组是不可变的！
print(type(pokerSet), 'id:', id(pokerSet))
# 我们得到了一副正牌，共52张
print(f"There are {len(pokerSet)} poker cards in one set")
# 我们需要的是两副牌，既然强调了两幅牌，那我们做个区分叭
# 第一幅牌有p1_前缀
pokerSet1 = tuple(map(lambda x: '1_' + x, pokerSet))
# 第二幅牌则是p2_前缀
pokerSet2 = tuple(map(lambda x: '2_' + x, pokerSet))

'''
这里运用了函数简写式lambda，扩写相当于
def f(x):
    return '1_'+x
然后map做的事就是把元组中**每个值**都带入这个函数，然后替换成对应的返回值
我们得到的便是一个被函数处理过的新元组
'''

pokerSets = pokerSet1 + pokerSet2
# print里要输出的内容采用逗号分隔，以防内容不同拼接发生错误，逗号输出后会转换成一个空格哦
print('这里有两副牌，每副牌52张卡', '\n', pokerSets, '\n',
      '经过清点一共有' + str(len(pokerSets)) + '张牌')

# 接下来我们创造四个小老弟，后面有多次利用
bros = {}
for i in range(1, 5):
    bros['bro'+str(i)] = []

# 先把事情做周全，我们做个转换为中文卡牌名的函数


def styleName(cardStyle):
    cnStyleName = {
        "club": "梅花",
        "diamond": "方块",
        "hearts": "红心",
        "spade": "黑桃"
    }
    # 防止过多if
    return cnStyleName[cardStyle]


def cardName(cardId):
    splitted = cardId.split('_')
    cardStyle = splitted[1]
    return f"{splitted[0]}副{styleName(cardStyle)}{splitted[2]}"

# 我们做个输出四位老哥手中牌的函数


def showYourCards(bros):
    for theBro in bros:
        myPrint(
            f"//////////////////////{theBro}的牌，共{len(bros[theBro])}张//////////////////////")
        # 注意字典遍历时theBro是键名
        outPut = ""
        for theCard in bros[theBro]:
            outPut += cardName(theCard)+" | "
        myPrint("\t"+outPut)
        myPrint("//////////////////////////////////////////////////////////////\n")


# 写一个输出控制函数
# 静默模式，如果开了将不会输出任何内容
quietMode = False
# 经过网上查询得知*contents可以接受多个参数


def myPrint(*contents):
    if quietMode == False:
        print(*contents)

# 我们再做个收集并清点牌的函数，以防分配有问题没有发现


def collectCards(bros):
    myPrint("\n>>>>>>>>>>>>>>>>>>>>>>>清点结果<<<<<<<<<<<<<<<<<<<<<<<<")
    # 初始化收的两副牌
    collectedSets = {1: [], 2: []}
    # 我们先将花色容器做成一个字典
    styleDict = {
        'spade': [],
        'diamond': [],
        'hearts': [],
        'club': []
    }
    # 为了防止重复，我们给他全变成集合
    # 此处曾经出现错误ValueError: dictionary update sequence element #0 has length 5; 2 is required，我们打断点
    # 大意了大意了，map对象可没法转换成字典，没办法，用for吧
    # styleDict = dict(map(set, styleDict))
    # myPrint(styleDict)
    # return 0
    for i in styleDict:
        styleDict[i] = set(styleDict[i])
    # 我们再把这个字典深复制给两副牌
    styleOfSets = {1: copy.deepcopy(styleDict), 2: copy.deepcopy(styleDict)}
    for theBro in bros:
        for theCard in bros[theBro]:
            # 先把卡牌代号按下划线分割成三个部分
            splitted = theCard.split('_')
            # 第一部分是卡牌所属哪一副，为了方便判断，我们将其转换为整数
            targetSet = int(splitted[0])
            # 第二部分是卡牌花色
            targetStyle = splitted[1]
            # 第三部分是卡牌号
            targetID = splitted[2]
            # 将牌回收到对应的组
            collectedSets[targetSet].append(theCard)
            # 将扑克牌号'A', '2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K'输入对应**那一副牌**中的对应**花色**列表中，以便清点
            # 注意这里是集合，用add方法
            styleOfSets[targetSet][targetStyle].add(targetID)
    for theIndex in collectedSets:
        # 接下来我们再用集合的方法去重，按理来说这些牌根本就不会有重复的，如果去重后清点数字不对，就不妙了！
        # 先指定一下我们当前操作的的容器对象
        operatingList = collectedSets[theIndex]
        # 再指定我们当前操作的花色相关容器对象
        operatingStyleDict = styleOfSets[theIndex]
        # 在去重后我们再次用map和上面定义的cardName函数，将列表中所有元素替换成我们一眼能看懂的中文
        operatingList = list(map(cardName, list(set(operatingList))))
        # 该输出结果啦！
        myPrint(f"回收后第{theIndex}副牌共有{len(operatingList)}张，其中")
        for theStyle in operatingStyleDict:
            # 利用join函数我们能快速输出**当前副**中**对应花色**的牌号们
            targetGroup = operatingStyleDict[theStyle]
            strOfIDs = ", ".join(targetGroup)
            # 使用制表符来进行一下排版，这样好看一点
            myPrint(
                f"\t{styleName(theStyle)}组的卡牌有：{strOfIDs}，共{len(targetGroup)}张")

    myPrint("\n>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<")


# 好了，问题来了，怎么样才能随机发给四位小老弟呢，说到随机那必不可少的便是标准库random了
# 接下来有请我们的第一位嘉宾shuffle~

# 实打实的元组哈
print(type(pokerSets))

# 打包成一个主函数，方便我们多次测试，以得到最高性能


def main():
    # quietMode我们采用全局变量
    global quietMode
    # 初始化运行时间统计
    runningTimes = [0 for i in range(0, 4)]
    # 四个人每人能分到多少张牌
    cardSumForEach = int(len(pokerSets)/4)
    myPrint(f"每个人能分到{cardSumForEach}张牌")
    myPrint("\n有四位小老弟：", bros)
    myPrint("\n<----------------------------------------分发方法1---------------------------------------->\n")
    firstStartTime = timeit.default_timer()
    # 创建这几位老哥的克隆人，利用Python的深复制
    firstFourBros = copy.deepcopy(bros)
    # 我们首先把扑克牌组转换为列表
    pokerList = list(pokerSets)
    # 利用shuffle打乱扑克
    random.shuffle(pokerList)
    # 从第一个人开始分配
    for step in range(0, len(pokerList)//cardSumForEach):
        firstFourBros['bro' + str(step+1)] = pokerList[(step*cardSumForEach):(step+1)*cardSumForEach]
    '''
    这里的 len(pokerList)//cardSumForEach 是算出了分配的步数，要分配四次
    步数step默认从0开始，我们将step+1便能把牌给对应的兄die了
    在这之前我们已经用shuffle获得了一个随机列表pokerList，现在我们只需要分步将其中的牌给兄弟们就行了
    利用了Python的切片性质，比如我们给第一个兄弟的便是随机列表中切片[0,26]的牌，第二个兄弟则是切片[27,52]的牌
    '''
    # 记录运行时间
    runningTimes[0] = timeit.default_timer()-firstStartTime
    # 老铁们，秀个牌！
    showYourCards(firstFourBros)
    collectCards(firstFourBros)
    myPrint("<----------------------------------------分发方法1结束---------------------------------------->")

    # 用shuffle分配行不行，那当然彳亍！但是吧，如果不用shuffle呢？
    myPrint("\n<----------------------------------------分发方法2---------------------------------------->\n")
    # 记录开始运行时间
    secondStartTime = timeit.default_timer()
    # 我们先再生产这几位老哥的克隆人，使用深复制
    secondFourBros = copy.deepcopy(bros)
    # 可以接受卡牌分发的老哥，每个老哥可以拿到26张牌，如果拿满了26张就判定为notAcceptable，从该列表中移除，不再接受卡牌
    acceptableBros = [1, 2, 3, 4]
    # 接下来我们遍历pokerSets这个具有两副扑克的元组，不用打乱这个元组的顺序，也打乱不了
    for theCard in pokerSets:
        # 从四个老哥中随机选一个人出来，用random标准库中的choice
        theBroToGive = random.choice(acceptableBros)
        # 接着我们把遍历到的当前卡牌分配给这位老哥
        # 这样写是为了方便后期修改哦
        operatingList = secondFourBros[f'bro{theBroToGive}']
        operatingList.append(theCard)
        # 这里我们沿用第一种分发方法中的变量cardSumForEach
        if len(operatingList) >= cardSumForEach:
            # 如果这位老哥已经拿了(26)张牌了，就不能再接受更多牌了，从acceptableBros中移除
            acceptableBros.remove(theBroToGive)
        if len(acceptableBros) == 0:
            # 以防万一的写法，如果列表中已经没人可以接受了就该退出循环了
            break
    # 记录运行时间
    runningTimes[1] = timeit.default_timer()-secondStartTime
    # 老铁们，再秀个牌！
    showYourCards(secondFourBros)
    collectCards(secondFourBros)

    myPrint("<----------------------------------------分发方法2结束---------------------------------------->")

    # 在这之后，我们又发现了一个神奇的方法，random.sample(iterable,length)，正如字面意思，它能从可迭代对象中随机取出一段样本
    myPrint("\n<----------------------------------------分发方法3---------------------------------------->\n")
    # 记录开始运行时间
    thirdStartTime = timeit.default_timer()
    # 这里我们首先又要把两副牌转换为一个列表，我们暂且叫它卡池吧！
    pokerPool = list(pokerSets)
    # 再次克隆兄弟们
    thirdFourBros = copy.deepcopy(bros)
    # 每个人能分配到的卡牌数我们继续用cardSumForEach
    # 遍历一下四位老哥
    for index in thirdFourBros:
        # 按照cardSumForEach(26)抽取一个随机样本，给这个老哥
        theSample = random.sample(pokerPool, cardSumForEach)
        thirdFourBros[index] = theSample
        # 接下来我们运用集合的差集运算去掉已经选出的这部分样本
        pokerPool = list(set(pokerPool)-set(theSample))
    # 记录运行时间
    runningTimes[2] = timeit.default_timer()-thirdStartTime
    # 老铁们，再秀个牌！
    showYourCards(thirdFourBros)
    collectCards(thirdFourBros)
    myPrint("<----------------------------------------分发方法3结束---------------------------------------->")

    # 已经...不用再战斗了吗？哦不！还有random.randrange
    # 哈What's up老哥，你这random库真够用啊
    # 我给哥们分牌的，能给你假随机库？
    # 没错，我们之前是用choice随机选哥们，如果是用随机选牌给哥们呢？

    myPrint("\n<----------------------------------------分发方法4---------------------------------------->\n")
    # 记录开始运行时间
    fourthStartTime = timeit.default_timer()
    # 我们还是需要一个序列，重制一下卡池哈
    pokerPool = list(pokerSets)
    # 再次克隆老哥们
    fourthFourBros = copy.deepcopy(bros)
    # 现在给的是哪个兄弟
    theBroToGive = 1
    for index in range(0, len(pokerPool)):
        # 我们从卡池中抽出一张，为了统一，我们这里采用randrange，用pop返回选出值的时候从列表中去除这个值
        theCard = pokerPool.pop(random.randrange(0, len(pokerPool)))
        # 指定给牌的兄弟列表
        operatingBro = fourthFourBros[f'bro{theBroToGive}']
        # 把牌给兄弟
        operatingBro.append(theCard)
        # 如果兄弟已经拿满了cardSumForEach(26)张牌，就该给下一个兄弟了
        if len(operatingBro) >= cardSumForEach:
            theBroToGive += 1
    runningTimes[3] = timeit.default_timer()-fourthStartTime
    # 老铁们，最后秀牌了！
    showYourCards(fourthFourBros)
    collectCards(fourthFourBros)

    myPrint("<----------------------------------------分发方法4结束---------------------------------------->")
    # 只有第一次运行会输出
    quietMode = True
    # main函数返回runningTimes
    return runningTimes


# 我们运行十次程序来计算性能
testTimes = 100
totalRunningTimes = []
for i in range(0, testTimes):
    # 运行一次程序
    theTest = main()
    totalRunningTimes.append(theTest)
totalPlus = list(zip(*totalRunningTimes))
'''
这里的星号非常有意思,将totalRunningTimes转换为了一个**元组**，作为**无名参数**传入了zip函数，我们具体做了什么呢？
每次运行main()函数会返回四个分发方法的耗时（秒数浮点数），按顺序放在列表里，像这样：
[0.1,0.2,0.3,0.4]
这就代表第一个方法耗时0.1，第二个方法耗时0.2，第三个方法耗时0.3，第四个方法耗时0.4
然后我们以共有非常多组数据：（假如运行了三次，有三组耗时数据）
[0.1,0.2,0.3,0.4]
[0.8,0.3,0.5,0.6]
[0.53,0.34,0.56,0.10]
我们把这三组数据放到了一个列表totalRunningTimes里，也就是这样：
[
    [0.1,0.2,0.3,0.4]
    [0.8,0.3,0.5,0.6]
    [0.53,0.34,0.56,0.10]
]
当我们使用*将其转换为无名参数*totalRunningTimes，他实质上变成了元组：
(
    [0.1,0.2,0.3,0.4]
    [0.8,0.3,0.5,0.6]
    [0.53,0.34,0.56,0.10]
)
当我们将*totalRunningTimes传入zip函数，相当于把元组中每项作为了一个**无名参数**传入了，zip(*totalRunningTimes)其实就相当于：
zip([0.1,0.2,0.3,0.4],[0.8,0.3,0.5,0.6],[0.53,0.34,0.56,0.10])
这下应该明了了吧，我们把**不同程序执行结果**中**同一种分发方法**的耗时打包成了一个元组，通过list列表转换后变成了下面这个样：
[(0.1,0.8,0.53),(0.2,0.3,0.34),(0.3,0.5,0.56),(0.4,0.6,0.10)]
为什么要这样做呢?那当然是为了算实验中**每种分发方法的**平均结果啦！
'''
# 通过列表简写式我们快速算出他们的平均值，并放回
totalInAverage = [sum(timeTuple)/testTimes for timeTuple in totalPlus]
# 获得最小值和索引
minValue = min(totalInAverage)
minIndex = totalInAverage.index(minValue)
print("\n<----------------------------------------运行性能---------------------------------------->\n")
print(f"\t一共进行了{testTimes}次实验")
# 使用enumerate我们将列表拆分为索引index和值value
for index, value in enumerate(totalInAverage):
    timeTakes = "%.15f" % value
    print(f"\t\t分发方法{index+1}平均耗时: {timeTakes}")
print(f"\t其中最好的是分发方法{minIndex+1}")
print("\n<--------------------------------------------------------------------------------------->\n")
input("Press any key to continue~\n")
