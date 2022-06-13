/**
 * MySQL C API文档：https://dev.mysql.com/doc/c-api/8.0/en/c-api-function-reference.html
 * 这是一个C语言+MySQL小应用：小账本
 * Written by SomeBottle 2022.6.10
 */
#include "main.h"
#include "tableFuncs.h"

static int TableCheck(MYSQL *connection);

static void Menu(MYSQL *connection);

static void PrintItems(MYSQL *connection, int sort, int tid, int direction);

static void CheckAccounts(MYSQL *connection);

static void EditFlowTypes(MYSQL *connection);

static void MakeAccount(MYSQL *connection);

/**
 * 用于二分查找的比较
 * @param a 前一个元素
 * @param b 后一个元素
 * @return 元素值相减
 */
int BinCmp(const void *a, const void *b) {
    const int *before = (int *) a;
    const int *after = (int *) b;
    return (*before) - (*after);
}

/**
 * 设置初始预算
 * @param connection MYSQL指针
 */
void SetBudget(MYSQL *connection) {
    // 先查询一下目前的预算
    QueryResult current = SelectQuery(connection, "SELECT * FROM MoneyTypes WHERE `mtype`='budget';");
    if (current.result == NULL) // 查询不成功
        return;
    if (current.rowNum > 0) { // 查出来起码要有一行
        float newBudget;
        while (1) {
            CLEAR;
            newBudget = 0;
            printf("\t===============Initial Budget============\n");
            printf("\t ￥ %s\n", current.result[0][1]);
            printf("\t================当前的初始预算==============\n");
            printf("\t[输入-1以取消 Enter -1 to Cancel]\n");
            printf("\t设置初始预算为 Set the Initial Budget to: ");
            fflush(stdout);
            scanf(" %f", &newBudget);
            CLEAR_EOF;
            if (newBudget == -1) {
                break;
            } else if (newBudget > 0) {
                if (UpdateMoney(connection, 2, newBudget)) {
                    float balance = newBudget; // 余额
                    float amountTmp;
                    int directionTmp;
                    size_t i = 0;
                    // 成功更新
                    // 接下来计算余额，选择amount和direction两列
                    QueryResult items = SelectQuery(connection, "SELECT amount,direction FROM FlowItems;");
                    for (i = 0; i < items.rowNum; i++) {
                        amountTmp = strtof(items.result[i][0], NULL);
                        directionTmp = (int) strtol(items.result[i][1], NULL, 10);
                        if (directionTmp == 1)
                            balance -= amountTmp; // 支出
                        else
                            balance += amountTmp; // 收入
                    }
                    if (!UpdateMoney(connection, 1, balance))
                        printf("余额更新失败！Failed to Update the Balance.\n");
                    FreeQueryRes(&items); // 用完后释放
                    printf("更新成功！Update Successfully!\n");
                } else {
                    printf("更新初始预算失败 Failed to Update Initial Budget: %s\n", mysql_error(connection));
                }
                break;
            } else {
                printf("预算应该是正数 Budget amount should be a positive Number!(>0)\n");
            }
        }
    } else {
        printf("Error! Property missing when getting budget.\n");
    }
    FreeQueryRes(&current); // 用完了别忘了释放
    PAUSE;
}

/**
 * 修改记账类型部分
 * @param connection MYSQL指针
 */
