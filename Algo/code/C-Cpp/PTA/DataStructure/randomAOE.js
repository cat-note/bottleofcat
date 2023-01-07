/*
本代码用作【随机可行AOE网】(无环有向图)的测试数据的生成
使用方法：
	- 在命令行中输入 node randomAOE.js [vertexNum]
	- vertexNum为图的顶点数
	- 生成的测试数据将会被写入同目录的testdata.txt文件中

	SomeBottle 2023.1.7
*/
const { rmSync, existsSync, writeFileSync } = require('fs');
const FILE_NAME = "./testdata.txt";
const args = process.argv.slice(2);
const vertexNum = parseInt(args[0]);
if (isNaN(vertexNum)) {
	console.log("Invalid vertex num");
	process.exit(1);
}
if (existsSync(FILE_NAME))
	rmSync(FILE_NAME);

const unusedVertex = new Set(); // 初始化未使用顶点
for (let i = 1; i <= vertexNum; i++)
	unusedVertex.add(i);
const usedVertex = new Set(); // 已经使用过的顶点集

// [min,max]随机数
function rand(min, max) {
	return Math.floor(Math.random() * (max - min + 1) + min);
}

// 打乱数组
function shuffle(arr) {
	arr.sort((a, b) => (Math.random() > 0.5 ? -1 : 1));
}

// 获得未使用过的一个顶点集
function randomVertexSet(percent = 1) {
	let eNum = rand(1, Math.round(unusedVertex.size * percent)); // 这次取出多少顶点
	let vertexes = Array.from(unusedVertex.values());
	//shuffle(vertexes);
	let result = vertexes.slice(0, eNum);
	for (let v of result) {
		unusedVertex.delete(v);
		usedVertex.add(v);
	}
	return result;
}

// 邻接表节点
class LNode {
	constructor(data) {
		this.data = data;
		this.next = null;
	}
}

const GRAPH = new Array(vertexNum); // 图的邻接表
for (let i = 0; i < vertexNum; i++)
	GRAPH[i] = new LNode(i + 1); // 初始化头节点

// 尽量为顶点集中每一个顶点都分配一个邻接点
function allocateAdjList(vArr, vAdjs) {
	let vNum = vArr.length; // 顶点集的顶点数
	let aNum = vAdjs.length; // 邻接点集的顶点数
	for (let i = 0; i < vNum; i++) {
		let v = vArr[i]; // 顶点v
		let adjArr = vAdjs.slice(rand(0, aNum - 1), aNum); // 邻接点集
		for (let adj of adjArr) { // 把邻接点加入到顶点v的邻接表中
			let node = new LNode(adj);
			node.next = GRAPH[v - 1].next;
			GRAPH[v - 1].next = node;
		}
	}
}

// 生成测试数据
function generate() {
	while (unusedVertex.size > 0) {
		let vArr = randomVertexSet(0.5); // 随机取出一个顶点集作为起点
		if (unusedVertex.size == 0) break; // 如果已经没有顶点了，就不再生成边了
		let vAdjs = randomVertexSet(); // 再取出一个顶点集，作为邻接点
		allocateAdjList(vArr, vAdjs); // 为顶点集中每一个顶点随机分配邻接点
	}
}

generate();

// 输出测试数据
function output() {
	let result = '';
	let activityNum = 0;
	for (let i = 0; i < vertexNum; i++) {
		let curr = GRAPH[i].next;
		while (curr) {
			result += `${i + 1} ${curr.data} ${rand(1, 10)}\n`;
			activityNum++;
			curr = curr.next;
		}
	}
	result = `${vertexNum} ${activityNum}\n` + result;
	console.log(result);
	writeFileSync(FILE_NAME, result);
}

output();