#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef _WIN32 // 根据不同系统编译环境定义暂停和清屏的宏
#define PAUSE fflush(stdout);\
    system("pause")
#define CLEAR system("cls")
#else
#define PAUSE printf("Press Enter to continue.\n"); \
    fflush(stdout);\
    while(getchar()!='\n');\
    getchar()
#define CLEAR system("clear")
#endif

struct FN { // 空闲内存链表节点
    size_t mSize; // 该分块大小
    size_t address; // 该分块的起始地址
    struct FN *prev; // 上一个节点（双向链表）
    struct FN *next; // 下一个节点
};

struct ON { // 被占用内存链表的节点
    char name; // 占用该内存的进程名
    size_t mSize; // 该分块大小
    size_t address; // 该分块的起始地址
    struct ON *next; // 下一个节点
    struct ON *tail; // 仅用于头节点，标记链表尾部
};

typedef struct FN FreeNode; // 空闲节点结构体类型的别名为FreeNode

typedef struct ON OccupiedNode; // 占用节点结构体类型的别名为FreeNode

// 声明函数
void Initialize(FreeNode **fList, OccupiedNode **oList);

void DelFreeNode(FreeNode *fList, FreeNode *node);

short int AdjoinMem(FreeNode *fList, size_t address, size_t oSize);

void InsertFreeNode(FreeNode *fList, FreeNode *node);

void PrintOccupiedList(OccupiedNode *oList);

void PrintFreeList(FreeNode *fList);

void SortOListByAddr(OccupiedNode *oList);

void PrintLinearMem(FreeNode *fList, OccupiedNode *oList);

short int MemAlloc(FreeNode *fList, OccupiedNode *oList, short int *identifier, char name, size_t reqSize);

short int MemFree(FreeNode *fList, OccupiedNode *oList, short int *identifier, char name);

void CompactMemory(FreeNode *fList, OccupiedNode *oList);

/**
 * 初始化用户内存区以及链表
 * @param fList 指向空闲分区链表的地址的指针
 * @param oList 指向被占用分区链表的地址的指针
 */
void Initialize(FreeNode **fList, OccupiedNode **oList) { // 初始化：设置用户内存区大小，生成链表节点
    if (*fList != NULL || *oList != NULL) {
        printf("\n请不要重复初始化\n");
        printf("Do not initialize repeatedly.\n");
        PAUSE;
        return;
    }
    size_t totalMem; // 总内存大小
    long long int sizeTemp;
    // 先指定用户区(User Space)内存大小
    while (1) {
        CLEAR;
        printf("请指定用户内存区大小(按字节算)\n");
        printf("Please specify the Memory Size of User Space(In Bytes): ");
        fflush(stdout); // 输出输出缓冲区
        scanf(" %lld", &sizeTemp);
        if (sizeTemp > 0) {
            totalMem = (size_t) sizeTemp;
            break;
        } else {
            printf("Illegal input. 输入错误.\n");
            PAUSE;
        }
    }
    // 初始化链表头节点，记得最后free!
    *fList = (FreeNode *) calloc(1, sizeof(FreeNode));
    *oList = (OccupiedNode *) calloc(1, sizeof(OccupiedNode));
    // 将一大块用户内存接到空闲链表上
    FreeNode *userSpace = (FreeNode *) calloc(1, sizeof(FreeNode));
    userSpace->mSize = totalMem; // 用户区内存大小
    userSpace->address = 0; // 内存起址0
    InsertFreeNode(*fList, userSpace);
}

/**
 * 移除空闲链表中的一个节点
 * @param fList 指向空闲链表地址的指针
 * @param node 指向待移除节点的指针
 */
void DelFreeNode(FreeNode *fList, FreeNode *node) {
    if (node->prev == NULL) // 上一个是头节点
        fList->next = node->next;
    else
        node->prev->next = node->next; // 此空闲节点被分配出去了，从空闲链表中移除
    if (node->next != NULL) // 保证有下一个节点，避免非法内存访问
        node->next->prev = node->prev;
    fList->mSize--; // 空闲链表长度减短
    free(node); // 释放移除的节点，这是好习惯
}

