'''
2021.11.28
这个stu.py是我根据老师实验要求写的一个py命令行小例子，初步熟悉了一下Python的面向对象编程OOP，同时结合了之前的函数模块化思想

4. 设计一个用于学生管理的类Student(学生)。考虑到通用性，这里只抽象出所有学生都具有的数据成员：学号、姓名、性别、年龄、班级等。要求给每个数据成员声明对应的读写属性，定义下列成员方法：学生信息的录入、显示以及修改，另外根据需要适当添加特殊方法（如构造方法）。实现并测试这个类。

5. 在第4题的基础上定义并实现一个学生数组类StudentArray，支持一批学生信息的处理，能实现以下菜单功能：
（1）学生信息录入（执行一次菜单录入一个学生信息，考虑学号的唯一性） √
（2）学生信息显示（显示所有学生信息）√
（3）学生信息删除（按学号）√
（4）学生信息修改（按学号）√
（5）学生信息查询（按学号或所学专业）√
（6）学生信息统计（按所在学院或所学专业）√

'''

import os
import platform as pt
system = pt.system().lower()


class Student:
    order = ('学号', '名字', '性别', '年龄', '专业名')

    def __init__(self, st_id, name, sex, age, major_in):
        self.__data = [st_id, name, sex, age, major_in]

    @property
    def st_id(self):
        return self.__data[0]

    @st_id.setter
    def st_id(self, value):
        self.__data[0] = value

    @property
    def name(self):
        return self.__data[1]

    @name.setter
    def name(self, value):
        self.__data[1] = value

    @property
    def sex(self):
        return self.__data[2]

    @sex.setter
    def sex(self, value):
        self.__data[2] = value

    @property
    def age(self):
        return self.__data[3]

    @age.setter
    def age(self, value):
        self.__data[3] = value

    @property
    def class_name(self):
        return self.__data[4]

    @class_name.setter
    def class_name(self, value):
        self.__data[4] = value

    @property
    def details(self):
        total = ''
        for i in zip(Student.order, self.__data):
            total += '{}:{}\n'.format(*i)
        return total

    @property
    def to_tuple(self):
        return tuple(self.__data)


# st1 = Student("123456", "天打雷", "男", 19, "信息计算2803")

# Experiment 5

