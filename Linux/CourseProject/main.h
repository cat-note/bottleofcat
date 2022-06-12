/**
 * 主头文件
 */

#ifndef ACCOUNTBOOK_MAIN_H
#define ACCOUNTBOOK_MAIN_H

#include <stdio.h>
#include <mysql/mysql.h>
#include <string.h>
#include <stdlib.h>

#define DATABASE_NAME "account_data"
#define DATABASE_HOST "localhost"

#define CLEAR_EOF while (getchar() != '\n') // 利用getchar取走多余空格
#define FREE_RESULTS(conn) do { \
        mysql_free_result(mysql_use_result(conn)); \
    }while(!mysql_next_result(conn)) // 清理查询残余
#define CLEAR system("clear") // 清屏
#define PAUSE fflush(stdout);CLEAR_EOF // 暂停

extern int BinCmp(const void *a, const void *b);

#endif //ACCOUNTBOOK_MAIN_H