/**
 * 往空闲分区链表中插入节点
 * @param fList 指向空闲分区链表头部地址的指针
 * @param node 待插入内存节点
 * @note 为了保证最佳适应算法，插入的时候会按照容量升序找到插入的位置
 */
void InsertFreeNode(FreeNode *fList, FreeNode *node) {
    if (fList == NULL) return; // 尚未初始化
    FreeNode *currentNode = fList->next; // 从首个节点开始遍历
    if (currentNode == NULL) { // 除了头节点外还没有任何节点，链表长度为0
        node->prev = NULL; // 不计入头节点
        node->next = NULL;
        fList->next = node; // 接到头节点后面
    } else {
        while (currentNode != NULL) {
            // 在插入的时候找到合适的地方，使得整个链表按地址升序
            if (currentNode->prev == NULL && node->address < currentNode->address) {
                // 顶头，如果下一项比待插入项长度长，就把待插入项插入在头节点后面
                fList->next = node;
                node->prev = NULL;
                node->next = currentNode;
                currentNode->prev = node;
                break;
            } else if (currentNode->next == NULL && node->address > currentNode->address) {
                // 遍历到了尾部，如果待插入项比尾部项还长，就接到尾部后面去
                currentNode->next = node;
                node->prev = currentNode;
                node->next = NULL;
                break;
            } else if (currentNode->prev != NULL && currentNode->prev->address < node->address &&
                       currentNode->address > node->address) {
                // 在中间，左边的项比待插入项小，右边的项比待插入项大
                currentNode->prev->next = node;
                node->prev = currentNode->prev;
                node->next = currentNode;
                currentNode->prev = node;
                break;
            }
            currentNode = currentNode->next; // 遍历链表
        }
    }
    fList->mSize++; // 链表长度增加(头节点的mSize代表链表长度)
}

/**
 * 打印被占用分区链表的情况
 * @param oList 指向被占用分区链表的指针
 */
void PrintOccupiedList(OccupiedNode *oList) {
    if (oList == NULL) { // 链表尚未初始化
        printf("\n请先进行初始化!\n");
        printf("You should initialize first!\n");
        PAUSE;
        return;
    }
    OccupiedNode *currentNode = oList->next;
    CLEAR;
    printf("(In Bytes 以字节为单位)\n");
    printf("\t|--Occupied Memory--被占用内存--|\n");
    if (currentNode == NULL) {
        printf("\t(Empty)(无被占用内存)\n");
    } else {
        size_t i = 1, j;
        while (currentNode != NULL) {
            printf("\t\t");
            printf("->");
            printf("[ Name(标识):%c Addr(地址): %zu, Size(大小): %zu ]\n\t", currentNode->name, currentNode->address,
                   currentNode->mSize);
            for (j = 0; j < i; j++)
                printf("\t");
            i++;
            currentNode = currentNode->next;
        }
        printf("\n");
    }
    PAUSE;
}

/**
 * 打印空闲内存链表的情况
 * @param fList 指向空闲分区链表头地址的指针
 */
void PrintFreeList(FreeNode *fList) {
    if (fList == NULL) { // 链表尚未初始化
        printf("\n请先进行初始化!\n");
        printf("You should initialize first!\n");
        PAUSE;
        return;
    }
    FreeNode *currentNode = fList->next;
    CLEAR;
    printf("(In Bytes 以字节为单位)\n");
    printf("\t|--Available Memory--空闲内存--|\n");
    if (currentNode == NULL) { // 没有空闲内存节点的情况
        printf("\t(Empty)(无空闲内存)\n");
    } else {
        size_t i = 1, j;
        while (currentNode != NULL) {
            printf("\t\t");
            printf("->");
            printf("[ Addr(地址): %zu, Size(大小): %zu ]\n\t", currentNode->address, currentNode->mSize);
            for (j = 0; j < i; j++)
                printf("\t");
            i++;
            currentNode = currentNode->next;
        }
        printf("\n");
    }
    PAUSE;
}