void EditFlowTypes(MYSQL *connection) {
    FTResult current = GetFlowTypes(connection);
    if (current.tidArr != NULL) {
        short int cycle = 1;
        int choice;
        while (cycle) {
            CLEAR;
            choice = 0;
            printf("\t===============Current Flow Types============\n");
            printf(current.listStr);
            printf("\t==================目前的记账类型================\n");
            printf("\t 1. Delete Flow Type 删除一个分类\n");
            printf("\t 2. Add Flow Type 新建一个分类\n");
            printf("\t 3. Edit Name 编辑分类名\n");
            printf("\t 4. Return to Menu 返回菜单\n");
            printf("\t请输入对应序号 Enter a correspond Number: ");
            fflush(stdout);
            scanf(" %d", &choice);
            CLEAR_EOF;
            int tid = 0;
            switch (choice) {
                case 1: {
                    printf("\t输入分类前的序号以删除 \n\t"
                           "Enter the Number correspond to the Type to delete: ");
                    fflush(stdout);
                    scanf(" %d", &tid); // 输入tid
                    CLEAR_EOF;
                    // 看看tid能不能找到
                    if (bsearch(&tid, current.tidArr, current.tidArrLen, sizeof(int), BinCmp) != NULL) {
                        if (tid != 1) {
                            char confirm;
                            printf("\tWARNING: This may cause DESCADE DELETION of FlowItems!\n"
                                   "\t警告：这会导致部分账目被级联删除！\n");
                            printf("\t确定要继续吗？Are you sure to continue? [Y/N]: ");
                            fflush(stdout);
                            scanf(" %c", &confirm);
                            CLEAR_EOF;
                            if (confirm == 'y' || confirm == 'Y') {
                                if (OperateFlowTypes(connection, 1, tid, "")) {
                                    printf("\t成功删除分类！Successfully deleted!\n");
                                    FreeFTRes(&current);
                                    // 更新展示列表
                                    current = GetFlowTypes(connection);
                                } else {
                                    printf("\t删除分类失败！Failed to delete.\n");
                                }
                            }
                        } else {
                            printf("\t默认分类不能被删除 Default Type Cannot be Deleted!\n");
                        }
                    } else {
                        printf("\t没有找到对应序号 Flow Type not Found!\n");
                    }
                }
                    break;
                case 2: {
                    char *newName = (char *) calloc(61, sizeof(char));
                    printf("\t请输入新分类的名字 Enter the new Name for the type( < 60 Bytes): ");
                    fflush(stdout);
                    scanf(" %60[^\n]", newName); // 最多接受60个字节
                    CLEAR_EOF;
                    // 插入新类型
                    if (OperateFlowTypes(connection, 2, 0, newName)) {
                        FreeFTRes(&current);
                        // 更新展示列表
                        current = GetFlowTypes(connection);
                        printf("\t添加新分类成功！Successfully added new Type %s\n", newName);
                    } else {
                        printf("\t添加新分类失败！Failed to add new Type.\n");
                    }
                    free(newName); // 用完就释放
                }
                    break;
                case 3: {
                    printf("\t输入分类前的序号以编辑 \n"
                           "Enter the Number correspond to the Type to edit: ");
                    fflush(stdout);
                    scanf(" %d", &tid); // 输入tid
                    CLEAR_EOF;
                    // 看看tid能不能找到
                    if (bsearch(&tid, current.tidArr, current.tidArrLen, sizeof(int), BinCmp) != NULL) {
                        char *newName = (char *) calloc(61, sizeof(char));
                        printf("\t输入新的分类名 Enter a new Name for the type (<60 Bytes): ");
                        fflush(stdout);
                        scanf(" %60[^\n]", newName); // 最多接受60个字节
                        CLEAR_EOF;
                        if (OperateFlowTypes(connection, 3, tid, newName)) {
                            FreeFTRes(&current);
                            // 更新展示列表
                            current = GetFlowTypes(connection);
                            printf("\t修改分类名成功！Successfully change the name to %s\n", newName);
                        } else {
                            printf("\t修改分类名失败 Failed to change the name.\n");
                        }
                        free(newName);
                    } else {
                        printf("\t没有找到对应序号 Flow Type not Found!\n");
                    }
                }
                    break;
                case 4:
                    cycle = 0;
                    break;
                default:
                    printf("\t哦豁，这不是个有效的选项 Oops, invalid choice!\n");
                    break;
            }
            PAUSE;
        }
    } else {
        printf("无法获得记账分类 Failed to get Flow Types!\n");
        PAUSE;
    }
    FreeFTRes(&current); // 用完后释放是好习惯
}

/**
 * 打印账目
 * @param connection MYSQL指针
 * @param sort 排序方式(时间升序11/降序12，金额升序21/降序22）
 * @param tid 分类对应ID(查看某种分类的)
 * @param direction 收入(0)还是支出(1)
 */
