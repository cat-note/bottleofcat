'use strict';

const PLATE_HEIGHT = 20; // 盘子高度(像素)
const PLATE_MIN_HEIGHT = 10 + 8; // 盘子最小高度(像素)
const PLATE_MAX_WIDTH = 20; // 盘子最大宽度(百分比)
const PLATE_COLORS = [
    ['#FF0000', '#FFF'],
    ['#FF7F00', '#FFF'],
    ['#FFFF00', '#000'],
    ['#00FF00', '#FFF'],
    ['#0000FF', '#FFF'],
    ['#4B0082', '#FFF'],
    ['#9400D3', '#FFF']
]; // 盘子颜色
const plateElems = []; // 所有盘子的元素
const poleElems = [0, 0, 0]; // 记录每根柱子上元素数量
const selectId = (query) => {
    return document.querySelector(`#${query}`);
}

const NEXT_BTN = selectId('nextBtn'); // 开始播放/下一步按钮
const RESET_BTN = selectId('resetBtn'); // 重置按钮
const AUTO_CHECK = selectId('autoCheck'); // 是否自动播放勾选框

AUTO_CHECK.addEventListener('change', () => {
    console.log('hello');
    if (AUTO_CHECK.checked) {
        NEXT_BTN.value = '开始播放';
    } else {
        NEXT_BTN.value = '播放下一步';
    }
});

/**
 * 初始化汉诺塔
 * @param {Number} plateNum 汉诺塔初始盘数
 */
function initialize(plateNum) {
    let plateHeight = PLATE_MIN_HEIGHT; // 盘子所在高度(像素)
    let plateWidth = PLATE_MAX_WIDTH;
    let widthDecrement = (PLATE_MAX_WIDTH / 2) / plateNum; // 盘子宽度减量(百分比)
    let showCase = selectId('showcase');
    let plateColor = 0;
    for (let i = 0; i < plateNum; i++) {
        let plateElem = document.createElement('div');
        plateElem.className = 'plate';
        plateElem.style.left = `${100 / 6}%`;
        plateElem.style.bottom = `${plateHeight}px`;
        plateElem.style.width = `${plateWidth}%`;
        plateElem.style.height = `${PLATE_HEIGHT}px`;
        let [bgColor, fontColor] = PLATE_COLORS[plateColor++ % PLATE_COLORS.length];
        plateElem.style.backgroundColor = bgColor;
        plateElem.style.color = fontColor;
        plateElem.innerText = `${i + 1}`;
        plateHeight += PLATE_HEIGHT + 1;
        plateWidth -= widthDecrement;
        showCase.appendChild(plateElem);
        plateElems.push(plateElem);
    }
}

initialize(4);