class StudentArray:
    faculties = {
        '大数据': '数学与计算机学院',
        '大类': '数学与计算机学院'
    }

    def __init__(self):
        self.datas = []
        self.check_lambdas = {
            'sex': lambda x: x in ('男', '女', '其他'),
            'age': lambda x: x.isdigit()
        }

    def lb(self, key):  # 联合check_lambdas返回判断输入的函数
        if key in self.check_lambdas.keys():
            return self.check_lambdas[key]
        return False

    def id_check(self):
        all = tuple(zip(*self.datas))
        id_recv = input("请输入学生的学号：")
        if (len(self.datas) > 0 and id_recv in all[0]) or not id_recv.isdigit():
            return self.id_check()
        else:
            return id_recv

    def input_check(self, msg, judge_func=None):  # 利用匿名函数判断输入
        recv = input(msg)
        if len(recv) > 0 and (judge_func(recv) if judge_func else True):
            return recv
        else:
            return self.input_check(msg, judge_func)

    def add(self):
        print('{:-^40}'.format('输入学生信息'))
        id_recv = self.id_check()
        name_recv = self.input_check('输入学生名字：')
        sex_recv = self.input_check(
            '输入学生性别（男/女/其他）：', self.lb('sex'))
        age_recv = int(self.input_check('请输入学生年龄：', self.lb('age')))
        class_recv = self.input_check('请输入专业名字：')
        new_stu = Student(id_recv, name_recv, sex_recv, age_recv, class_recv)
        self.datas.append(new_stu.to_tuple)
        return True

    def query(self, st_id=False, major=False, ind=False, pause=True):  # 根据学号和专业或者索引查询学生
        print('\n')
        ind_f = False
        if major:
            ind_f = self.find_ind(major=major, quiet=True)  # 按专业查询
        elif not st_id is False:
            ind_f = self.find_ind(st_id=st_id, quiet=True)  # 按学号查询
        elif not ind is False:
            ind_f = [ind]  # 因为find_ind返回的是一个列表
        target = self.datas if not ind_f else [self.datas[x] for x in ind_f]
        for s in target:
            total = ''
            for i in zip(Student.order, s):
                total += '{}:{}\n'.format(*i)
            print(total)
            print('---------------------')
        if pause:
            input()
        return True

    def query_all(self, faculty=False, major=False):  # 根据学院和专业查询统计对应学生信息
        print('\n')
        all = tuple(zip(*self.datas))
        if major:
            target_data = all[4]
            target_search = major
        elif faculty:
            target_data = tuple(map(lambda x: self.faculties[x] if x in self.faculties.keys(
            ) else tuple(), all[4]))
            target_search = faculty
        else:
            return False

        counter = 0
        for ind, x in enumerate(all[4]):
            if target_data[ind] == target_search:
                self.query(ind=ind, pause=False)
                counter += 1
        print(f'\n本次查询符合条件的学生一共有{counter}位')
        input()
        return True

    def find_ind(self, st_id=False, major=False, faculty=False, quiet=False):  # 根据学号，专业，学院来寻找学生对应的索引ind
        if len(self.datas) > 0:
            packed_data = tuple(zip(*self.datas))
            if faculty:
                items4exam = map(lambda x: self.faculties[x] if x in self.faculties.keys(
                ) else tuple(), packed_data[4])  # 取出所有的专业，处理成所有的学院
                target_item = faculty
            elif major:
                items4exam = packed_data[4]  # 取出所有的专业
                target_item = major
            elif st_id:
                items4exam = packed_data[0]  # 取出所有的学号
                target_item = st_id
            else:
                return False
            if target_item in items4exam:
                ind = [items4exam.index(x)
                       for x in items4exam if x == target_item]
                return ind
            elif not quiet:
                print(f'查找的索引不存在')
        return False

    def del_stu(self, st_id):  # 根据学号删除学生信息
        ind = self.find_ind(st_id)
        if not ind is False:
            ind = ind[0]
            del self.datas[ind]
            print('学生信息成功移除')
            return True
        return False

    def mod_stu(self, st_id):  # 根据学号编辑学生信息
        ind = self.find_ind(st_id)  # 找到了学号才会返回索引
        item_tb = {
            1: 'name',
            2: 'sex',
            3: 'age',
            4: 'major_in'
        }
        if not ind is False:
            ind = ind[0]
            target = list(self.datas[ind])
            name = target[1]
            print('{:-^40}'.format(f'你正在修改学生{name}的信息'))
            choice = int(self.input_check('请输入你要修改的项目：(0)学号，(1)名字，(2)性别，(3)年龄，(4)专业名：',
                         lambda x: x.isdigit() and int(x) <= 4 and int(x) >= 0))
            if choice == 0:  # 学号特殊处理
                recv = self.id_check()
            else:
                recv = self.input_check(
                    f'请输入学生{Student.order[choice]}：', self.lb(item_tb[choice]))
            target[choice] = recv
            modified = Student(*target)
            self.datas[ind] = modified.to_tuple
            return True
        return False

    def menu(self, choice=False):
        os.system('cls') if system == 'windows' else os.system('clear')
        print('{:-^40}'.format('学生管理'))
        print('请输入你要执行的操作对应的序号')
        print('(0)录入学生信息\n(1)显示所有学生信息\n(2)按学号删除学生信息\n(3)按学号修改学生信息\n(4)学生信息查询\n(5)学生信息统计\n(6)退出')
        choice = choice if not choice is False else int(self.input_check(
            '请输入你的选择：', lambda x: x.isdigit() and int(x) >= 0 and int(x) <= 6))
        choice_dict = {
            0: self.add,
            1: self.query,
            2: self.rmv_menu,
            3: self.mod_menu,
            4: self.query_menu,
            5: self.query_all_menu,
            6: exit
        }
        exec_result = choice_dict[choice]()  # 获得执行结果Bool
        return self.menu() if exec_result else self.menu(choice)

    def rmv_menu(self):
        in_id = self.input_check(
            '你的选择：移除学生信息\n（输入-1取消当前操作）请输入学号：', lambda x: x.replace('-', '').isdigit())
        if in_id == '-1':
            return True
        exec = self.del_stu(in_id)
        input()
        return True if exec else self.rmv_menu()

    def mod_menu(self):
        in_id = self.input_check(
            '你的选择：修改学生信息\n（输入-1取消当前操作）请输入学号：', lambda x: x.replace('-', '').isdigit())
        if in_id == '-1':
            return True
        exec = self.mod_stu(in_id)
        input()
        return True if exec else self.rmv_menu()

    def query_menu(self):
        choice = int(self.input_check(
            '你的选择：查询学生信息\n请输入查询方式：\n(-1)取消\n(0)按学号\n(1)按专业\n', lambda x: x in ('-1', '0', '1')))
        exec = False
        if choice == 0:
            in_id = self.input_check('请输入学号：', lambda x: x.isdigit())
            exec = self.query(st_id=in_id)  # 按学号查询
        elif choice == 1:
            in_major = self.input_check('请输入专业名：')
            exec = self.query(major=in_major)
        elif choice == -1:
            return True
        return True if exec else self.query_menu()

    def query_all_menu(self):
        choice = int(self.input_check(
            '你的选择：统计学生信息\n请输入查询方式：\n(-1)取消\n(0)按学院\n(1)按专业\n', lambda x: x in ('-1', '0', '1')))
        exec = False
        if choice == 0:
            in_faculty = self.input_check('请输入院系名：')
            exec = self.query_all(faculty=in_faculty)  # 用院名来查询统计
        elif choice == 1:
            in_major = self.input_check('请输入专业名：')
            exec = self.query_all(major=in_major)
        elif choice == -1:
            return True
        return True if exec else self.query_all_menu()


students = StudentArray()
students.menu()

# isinstance(False,int) Python会判断为True
# 0==False判断为真，但我们可以用0 is False
