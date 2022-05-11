/* 动态分区存储管理
 * 实验要求
 * 编写程序模拟实现内存的动态分区法存储管理。
 * 内存空闲区使用空闲分区链管理，采用最坏适应算法从空闲分区链中寻找空闲区进行分配，
 * 内存回收时假定不做与相邻空闲区的合并。
 * 假定系统的内存共640K，初始状态为操作系统本身占用64K。
 * 在t1时间之后，有作业A、B、C、D分别请求8K、16K、64K、124K的内存空间；
 * 在t2时间之后，作业C完成；
 * 在t3时间之后，作业E请求50K的内存空间；
 * 在t4时间之后，作业D完成。
 * 要求编程序分别输出t1、t2、t3、t4时刻内存的空闲区的状态。
 */

/* 小记-SomeBottle 2022.5.11
 * 最坏适应算法即容量降序遍历算法
 * 在扫描整个空闲分区表时它总是会挑选最大的一个空闲区，从中分割内存给进程使用
 * 算法的实现方法便是将所有空闲内存分区按容量由大到小进行排序
 * 而要实现这点：
 *      1. 在初始化的时候容量就从大到小进行排序
 *      2. 在分配内存后进行排序
 *      3. 在内存释放后也进行排序
 * 为求单位一致，这里内存地址单位也定为KB
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32 // 根据不同系统编译环境定义暂停的宏
#define PAUSE system("pause")
#else
#define PAUSE printf("Press Enter to continue.\n");\
    getchar()
#endif
#define TOTAL_MEM 640
#define MEM_USED_BY_SYSTEM 64


struct FN { // 空闲内存链表节点
    size_t len; // 该分块长度
    unsigned int address; // 该分块的起始地址
    struct FN *next; // 下一个节点
};

struct ON { // 被占用内存链表的节点
    char name; // 占用该内存的进程名
    size_t len; // 该分块长度
    unsigned int address; // 该分块的起始地址
    struct ON *next; // 下一个节点
};

typedef struct FN FreeNode; // 空闲节点结构体类型的别名为FreeNode

typedef struct ON OccupiedNode; // 占用节点结构体类型的别名为FreeNode

struct { // 空闲内存链表头节点，用于储存链表长度和首个节点的地址
    size_t len; // 空闲内存链表长度
    FreeNode *next; // 指向第一个节点
} FreeHead = {
        .next=NULL,
        .len=0
};

struct { // 被占用内存链表头节点
    size_t len;
    OccupiedNode *next; // 指向第一个节点
    OccupiedNode *tail; // 末尾节点
} OccupiedHead = {
        .next=NULL,
        .tail=NULL,
        .len=0
};

// 全局时间

unsigned int globalTime = 0;

// 声明函数

void Initialize();

int MemAlloc(char name, size_t size);

int MemFree(char name);

void Timing(unsigned int time);

void PrintFreeList();

void PrintOccupiedList();

void InsertFreeNode(FreeNode *node);

void Initialize() { // 初始化，包括生成链表节点
    // 最开始系统便占用了64KB的内存，所以往占用内存链表中添加一个节点
    // 在堆内存中创建一个节点
    OccupiedNode *newON = (OccupiedNode *) malloc(sizeof(OccupiedNode));
    newON->name = 'S'; // 系统占用
    newON->address = 0; // 地址为0
    newON->len = MEM_USED_BY_SYSTEM; // 系统占用的内存
    newON->next = NULL;
    OccupiedHead.next = newON; // 添加到被占用内存链表中
    OccupiedHead.tail = newON; // 指定末尾节点
    OccupiedHead.len++; // 链表长度增长
    // 在堆内存中创建一个空闲内存节点
    FreeNode *newFN = (FreeNode *) malloc(sizeof(FreeNode));
    newFN->len = TOTAL_MEM - MEM_USED_BY_SYSTEM; // 空闲的内存
    newFN->address = MEM_USED_BY_SYSTEM; // 空闲内存的起始地址
    InsertFreeNode(newFN); // 插入到空闲链表中
}

void InsertFreeNode(FreeNode *node) { // 将节点插入空闲内存链表
    int i;
    if (FreeHead.next == NULL) {
        // 链表为空最好办了，直接把节点接到头节点上就行
        node->next = NULL;
        FreeHead.next = node;
        FreeHead.len++;
    } else {
        FreeNode *current = NULL;
        FreeNode *prev = NULL; // 指向当前遍历的上一个节点,NULL代表是FreeHead
        for (i = 0; i < FreeHead.len; i++) {
            if (i == 0)
                current = FreeHead.next;
            else
                current = current->next;
            if (node->len >= current->len) { // 寻找节点的插入位置，保证整个链表从大到小
                // 找到了插入的位置
                node->next = current;
                if (prev != NULL) {
                    prev->next = node;
                } else { // prev=NULL说明上一个节点是FreeHead
                    FreeHead.next = node;
                }
                FreeHead.len++;
                break;
            } else if (current->next == NULL) {
                /* 当前遍历到了最后一项，但是最后一项仍然比插入的内存块大
                 * 此时将内存块插入到链表尾部
                */
                node->next = NULL;
                current->next = node; // 接到链表的末尾
                FreeHead.len++;
                break;
            } else { // 下一个节点比插入的节点容量要大
                prev = current; // 记录节点
            }
        }
    }
}