void PrintItems(MYSQL *connection, int sort, int tid, int direction) {
    char *formatStr = "SELECT  `id`,`amount`,`time`,`direction`,`tname`,`note` "
                      "FROM FlowItems,FlowTypes "
                      "WHERE FlowItems.tid=FlowTypes.tid " // 连接查询
                      "%s "
                      "%s ";
    char *whereQuery = (char *) calloc(80, sizeof(char));
    if (tid != -1) {
        sprintf(whereQuery, "%sAND %s%d ", whereQuery, "FlowItems.tid=", tid);
    }
    if (direction != -1) {
        sprintf(whereQuery, "%sAND %s%d ", whereQuery, "`direction`=", direction);
    }
    char *orderQuery = (char *) calloc(80, sizeof(char));
    switch (sort / 10) {
        case 1:
            strcpy(orderQuery, "ORDER BY `time` "); // 按时间排序
            break;
        case 2:
            strcpy(orderQuery, "ORDER BY `amount` "); // 按金额排序
            break;
        default:
            strcpy(orderQuery, "ORDER BY `id` "); // 默认按ID
    }
    switch (sort % 10) {
        case 1:
            strcat(orderQuery, "ASC"); // 升序
            break;
        case 2:
            strcat(orderQuery, "DESC"); // 降序
            break;
        default:
            strcat(orderQuery, "ASC"); // 默认升序
            break;
    }
    char *queryStr = (char *) calloc(strlen(formatStr) + strlen(whereQuery) + strlen(orderQuery) + 3, sizeof(char));
    sprintf(queryStr, formatStr, whereQuery, orderQuery);
    QueryResult listQuery = SelectQuery(connection, queryStr);
    char *rowFormat = "\t| %-10s | %-10s | %-21s | %-14s | %-10s | %-20s |\n";
    if (listQuery.result != NULL) { // 查询成功
        size_t i;
        printf(rowFormat,
               "ID", "AMOUNT", "TIME", "TYPE", "CATEGORY", "NOTE");
        char **temp;
        int inOrOut = 0;
        for (i = 0; i < listQuery.rowNum; i++) {
            temp = listQuery.result[i];
            inOrOut = (int) strtol(temp[3], NULL, 10);
            printf(rowFormat,
                   temp[0], temp[1], temp[2],
                   inOrOut == 0 ? "Income" : "Expend", temp[4], temp[5]);
        }
        FreeQueryRes(&listQuery);
    } else {
        printf("查询账目失败 Failed to List the Accounts.\n");
    }
    free(queryStr);
    free(orderQuery);
    free(whereQuery);
}

/**
 * 查账部分
 * @param connection MYSQL指针
 */
