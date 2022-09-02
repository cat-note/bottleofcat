/* 二分查找/折半查找 Written by SomeBottle
 * Leetcode.704
 * 2022.4.15 23:19
*/
#include <stdio.h>

int search(int *nums, int numsSize, int target) {
    int left = 0, right = numsSize - 1, middle;
    while (left <= right) {
        middle = (left + right) / 2;
        if (nums[middle] == target) {
            return middle;
        } else if (target > nums[middle]) {
            left = middle + 1;
        } else if (target < nums[middle]) {
            right = middle - 1;
        }
    }
    return -1;
}

int main() {
    int testArr[] = {1, 3, 8, 10, 37, 48};
    int arrLen = sizeof(testArr) / sizeof(int);
    int result = search(testArr, arrLen, 37);
    printf("Search result: %d\n", result);
    return 0;
}