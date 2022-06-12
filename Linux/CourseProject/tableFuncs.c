/**
 * 涉及到的数据表处理的有关函数
 * By SomeBottle 20220610
 */
#include "main.h"
#include "tableFuncs.h"

static int CombineQuery(MYSQL *conn, char *before, char *middle, char *after);

/**
 * 操作记账类型
 * @param conn MYSQL指针
 * @param mode 模式，1代表删除分类，2代表新建分类，3代表编辑分类名
 * @param typeId
 * @param typeName
 * @return 1/0 代表是/否成功
 */
int OperateFlowTypes(MYSQL *conn, int mode, int typeId, char *typeName) {
    int returnVal = 1;
    char *deleteFormat = "DELETE FROM FlowTypes WHERE `tid`=%d";
    char *insertFormat = "INSERT INTO FlowTypes (tname) VALUES ('%s');";
    char *updateFormat = "UPDATE FlowTypes SET `tname`='%s' WHERE `tid`=%d;";
    // 转义名字字符串
    char *escapedName = (char *) calloc(strlen(typeName) * 2 + 1, sizeof(char));
    mysql_real_escape_string(conn, escapedName, typeName, strlen(typeName));
    char *queryStr = (char *) calloc(strlen(updateFormat) + strlen(escapedName) + 12, sizeof(char));
    switch (mode) {
        case 1:
            sprintf(queryStr, deleteFormat, typeId);
            break;
        case 2:
            sprintf(queryStr, insertFormat, escapedName);
            break;
        case 3:
            sprintf(queryStr, updateFormat, escapedName, typeId);
            break;
    }
    if (mysql_real_query(conn, queryStr, strlen(queryStr)) == 0) {
        // 请求成功
        FREE_RESULTS(conn);
    } else {
        returnVal = 0;
        printf("操作记账类型失败！Failed to operate on Flow Types!\n");
    }
    free(queryStr);
    free(escapedName);
    return returnVal;
}

/**
 * 获得记账分类信息
 * @param conn MYSQL指针
 * @return 一个FTResult结构体
 */
FTResult GetFlowTypes(MYSQL *conn) {
    FTResult result = {.tidArr=NULL, .listStr=NULL, .tidArrLen=0};
    // 查询记账分类
    QueryResult flowTypes = SelectQuery(conn, "SELECT * FROM FlowTypes ORDER BY `tid` ASC");
    if (flowTypes.result != NULL && flowTypes.rowNum > 0) {
        size_t i, totalLen = 0;
        char *formatPattern = "\t %s - %s\n";
        int *tidArr = (int *) calloc(flowTypes.rowNum, sizeof(int)); // 储存tid的数组
        for (i = 0; i < flowTypes.rowNum; i++)  // 打印类型
            // 写入typeList字符串
            totalLen += strlen(formatPattern) + strlen(flowTypes.result[i][0]) +
                        strlen(flowTypes.result[i][1]);// 储存记账分类列表字符串
        char *typeList = (char *) calloc(totalLen + 1, sizeof(char));
        char *typeListPtr = typeList; // 类型列表字符串指针
        for (i = 0; i < flowTypes.rowNum; i++) { // 写入字符串
            sprintf(typeListPtr, formatPattern, flowTypes.result[i][0], flowTypes.result[i][1]);
            typeListPtr += strlen(typeListPtr); // 后移指针到这一部分字符串的尾部
            tidArr[i] = (int) strtol(flowTypes.result[i][0], NULL, 10); // 储存typeid
        }
        result.listStr = typeList;
        result.tidArr = tidArr;
        result.tidArrLen = flowTypes.rowNum;
    }
    FreeQueryRes(&flowTypes); // 用完释放好习惯
    return result;
}

/**
 * 释放FTResult结构体中的堆内存
 * @param res 指向FTResult结构体的指针
 */
void FreeFTRes(FTResult *res) {
    if (res->tidArr != NULL)
        free(res->tidArr);
    if (res->listStr != NULL)
        free(res->listStr);
}

/**
 * 释放QueryResult结构体中的堆内存
 * @param res 指向QueyResult结构体的指针
 */
