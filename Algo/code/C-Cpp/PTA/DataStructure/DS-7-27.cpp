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

// 家庭成员
struct Member
{
    int parent;    // 父节点下标
    Childs childs; // 这个成员的所有孩子的下标，用于判断兄弟
};

bool contains(Childs &vec, int val); // 查看vector中有没有符合要求的元素

int main()
{
    int serial = 0;            // 名字编号
    map<string, int> nameInds; // 将名字映射到一个整数
    vector<Member> members;    // 记录每个家庭成员的父节点和后代
    int nameNum, sentenceNum;  // 名字数量和陈述句数量
    // 注意这里顺带取走了行末的换行符
    scanf("%d %d%*[\n]", &nameNum, &sentenceNum);
    // 初始化父子关系数组
    members.resize(nameNum);
    // 读入家谱
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
        members[serial].parent = parentNodes.top().first;          // 指定当前节点的父节点下标
        members[parentNodes.top().first].childs.push_back(serial); // 在父节点上记录孩子结点
        serial++;
        prevIndent = indent;
    }
    /*
    for (int i = 0; i < nameNum; i++)
    {
        printf("parent-%d child-%d child->childs:", parents[i].parent, i);
        for (int j = 0, len = parents[i].childs.size(); j < len; j++)
        {
            printf("%d ", parents[i].childs[j]);
        }
        printf("\n");
    }*/
    // 接下来读入陈述句
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
        bool found = false;     // 是否符合家谱
        if (relation[0] == 'c') // childs
        {
            // X is a child of Y, X的父节点是Y
            found = members[nameFrontInd].parent == nameBackInd;
        }
        else if (relation[0] == 'p') // parents
        {
            // X is the parent of Y, Y的父节点是X
            found = members[nameBackInd].parent == nameFrontInd;
        }
        else if (relation[0] == 's') // siblings
        {
            // X is a sibling of Y, X和Y的父节点的孩子中有X和Y
            // 根据Y找到父节点
            int parentInd = members[nameBackInd].parent;
            found = contains(members[parentInd].childs, nameFrontInd); // 如果X也是父节点的孩子就符合家谱
        }
        else if (relation[0] == 'd') // descendants
        {
            // X is a descendant of Y, 从X往上找父节点，有父节点Y
            int parentInd = nameFrontInd;
            while (parentInd != 0)
            {
                parentInd = members[parentInd].parent; // 向上找父节点
                // 这样写不会漏掉父亲是0号节点的情况
                if (parentInd == nameBackInd)
                {
                    found = true;
                    break;
                }
            }
        }
        else if (relation[0] == 'a') // ancestors
        {
            // X is an ancestor of Y, 从Y向上找父节点，有父节点X
            int parentInd = nameBackInd;
            while (parentInd != 0)
            {
                parentInd = members[parentInd].parent; // 同样是向上找父节点
                // 这样写不会漏掉父亲是0号节点的情况
                if (parentInd == nameFrontInd)
                {
                    found = true;
                    break;
                }
            }
        }
        if (found)
            printf("True\n");
        else
            printf("False\n");
    }
    return 0;
}

// 查看vec中有没有元素val
bool contains(Childs &vec, int val)
{
    for (Childs::iterator i = vec.begin(); i != vec.end(); i++)
    {
        if (*i == val)
            return true;
    }
    return false;
}