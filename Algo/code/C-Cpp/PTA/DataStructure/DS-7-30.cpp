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
            if (readChr == '\\')
            {
                // 读完了一个目录名
                currDir = seekDir(currDir, buf); // 根据目录名，在当前目录中寻找子目录，如果不存在就创建
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

// 在某一级目录dir中寻找目录dirName，找不到就新建
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
    return target;
}

// 对目录进行比较
bool compareDir(Dir prev, Dir next)
{
    // 字符串字典序比较
    return (prev->name.compare(next->name) < 0);
}

void indent(int num)
{
    for (int i = 0; i < num; i++) // 输出缩进填充空格
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
    本题考察多叉树的构建和遍历。
*/