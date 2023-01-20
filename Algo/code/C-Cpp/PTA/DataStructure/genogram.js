/*
本代码用作【PTA数据结构中文习题集7-27 家谱图】的测试数据的生成
使用方法：
	- 在命令行中输入 node genogram.js <名字数量> <陈述句数量>
	- 生成的测试数据将会被写入testdata.txt文件中

	SomeBottle 2023.1.20
*/
const { appendFileSync, rmSync, existsSync } = require('fs');
const FILE_NAME = "./testdata.txt";
const args = process.argv.slice(2);
const nameNum = parseInt(args[0]);
const sentenceNum = parseInt(args[1]);
const MAX_INDENT = 25;
if (isNaN(nameNum) || isNaN(sentenceNum)) {
	console.log("Invalid input");
	process.exit(1);
}
if (existsSync(FILE_NAME))
	rmSync(FILE_NAME);

function rand(min, max, except = false) {
	let result = Math.floor(Math.random() * (max - min + 1) + min);
	if (except && result == except)
		return rand(min, max, except);
	return result;
}

let generate = () => {
	let result = `${nameNum} ${sentenceNum}\n`;
	result += `Member0\n`;
	let currentIndent = 1;
	let dec = false;
	for (let i = 1; i < nameNum; i++) {
		result += `${' '.repeat(currentIndent * 2)}Member${i}\n`;
		if (!dec) {
			if (currentIndent < MAX_INDENT)
				currentIndent++;
			else
				dec = true;
			if (rand(1, 5) == 3)
				dec = true;
		} else {
			if (currentIndent > 1)
				currentIndent--;
			else
				dec = false;
			if (rand(1, 5) == 2)
				dec = false;
		}
	}
	for (let i = 0; i < sentenceNum; i++) {
		let front = rand(0, nameNum - 1);
		let back = rand(0, nameNum - 1, front);
		switch (rand(1, 5)) {
			case 1:
				result += `Member${front} is a child of Member${back}\n`;
				break;
			case 2:
				result += `Member${front} is the parent of Member${back}\n`;
				break;
			case 3:
				result += `Member${front} is a sibling of Member${back}\n`;
				break;
			case 4:
				result += `Member${front} is a descendant of Member${back}\n`;
				break;
			case 5:
				result += `Member${front} is an ancestor of Member${back}\n`;
				break;
		}
	}
	appendFileSync(FILE_NAME, result);
}
generate();
