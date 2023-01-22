#include <cstdio>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

// 目录树节点
typedef struct DirNode
{
    string name;
    vector<string> files;     // 这个目录中的文件
    vector<DirNode *> childs; // 这个目录中的子目录
} *Dir;

Dir seekDir(Dir currDir, const string &dirName); // 在某一级目录中寻找dirName目录，不存在就创建
bool compareDir(Dir prev, Dir next);             // 自定义比较函数
void indent(int num);                            // 输出缩进
void printPreorder(Dir root, int depth = 0);     // 打印先序遍历目录的序列

int main()
{
    Dir root = new DirNode();    // 目录树根
    root->name = "root";         // 树根名为root
    int pathNum;                 // 路径数量
    scanf("%d%*[\n]", &pathNum); // 这里顺带把行末的换行符给收了，因为底下要靠\n来判断一行结束
    // 读入路径
    for (int i = 0; i < pathNum; i++)
    {
        Dir currDir = root; // 当前所在的目录
        string buf;         // 临时字符串
        char readChr;
        while (scanf("%c", &readChr) != EOF)
        {
            if (readChr == '\n')
            {
                // 读到行末
                if (buf.length() > 0)              // 如果临时字符串不为空，这个字符串就代表一个文件名
                    currDir->files.push_back(buf); // 加入新文件
                break;
            }
            if (readChr == '\\') // 遇到路径分隔符
            {
                // 读完了一个目录名
                currDir = seekDir(currDir, buf); // 根据目录名，在当前目录中寻找子目录，如果不存在就创建，然后进入这个目录
                buf.clear();                     // 临时字符串清空
                continue;
            }
            buf.push_back(readChr); // 读入的字符塞入临时字符串
        }
    }
    // 输出目录和文件
    printPreorder(root);
    return 0;
}

// 在某一级目录dir中寻找名为dirName的子目录，找不到就新建
Dir seekDir(Dir currDir, const string &dirName)
{
    Dir target = NULL;
    // 先寻找子目录是否存在
    for (int i = 0, len = currDir->childs.size(); i < len; i++)
        if (currDir->childs[i]->name == dirName)
            target = currDir->childs[i];
    if (!target)
    {
        // 如果不存在就新建
        target = new DirNode();
        target->name = dirName;
        currDir->childs.push_back(target);
    }
    // 返回子目录指针
    return target;
}

// 对目录进行比较，用于按目录名字典排序
bool compareDir(Dir prev, Dir next)
{
    // 字符串字典序比较
    // compare < 0说明前者小于后者
    return (prev->name.compare(next->name) < 0);
}

void indent(int num)
{
    for (int i = 0; i < num; i++) // 输出缩进(填充空格)
        printf("  ");
}

// 先序遍历目录
void printPreorder(Dir root, int depth)
{
    int childsNum = root->childs.size(); // 子目录数量
    int fileNum = root->files.size();    // 文件数量
    indent(depth);
    printf("%s\n", root->name.c_str()); // 打印这个目录的名字
    // 先找所有的子目录
    if (childsNum > 0) // 这个分支有子目录时就先输出子目录
    {
        // 对所有子目录进行字典排序
        sort(root->childs.begin(), root->childs.end(), compareDir);
        for (int i = 0; i < childsNum; i++)
            printPreorder(root->childs[i], depth + 1); // DFS遍历目录
    }
    // 接着输出本目录下的文件
    sort(root->files.begin(), root->files.end()); // 先按字典序排序
    for (int i = 0; i < fileNum; i++)
    {
        indent(depth + 1);                      // 文件是目录下的，因此要多一个缩进
        printf("%s\n", root->files[i].c_str()); // 输出文件名
    }
    delete root; // 释放内存
}

/*
    目录树是一棵可能每个节点都能延伸出多条分支的树，因此本题考察的是【多叉树】的构建和遍历。
    --------------------
    本题中我采用了链式的多叉树储存结构，在多叉树的每个节点中储存了三部分数据:

        - 本目录名
        - 本目录下的所有文件(vector数组)
        - 本目录下的所有子目录(vector数组)

    和二叉树不同，这里因为【分支的数量不确定】，我需要用一个【数组】来储存【所有后继节点的指针】。
    --------------------
    读入数据的时候按行读取。
    每行文本中有两个比较重要的标识符:

        - 行末的换行符 \n
            代表这一行读取结束，也可能代表【文件名】读取结束。
            (因为每条路径的文件名必然出现在【最后一个路径分隔符之后】)

        - 路径的分隔符 \
            每读入一个分隔符，相当于【进入了更深一层的目录】，完全可以把这个过程看作【Shell中的cd命令】
            比如读入路径 a\b\c\d\

            root 此时指针: root节点
              -> a 此时指针: a节点
                -> b 此时指针: b节点
                  -> c 此时指针: c节点
                   -> d 此时指针: d节点

            每读到一个【目录名】，就在【当前指针指向的目录下面】找这个【目录名】，如果找到了就把指针移动到这个【目录名】所指的目录下(进入深层目录)；否则【创建新的目录节点】，再把指针移动到这个目录下。

    ------------------------
    关于遍历多叉树

    从样例可以看到，本题采用的是先序遍历。遍历过程用递归就能解决，不过题目另加了一个条件: 按【字典序输出】.

        * 这里我采用了algorithm的sort函数+自定义比较函数，sort的底层实现是快速排序，因此能较为高效地解决排序问题。

    在遍历输出前先对目录下【所有子目录】进行【字典升序排序】，然后依次递归遍历输出。
    之后再对目录下【所有文件】按名字进行【字典升序排序】，再依次输出。

        - SomeBottle 2023.1.23
*/
/*
7-30 目录树
    在ZIP归档文件中，保留着所有压缩文件和目录的相对路径和名称。当使用WinZIP等GUI软件打开ZIP归档文件时，可以从这些信息中重建目录的树状结构。请编写程序实现目录的树状结构的重建工作。

输入格式:
    输入首先给出正整数N（≤10^4），表示ZIP归档文件中的文件和目录的数量。随后N行，每行有如下格式的文件或目录的相对路径和名称（每行不超过260个字符）：
        - 路径和名称中的字符仅包括英文字母（区分大小写）；
        - 符号“\”仅作为路径分隔符出现；
        - 目录以符号“\”结束；
        - 不存在重复的输入项目；
        - 整个输入大小不超过2MB。
输出格式:
    假设所有的路径都相对于root目录。从root目录开始，在输出时每个目录首先输出自己的名字，然后以字典序输出所有子目录，然后以字典序输出所有文件。注意，在输出时，应根据目录的相对关系使用空格进行缩进，每级目录或文件比上一级多缩进2个空格。

输入样例:
    7
    b
    c\
    ab\cd
    a\bc
    ab\d
    a\d\a
    a\d\z\
输出样例:
    root
      a
        d
          z
          a
        bc
      ab
        cd
        d
      c
      b
代码长度限制
    16 KB
时间限制
    400 ms
内存限制
    64 MB
*/