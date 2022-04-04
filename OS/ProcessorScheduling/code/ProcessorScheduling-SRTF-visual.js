'use strict';
// Shortest Remaining Time First (SRTF) Algorithm 最短剩余时间优先调度算法（抢占式）
let processes = [
    ['P1', 10, 0],
    ['P2', 1, 1],
    ['P3', 5, 2],
    ['P4', 1, 3],
    ['P5', 2, 4]
], // [进程名,运行时间,到达输入井时间(从0ms开始)]
    readyQueue = [], // 就绪队列
    running = null, // 运行中的进程
    runningStart = 0, // 运行中的进程开始时间
    globalTimer = 0, // 全局时间
    finishTime = {}, // 进程完成时间
    illustrations = {}; // 直观化输出
function illustrate(name, pos, chr) { // 制作直观化输出
    if (!illustrations[name]) illustrations[name] = [];
    let arr = illustrations[name];
    for (let i = 0; i < pos; i++) { // 填充数组中间的空部分
        if (!arr[i]) arr[i] = '-';
    }
    arr[pos] = chr;
}
while (true) {
    let schFlag = false; // 抢占调度标志
    for (let i = 0, len = processes.length; i < len; i++) { // 模拟进程按不同时间到达就绪队列
        let [name, spend, arrivalTime] = processes[i];
        if (arrivalTime <= globalTimer) {
            let currentRemaingTime = running ? running.spend - running.finished : 0; // 获得当前进程的剩余时间
            processes.splice(i, 1);
            illustrate(name, globalTimer, '|'); // 用竖线标记到达时间
            readyQueue.push({
                name: name, // 进程信息
                spend: spend, // 运行时间
                arrivalTime: arrivalTime, // 到达就绪队列时间
                finished: 0 // 进程已经执行的时间
            });
            if (spend <= currentRemaingTime) { // 如果新加入就绪队列的进程剩余时间小于当前进程的剩余时间，抢占当前进程
                schFlag = true;
            }
            i--;
            len--;
        }
    }
    if (processes.length === 0 && readyQueue.length === 0 && !running) {
        break;
    }
    // -----------以下部分模拟的是调度程序
    if (!running) {
        // 按最短剩余时间排序从小到大排序
        readyQueue.sort((a, b) => (a.spend - a.finished) - (b.spend - b.finished));
        running = readyQueue.shift();
        runningStart = globalTimer;
        illustrate(running.name, globalTimer, '#');
    } else {
        let { name, spend, finished, arrivalTime } = running;
        if (finished >= spend) { // 进程已经执行完毕
            running = null;
            finishTime[name] = [arrivalTime, globalTimer - arrivalTime]; // [程序到达时间,周转时间]
            continue; // 在当前进程完成的瞬间调度下一个进程，不要等globalTimer++
        } else if (schFlag) { // 就绪队列加入了剩余时间更短的进程，被抢占
            readyQueue.push(running);
            running = null;
            continue; // 在当前进程移走的瞬间调度下一个进程，不要等globalTimer++
        } else {
            illustrate(name, globalTimer, '#');
        }
    }
    running.finished++;
    globalTimer++;
}

console.log('All Processes are Completed.');
let sum = 0,
    processNum = 0;
for (let i in illustrations) {
    let [arrivalTime, turn] = finishTime[i];
    sum += turn;
    processNum++;
    console.log(`${i} ${illustrations[i].join('')}    %c${turn}ms (Arrive at ${arrivalTime}ms)`, 'color:#585858');
}
let average = sum / processNum;
console.log(`Average Turnaround Time: ${average}ms`);