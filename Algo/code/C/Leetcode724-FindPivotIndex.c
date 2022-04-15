/* 寻找数组的中心索引 Written by SomeBottle
 * 这道题一定要注意审题，说的是找中间元素，但要求是中间元素的**左边元素的和**与**右边元素的和**相等！
 * 所以思路就出来了，只用算左边部分， 左边部分×2+中间元素 的总和是整个数组的元素和
 * Leetcode.724
 * 2022.4.15 19:04
*/
#include <stdio.h>

int pivotIndex(int *nums, int numsSize) {
    int i, total = 0, sum = 0;
    for (i = 0; i < numsSize; i++) {
        total += nums[i];
    }
    for (i = 0; i < numsSize; i++) {
        if (sum * 2 + nums[i] == total) return i;
        sum += nums[i];
    }
    return -1;
}

int main() {
    int testArr[] = {1, 7, 3, 6, 5, 6};
    int arrLen = sizeof(testArr) / sizeof(int);
    int result = pivotIndex(testArr, arrLen);
    printf("Search result: %d\n", result);
    return 0;
}