void CheckAccounts(MYSQL *connection) {
    // 获得所有的分类
    FTResult ftItems = GetFlowTypes(connection);
    // 获得元组（行）总数量
    QueryResult checkTotalNum = SelectQuery(connection, "SELECT COUNT(*) FROM FlowItems;");
    // 获得余额和预算信息
    QueryResult checkBalance = SelectQuery(connection, "SELECT * FROM MoneyTypes;");
    int totalRowNum = 0;
    if (checkTotalNum.result != NULL && checkBalance.result != NULL) // 查询成功
        totalRowNum = (int) strtol(checkTotalNum.result[0][0], NULL, 10); // 获得行数
    if (totalRowNum > 0) { // 至少有一行
        short int cycle = 1;
        int sortBy = 0; // 默认ID升序
        int chosenTid = -1; // 筛选的分类
        int chosenDirection = -1; // 展示支出/收入
        int optionChoice = 0; // 选择的选项
        float balance = 0; // 暂存余额
        size_t i;
        while (cycle) {
            CLEAR;
            printf("\t=====================================\n\t");
            for (i = 0; i < checkBalance.rowNum; i++) {
                char **temp = checkBalance.result[i];
                if (strcmp(temp[0], "balance") == 0) {
                    printf("余额Balance: %s ￥ ", temp[1]);
                    balance = strtof(temp[1], NULL);
                } else if (strcmp(temp[0], "budget") == 0) {
                    printf("原预算Budget: %s ￥", temp[1]);
                }
            }
            printf("\n\t=====================================\n");
            PrintItems(connection, sortBy, chosenTid, chosenDirection);
            printf("\t---[");
            switch (sortBy / 10) {
                case 1:
                    printf("Time ");
                    break;
                case 2:
                    printf("Amount ");
                    break;
                default:
                    printf("ID ");
                    break;
            }
            switch (sortBy % 10) {
                case 1:
                    printf("Ascending");
                    break;
                case 2:
                    printf("Descending");
                    break;
                default:
                    printf("Ascending");
                    break;
            }
            printf("]---");
            if (chosenTid != -1)
                printf("[Chosen Tid:%d]---", chosenTid);
            if (chosenDirection != -1)
                printf(chosenDirection == 1 ? "[Expend Only]---" : "[Income Only]---");
            printf("\n\n");
            printf("\t 1. 筛选展示账目 Make Filters\n");
            printf("\t 2. 删除账目 Delete Account Item\n");
            printf("\t (Other Input). 返回 Return to Menu\n");
            printf("\t输入选项前的序号 Enter the Number correspond to the option: ");
            fflush(stdout);
            scanf(" %d", &optionChoice);
            CLEAR_EOF;
            switch (optionChoice) {
                case 1: { // 改变展示方式
                    CLEAR;
                    int choice;
                    printf("\t===============Current Flow Types============\n");
                    printf(ftItems.listStr);
                    printf("\t==================目前的记账类型================\n");
                    printf("\n\t输入 2/1/0 按 数额/时间/ID 排序\n");
                    printf("\tEnter 2/1/0 to sort by AMOUNT/TIME/ID: ");
                    fflush(stdout);
                    scanf(" %d", &choice);
                    CLEAR_EOF;
                    if (choice >= 0 && choice <= 2) {
                        sortBy = choice * 10;
                    } else {
                        printf("\t输入无效 INPUT INVALID.\n");
                        PAUSE;
                        break;
                    }
                    printf("\n\t输入 2/1 按 降序/升序 排序\n");
                    printf("\tEnter 2/1 to use DESCENDING/ASCENDING Sort: ");
                    fflush(stdout);
                    scanf(" %d", &choice);
                    CLEAR_EOF;
                    if (choice >= 1 && choice <= 2) {
                        sortBy += choice;
                    } else {
                        printf("\t输入无效 INPUT INVALID.\n");
                        PAUSE;
                        break;
                    }
                    printf("\n\t筛选出对应分类的账目，输入分类的ID（-1代表不筛选）: \n");
                    printf("\tEnter the TID correspond to the FlowType to filter(-1 for no filter): ");
                    fflush(stdout);
                    scanf(" %d", &choice);
                    CLEAR_EOF;
                    if (choice != -1 &&
                        bsearch(&choice, ftItems.tidArr, ftItems.tidArrLen, sizeof(int), BinCmp) == NULL) {
                        printf("\t找不到TID对应的类型 Cannot find the correspond FlowType.\n");
                        PAUSE;
                        break;
                    }
                    chosenTid = choice;
                    printf("\n\t输入 1/0 筛选 支出/收入 账目（-1代表不筛选）: \n");
                    printf("\tEnter 1/0 to filter Expend/Income Accounts(-1 for no filter): ");
                    fflush(stdout);
                    scanf(" %d", &choice);
                    CLEAR_EOF;
                    if (choice >= -1 && choice <= 1) {
                        chosenDirection = choice;
                    } else {
                        printf("\t输入无效 INPUT INVALID.\n");
                        PAUSE;
                        break;
                    }
                }
                    break;
                case 2: {
                    int idToDel = -1;
                    printf("\n[输入-1以取消 Enter -1 to Cancel]\n");
                    printf("输入你想删除的账目的ID Enter the ID corresponding to the item you want to delete: ");
                    fflush(stdout);
                    scanf(" %d", &idToDel);
                    CLEAR_EOF;
                    if (idToDel == -1) // -1取消
                        break;
                    if (DelFlowItem(connection, idToDel, balance)) {
                        FreeQueryRes(&checkBalance);
                        // 重新获得余额和预算信息
                        checkBalance = SelectQuery(connection, "SELECT * FROM MoneyTypes;");
                    }
                    PAUSE;
                }
                    break;
                default:
                    cycle = 0;
                    break;
            }
        }
    } else {
        printf("\t哦吼，数据库中还没有任何条目呢\n"
               "\tOops, there's no item in the Database.\n");
        PAUSE;
    }
    FreeQueryRes(&checkBalance); // 用完后释放
    FreeQueryRes(&checkTotalNum);
    FreeFTRes(&ftItems);
}