int MemAlloc(char name, size_t size) { // 模拟内存分配
    int i;
    // 此时空闲内存链表中的内存分区已经按容量从大到小排列好了
    if (FreeHead.next != NULL) { // 前提是空闲内存链表中要有节点
        int leftSize; // 分配后这个分区剩余的内存大小
        if ((leftSize = FreeHead.next->len - size) >= 0) { // 首个节点足够大
            // 这块内存被分配给进程
            OccupiedNode *newON = (OccupiedNode *) malloc(sizeof(OccupiedNode));
            newON->len = size;
            newON->name = name;
            newON->address = FreeHead.next->address;
            newON->next = NULL;
            if (OccupiedHead.next == NULL) { // 考虑链表中没有节点的情况
                OccupiedHead.next = newON;
            } else {
                OccupiedHead.tail->next = newON; // 附加到占用内存链表末尾
            }
            OccupiedHead.tail = newON; // 让末指针指向这个节点
            OccupiedHead.len++; // 占用内存链表长度增长
            // 将分区节点从空闲链表中去除
            FreeNode *temp = FreeHead.next; // 留个地址备份
            FreeHead.next = FreeHead.next->next; // 删除节点
            free(temp); // 释放节点
            FreeHead.len--; // 链表长度减少
            if (leftSize > 0) {
                // 如果还有剩余，剩余的内存就又是一块更小的空闲分区
                FreeNode *newFN = (FreeNode *) malloc(sizeof(FreeNode));
                newFN->len = leftSize;
                newFN->address = newON->address + size;
                InsertFreeNode(newFN); // 接入空闲链表
            }
        } else { // 因为已经按容量从大到小排好了，首个节点不够大其他的更不用说了
            return 0; // 分配失败
        }
    } else {
        return 0; // 分配失败
    }
    return 1;
}

int MemFree(char name) { // 模拟内存回收
    int i = 0, found = 0;
    OccupiedNode *current = NULL;
    OccupiedNode *prev = NULL; // prev=NULL时代表指向FreeHead
    for (i = 0; i < OccupiedHead.len; i++) {
        if (i == 0)
            current = OccupiedHead.next;
        else
            current = current->next;
        if (current->name == name) { // 找到了对应的占用块
            // 移除current节点
            if (prev == NULL) {
                OccupiedHead.next = current->next;
                // 一定要注意处理OccupiedHead中的尾指针tail
                if (current->next == NULL) // 删除的是尾部的节点
                    OccupiedHead.tail = NULL; // 此时占用链表中已经没有节点了，置NULL
            } else {
                prev->next = current->next;
                if (current->next == NULL) // 删除的是尾部的节点
                    OccupiedHead.tail = prev; // 把删除的这个节点的前一个节点作为尾部节点
            }
            // 将释放出来的内存接回空闲链表
            FreeNode *newFN = (FreeNode *) malloc(sizeof(FreeNode));
            newFN->address = current->address;
            newFN->len = current->len;
            InsertFreeNode(newFN); // 放回空闲链表
            OccupiedHead.len--; // 链表长度减短
            free(current); // 释放掉current节点
            found = 1;
        } else {
            prev = current;
        }
    }
    return found;
}

void Timing(unsigned int time) { // 模拟系统所处时间
    globalTime = time;
    printf("\nTime: %d\n", time);
}

void PrintFreeList() { // 打印空闲内存链表的情况
    int i, j;
    if (FreeHead.next == NULL) return; // 如果链表中没有节点，就直接返回
    FreeNode *current = NULL;
    printf("\tAvailable Memory");
    for (i = 0; i < FreeHead.len; i++) {
        printf(" -> ");
        if (i == 0)
            current = FreeHead.next; // 指向首个节点
        else
            current = current->next; // 不是首个节点，就访问下一个节点
        printf("[ Address: %d, Size: %d ]\n", current->address, current->len);
        for (j = 0; j < i + 4; j++)
            printf("\t");
    }
    printf("\n");
}

void PrintOccupiedList() { // 打印被占用内存链表的情况
    int i, j;
    if (OccupiedHead.next == NULL) return; // 如果链表中没有节点，就直接返回
    OccupiedNode *current = NULL;
    printf("\tUsed Memory");
    for (i = 0; i < OccupiedHead.len; i++) {
        printf(" -> ");
        if (i == 0)
            current = OccupiedHead.next; // 指向首个节点
        else
            current = current->next;
        printf("[ Used-by: %c, Address: %d, Size: %d ]\n", current->name, current->address, current->len);
        for (j = 0; j < i + 4; j++)
            printf("\t");
    }
    printf("\n");
}

int main() {
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    int t4 = 4;
    Initialize();
    Timing(0);
    PrintFreeList();
    PrintOccupiedList();
    Timing(t1);
    if (!(MemAlloc('A', 8) &&
          MemAlloc('B', 16) &&
          MemAlloc('C', 64) &&
          MemAlloc('D', 124))) {
        // 分配内存失败
        printf("Failed to Allocate Memory. \n");
        return 0;
    }
    PrintFreeList();
    PrintOccupiedList();
    Timing(t2);
    if (!MemFree('C')) {
        printf("Failed to Free the Memory.\n");
        return 0;
    }
    PrintFreeList();
    PrintOccupiedList();
    Timing(t3);
    if (!MemAlloc('E', 50)) {
        // 分配内存失败
        printf("Failed to Allocate Memory. \n");
        return 0;
    }
    PrintFreeList();
    PrintOccupiedList();
    Timing(t4);
    if (!MemFree('D')) {
        printf("Failed to Free the Memory.\n");
        return 0;
    }
    PrintFreeList();
    PrintOccupiedList();
    PAUSE;
    return 0;
}