/**
 * 按照内存地址升序对被占用链表进行选择排序
 * @param oList 指向被占用分区链表地址的指针
 */
void SortOListByAddr(OccupiedNode *oList) {
    if (oList != NULL && oList->next != NULL) {
        // 将被占用链表按地址升序进行选择排序
        OccupiedNode *current;
        OccupiedNode *innerCurrent;
        OccupiedNode *selected;
        for (current = oList->next; current != NULL; current = current->next) {
            selected = current;
            for (innerCurrent = current->next; innerCurrent != NULL; innerCurrent = innerCurrent->next) {
                if (innerCurrent->address < selected->address) // 在后面找到了更小的地址
                    selected = innerCurrent;
            }
            if (selected != current) { // 有可以交换的项目，交换数据就够了，不用拆掉重连
                OccupiedNode temp = *current; // 储存current节点的数据
                OccupiedNode *currentNext = current->next; // 备份current的下一个节点
                OccupiedNode *selectedNext = selected->next; // 备份selected的下一个节点
                *current = *selected; // 将selected的数据给current
                *selected = temp; // 将current数据给selected
                current->next = currentNext; // 还原两者指向后一个节点的指针
                selected->next = selectedNext;
            }
        }
    }
}

/**
 * 打印线性内存的情况，能一目了然看到邻接情况和哪些被占用了
 * @param fList 指向空闲内存链表地址的指针
 * @param oList 指向被占用分区链表地址的指针
 */
void PrintLinearMem(FreeNode *fList, OccupiedNode *oList) {
    if (fList == NULL || oList == NULL) {
        printf("\n请先进行初始化!\n");
        printf("You should initialize first!\n");
        PAUSE;
        return;
    }
    CLEAR;
    FreeNode *currentFNode = NULL;
    OccupiedNode *currentONode = NULL;
    if (oList->next != NULL) {
        // 将被占用链表按地址升序进行选择排序
        SortOListByAddr(oList);
        // 排序完毕，开始打印内存情况
        FreeNode *prevFNode = NULL; // 记录遍历的空闲链表前一个节点
        if (fList->next != NULL) { // 被占用链表中和空闲链表中都有节点
            // 这一种情况最复杂，要把空闲链表和被占用链表的节点按地址理顺打印出来
            currentFNode = fList->next;
            // 从地址为0的部分开始打印
            size_t addressToReach = 0; // 待在被占用链表中接上的地址
            short int findOccupied = 0; // 是否寻找被占用链表
            if (currentFNode->address != 0) {
                // 空闲链表首个节点地址不是0，这说明0号地址肯定是一个被占用的内存
                addressToReach = currentFNode->address;
                findOccupied = 1; // 要在被占用链表中接上该节点
            }
            while (1) {
                if (findOccupied) { // 要去被占用链表中寻找
                    currentONode = oList->next;
                    while (currentONode != NULL) {
                        // 这里就可以接着打印了
                        if (prevFNode == NULL || prevFNode->address + prevFNode->mSize <= currentONode->address) {
                            // 找到前一项打印的节点的在物理地址上的后一项
                            printf("+-------------------+ %zu\n", currentONode->address);
                            printf("X Size: %9zu B X Occupied 被占用 (%c)\n", currentONode->mSize, currentONode->name);
                            if (currentONode->address + currentONode->mSize == addressToReach) // 接上后面的空闲节点的地址了
                                break;
                        }
                        currentONode = currentONode->next;
                    }
                }
                if (currentFNode == NULL) // 在这里判断跳出循环
                    break;
                printf("+-------------------+ %zu\n", currentFNode->address);
                printf("| Size: %9zu B | Available 空闲\n", currentFNode->mSize);
                if (currentFNode->next != NULL) { // 后一个节点不为NULL
                    if (currentFNode->next->address ==
                        currentFNode->address + currentFNode->mSize) { // 且后一个节点还能接上
                        findOccupied = 0;
                    } else { // 接不上后一个空闲节点，需要去被占用链表找
                        findOccupied = 1; // 要在被占用链表上接上下一个节点
                        addressToReach = currentFNode->next->address;
                    }
                } else { // 空闲链表下一项是NULL，已经到最后一项了，接着把后面的被占用节点遍历完（如果有的话）
                    findOccupied = 1;
                }
                prevFNode = currentFNode;
                currentFNode = currentFNode->next;
            }

        } else { // 被占用链表中有节点，但是空闲链表中没有节点
            currentONode = oList->next;
            while (currentONode != NULL) {
                printf("+-------------------+ %zu\n", currentONode->address);
                printf("X Size: %9zu B X Occupied 被占用 (%c)\n", currentONode->mSize, currentONode->name);
                currentONode = currentONode->next;
            }
        }
    } else if (fList->next != NULL) { // 被占用链表中没有节点，但是空闲链表中有节点
        currentFNode = fList->next;
        while (currentFNode != NULL) {
            printf("+-------------------+ %zu\n", currentFNode->address);
            printf("| Size: %9zu B | Available 空闲 \n", currentFNode->mSize);
            currentFNode = currentFNode->next;
        }
    } else {
        printf("(Empty)(空)\n");
    }
    printf("+-------------------+\n");
    PAUSE;
}