void FreeQueryRes(QueryResult *res) {
    size_t i, j;
    if (res->result != NULL) {
        for (i = 0; i < res->rowNum; i++) {
            for (j = 0; j < res->colNum; j++) {
                free(res->result[i][j]); // 释放字符串
            }
            free(res->result[i]); // 释放第二级
        }
        free(res->result); // 释放第三级
    }
    res->result = NULL;
    res->colNum = 0;
    res->rowNum = 0;
}

/**
 * 更新MoneyTypes的表项
 * @param conn MYSQL指针
 * @param type 类别，1代表balance余额，2代表budget初始预算
 * @param amount
 * @return 1/0 代表是/否成功
 */
int UpdateMoney(MYSQL *conn, int type, float amount) {
    int returnVal = 1;
    char *formatStr = NULL;
    switch (type) {
        case 1:
            formatStr = "UPDATE MoneyTypes SET `amount`=%.2f WHERE `mtype`='balance';";
            break;
        case 2:
            formatStr = "UPDATE MoneyTypes SET `amount`=%.2f WHERE `mtype`='budget';";
            break;
        default:
            return 0;
    }
    char *queryStr = (char *) calloc(strlen(formatStr) + 41, sizeof(char));
    sprintf(queryStr, formatStr, amount);
    if (mysql_real_query(conn, queryStr, strlen(queryStr)) != 0) {
        printf("Failed to Update table MoneyTypes: %s\n", mysql_error(conn));
        returnVal = 0;
        FREE_RESULTS(conn);
    }
    free(queryStr); // 用完释放好习惯
    return returnVal;
}

/**
 * 从FlowItems表中移除账目
 * @param conn MYSQL指针
 * @param id 要删除的账目的ID
 * @param balance 余额
 * @return 1/0 代表 是/否成功
 */
int DelFlowItem(MYSQL *conn, int id, float balance) {
    int returnVal = 1;
    size_t i;
    // 获得所有的ID
    QueryResult allIds = SelectQuery(conn, "SELECT `id` FROM FlowItems;");
    int *idArr = (int *) calloc(allIds.rowNum, sizeof(int));
    for (i = 0; i < allIds.rowNum; i++) {
        // 将ID转化为整型加入到数组中
        idArr[i] = (int) strtol(allIds.result[i][0], NULL, 10);
    }
    if (bsearch(&id, idArr, allIds.rowNum, sizeof(int), BinCmp) != NULL) {
        // 能找到对应ID
        char *formatStr = "WHERE `id`=%d;";
        char *selectStr = "SELECT `amount`,`direction` FROM FlowItems ";
        char *delStr = "DELETE FROM FlowItems ";
        char *whereStr = (char *) calloc(strlen(formatStr) + 12, sizeof(char));
        sprintf(whereStr, formatStr, id); // 构造WHERE条件查询
        size_t queryLen = strlen(selectStr) + strlen(whereStr);
        char *queryStr = (char *) calloc(queryLen + 2, sizeof(char));
        sprintf(queryStr, "%s%s", selectStr, whereStr);
        // 先查询待删除账目的金额
        QueryResult getAmount = SelectQuery(conn, queryStr);
        if (getAmount.result != NULL) {
            float accountAmount = strtof(getAmount.result[0][0], NULL);
            int direction = (int) strtol(getAmount.result[0][1], NULL, 10);
            sprintf(queryStr, "%s%s", delStr, whereStr);
            if (mysql_real_query(conn, queryStr, strlen(queryStr)) == 0) {
                // 删除成功
                switch (direction) {
                    case 1: // 删除了一个支出项
                        balance += accountAmount; // 加回去
                        break;
                    case 0:
                        balance -= accountAmount; // 减回去
                        break;
                    default:
                        printf("ERROR: Unknown flowDirection appears!\n");
                        break;
                }
                if (UpdateMoney(conn, 1, balance)) {
                    printf("删除成功 DELETE Successfully!");
                } else {
                    printf("\t余额更新失败！ Failed to update Balance.\n");
                    returnVal = 0;
                }
                FREE_RESULTS(conn);
            } else {
                // 删除失败
                printf("删除失败 Failed to DELETE: %s\n", mysql_error(conn));
                returnVal = 0;
            }
        } else {
            printf("删除失败，无法获得账目金额 Failed because Amount corresponding to the Account is not available.\n");
            returnVal = 0;
        }
        FreeQueryRes(&getAmount);
        free(queryStr);
        free(whereStr); // 用完后记得释放
    } else {
        printf("找不到对应ID的账目 Can't find the account corresponding to the ID\n");
        returnVal = 0;
    }
    FreeQueryRes(&allIds);
    free(idArr); // 用完释放好习惯
    return returnVal;
}

