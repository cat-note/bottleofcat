/* Leetcode 面试题1.08 https://leetcode-cn.com/problems/zero-matrix-lcci/
 * 零矩阵：编写一种算法，若M × N矩阵中某个元素为0，则将其所在的行与列清零。
 * 下面有两种写法，第一种是用一个矩阵记录所有零所在的位置，然后再遍历一遍矩阵将对应位置设为0；
 *      而第二种是记录出现0的行号和列号，最后将有0的行和列全设为0就行了！
 *  Written by SomeBottle 2022.4.25 11:55
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*void setZeroes(int** matrix, int matrixSize, int* matrixColSize){
    int i,j,k,l;
    int temp[matrixSize][*matrixColSize];
    memset(temp,0,sizeof(temp));
    for(i=0;i<matrixSize;i++){
        for(j=0;j<*matrixColSize;j++){
            if(matrix[i][j]==0){
                for(l=0;l<matrixSize;l++){
                    temp[l][j]=1;
                }
                for(k=0;k<*matrixColSize;k++){
                    temp[i][k]=1;
                }
            }
        }
    }

    for(i=0;i<matrixSize;i++){
        for(j=0;j<*matrixColSize;j++){
            if(temp[i][j]==1) matrix[i][j]=0;
        }
    }
}*/

void setZeroes(int **matrix, int matrixSize, int *matrixColSize) {
    int i, j;
    int rows[matrixSize];
    int cols[*matrixColSize];
    memset(rows, 0, sizeof(rows));
    memset(cols, 0, sizeof(cols));
    for (i = 0; i < matrixSize; i++) {
        for (j = 0; j < *matrixColSize; j++) {
            if (matrix[i][j] == 0) {
                rows[i] = 1;
                cols[j] = 1;
            }
        }
    }
    for (i = 0; i < matrixSize; i++) {
        if (rows[i] == 1) {
            for (j = 0; j < *matrixColSize; j++) {
                matrix[i][j] = 0;
            }
        }
    }
    for (j = 0; j < *matrixColSize; j++) {
        if (cols[j] == 1) {
            for (i = 0; i < matrixSize; i++) matrix[i][j] = 0;
        }
    }
}

void printMatrix(int **matrix, int n) { // 打印矩阵
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) printf("%2d ", matrix[i][j]);
        printf("\n");
    }
}

int main() {
    int n = 4, m = 5, i, j, test = 1;
    int **matrix = (int **) malloc(sizeof(int *) * n);
    for (i = 0; i < n; i++) {
        matrix[i] = (int *) malloc(sizeof(int) * m);
        for (j = 0; j < m; j++) {
            matrix[i][j] = test++;
        }
    }
    matrix[2][2] = 0;
    printf("\nMatrix before:\n");
    printMatrix(matrix, n);
    setZeroes(matrix, n, &m);
    printf("\nMatrix after:\n");
    printMatrix(matrix, n);
    // 释放二维数组占用
    for (i = 0; i < n; i++) free(matrix[i]);
    free(matrix);
    return 0;
}