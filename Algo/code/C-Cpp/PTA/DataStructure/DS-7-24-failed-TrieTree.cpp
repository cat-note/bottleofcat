#include <cstdio>
#include <string>
#include <vector>

using namespace std;

typedef struct KeyNode
{
    string key;
    KeyNode *next;
} *Key;

// 忽略大小写的Trie树(字典树)
class TrieTree
{
private:
    mutable vector<vector<int>> treeArr; // 用二维数组按层存放树
    mutable Key keys;                    // 字符串作为键，按字典排序

    // 转换为小写
    char toLower(char chr) const
    {
        if (chr >= 'A' && chr <= 'Z')
            return chr + ('a' - 'A');
        return chr;
    }
    // 随心所欲写的一个字符串字典比较，比较字符串a字典序是否>b的
    bool compareStr(const string &a, const string &b) const
    {
        int lenA = a.length();
        int lenB = b.length();
        int minLen = lenA;
        bool result = false;
        if (lenB < lenA)
        {
            minLen = lenB;
            result = true;
        }
        for (int i = 0; i < minLen; i++)
        {
            char aChr = toLower(a[i]);
            char bChr = toLower(b[i]);
            if (aChr < bChr)
                return false;
            else if (aChr > bChr)
                return true;
        }
        return result;
    }

public:
    TrieTree()
    {
        // 名称字符串不超过30个字符，也就是最多30层
        // 忽略大小写后，每层最多27个分支(有个空格)
        // 注意，这里空格在字典序中当作是最小的, 因此放在最前面
        // 默认值为-1，代表未被访问过
        // 被访问过后会变为0
        treeArr.resize(30, vector<int>(27, -1));
        keys = new KeyNode();
        keys->next = NULL;
    }
    // 储存键
    void insertKey(const string &str) const
    {
        Key curr = keys;
        Key node = new KeyNode();
        node->key = str;
        while (curr->next)
        {
            if (compareStr(curr->next->key, str)) // 找到插入位置
                break;
            curr = curr->next;
        }
        node->next = curr->next;
        curr->next = node;
    }
    // 获得字典排序好的键
    Key getKeys()
    {
        return keys;
    }
    // 根据字符串访问字典树结点
    int &operator[](const string &str) const
    {
        int len = str.length();
        short lastIndex;
        for (int i = 0; i < len; i++)
        {
            // 让字典树的边上全是【小写】字母，方便查找
            char clueChr = toLower(str[i]);
            // 求得这一层这一个字符的下标。这里+1是因为空格在最前面
            short index = (clueChr == ' ' ? 0 : clueChr - 'a' + 1);
            // 如果未被创建
            if (treeArr[i][index] == -1)
            {
                treeArr[i][index] = 0; // 访问过的地方都置为0
            }
            lastIndex = index;
        }
        printf("#########%s - LEN:%p\n", &treeArr[len - 1][lastIndex]);
        return treeArr[len - 1][lastIndex];
    }
};

int main()
{
    int recordNum; // 记录数
    scanf("%d", &recordNum);
    TrieTree tree;  // 初始化
    char temp[31];  // 临时字符数组
    string tempStr; // 临时字符串
    for (int i = 0; i < recordNum; i++)
    {
        getchar(); // 取走前一个换行符
        scanf("%[^\n]", temp);
        tempStr.assign(temp);     // 转为C++ string
        if (tree[tempStr]++ == 0) // 统计个数
            tree.insertKey(tempStr);
        printf("-----------------%s    ASPEN:%d\n", tempStr.c_str(), tree["Aspen"]);
    }
    Key curr = tree.getKeys()->next;
    while (curr)
    {
        printf("%s %.4lf%%\n", curr->key.c_str(), (double)tree[curr->key] / recordNum * 100);
        curr = curr->next;
    }
    return 0;
}
