//
// Created by 58379 on 2022/6/10.
//

#ifndef ACCOUNTBOOK_TABLEFUNCS_H
#define ACCOUNTBOOK_TABLEFUNCS_H

/** 查询返回结果结构体*/
typedef struct {
    char ***result;
    size_t rowNum; // 行数
    size_t colNum; // 列数
} QueryResult;

typedef struct {
    char *listStr; // 用于打印分类列表的字符串
    int *tidArr; // 分类tid数组
    size_t tidArrLen; // tids数组元素个数
} FTResult;

extern int TableExist(MYSQL *conn, char *tableName);

extern int InitializeTables(MYSQL *conn);

extern QueryResult SelectQuery(MYSQL *conn, char *queryString);

extern void FreeQueryRes(QueryResult *res);

extern int InsertFlowItem(MYSQL *conn, int direction, float amount, int type, char *note);

extern int DelFlowItem(MYSQL *conn, int id, float balance);

extern int UpdateMoney(MYSQL *conn, int type, float amount);

extern FTResult GetFlowTypes(MYSQL *conn);

extern void FreeFTRes(FTResult *res);

extern int OperateFlowTypes(MYSQL *conn, int mode, int typeId, char *typeName);

#endif //ACCOUNTBOOK_TABLEFUNCS_H