/**
 * 找到邻接内存
 * @param fList 指向空闲链表的指针变量
 * @param address 待寻找邻接的内存地址
 * @param oSize 待寻找邻接的内存大小
 * @return 1/0 代表 是/否找到并处理了邻接
 * @note 注意，这个函数只会处理空闲链表，被占用链表需要MemFree额外处理哦
 */
short int AdjoinMem(FreeNode *fList, size_t address, size_t oSize) {
    if (fList->next != NULL) { // 空闲分区中有项
        FreeNode *currentNode = fList->next;
        while (currentNode != NULL) {
            if ((currentNode->address + currentNode->mSize == address) &&
                (currentNode->next != NULL && address + oSize == currentNode->next->address)) {
                // 前后都邻接了
                // 以三个节点之中最前面一个节点地址为起始地址
                currentNode->mSize += (oSize + currentNode->next->mSize); // 将第一个节点扩大成三个节点的尺寸
                if (currentNode->next->next != NULL) {
                    currentNode->next->next->prev = currentNode; // 接上后面的节点
                }
                free(currentNode->next); // 释放移除的节点
                currentNode->next = currentNode->next->next; // 从空闲链表上移除后面一项
                fList->mSize--; // 空闲链表长度减短
                printf("进行了回收区两侧内存邻接\nMemory around Recycle Area has been joined.\n");
                break;
            } else if (currentNode->address + currentNode->mSize == address) {
                // 前邻接,这里顺带运算后邻接
                // 前邻接，保持节点起始地址不变，但是大小扩展到两个节点的大小
                currentNode->mSize += oSize;
                printf("进行了回收区低地址侧内存邻接\nLower address memory beside the Recycle Area has been joined.\n");
                break;
            } else if (address + oSize == currentNode->address) {
                // 后邻接
                // 后邻接只需要改变当前节点的起始地址和大小即可
                currentNode->mSize += oSize;
                currentNode->address = address;
                printf("进行了回收区高地址侧内存邻接\nHigher address memory beside the Recycle Area has been joined.\n");
                break;
            }
            currentNode = currentNode->next;
        }
        if (currentNode == NULL) // 没有找到邻接项
            return 0;
    } else {
        return 0;
    }
    return 1;
}

/**
 * 模拟内存回收
 * @param fList 指向空闲内存链表地址的指针
 * @param oList 指向被占用分区链表地址的指针
 * @param identifier 指向标识符记录数组的指针
 * @param name 被占用分区的标识符
 * @return 1/0 代表 是/否 释放成功
 * @note 包括对邻接区的判断
 */
