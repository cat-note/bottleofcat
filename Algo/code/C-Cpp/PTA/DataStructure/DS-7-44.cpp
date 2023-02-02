#include <cstdio>
#include <string>
#include <vector>

using namespace std;

typedef vector<string> Words; // 字符串数组储存单词

// 哈希表项
typedef struct TableItem{
    string key; // 键值
    int access; // 访问次数
    TableItem* next;
    TableItem(){
        access=0;
        next=NULL;
    }
} *List;

// 哈希表
class HashTable{
    private:
        int size; // 哈希表长度, 2的n次方，方便取余运算
        vector<List> table; // 哈希表底层数组
        vector<List> queryBuf; // 查询缓存，用于重置
    public:
        HashTable(){
            size=4096; 
            table.resize(4096,NULL);
        }
        // 通过字符串键值访问哈希表元素
        int& operator[](const string& key){
            // 计算字符串哈希
            int hash=0;
            int weights[10]={1}; // 计算位权32^n
            for(int i=0,len=key.length();i<len;i++){
                int weight=1;
                for(int j=0;j<i;j++) 
                    weight*=32;
                int tmpHash=weight*(int)(key[i]-'a');
                // 注意，因为最多有10个字符，这里可能会有溢出
                if(tmpHash<0)
                    tmpHash=-tmpHash; // 如果溢出到负数就取相反数
                
            }
        }
};

int main(){
    int fileNum,queryNum; // 文件数和查询数
    scanf("%d",&fileNum);
    vector<Words> files(fileNum); // 储存每个文件的词
    // 开始读取文件内容
    int currentFile=0; // 当前正在读取哪个文件的内容(编号0~N-1)
    char wordBuf[11]; // 单词缓存，一个单词最多只考虑10个字母
    int wordLen=0; // 单词长度
    string wordTmp; // 临时储存单词的字符串
    while(currentFile<fileNum){
        char readChar; // 读入的字符
        scanf("%c",&readChar);
        if(readChar>='A'&&readChar<='Z'){
            // 如果是大写字符，就转换为小写
            readChar+=('a'-'A');
        }
        if(readChar>='a'&&readChar<='z'){
            // 如果读入的是英文字母，说明正在读取字符
            if(wordLen<10) // 长度>10的单词只考虑前10个字母
                wordBuf[wordLen++]=readChar;
        }else{
            // 否则读到的就是非英文字母，这说明【一个单词读取完毕】
            if(wordLen>=3){
                // 只考虑长度>=3的单词
                wordBuf[wordLen]='\0'; // 封装成字符串
                wordTmp.assign(wordBuf); // 转换为string
                files[currentFile].push_back(wordTmp); // 将词加入文件中
                wordLen=0; // 清空单词缓存
            }
            if(readChar=='#') // 一个文件读取完毕
                currentFile++;
        }
    }
    // 开始对比文件
    scanf("%d",&queryNum);
    
    return 0;
}

/*
    看到这种统计词频的题，首先能想到的解决方法就是哈希表。
    
*/