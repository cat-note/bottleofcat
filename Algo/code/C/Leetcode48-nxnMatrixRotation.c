/* 顺时针旋转N*N的二维数组矩阵 Written by SomeBottle
 * 思路是先沿着从左上到右下对角线翻转元素，再将整个矩阵进行左右翻转
 * Leetcode.48
 * 2022.4.25 10:41
*/
#include <stdio.h>
#include <stdlib.h>

void rotate(int **matrix, int matrixSize) {
    int i, j, temp;
    for (i = 0; i < matrixSize; i++) {
        for (j = 0; j < i; j++) { // 沿着从左上到右下对角线翻转
            temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
    int tempPos;
    for (i = 0; i < matrixSize; i++) { // 再进行左右翻转
        for (j = 0; j < matrixSize / 2; j++) {
            tempPos = matrixSize - j - 1;
            temp = matrix[i][j];
            matrix[i][j] = matrix[i][tempPos];
            matrix[i][tempPos] = temp;
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
    int n = 4, i, j, test = 1;
    int **matrix = (int **) malloc(sizeof(int *) * n);
    for (i = 0; i < n; i++) {
        matrix[i] = (int *) malloc(sizeof(int) * n);
        for (j = 0; j < n; j++) {
            matrix[i][j] = test++;
        }
    }
    printf("\nMatrix before:\n");
    printMatrix(matrix, n);
    rotate(matrix, n);
    printf("\nMatrix after:\n");
    printMatrix(matrix, n);
    // 释放二维数组占用
    for (i = 0; i < n; i++) free(matrix[i]);
    free(matrix);
    return 0;
}