short int MemFree(FreeNode *fList, OccupiedNode *oList, short int *identifier, char name) {
    if (fList == NULL || oList == NULL) { // 未经初始化
        printf("\n请先进行初始化!\n");
        printf("You should initialize first!\n");
        return 0;
    }
    if (oList->next == NULL) { // 没有被占用的内存
        return 0;
    } else {
        short int found = 0; // 是否找到标识符对应的内存区
        size_t nameHash = (size_t) (name) - 48; // 计算出哈希
        OccupiedNode *currentNode = oList->next;
        OccupiedNode *prevNode = NULL; // 记录前一个节点
        while (currentNode != NULL) {
            if (currentNode->name == name) {
                found = 1;
                break;
            }
            prevNode = currentNode; // 储存前一个节点
            currentNode = currentNode->next;
        }
        if (found) {
            // 找到了，准备释放
            // 先尝试邻接
            if (!AdjoinMem(fList, currentNode->address, currentNode->mSize)) {
                // 没有找到邻接
                FreeNode *new = (FreeNode *) calloc(1, sizeof(FreeNode)); // 创建新节点接回空闲链表
                new->address = currentNode->address;
                new->mSize = currentNode->mSize;
                InsertFreeNode(fList, new); // 插回到空闲链表中
            }
            // 接下来将节点从被占用链表中移除
            if (prevNode == NULL) { // 说明前一个是头节点
                oList->next = currentNode->next; // 将后一项紧接头节点后面
            } else {
                prevNode->next = currentNode->next; // 后一项接到前一项后面
            }
            if (currentNode->next == NULL)
                oList->tail = prevNode; // 删掉了最后一项，将尾指针前移指向前一个节点
            oList->mSize--; // 空闲链表长度减短
            free(currentNode); // 释放移除的节点
            identifier[nameHash] = 0; // 标记对应标识符为未使用
        } else {
            printf("未找到标识符 %c 对应的分区\n", name);
            printf("Can't find the Memory corresponding to the Identifier %c\n", name);
        }
        return found;
    }
}

/**
 * 模拟内存分配过程
 * @param fList 指向空闲内存链表地址的指针
 * @param oList 指向被占用分区链表地址的指针
 * @param identifier 指向标识符记录数组的指针
 * @param name 这块被占用内存的标识符(char)
 * @param reqSize 请求分配内存的大小
 * @return 1/0 代表 是/否 成功
 */
