'''
简单的在一排洞里抓狐狸的命令行小游戏

这是咱写的一个Python实验练习，有点写上头了，干脆发出来算了
实验题目：
编程模拟抓狐狸小游戏。假设有一排洞口（至少5个），小狐狸最开始的时候在其中一个洞口，然后玩家随机打开一个洞口，如果里面有狐狸就抓到了，如果里面没有狐狸就到第二天再来抓，但是第二天狐狸会在玩家来抓之前跳到隔壁洞口里。如果在规定的次数（最多5次）内抓到了狐狸就提前结束游戏并提示成功；如果规定的次数用完还没有抓到狐狸，就结束游戏并提示失败。允许用户连续多次玩游戏。
'''

# 使用Python版本：3.8.5

import random as rd
import os
import platform as pt
system = pt.system().lower()


def start_game():
    day = 1  # 初始化天数
    day_max = 5  # 最多能抓几天
    hole_num = rd.randint(5, 10)  # 随机生成5-10个洞
    hole_list = [0]*hole_num  # 生成洞列表
    hole_no = list(map(lambda x: str(x), list(range(1, hole_num+1))))
    fox_lies_in = rd.randrange(0, hole_num)
    hole_list[fox_lies_in] = 1

    def error_show():
        print('输入错误，请重输入')
        input()
        return game_round()

    def fox_move():
        nonlocal hole_list
        fox_pos = hole_list.index(1)  # 找到这狐狸崽子现在在哪
        jump_choice = rd.randint(1, 2)  # 1就是往左跳，2就是往右跳
        if fox_pos == 0:
            jump_choice = 2  # 在最左边的洞就只能向右跳了
        elif fox_pos == len(hole_list)-1:
            jump_choice = 1  # 在最右边的洞就只能向左跳了
        hole_list = hole_list[1:] + \
            hole_list[:1] if jump_choice == 1 else hole_list[-1:] + \
            hole_list[:-1]  # 利用分片来循环偏移列表，达到狐狸左右跳的效果

    def game_round():
        nonlocal day, day_max, hole_num, hole_list, hole_no
        hole_txt = "Hole identity: " + \
            (" ".join(hole_no))
        pop_msg('', day, "{:#^42s}".format(f"{' O ' * hole_num}"), hole_txt)
        player_guess = input('输入狐狸所在的洞的序号：')
        try:
            player_guess = int(player_guess)-1
        except:
            error_show()
        if player_guess > 0 and hole_list[player_guess:]:
            if hole_list[player_guess] == 1:
                print('你抓住了狐狸崽子')
                input()
            elif day < day_max:
                fox_move()
                print(f'第{day}天你没有抓住狐狸崽子，狐狸跳到旁边的洞去了')
                day += 1
                input()
                return game_round()
            else:
                print(f'过了{day_max}天你还没找到狐狸，狐狸已经跑到月球上了！')
                input()
                exit()
        else:
            error_show()
    game_round()


def pop_msg(msg, *args):
    os.system('cls') if system == 'windows' else os.system('clear')
    print("{:-^42s}".format(f"Where is the fox? Day: {args[0]}"))
    if msg:
        print('\n'+msg+'\n')
    left_args = args[1:]
    for i in left_args:
        print(i)


def menu():
    action = {
        0: start_game,
        1: exit
    }
    choice = input('0 to start the game, 1 to exit: ')
    if not choice.isdigit() or int(choice) not in action.keys():
        print('Wrong choice')
        return menu()
    else:
        func = action.get(int(choice))
        func()


menu()