/**
 * 向FlowItems表中插入新账目
 * @param conn MYSQL指针
 * @param direction 1代表支出，0代表收入
 * @param amount 数额
 * @param type 支出分类
 * @param note 备注
 * @return 1/0 代表 是/否插入成功
 */
int InsertFlowItem(MYSQL *conn, int direction, float amount, int type, char *note) {
    int returnVal = 0;
    char const *formatBefore = "INSERT INTO FlowItems (amount,note,tid,direction) "
                               "VALUES (%.2f,'";
    char const *formatAfter = "',%d,%d);";
    char *endPtr; // 指向查询字符串末尾的指针
    size_t queryLen = 10 * 3 + strlen(formatBefore) + strlen(note) * 2 + 2; // 查询字符串的长度
    char *queryString = (char *) calloc(queryLen, sizeof(char));
    sprintf(queryString, formatBefore, amount);
    endPtr = queryString + strlen(queryString); // 指向queryString末尾
    // 转义后将指针指向转义后字符串的末尾
    endPtr += mysql_real_escape_string(conn, endPtr, note, strlen(note));
    sprintf(endPtr, formatAfter, type, direction); // 打印剩余的查询字符串
    if (mysql_real_query(conn, queryString, strlen(queryString)) == 0) {
        // 插入成功
        returnVal = 1;
        FREE_RESULTS(conn);
    } else {
        printf("Failed to make account: %s\n", mysql_error(conn));
    }
    free(queryString); // 用完就释放
    return returnVal;
}

/**
 * 将SELECT选择的部分解析为二维字符串数组
 * @param conn MYSQL指针
 * @param queryString 存放查询语句的字符串
 * @return QueryResult结构体
 * @note 返回内容有部分存在堆中，记得free
 */
QueryResult SelectQuery(MYSQL *conn, char *queryString) {
    QueryResult returnStruct = {.result=NULL};
    if (mysql_real_query(conn, queryString, strlen(queryString)) == 0) {
        // 请求成功
        MYSQL_RES *resource;
        if ((resource = mysql_store_result(conn)) != NULL) {
            // 成功抓取结果
            size_t i = 0, j, totalLen = 0;
            MYSQL_ROW row;
            size_t rowNum = mysql_num_rows(resource); // 获得行数量
            size_t colNum = mysql_num_fields(resource); // 获得列数量
            // 初始化返回
            char ***result = (char ***) calloc(rowNum, sizeof(char **));
            while ((row = mysql_fetch_row(resource))) {
                result[i] = (char **) calloc(colNum, sizeof(char *));
                for (j = 0; j < colNum; j++) { // 横向遍历当前行
                    size_t currentLen = strlen(row[j]); // 获得当前字符串的长度
                    totalLen += currentLen; // 计算总长度
                    result[i][j] = (char *) calloc(currentLen + 1, sizeof(char));
                    strcpy(result[i][j], row[j]); // 拷贝字符串到返回数组中
                }
                i++;
            }
            returnStruct.rowNum = i;
            returnStruct.colNum = colNum;
            returnStruct.result = result;
            // 清理工作
            mysql_free_result(resource);
            FREE_RESULTS(conn);
        } else {
            printf("Failed to store SELECT Result: %s\n", mysql_error(conn));
        }
    } else {
        printf("Failed to perform SELECT Query: %s\n", mysql_error(conn));
    }
    return returnStruct;
}

/**
 * 将query字符串组合起来作为MySQL语句执行
 * @param conn MYSQL指针
 * @param before 前部字符串
 * @param middle 中间的字符串
 * @param after 后部的字符串（可以为NULL）
 * @return 1/0 代表 是/否执行成功
 */