short int MemAlloc(FreeNode *fList, OccupiedNode *oList, short int *identifier, char name, size_t reqSize) {
    if (fList == NULL || oList == NULL) { // 未经初始化
        printf("\n请先进行初始化!\n");
        printf("You should initialize first!\n");
        return 0;
    }
    if (!isalnum(name)) { // 标识字符不合法，必须是数字或者字母
        printf("\n标识符只能是数字或者字母\n");
        printf("Identifier can only be numbers or letters.\n");
        return 0;
    }
    size_t nameHash = (size_t) (name) - 48; // 计算出哈希
    if (identifier[nameHash] != 0) { // 已经使用了这个标识符
        printf("\n标识符名已经被使用过了: %c\n", name);
        printf("Identifier name has been used.\n");
        return 0;
    }
    if (fList->next == NULL) { // 没有空闲的内存了，分配失败
        printf("\n没有任何空闲的内存，分配失败\n");
        printf("No Memory Fragment available.\n");
        return 0;
    } else {
        size_t totalAvaSize = 0; // 记录遍历到的总空闲内存大小
        size_t currentMinSize = 0; // 当前找到的最小容量
        FreeNode *currentMinNode = NULL; // 当前找到的容量最小的节点
        FreeNode *currentNode = fList->next;
        while (currentNode != NULL) {
            // 从头部开始搜寻足够大的空闲分区节点，且容量最小
            if (currentNode->mSize >= reqSize) { // 找到了足够大的空闲分区
                if (currentMinNode == NULL || (currentNode->mSize < currentMinSize)) { // 刚找到第一个空闲分区，或者新找到的空闲分区更小，进行替换
                    currentMinSize = currentNode->mSize;
                    currentMinNode = currentNode;
                }
            }
            totalAvaSize += currentNode->mSize;
            currentNode = currentNode->next;
        }
        if (currentMinNode != NULL) { // 成功找到了
            // 创建一个占用链表节点
            size_t leftSize = currentMinNode->mSize - reqSize; // 分配后剩余的碎片大小
            OccupiedNode *oNode = (OccupiedNode *) calloc(1, sizeof(OccupiedNode));
            oNode->name = name; // 标记符
            oNode->mSize = reqSize; // 占用的大小
            oNode->address = currentMinNode->address; // 地址
            oNode->next = NULL; // 新的被占用节点加入到链表尾部
            if (oList->next == NULL) { // 被占用分区链表中还没有节点
                oList->next = oNode;
            } else {
                // 添加tail指针就是为了在加入链表时时间复杂度O(1)
                oList->tail->next = oNode;
            }
            oList->tail = oNode; // 移动尾部指针变量指向新节点
            oList->mSize++; // 链表长度增加
            DelFreeNode(fList, currentMinNode); // 从空闲链表移除分配出去的分区
            if (leftSize > 0) { // 分配之后还有多余的空余碎片
                FreeNode *fragment = (FreeNode *) calloc(1, sizeof(FreeNode));
                fragment->mSize = leftSize;
                fragment->address = oNode->address + reqSize; // 碎片的起始地址
                InsertFreeNode(fList, fragment); // 将碎片接回到空闲链表上
            }
            identifier[nameHash] = 1; // 标记该标识名已被使用
        } else { // 没有找到足够大的分区
            printf("\n没有整块的足够大的内存，分配失败\n");
            printf("No Memory Fragment has enough size to be allocated.\n");
            if (totalAvaSize >= reqSize) { // 所有空闲分区总大小还是可以的，提示可以使用内存紧凑
                printf("\n但是，所有空闲分区的总容量能满足分配需求，可以进行碎片拼接紧凑。\n");
                printf("However, the sum of available memory sizes meets the need.\n ");
                printf("Maybe Memory Compaction will work.\n");
            }
            return 0;
        }
    }
    return 1;
}

/**
 * 对内存进行紧凑处理
 * @param fList 指向空闲内存链表地址的指针
 * @param oList 指向被占用分区链表地址的指针
 */
