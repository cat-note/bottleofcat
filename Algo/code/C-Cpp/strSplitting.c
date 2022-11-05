/* 测试按字符分隔字符串，返回一个二维数组 - SomeBottle 20220424 23:13*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { // 分隔字符串返回结果
    char **split;
    int len;
} SplitResult;

SplitResult SplitByChr(char *str, int strLen, char chr) { // (字符串,字符) 按字符分隔字符串，会返回一个二维数组
    int i;
    int bufferSize = sizeof(char) * strLen;
    char *buffer = (char *) malloc(bufferSize); // 字符串暂存区
    int bufferLen = 0; // 暂存区字符数组长度
    char **arr = (char **) malloc(sizeof(char *) * strLen); // 数组第一维
    int arrLen = 0; // 返回二维数组第一维的大小
    for (i = 0; i < strLen + 1; i++) {
        int currentChr;
        int lastOne = 0; // 最后一项单独处理
        if (i < strLen) {
            currentChr = str[i];
        } else {
            lastOne = 1;
        }
        if (lastOne || currentChr == chr) {
            arr[arrLen] = (char *) malloc(sizeof(char) * (bufferLen + 1)); // 初始化第二维数组
            strncpy(arr[arrLen], buffer, bufferLen); // 将字符装入第二维数组
            arr[arrLen][bufferLen] = '\0';
            memset(buffer, 0, bufferSize); // 清空字符串暂存区
            bufferLen = 0; // 暂存区长度归零
            arrLen++;
        } else {
            buffer[bufferLen++] = currentChr; // 存入字符串暂存区
        }
    }
    free(buffer); // 释放暂存区
    SplitResult result = {
            arr, // 一定要记得用完释放！！！
            arrLen
    };
    return result; // 返回结果
}

int freeSplitArr(SplitResult *rs) { // 门当对户地释放SplitByChr的返回结果中的字符二维数组
    int i;
    for (i = 0; i < rs->len; i++) {
        free(rs->split[i]);
    }
    free(rs->split);
    rs->split = NULL;
    return 1;
}

int main() {
    char testStr[] = "Hello,This,is, a ,test,string";
    int i;
    int strLen = sizeof(testStr) / sizeof(testStr[0]);
    SplitResult test = SplitByChr(testStr, strLen, ',');
    for (i = 0; i < test.len; i++) {
        printf("Result[%d]=%s\n", i, test.split[i]);
    }
    freeSplitArr(&test);
    return 0;
}