/**
 * 记账部分
 * @param connection MYSQL指针
 */
void MakeAccount(MYSQL *connection) {
    CLEAR;
    size_t i, totalLen = 0;
    int flowDirection = 1;
    float flowAmount = 0;
    int flowType = 0;
    // 查询余额
    QueryResult balanceQuery = SelectQuery(connection, "SELECT amount FROM MoneyTypes WHERE `mtype`='balance';");
    if (balanceQuery.result == NULL) // 查询失败
        return;
    // 获取记账分类信息
    FTResult flowTypesInfo = GetFlowTypes(connection);
    if (balanceQuery.rowNum > 0 && flowTypesInfo.tidArr != NULL) { // 查询出来分类至少要有一项，否则就出问题了
        float balance = strtof(balanceQuery.result[0][0], NULL); // 获得余额
        char *note = NULL;
        while (1) {
            CLEAR;
            if (note != NULL)
                free(note);
            note = (char *) calloc(65536, sizeof(char)); // MySQL TEXT类型最多储存65535个字符
            printf("\t===============Flow Types 记账分类============\n");
            printf("%s", flowTypesInfo.listStr);
            printf("\t=============================================\n");
            printf("\t[输入-1以退出记账 Enter -1 to return to the Menu]\n");
            printf("\t请输入1/0代表支出/收入 Enter 1/0 for Expenditure/Income: ");
            fflush(stdout);
            scanf(" %d", &flowDirection);
            CLEAR_EOF;
            if (flowDirection == -1) { // 返回主菜单
                break;
            } else if (flowDirection != 0 && flowDirection != 1) { // 检查值不是1/0
                printf("\t只能是1/0 Flow Direction can only be 1 or 0!\n");
                PAUSE;
                continue;
            }
            printf("\n\t输入金额 Enter the Amount: ");
            fflush(stdout);
            scanf(" %f", &flowAmount);
            CLEAR_EOF;
            if (flowAmount <= 0) {
                printf("\t金额应该是正数！Amount should be a positive number!(>0)\n");
                PAUSE;
                continue;
            }
            printf("\n\t输入记账分类对应的序号 Enter the Number before each Flow Type: ");
            fflush(stdout);
            scanf(" %d", &flowType);
            CLEAR_EOF;
            if (bsearch(&flowType, flowTypesInfo.tidArr, flowTypesInfo.tidArrLen, sizeof(int), BinCmp) == NULL) {
                // typeid中找不到对应的
                printf("分类ID错误 Cannot find a correspond Flow Type.\n");
                PAUSE;
                continue;
            }
            printf("\n\t输入备注 Enter the note: ");
            fflush(stdout);
            scanf(" %65535[^\n]", note);
            CLEAR_EOF;
            if (InsertFlowItem(connection, flowDirection, flowAmount, flowType, note)) {
                switch (flowDirection) {
                    case 1:
                        balance -= flowAmount;
                        break;
                    case 0:
                        balance += flowAmount;
                        break;
                    default:
                        printf("ERROR: Unknown flowDirection appears!\n");
                        break;
                }
                // 更新余额
                if (UpdateMoney(connection, 1, balance)) {
                    printf("\t成功记入！ Successfully made.\n");
                } else {
                    printf("\t余额更新失败！ Failed to update Balance.\n");
                }
            } else {
                printf("\t记账失败！Failed to make account.\n");
            }
            PAUSE;
        }
        free(note); // 用完就释放
        FreeFTRes(&flowTypesInfo);
    } else {
        printf("ERROR! Missing necessary items.You should reinitialize the Database.\n");
    }
    FreeQueryRes(&balanceQuery); // 用完后进行回收
}