void CompactMemory(FreeNode *fList, OccupiedNode *oList) {
    CLEAR;
    if (fList == NULL || oList == NULL) { // 未经初始化
        printf("\n请先进行初始化!\n");
        printf("You should initialize first!\n");
        return;
    }
    if (fList->next != NULL && oList->next != NULL) {
        // 只有两张链表中均有内容时才有可能进行内存紧凑
        // 思路：先把所有空闲节点连成一大块，起始地址为0
        // 然后将这一大块空闲节点影响到的被占用分区节点全部往后移动（指address方面的移动）
        FreeNode *currentFNode = fList->next;
        FreeNode *fTemp;
        size_t totalAvailable = 0; // 总空闲空间
        while (currentFNode != NULL) {
            totalAvailable += currentFNode->mSize;
            fTemp = currentFNode;
            currentFNode = currentFNode->next;
            free(fTemp); // 释放当前节点
        }
        // 分配一大块新空闲区以替代原来的
        FreeNode *newFNode = (FreeNode *) calloc(1, sizeof(FreeNode));
        newFNode->next = NULL;
        newFNode->prev = NULL;
        newFNode->mSize = totalAvailable; // 这一大块空闲区的容量
        newFNode->address = 0;// 地址从0开始
        fList->next = newFNode; // 接到头节点后面
        fList->mSize = 1; // 空闲区数量整合为一个
        // 重头戏：移动作业
        SortOListByAddr(oList); // 先把被占用链表按地址升序排序
        OccupiedNode *currentONode = oList->next;
        OccupiedNode *prevONode = NULL; // 记录遍历的上一个节点
        OccupiedNode *oNextTemp;
        while (currentONode != NULL) { // 遍历一遍被占用链表
            oNextTemp = currentONode->next; // 暂存当前节点的下一个节点
            if (currentONode->address >= newFNode->address && // 作业地址在大空闲区内，就需要移动
                (currentONode->address < newFNode->address + newFNode->mSize)) { // 注意这里是<,邻接是可以的
                // 从链表中暂移走该项
                if (prevONode == NULL) // 前一个是头节点
                    oList->next = currentONode->next;
                else
                    prevONode->next = currentONode->next;
                currentONode->next = NULL;
                currentONode->address = oList->tail->address + oList->tail->mSize; // 尾部的地址
                oList->tail->next = currentONode; // 把该节点移动到被占用链表的尾部
                oList->tail = currentONode; // 更新尾部指针指向该节点
            } else {
                prevONode = currentONode;
            }
            currentONode = oNextTemp;
        }
        // 作业全部移动到后面之后要检查头一个作业占用的内存地址
        // 如果该地址没有紧接一大块作业的尾部，整体还需要移动一下
        size_t addrDiff; // 空闲区和被占用区地址差距
        if ((addrDiff = oList->next->address - (newFNode->address + newFNode->mSize)) > 0) {
            currentONode = oList->next;
            while (currentONode != NULL) { // 再次遍历链表
                currentONode->address -= addrDiff; // 每一项的地址都前移，使得被占用区紧接空闲区
                currentONode = currentONode->next;
            }
        }
        printf("内存紧凑成功！获得了%zu Byte(s)的空闲分区\n", totalAvailable);
        printf("Memory compaction success! Now there's a Memory Partition of %zu Byte(s)\n", totalAvailable);
    } else {
        printf("无法进行内存紧凑！\n");
        printf("Unable to compact available memory.\n");
    }
    PAUSE;
}

