/*
本代码用作【随机考点成绩排名】的测试数据的生成，对应题目7-41
使用方法：
	- 在命令行中输入 node xxx.js 
	- 生成的测试数据将会被写入同目录的testdata.txt文件中

	SomeBottle 2023.1.31
*/
const { rmSync, existsSync, writeFileSync } = require('fs');
const FILE_NAME = "./testdata.txt";
const args = process.argv.slice(2);
const maxSiteNum = parseInt(args[0]);; // 最大考点数
const maxStuNumPerSite = parseInt(args[1]);; // 每个考点的最大考生数
if (isNaN(maxSiteNum) || isNaN(maxStuNumPerSite)) {
	console.log("Invalid input");
	process.exit(1);
}
if (maxSiteNum < 1 || maxStuNumPerSite < 1 || maxSiteNum > 100 || maxStuNumPerSite > 300) {
	console.log("MAX SITE NUM:100,MAX STU NUM:300");
	process.exit(1);
}
if (existsSync(FILE_NAME))
	rmSync(FILE_NAME);

// [min,max]随机数
function rand(min, max) {
	return Math.floor(Math.random() * (max - min + 1) + min);
}

// 输出测试数据
function generate() {
	let siteNum = rand(1, maxSiteNum);
	let result = `${siteNum}\n`;
	let currNo = 0;
	for (let i = 0; i < siteNum; i++) {
		let stuNum = rand(1, maxStuNumPerSite);
		result += `${stuNum}\n`;
		for (let j = 0; j < stuNum; j++) {
			result += `12345678${10000 + currNo++} ${rand(0, 100)}\n`;
		}
	}
	writeFileSync(FILE_NAME, result);
}

generate();