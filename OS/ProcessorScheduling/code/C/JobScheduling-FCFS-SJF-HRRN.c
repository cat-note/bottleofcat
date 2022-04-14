/*
 Written by SomeBottle 2022.4.13-2022.4.14
 操作系统-实验一: 处理机调度
 选题: 作业调度
 因为C语言才学到半路，还没熟悉，所以代码可能显得有点屎山
 这大概是我写的第一个有点意义的C语言程序了，颇具“里程碑性”。
 这一次我大致了解了结构体struct,数组指针,calloc内存分配的基本用法，同时写顺手了二分查找和选择排序算法。

 本次的惨痛教训: 在对数组进行排序时一定要考虑更改原数组内容造成的影响！这回很不巧触发了越界SIGSEGV异常，从下午两点找到四点才发现问题...浪不得啊

 务必使用支持C99以上的编译器进行编译，C11当然最好！
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct JCB { // 声明JCB结构体，用于存放作业信息
    char name; // 作业名
    int arrivalTime; // 到达时间
    int serviceSpend; // 服务时间（执行耗时）
    int startTime; // 开始时间
    int finishTime; // 完成时间
    int TATime; // 周转时间
    float TAWTime; // 带权周转时间
    int alreadyFinished; // 已经完成了多少（仅用于HRRN和SJF）
    int index; // 在后备队列中的下标（仅用于HRRN和SJF）
    char illustration[1000]; // 时间线图表
};

struct AlgoResult { // 声明一个算法函数返回结果用的结构体
    float averageTA; // 平均周转时间
    float averageTAW; // 平均带权周转时间
};

struct JCB jcbs[] = {
        {
                'A', 10, 30
        },
        {
                'B', 0,  20
        },
        {
                'C', 20, 25
        },
        {
                'D', 30, 10
        },
        {
                'E', 40, 15
        },
        {
                'F', 5,  8
        }
}; // 构造后备队列来存放作业，这里用一个结构体数组实现。


int totalJobNum = sizeof(jcbs) / sizeof(jcbs[0]); // 一共有多少作业

int *selectSort(int *arr, int arrLen) { // 选择排序算法（仅用于数组下标排序！）
    int i, j;
    int *result = (int *) calloc(arrLen, sizeof(*arr));
    // 创建一个拷贝用数组的内存空间，记得用完后free！
    for (i = 0; i < arrLen; i++) { // 浅拷贝
        result[i] = arr[i];
    }
    for (i = 0; i < arrLen; i++) {
        int minIndex = i;
        for (j = i + 1; j < arrLen; j++) {
            if (result[minIndex] > result[j]) minIndex = j;
        }
        if (minIndex != i) {
            int temp = result[i];
            result[i] = result[minIndex];
            result[minIndex] = temp;
        }
    }
    return result;
}

int BinarySearch(int *arr, int arrLen, int target) { // 二分查找数组元素（不存在返回-1）
    int left = 0, // 左边下标
    right = arrLen - 1, // 右边下标
    result = -1; // 结果
    int *sorted = selectSort(arr, arrLen); // 对数组进行排序，便于二分查找
    while (left <= right) {
        int middle = (right + left) / 2, // 找到中间元素下标，int类型自动抹掉小数
        midVal = sorted[middle];
        if (midVal == target) {
            result = middle; // 某一次查找中间元素正好是要找的
            break;
        } else if (midVal > target) { // 中间值大于目标，说明目标在左侧，往左移动右边下标
            right = middle - 1; // 中间项已经检查过，跳过！
        } else if (midVal < target) { // 中间值小于目标，说明目标在右侧，往右移动左边下标
            left = middle + 1; // 中间项已经检查过，跳过！
        }
    }
    free(sorted); // 释放分配的内存
    return result;
}

int PopJob(struct JCB *arr, int JobsLen, int pos) { // 从JCB结构体数组中移除一个作业
    for (int i = pos; i < JobsLen - 1; i++) {
        arr[i] = arr[i + 1];
    }
    return 0;
}

int PutIlluChar(char *arr, int pos, int chr, int from) { // (数组地址,下标,字符,从哪开始填充-）向数组放入字符，自动填充之前的部分
    for (int i = from; i < pos; i++) {
        if (arr[i] == 0) { // 未初始化的也要填充一下
            arr[i] = '-';
        }
    }
    arr[pos] = chr;
    return 0;
}

struct AlgoResult FCFS() {
    // First-Come-First-Serve 先来先服务算法（非抢占式）
    int i, j; // 预先定义变量
    int globalTimer = 0; // 全局耗时
    float totalTA, // 全局周转时间
    totalTAW; // 全局带权周转时间
    struct JCB jcbTemp[totalJobNum]; // C89标准对数组定义不能用变量长度，C99开始可以
    for (i = 0; i < totalJobNum; i++) // 浅拷贝JCBs一份，这里浅拷贝就够用了，只是用于排序
        jcbTemp[i] = jcbs[i];
    // 这里采用选择排序算法
    for (i = 0; i < totalJobNum; i++) {
        int minIndex = i; // 从i下标开始，与后面对比看有没有更小的
        for (j = i + 1; j < totalJobNum; j++) { // 从i+1开始检查
            if (jcbTemp[minIndex].arrivalTime > jcbTemp[j].arrivalTime) { // FCFS
                // 根据到达的先后时间对作业在数组中进行排序（从到达时间小到大）
                minIndex = j; // 出现了更小的！
            }
        }
        if (minIndex != i) { // 在i后面找到了更小的（这样写是为了防止平白无故增加交换次数）
            struct JCB temp = jcbTemp[i]; // 临时交换变量temp
            jcbTemp[i] = jcbTemp[minIndex]; // 将更小的交换到前面来
            jcbTemp[minIndex] = temp; // 把较大的交换到后面去
        }
    }
    // 接下来用for循环计算FCFS的作业服务状况
    printf("------------------------------\nFCFS Job Scheduling: \n\tName\tArrival\tSpend\tStart\tEnd\tTA\tTAW\n");
    for (i = 0; i < totalJobNum; i++) {
        struct JCB currentJob = jcbTemp[i]; // 当前正在服务的Job
        char illust[1000] = {}; // 当前图表
        int jobArrival = currentJob.arrivalTime,
                serviceSpend = currentJob.serviceSpend,
                jobStart = 0; // 作业开始的flag
        float turnAround,
                turnAroundW;
        PutIlluChar(illust, jobArrival, '|', 0); // 用 | 标记作业到达时间
        // 如果上一个作业执行完了后下一个作业还没到达，就把globalTimer指向arrivalTime
        if (globalTimer < jobArrival) globalTimer = jobArrival;
        currentJob.startTime = globalTimer; // 记录开始服务的时间
        globalTimer += serviceSpend; // globalTimer加上服务时间，也就是作业执行结束的时候
        currentJob.finishTime = globalTimer; // 标记作业完成时间
        for (j = currentJob.startTime; j <= globalTimer; j++) {
            int illustrateFrom = j;
            if (jobStart == 0) {
                jobStart = 1;
                illustrateFrom = jobArrival; // 作业刚开始，填充从到达到开始处的时间线
            }
            PutIlluChar(illust, j, '#', illustrateFrom);
            // 作业服务中标记为#号.这里有个fill参数=0，jobStart防止做重复绘制，因为非抢占式作业一旦开始执行一般不会停下来
        }
        turnAround = globalTimer - jobArrival; // 从到达到完成的耗时即为周转时间
        turnAroundW = (float) turnAround / serviceSpend; // 带权周转时间
        currentJob.TATime = turnAround;
        currentJob.TAWTime = turnAroundW; // 带权周转时间=周转时间/作业执行总时间（为了精确结果，这里转换为浮点）
        strcpy(jcbTemp[i].illustration, illust); // 将新图表部分推入illustration
        printf("\t%-4c\t%-5d\t%-6d\t%-7d\t%-6d\t%-2d\t%-4.3f\n", currentJob.name, currentJob.arrivalTime,
               currentJob.serviceSpend, currentJob.startTime, currentJob.finishTime, currentJob.TATime,
               currentJob.TAWTime);
        totalTA += turnAround;
        totalTAW += turnAroundW;
        globalTimer += 1; // 下一次调度从下一ms开始
    }
    printf("\n\t~~~~~~~~~~~~~~~~TimeLine~~~~~~~~~~~~~~~~\n\n");
    for (i = 0; i < totalJobNum; i++) { // 打印时间线
        char name = jcbTemp[i].name;
        char *illustration = jcbTemp[i].illustration; // 获得时间线
        printf("\t%c: ", name);
        printf("%s", illustration);
        printf("\n");
    }
    printf("\n");
    float averageTA = (float) totalTA / totalJobNum; // 算出平均周转时间
    float averageTAW = totalTAW / totalJobNum; // 算出平均带权周转时间
    struct AlgoResult result = {
            averageTA,
            averageTAW
    };
    return result;
};

struct AlgoResult SjfAndHrrn(int hrrn) {
    // 短作业优先（非抢占式）以及高响应比优先调度算法（非抢占式），这样写防止代码冗余
    int i, j; // 预先定义临时变量
    int runJobs[totalJobNum],// 运行了的作业下标数组，运行过的作业不再在循环中考虑
    runJobsInd = 0, // 已运行作业下标数组的尾部下标（用于push）
    readyQueueInd = 0;// 就绪队列数组的尾部下标（用于push） memset(illustrations, 0, sizeof(illustrations)); // （先初始化所有元素为0）
    for (i = 0; i < totalJobNum; i++) runJobs[i] = -1; // 初始化runJobs所有元素为-1
    int globalTimer = 0; // 全局耗时
    struct JCB jcbTemp[totalJobNum]; // 创建一个作业副本
    for (i = 0; i < totalJobNum; i++) {// 浅拷贝JCBs一份
        jcbTemp[i] = jcbs[i];
        jcbTemp[i].index = i;
    }
    struct JCB runningJob = {}; // 正在执行的作业（初始化所有内存为0）
    struct JCB readyQueue[totalJobNum]; // 就绪队列
    while (1) { // 主执行循环
        for (i = 0; i < totalJobNum; i++) {
            struct JCB currentJob = jcbTemp[i];
            if (BinarySearch(runJobs, totalJobNum, currentJob.index) != -1) continue; // 如果这个作业已经运行过就跳过
            if (globalTimer >= currentJob.arrivalTime) { // 作业到达,将作业放入就绪队列等待调度
                PutIlluChar(currentJob.illustration, globalTimer, '|', 0);
                runJobs[runJobsInd] = i; // 把当前作业的下标存入数组
                readyQueue[readyQueueInd] = currentJob; // 将到达的作业推入就绪队列
                runJobsInd++; // 下标移动到新尾部
                readyQueueInd++;
            }
        }
        if (runJobsInd >= totalJobNum && readyQueueInd <= 0 && runningJob.name == 0) {
            // 没有未到达的作业，就绪队列中也没有作业了，且没有正在运行的作业，就算是完成了
            break;
        }
        if (runningJob.name == 0) { // 没有正在执行的作业
            // 对就绪队列进行排序！
            for (i = 0; i < readyQueueInd; i++) { // 仍然是插入排序
                int priorIndex = i;
                for (j = i + 1; j < readyQueueInd; j++) {
                    // SJF，从小到大排序
                    if (hrrn == 0 && readyQueue[priorIndex].serviceSpend > readyQueue[j].serviceSpend) {
                        priorIndex = j; // 把j这一项提前
                    } else if (hrrn == 1) { // HRRN，从大到小排序
                        struct JCB prev = readyQueue[priorIndex],
                                next = readyQueue[j];
                        int prevSpend = prev.serviceSpend, // 要求服务时间
                        nextSpend = next.serviceSpend,
                                prevArrival = prev.arrivalTime, // 到达时间
                        nextArrival = next.arrivalTime;
                        int prevWaitTime = globalTimer - prevArrival, // 等待时间
                        nextWaitTime = globalTimer - nextArrival;
                        int prevRatio = (prevWaitTime + prevSpend) / prevSpend, //响应比
                        nextRatio = (nextWaitTime + nextSpend) / nextSpend;
                        if (nextRatio > prevRatio) priorIndex = j; // 如果后面一项响应比高，就提到前面来D
                    }
                }
                if (priorIndex != i) {
                    struct JCB temp = readyQueue[i];
                    readyQueue[i] = readyQueue[priorIndex];
                    readyQueue[priorIndex] = temp;
                }
            }
            runningJob = readyQueue[0]; // 此时排在最前面的被调度分配执行
            runningJob.startTime = globalTimer; // 标记开始运行的时间
            runningJob.alreadyFinished = 0; // 已经完成了多少
            PopJob(readyQueue, readyQueueInd, 0); // 运行的作业就从就绪队列中移除
            readyQueueInd--; // 就绪队列移除元素后长度变短
            PutIlluChar(runningJob.illustration, globalTimer, '#', runningJob.arrivalTime); // 用#标记开始运行
        } else {
            if (runningJob.alreadyFinished >= runningJob.serviceSpend) { // 作业执行完成了
                int index = runningJob.index; // 获得运行中作业在jcbTemp中的下标
                runningJob.finishTime = globalTimer; // 标记完成时间
                jcbTemp[index] = runningJob; // 放回去
                runningJob.name = 0; // 运行完成，标记为无运行中的作业
                continue; // 跳出本次循环
            } else {
                PutIlluChar(runningJob.illustration, globalTimer, '#', runningJob.arrivalTime); // #标记正在运行
            }
        }
        runningJob.alreadyFinished++; // 正在执行的作业完成进度+1
        globalTimer++; // 时间推进
    }
    // 所有作业执行完毕，进入统计阶段
    int totalTA = 0;
    float totalTAW = 0,
            averageTA,
            averageTAW;
    // 开始输出！
    printf("\n------------------------------\n%s Job Scheduling: \n\tName\tArrival\tSpend\tStart\tEnd\tTA\tTAW\n",
           hrrn == 1 ? "HRRN" : "SJF");
    for (i = 0; i < totalJobNum; i++) {
        struct JCB currentJob = jcbTemp[i];
        int TATime = currentJob.finishTime - currentJob.arrivalTime; // 算出周转时间
        float TAWTime = (float) TATime / currentJob.serviceSpend; // 算出带权周转时间
        jcbTemp[i].TATime = TATime; // 存入周转时间
        jcbTemp[i].TAWTime = TAWTime;
        totalTA += TATime;
        totalTAW += TAWTime;
        printf("\t%-4c\t%-5d\t%-6d\t%-7d\t%-6d\t%-2d\t%-4.3f\n", currentJob.name, currentJob.arrivalTime,
               currentJob.serviceSpend, currentJob.startTime, currentJob.finishTime, TATime,
               TAWTime);
    }
    // 打印时间线
    printf("\n\t~~~~~~~~~~~~~~~~TimeLine~~~~~~~~~~~~~~~~\n\n");
    for (i = 0; i < totalJobNum; i++) { // 打印时间线
        char name = jcbTemp[i].name;
        char *illustration = jcbTemp[i].illustration; // 获得时间线
        printf("\t%c: ", name);
        printf("%s", illustration);
        printf("\n");
    }
    averageTA = (float) totalTA / totalJobNum; // 平均周转时间
    averageTAW = totalTAW / totalJobNum; // 平均带权周转时间
    struct AlgoResult result = { // 构造结果
            averageTA,
            averageTAW
    };
    return result;
}

int main() {
    struct AlgoResult fcfsResult = FCFS(), // FCFS
    sjfResult = SjfAndHrrn(0),  // SJF
    hrrnResult = SjfAndHrrn(1); // HRRN
    printf("\n>>>>>>>>>>>>>>>Result<<<<<<<<<<<<<<<<\n");
    printf("\nFCFS------------------------------ \n\tAverage Weighted Turnaround Time: %.3fms\n\tAverage Turnaround Time: %.3fms\n",
           fcfsResult.averageTAW,
           fcfsResult.averageTA);
    printf("\nSJF------------------------------ \n\tAverage Weighted Turnaround Time: %.3fms\n\tAverage Turnaround Time: %.3fms\n",
           sjfResult.averageTAW,
           sjfResult.averageTA);
    printf("\nHRRN------------------------------ \n\tAverage Weighted Turnaround Time: %.3fms\n\tAverage Turnaround Time: %.3fms\n\n",
           hrrnResult.averageTAW,
           hrrnResult.averageTA);
    system("pause"); // 暂停程序以供观赏
}
