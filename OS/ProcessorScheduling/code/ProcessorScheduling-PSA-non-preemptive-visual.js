'use strict';
// PSA,优先级调度算法（非抢占式）
let arr = [['A', 10, 3], ['B', 6, 5], ['C', 2, 2], ['D', 4, 1], ['E', 8, 4]], // [进程名,运行时间,优先级(数字越大越优先)]
    overall = 0;
arr.sort((x, y) => y[2] - x[2]);
for (let i = 0, len = arr.length; i < len; i++) {
    let item = arr[i],
        spend = item[1],
        sumBefore = 0;
    for (let j = 0; j < i; j++) {
        sumBefore += arr[j][1];
    }
    let illustration = ' '.repeat(sumBefore) + '#'.repeat(spend) + ` %c${spend}ms`;
    console.log(`${item[0]}: ${illustration}`, 'color:#848484');
    overall = overall + spend + sumBefore;
}
let average = overall / arr.length;
console.log(`Final result: ${average}`);