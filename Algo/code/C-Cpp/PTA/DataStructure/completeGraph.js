/*
本代码用作【随机带正权完全图】的测试数据的生成
使用方法：
	- 在命令行中输入 node completeGraph.js [vertexNum]
	- vertexNum为图的顶点数
	- 生成的测试数据将会被写入testdata.txt文件中

	SomeBottle 2023.1.4
*/
const { appendFileSync, rmSync } = require('fs');
const FILE_NAME = "./testdata.txt";
const args = process.argv.slice(2);
const vertexNum = parseInt(args[0]);
if (isNaN(vertexNum)) {
	console.log("Invalid vertex num");
	process.exit(1);
}
rmSync(FILE_NAME);
let generate = () => {
	let result = '';
	let cnt = 0;
	for (let i = 1; i <= vertexNum; i++)
		for (let j = 1; j < i; j++) {
			let priority = Math.round(Math.random() * 200);
			console.log(i, j, priority);
			result += `${i} ${j} ${priority}\n`;
			cnt++;
		}
	result = `${vertexNum} ${cnt}\n` + result;
	appendFileSync(FILE_NAME, result);
}
generate();
