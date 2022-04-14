'use strict';
// Preemptive Priority Scheduling (PSA Preemptive) Algorithm 抢占式优先级调度算法
let smallerValHigherPriority = true, // 小值优先级高(为false就是大值优先级高)
    processes = [
        ['A', 10, 0, 5],
        ['B', 1, 1, 1],
        ['C', 5, 2, 3],
        ['D', 1, 3, 2],
        ['E', 2, 4, 4]
    ], // [进程名,运行时间,到达输入井时间(从0ms开始),指定的优先级]
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
        let [name, spend, arrivalTime, priority] = processes[i];
        if (arrivalTime <= globalTimer) {
            processes.splice(i, 1);
            illustrate(name, globalTimer, '|'); // 用竖线标记到达时间
            readyQueue.push({
                name: name, // 进程信息
                spend: spend, // 运行时间
                priority: priority, // 优先级
                arrivalTime: arrivalTime, // 到达就绪队列时间
                finished: 0 // 进程已经执行的时间
            });
            if (running && (smallerValHigherPriority ? priority < running.priority : priority > running.priority)) { // 如果当前运行的进程优先级比新到达的进程优先级低
                schFlag = true; // 标记为抢占调度
            }
            i--;
            len--;
        }
    }
    if (processes.length === 0 && readyQueue.length === 0 && !running) {
        break;
    }
    if (!running) {
        // 优先级排序
        readyQueue.sort((a, b) => {
            return smallerValHigherPriority ? a.priority - b.priority : b.priority - a.priority;
        });
        running = readyQueue.shift();
        runningStart = globalTimer;
        illustrate(running.name, globalTimer, '#');
    } else {
        let { name, spend, finished, arrivalTime } = running;
        if (finished >= spend) { // 进程已经执行完毕
            running = null;
            finishTime[name] = [arrivalTime, globalTimer - arrivalTime]; // [程序到达时间,周转时间]
            continue; // 在当前进程完成的瞬间调度下一个进程，不要等globalTimer++
        } else if (schFlag) { // 时间片轮转
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