int main() {
    short int cycle = 1; // 为0时程序退出
    int choice;
    // 分配一个数组，用于记录已经被占用的标识符
    // z的ASCII码为122，0的ASCII码为48，122-48+1就能储存所有的字母和数字
    // 这里可以看作一个极简的哈希表结构
    short int *identifiers = (short int *) calloc(122 - 48 + 1, sizeof(short int));
    FreeNode *freeList = NULL; // 定义空闲分区链表
    OccupiedNode *occupiedList = NULL; // 定义被占用分区链表
    // TEST CODE
    Initialize(&freeList, &occupiedList);
    MemAlloc(freeList, occupiedList, identifiers, 'A', 1);
    MemAlloc(freeList, occupiedList, identifiers, 'B', 2);
    MemAlloc(freeList, occupiedList, identifiers, 'C', 3);
    MemAlloc(freeList, occupiedList, identifiers, 'D', 4);
    MemAlloc(freeList, occupiedList, identifiers, 'E', 5);
    MemAlloc(freeList, occupiedList, identifiers, 'F', 6);
    MemAlloc(freeList, occupiedList, identifiers, 'G', 7);
    MemAlloc(freeList, occupiedList, identifiers, 'H', 8);
    MemAlloc(freeList, occupiedList, identifiers, 'I', 9);
    MemAlloc(freeList, occupiedList, identifiers, 'J', 10);
    MemFree(freeList, occupiedList, identifiers, 'B');
    MemFree(freeList, occupiedList, identifiers, 'D');
    MemFree(freeList, occupiedList, identifiers, 'G');
    CompactMemory(freeList, occupiedList);
    MemAlloc(freeList, occupiedList, identifiers, 'Y', 57);
    // TEST CODE END
    while (cycle) {
        CLEAR;
        printf("==========MENU==========\n");
        printf("\t0. Exit 退出\n");
        printf("\t1. Initialize 初始化\n");
        printf("\t2. Allocate Memory 分配内存\n");
        printf("\t3. Free Memory 释放内存\n");
        printf("\t4. Print the List of Free Memory  打印空闲分区链表\n");
        printf("\t5. Print the List of Occupied Memory 打印被占用分区链表\n");
        printf("\t6. Print Memory Partitions in order of Address 按地址升序打印当前内存分区情况\n");
        printf("\t7. Compact Memory Fragments 紧凑内存碎片\n");
        printf("---------------------\n");
        printf("输入选项前对应数字以继续\n");
        printf("Type in the correspond Number before the option: ");
        fflush(stdout); // 输出输出缓冲区中的内容
        scanf(" %d", &choice);
        switch (choice) {
            case 0:
                cycle = 0;
                break;
            case 1:
                Initialize(&freeList, &occupiedList);
                break;
            case 2: {
                char memName;
                long long int allocSize;
                while (1) {
                    CLEAR;
                    printf("(分配大小输入 -1 以返回)(Enter -1 in alloc size to cancel)\n---------------\n");
                    printf("请输入分配的大小和标识符\n");
                    printf("（格式如: 100 A )(Format example: 100 A )\n");
                    printf("Please type in the size to allocate and the identifier(char): ");
                    fflush(stdout);
                    scanf(" %zu %c", &allocSize, &memName);
                    if (allocSize == -1) {
                        break;
                    } else if (allocSize <= 0) {
                        printf("\n分配的内存大小不能<=0！\n");
                        printf("Illegal input of size.\n");
                        PAUSE;
                    }
                    if (!MemAlloc(freeList, occupiedList, identifiers, (char) memName, (size_t) allocSize)) {
                        printf("内存分配失败\n");
                        printf("Failed to allocate memory.\n");
                        PAUSE;
                        break;
                    }
                }
            }
                break;
            case 3: {
                char target;
                while (1) {
                    CLEAR;
                    printf("(输入字符 - 以返回)(Type char '-' to cancel)\n---------------\n");
                    printf("输入要回收的内存分区的对应标识符\n");
                    printf("Type in the correspond identifier of the Allocated Memory to Free: ");
                    fflush(stdout);
                    scanf(" %c", &target);
                    if (target == '-') {
                        break;
                    }
                    if (MemFree(freeList, occupiedList, identifiers, target)) {
                        printf("分区回收成功\nMemory recycled successfully.\n");
                    } else {
                        printf("分区回收失败\nFailed to recycle Memory.\n");
                    }
                    PAUSE;
                }
            }
                break;
            case 4:
                PrintFreeList(freeList);
                break;
            case 5:
                PrintOccupiedList(occupiedList);
                break;
            case 6:
                PrintLinearMem(freeList, occupiedList);
                break;
            case 7:
                CompactMemory(freeList, occupiedList);
                break;
            default:
                break;
        }
        choice = 0;
    }

    // 回收堆内存
    free(identifiers); // 释放标识符数组
    if (freeList != NULL && freeList->next != NULL) { // 如果空闲链表中有内容
        FreeNode *currentNode = freeList->next;
        FreeNode *temp = NULL;
        while (currentNode != NULL) { // 遍历释放每一项节点
            temp = currentNode; // 暂存地址
            currentNode = currentNode->next;
            free(temp); // 释放移除的节点
        }
        freeList->next = NULL;
    }
    if (occupiedList != NULL && occupiedList->next != NULL) { // 如果被占用链表中有节点
        OccupiedNode *currentNode = occupiedList->next;
        OccupiedNode *temp = NULL;
        while (currentNode != NULL) {
            temp = currentNode;
            currentNode = currentNode->next;
            free(temp);
        }
        occupiedList->next = NULL;
    }
    free(freeList); // 释放头节点
    free(occupiedList);
    return 0;
}
