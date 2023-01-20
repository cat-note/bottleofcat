#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include <stack>
// 注: map头文件已经引入<utility>头文件，因此可以使用pair

using namespace std;

/*
    观察样例:
        parent和child相对应，表述父子关系
        ancestor和descendant(后代)相对应，包含父子关系

    注意不要把这里的ancestor理解成祖先，在这里的意思是前一代。
*/

typedef pair<int, int> IndPair;
typedef vector<int> Childs;

int main()
{
    int serial = 0;            // 名字编号
    map<string, int> nameInds; // 将名字映射到一个整数
    vector<int> parents;       // 记录每个家庭成员的父节点和后代
    int nameNum, sentenceNum;  // 名字数量和陈述句数量
    // 注意这里顺带取走了行末的换行符
    scanf("%d %d%*[\n]", &nameNum, &sentenceNum);
    // 初始化父子关系数组
    parents.resize(nameNum);
    // ---------------------------------------------------------------------------读入家谱
    stack<IndPair> parentNodes;      // 储存父节点
    parentNodes.push(IndPair{0, 0}); // 最早祖先的下标为0，缩进为0
    int prevIndent = 0;              // 之前的缩进
    for (int i = 0; i < nameNum; i++)
    {
        string name;
        int indent = 0;
        char readChr;
        // 读入一行数据
        while (scanf("%c", &readChr) != EOF && readChr != '\n')
        {
            if (readChr == ' ') // 如果读入的是空格，缩进就增加
                indent++;
            else // 否则读入的就是名字
                name.push_back(readChr);
        }
        nameInds[name] = serial; // 给每个名字编个号(下标)
        if (indent > prevIndent)
        {
            // 如果缩进增加了
            // 先寻找有没有相同缩进的父节点，如果没有就把父亲节点的下标和缩进压入
            parentNodes.push(IndPair{serial - 1, prevIndent});
        }
        else if (indent < prevIndent)
        {
            // 如果缩进减小了
            // 出栈，找到当前成员的父节点，父节点的缩进肯定【少于】当前节点
            // 最多出栈到缩进=0时就不再继续了
            while (parentNodes.top().second > 0 && parentNodes.top().second >= indent)
                parentNodes.pop();
        }
        if (serial == 0)
            parents[serial] = -1; // 💡 !!!!!!!!!!!最早祖先的父节点是-1，避免后面判断兄弟时出错!!!!!!!!!!!!!
        else
            parents[serial] = parentNodes.top().first; // 指定当前节点的父节点下标
        serial++;
        prevIndent = indent;
    }
    // ---------------------------------------------------------------------------接下来读入陈述句
    string nameFront;  // 句子中首个名字
    string nameBack;   // 句子中最后一个名字
    char relation[11]; // 表示关系的名词
    char temp[11];     // 临时字符数组
    for (int i = 0; i < sentenceNum; i++)
    {
        scanf("%s", temp); // 读入句子中首个名字
        nameFront.assign(temp);
        scanf("%*s%*s"); // 吸收掉系动词和冠词
        scanf("%s", relation);
        scanf("%*s");      // 吸收掉介词of
        scanf("%s", temp); // 读入句子中最后一个名字
        nameBack.assign(temp);
        int nameFrontInd = nameInds[nameFront]; // 获得名字的编号(下标)
        int nameBackInd = nameInds[nameBack];
        bool found = false; // 是否符合家谱
        switch (relation[0])
        {
        case 'c': // childs
            // X is a child of Y, X的父节点是Y
            found = parents[nameFrontInd] == nameBackInd;
            break;
        case 'p': // parents
            // X is the parent of Y, Y的父节点是X
            found = parents[nameBackInd] == nameFrontInd;
            break;
        case 's': // siblings
            // X is a sibling of Y, X和Y的父节点的孩子中有X和Y
            // 如果X的父节点也是Y的父节点，二者就互为兄弟姐妹
            found = parents[nameBackInd] == parents[nameFrontInd];
            break;
        case 'd': // descendants
        {
            // X is a descendant of Y, 从X往上找父节点，有父节点Y
            int parentInd = nameFrontInd;
            while (parentInd != -1) // 下标为-1就遇到最早祖先了
            {
                parentInd = parents[parentInd]; // 向上找父节点
                // 这样写不会漏掉父亲是0号节点的情况
                if (parentInd == nameBackInd)
                {
                    found = true;
                    break;
                }
            }
        }
        break;
        case 'a': // ancestors
        {

            // X is an ancestor of Y, 从Y向上找父节点，有父节点X
            int parentInd = nameBackInd;
            while (parentInd != -1) // 下标为-1就遇到最早祖先了
            {
                parentInd = parents[parentInd]; // 同样是向上找父节点
                // 这样写不会漏掉父亲是0号节点的情况
                if (parentInd == nameFrontInd)
                {
                    found = true;
                    break;
                }
            }
        }
        break;
        }
        if (found)
            printf("True\n");
        else
            printf("False\n");
    }
    return 0;
}

/*
    这题我是真的非常佩服出题老师的挖坑能力，这个坑就坑在【测试点3】- 最大N和M，随机

    乍一看好像没什么特别的，但我刚开始死活过不了这个测试点。后来写了个家谱数据生成器加上bash脚本才找到了问题所在——

        最开始我是按并查集的parents数组去初始化本题中的parents数组的，也就是初始值parents[i]=i，这样一来，树根(最早祖先)的父节点就是【它自己】。

        然而后面我在判断【两节点是否为兄弟】时，判断条件写的是 parents[节点1]==parents[节点2] 。

        假如我有这样一组数据:

            3 1
            Simon      
              Steve
              Alex
            Simon is a sibling of Steve

        那么程序就会判断 parents[Simon]==parents[Steve] 是否成立。很明显，这里parents[Simon]=Simon，而parents[Steve]=Simon，所以导致了判断失误，💡 这也是【测试点3】故意卡的一个地方。

        解决方法很简单，在初始化时将【最早祖先】(树根)的【父节点设为其他值】。
    -------------------------------------------
*/