/**
 * 主菜单
 * @param connection MYSQL指针
 */
void Menu(MYSQL *connection) {
    short int cycle = 1;
    int choice;
    while (cycle) {
        CLEAR;
        printf("\t======================\n");
        printf("\t Small Account Book\n");
        printf("\t 小小记账本\n");
        printf("\t By SomeBottle\n");
        printf("\t======================\n");
        printf("\t 1. Make Account 记账\n");
        printf("\t 2. Check Accounts 查账\n");
        printf("\t 3. Edit Flow Types 修改记账种类\n");
        printf("\t 4. Set initial Budget 设置初始预算\n");
        printf("\t (Other Input). Exit 其他输入退出\n\n");
        printf("\tPlease enter your choice: ");
        fflush(stdout);
        scanf(" %d", &choice); // 接受一个整型
        CLEAR_EOF;
        switch (choice) {
            case 1:
                MakeAccount(connection);
                break;
            case 2:
                CheckAccounts(connection);
                break;
            case 3:
                EditFlowTypes(connection);
                break;
            case 4:
                SetBudget(connection);
                break;
            default:
                cycle = 0;
                break;
        }
    }
    printf("Bye~\n");
}

/**
 * 刚刚建立数据库连接后检查表是否正常
 * @param connection MYSQL指针
 * @return 1/0 代表 是/否 成功
 * @note 包括表的初始化
 */
int TableCheck(MYSQL *connection) {
    int returnVal = 1;
    if (!(TableExist(connection, "FlowItems") &&
          TableExist(connection, "FlowTypes") &&
          TableExist(connection, "MoneyTypes"))) { // 关系表不全，提示用户初始化
        char choice;
        printf("Table items incomplete, recreate Tables? [Y/N]: ");
        fflush(stdout);
        scanf(" %c", &choice);
        CLEAR_EOF;
        if (choice == 'Y' || choice == 'y') { // 确定初始化
            returnVal = InitializeTables(connection);
        } else {
            returnVal = 0;
        }
    }
    return returnVal;
}

int main() {
    MYSQL *connection; // mysql类型指针
    char mysqlUser[21]; // 数据库用户
    char mysqlPasswd[31]; // 数据库密码
    connection = mysql_init(NULL); // 初始化连接
    if (connection == NULL) {
        // 未能成功初始化连接
        printf("Failed to initialize the Connection.\n");
    }
    printf("Please enter the Username (Max Len:20) : ");
    fflush(stdout);
    scanf(" %20s", &mysqlUser); // 接受用户名输入（最多接受长度为20）
    system("stty -echo"); // 关闭Shell回显
    printf("Please enter the Password of User (Max Len:30) : ");
    fflush(stdout);
    scanf(" %30s", &mysqlPasswd); // 接受用户密码输入（最多接受长度为20）
    CLEAR_EOF;
    system("stty echo"); // 开启Shell回显
    if (mysql_real_connect(connection, DATABASE_HOST, mysqlUser, mysqlPasswd, DATABASE_NAME, 0, NULL, 0) !=
        NULL) { // 成功连接至数据库
        printf("\nSuccessfully connect to the Database: %s \n", DATABASE_NAME);
        fflush(stdout);
        mysql_set_character_set(connection, "utf8"); // 设置字符集编码UTF8
        if (TableCheck(connection)) {
            printf("Tables are fine!\n");
            Menu(connection); // 进入菜单
        }
    } else {
        printf("\nFailed to connect to the database: %s\n", mysql_error(connection));
    }
    mysql_close(connection);
    mysql_library_end(); // 避免发生内存泄漏，释放库使用的资源
    return 0;
}
