/*
 * Leetcode-27 移除元素 https://leetcode-cn.com/problems/remove-element/submissions/
 * 尝试了一下所谓的双指针，可谓是非常巧妙了
 * p1是用于遍历整个nums数组的指针，p2则是p1的跟班“小弟”，p2和p1相差1
 * 当p1指向了要删除的元素时，p2指向其前一个元素，而当p1越过要删除的元素后，再把p1指向的元素赋给p2的位置
 * [1, 2, 2, 4, 5, 2, 6]
 *  ↑  ↑
 *  p2 p1
 *
 * [1, 2, 2, 4, 5, 2, 6]
 *     ↑  ↑
 *     p2 p1
 *
 * [1, 4, 2, 4, 5, 2, 6]
 *     ↑     ↑
 *     p2   p1
 * [1, 4, 5, 4, 5, 2, 6]
 *        ↑     ↑
 *        p2    p1
 *
 *  可以说是非常巧妙了
 *  SomeBottle 2022.4.29 14:00
 */

#include <stdio.h>

int removeElement(int *nums, int numsSize, int val) {
    int p1, p2 = 0;
    for (p1 = 0; p1 < numsSize; p1++) { // 遍历一遍数组
        if (nums[p1] != val) { // 双指针法
            nums[p2++] = nums[p1];
        }
    }
    return p2; // 返回新数组长度
}

int main() {
    int i;
    int testArr[] = {1, 2, 3, 4, 5, 2, 6};
    int arrSize = sizeof(testArr) / sizeof(int);
    printf("Prev Arr: \n[");
    for (i = 0; i < arrSize; i++)
        printf("%d, ", testArr[i]);
    printf("]\nArr after:\n[");
    int newSize = removeElement(testArr, arrSize, 2);
    for (i = 0; i < newSize; i++)
        printf("%d, ", testArr[i]);
    printf("]");
    return 0;
}