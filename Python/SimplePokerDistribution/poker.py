#!/usr/bin/python
# -*- coding: UTF-8 -*-
import random
import copy

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

# 创建这几位老哥的克隆人，利用Python的深复制
firstFourBros = copy.deepcopy(bros)

print("\n有四位小老弟：", firstFourBros)

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
        print(
            f"//////////////////////{theBro}的牌，共{len(bros[theBro])}张//////////////////////")
        # 注意字典遍历时theBro是键名
        outPut = ""
        for theCard in bros[theBro]:
            outPut += cardName(theCard)+" | "
        print("\t"+outPut)
        print("//////////////////////////////////////////////////////////////\n")

# 我们再做个收集并清点牌的函数，以防分配有问题没有发现


def collectCards(bros):
    print("\n>>>>>>>>>>>>>>>>>>>>>>>清点结果<<<<<<<<<<<<<<<<<<<<<<<<")
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
    # print(styleDict)
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
        print(f"回收后第{theIndex}副牌共有{len(operatingList)}张，其中")
        for theStyle in operatingStyleDict:
            # 利用join函数我们能快速输出**当前副**中**对应花色**的牌号们
            targetGroup = operatingStyleDict[theStyle]
            strOfIDs = ", ".join(targetGroup)
            # 使用制表符来进行一下排版，这样好看一点
            print(f"\t{styleName(theStyle)}组的卡牌有：{strOfIDs}，共{len(targetGroup)}张")

    print("\n>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<")


# 好了，问题来了，怎么样才能随机发给四位小老弟呢，说到随机那必不可少的便是标准库random了
# 接下来有请我们的第一位嘉宾shuffle~


print("\n//////////////////////////////////////分发方法1//////////////////////////////////////\n")
# 我们首先把扑克牌组转换为列表
pokerList = list(pokerSets)
# 利用shuffle打乱扑克
random.shuffle(pokerList)
# 四个人每人能分到多少张牌
cardSumForEach = int(len(pokerList)/4)
print(f"每个人能分到{cardSumForEach}张牌")
# 从第一个人开始分配
for step in range(0, len(pokerList)//cardSumForEach):
    firstFourBros['bro' + str(step+1)] = pokerList[step * cardSumForEach:(step+1)*cardSumForEach]
'''
这里的 len(pokerList)//cardSumForEach 是算出了分配的步数，要分配四次
步数step默认从0开始，我们将step+1便能把牌给对应的兄die了
在这之前我们已经用shuffle获得了一个随机列表pokerList，现在我们只需要分步将其中的牌给兄弟们就行了
利用了Python的切片性质，比如我们给第一个兄弟的便是随机列表中切片[0,26]的牌，第二个兄弟则是切片[27,52]的牌
'''
# 老铁们，秀个牌！
showYourCards(firstFourBros)
collectCards(firstFourBros)
print("//////////////////////////////////////分发方法1结束//////////////////////////////////////")

# 用shuffle分配行不行，那当然彳亍！但是吧，如果不用shuffle呢？
print("\n//////////////////////////////////////分发方法2//////////////////////////////////////\n")
# 我们先再生产这几位老哥的克隆人，使用深复制
secondFourBros = copy.deepcopy(bros)
# 可以接受卡牌分发的老哥，每个老哥可以拿到26张牌，如果拿满了26张就判定为notAcceptable，从该列表中移除，不再接受卡牌
acceptableBros = [1, 2, 3, 4]
# 实打实的元组哈
type(pokerSets)
# 接下来我们遍历pokerSets这个具有两副扑克的元组，不用打乱这个元组的顺序，也打乱不了
for theCard in pokerSets:
    # 从四个老哥中随机选一个人出来，用random标准库中的choice
    theBroToGive = random.choice(acceptableBros)
    # 接着我们把遍历到的当前卡牌分配给这位老哥
    operatingList = secondFourBros[f'bro{theBroToGive}']
    operatingList.append(theCard)
    # 这里我们沿用第一种分发方法中的变量cardSumForEach
    if len(operatingList) >= cardSumForEach:
        # 如果这位老哥已经拿了(26)张牌了，就不能再接受更多牌了，从acceptableBros中移除
        acceptableBros.remove(theBroToGive)
    if len(acceptableBros) == 0:
        # 以防万一的写法，如果列表中已经没人可以接受了就该退出循环了
        break
# 老铁们，再秀个牌！
showYourCards(secondFourBros)
collectCards(secondFourBros)

print("//////////////////////////////////////分发方法2结束//////////////////////////////////////")

input("Press any key to continue~")
