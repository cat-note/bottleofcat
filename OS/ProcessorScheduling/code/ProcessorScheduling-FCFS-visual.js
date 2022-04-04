'use strict';
// First Come First Serve(FCFS) 先来先服务调度算法（非抢占式）
let processes = [
    ['P2', 30, 10],
    ['P1', 20, 0],
    ['P3', 25, 20],
    ['P4', 10, 30],
    ['P5', 15, 40]
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
    for (let i = 0, len = processes.length; i < len; i++) { // 模拟进程按不同时间到达就绪队列
        let [name, spend, arrivalTime] = processes[i];
        if (arrivalTime <= globalTimer) {
            processes.splice(i, 1);
            illustrate(name, globalTimer, '|'); // 用竖线标记到达时间
            readyQueue.push({
                name: name, // 进程信息
                spend: spend, // 运行时间
                arrivalTime: arrivalTime, // 到达就绪队列时间
                finished: 0 // 进程已经执行的时间
            });
            i--;
            len--;
        }
    }
    if (processes.length === 0 && readyQueue.length === 0 && !running) {
        break;
    }
    if (!running) {
        running = readyQueue.shift();
        runningStart = globalTimer;
        illustrate(running.name, globalTimer, '#');
    } else {
        let { name, spend, finished, arrivalTime } = running;
        if (finished >= spend) { // 进程已经执行完毕
            running = null;
            finishTime[name] = [arrivalTime, globalTimer - arrivalTime]; // [程序到达时间,周转时间]
            continue; // 在当前进程完成的瞬间调度下一个进程，不要等globalTimer++
        } else {
            illustrate(name, globalTimer, '#');
        }
    }
    running.finished++;
    globalTimer++;
}

console.log('All Processes are Completed.');
for (let i in illustrations) {
    let [arrivalTime, turn] = finishTime[i];
    console.log(`${i} ${illustrations[i].join('')}    %c${turn}ms (Arrive at ${arrivalTime}ms)`, 'color:#585858');
}