int CombineQuery(MYSQL *conn, char *before, char *middle, char *after) {
    size_t beforeLen, middleLen, afterLen = 0;
    beforeLen = strlen(before);
    middleLen = strlen(middle);
    if (after != NULL)
        afterLen = strlen(after);
    char *queryString = calloc(beforeLen + middleLen + afterLen + 3, sizeof(char));
    strcat(queryString, before);
    strcat(queryString, middle);
    if (after != NULL)
        strcat(queryString, after);
    int query = mysql_real_query(conn, queryString, strlen(queryString));
    free(queryString); // 使用后释放
    FREE_RESULTS(conn);
    return !query; // 返回查询是否成功（这里要逆转一下逻辑，查询时为0就代表成功，1就代表失败）
}

/**
 * 判断表是否存在于数据库中
 * @param conn MYSQL指针
 * @param tableName 表名
 * @return 1/0 代表该表 是/否 存在
 */
int TableExist(MYSQL *conn, char *tableName) {
    int returnVal = 1;
    char *formatStr = "SHOW TABLES LIKE '%s';";
    char *queryStr = (char *) calloc(strlen(formatStr) + strlen(tableName) + 1, sizeof(char));
    sprintf(queryStr, formatStr, tableName);
    QueryResult testing = SelectQuery(conn, queryStr);
    if (testing.rowNum <= 0)
        returnVal = 0;
    free(queryStr);
    FreeQueryRes(&testing); // 用完后释放
    return returnVal;
}

/**
 * 删除表
 * @param conn MYSQL指针
 * @param tableName 表名
 * @return 1/0 代表 是/否删除成功
 */
int DropTable(MYSQL *conn, char *tableName) {
    return CombineQuery(conn, "DROP TABLE ", tableName, NULL);
}

/**
 * 初始化该有的表项
 * @param conn MYSQL指针
 * @return 1/0 代表 是/否成功
 */
int InitializeTables(MYSQL *conn) {
    int success = 0; // 记录是否成功
    // 创建资金流动项目表
    char *const createFlowItems = "CREATE TABLE FlowItems("
                                  "    `id` INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,"
                                  "    `amount` FLOAT NOT NULL,"
                                  "    `note` TEXT,"
                                  "    `tid` SMALLINT UNSIGNED NOT NULL,"
                                  "    `time` DATETIME NOT NULL DEFAULT NOW(),"
                                  "    `direction` TINYINT NOT NULL,"
                                  "    FOREIGN KEY (tid) REFERENCES FlowTypes(tid)" // 外键
                                  "    ON DELETE CASCADE" // 级联删除
                                  ") AUTO_INCREMENT 0;";
    // 创建资金流动类型表
    char *const createFlowTypes = "CREATE TABLE FlowTypes("
                                  "    `tid` SMALLINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,"
                                  "    `tname` CHAR(20) NOT NULL"
                                  ") AUTO_INCREMENT 1;";
    // 初始化资金流动类型
    char *const insertFlowTypes = "INSERT INTO FlowTypes VALUES (1,'Default');";
    // 创建金额记录表
    char *const createMoneyTypes = "CREATE TABLE MoneyTypes("
                                   "   `mtype` CHAR(15) NOT NULL,"
                                   "   `amount` FLOAT NOT NULL"
                                   ");";
    // 初始化金额记录表
    char *const insertMoneyTypes = "INSERT INTO MoneyTypes VALUES ('balance',0), ('budget',0);";
    // 在初始化前先drop一遍，防止判断失误
    DropTable(conn, "FlowItems");
    DropTable(conn, "FlowTypes");
    DropTable(conn, "Users");
    DropTable(conn, "MoneyTypes");
    // 这里因为0代表成功，1代表失败，因此用逻辑OR连接，只要有一个失败了就是1
    success = mysql_real_query(conn, createFlowTypes, strlen(createFlowTypes)) ||
              mysql_real_query(conn, insertFlowTypes, strlen(insertFlowTypes)) ||
              mysql_real_query(conn, createFlowItems, strlen(createFlowItems)) ||
              mysql_real_query(conn, createMoneyTypes, strlen(createMoneyTypes)) ||
              mysql_real_query(conn, insertMoneyTypes, strlen(insertMoneyTypes));
    if (success == 0) {
        printf("Successfully initialized!\n");
        FREE_RESULTS(conn);
    } else {
        printf("Failed to Initialize tables: %s\n", mysql_error(conn));
    }
    return !success; // 同样是来一次非运算
}