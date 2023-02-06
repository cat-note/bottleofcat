/*
本代码用作【银行排队问题之单窗口“夹塞”版】的测试数据的生成，对应题目7-48
使用方法：
	- 在命令行中输入 node xxx.js [最多顾客数] [最多朋友圈数]
	- 生成的测试数据将会被写入同目录的testdata.txt文件中

	SomeBottle 2023.2.6
*/
const { rmSync, existsSync, writeFileSync } = require('fs');
const FILE_NAME = "./testdata.txt";
const args = process.argv.slice(2);
const maxClientNum = parseInt(args[0]); // 最多顾客数
const maxCircleNum = parseInt(args[1]); // 最多朋友圈数
if (isNaN(maxClientNum) || isNaN(maxCircleNum)) {
	console.log("Invalid input");
	process.exit(1);
}
if (maxClientNum < 1 || maxCircleNum < 0 || maxClientNum > 10000 || maxCircleNum > 100) {
	console.log("Value out of range: maxClientNum[1,10000], maxCircleNum[0,100]");
	process.exit(1);
}
if (existsSync(FILE_NAME))
	rmSync(FILE_NAME);

// [min,max]随机数
function rand(min, max) {
	return Math.floor(Math.random() * (max - min + 1) + min);
}

const clientNames = new Set(); // 用于存储已经生成的顾客名字

// 名字由3个大写英文字母组成
function randName() {
	const dict = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
	let name;
	do {
		name = '';
		for (let i = 0; i < 3; i++) {
			name += dict[rand(0, 25)];
		}
	} while (clientNames.has(name)); // 防止重复
	clientNames.add(name);
	return name;
}

// 输出测试数据
function generate() {
	let result = '';
	let clientNum = rand(1, maxClientNum);
	let circleUpper = clientNum / 2 > maxCircleNum ? maxCircleNum : clientNum;
	let circleNum = rand(0, circleUpper);
	// 生成所有客户的名字
	for (let i = 0; i < clientNum; i++)
		randName();
	let nameArr = Array.from(clientNames); // 将Set转换为数组，用于随机取名字
	// 生成朋友圈的情况
	let usedNames = new Set();
	for (let i = 0; i < circleNum; i++) {
		let sizeUpper = clientNum - usedNames.size;
		if (sizeUpper < 2) {
			// 防止朋友圈人数不足2人
			circleNum = i;
			break;
		}
		let circleSize = rand(2, sizeUpper);
		let circle = new Set();
		for (let j = 0; j < circleSize; j++) {
			let name;
			do {
				name = nameArr[rand(0, clientNum - 1)];
			} while (circle.has(name) || usedNames.has(name)); // 防止重复
			circle.add(name);
			usedNames.add(name);
		}
		result += `${circleSize} ${Array.from(circle).join(' ')}\n`;
	}
	result = `${clientNum} ${circleNum}\n` + result;
	// 生成顾客的到达时间和办理时间
	usedNames.clear();
	let timeLine = [];
	for (let i = 0; i < clientNum; i++) {
		let name;
		do {
			name = nameArr[rand(0, clientNum - 1)];
		} while (usedNames.has(name)); // 防止重复
		usedNames.add(name);
		let arriveTime = rand(0, 100);
		let handleTime = rand(1, 30);
		timeLine.push([name, arriveTime, handleTime]);
	}
	timeLine.sort((a, b) => a[1] - b[1]);

	for (let i = 0; i < clientNum; i++) {
		result += `${timeLine[i].join(' ')}\n`;
	}
	writeFileSync(FILE_NAME, result);
}

generate();