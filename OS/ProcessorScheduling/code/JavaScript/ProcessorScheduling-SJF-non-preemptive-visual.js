// 这个会在控制台打印一个可视的SJF进程调度图（非抢占式）
let arr = [['A', 10], ['B', 6], ['C', 2], ['D', 4], ['E', 8]],
    overall = 0;
arr.sort((x, y) => x[1] - y[1]);
for (let i = 0, len = arr.length; i < len; i++) {
    let item = arr[i],
        spend = item[1],
        sumBefore = 0;
    for (let j = 0; j < i; j++) {
        sumBefore += arr[j][1];
    }
    let illustration = ' '.repeat(sumBefore) + '#'.repeat(spend);
    console.log(`${item[0]}: ${illustration}`);
    overall = overall + spend + sumBefore;
}
let average = overall / arr.length;
console.log(`Final result: ${average}`);