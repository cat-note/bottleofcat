/* 二分查找/折半查找 Written by SomeBottle
 * Leetcode.35
 * 2022.4.15 13:34
*/
#include <stdio.h>

int searchInsert(int *nums, int numsSize, int target) {
    int left = 0;
    int right = numsSize - 1;
    while (left <= right) {
        int middle = (left + right) / 2;
        if (nums[middle] == target) {
            return middle;
        } else if (target > nums[middle]) {
            left = middle + 1;
        } else if (target < nums[middle]) {
            right = middle - 1;
        }
    }
    return left;
}

int main() {
    int testArr[] = {1, 3, 8, 10, 37, 48};
    int arrLen = sizeof(testArr) / sizeof(int);
    int result = searchInsert(testArr, arrLen, 9);
    printf("Search result: %d\n", result);
    return 0;
}