'use strict';

const PLATE_HEIGHT = 20; // 盘子高度(像素)
const PLATE_MIN_HEIGHT = 10 + 8; // 盘子最小高度(像素)
const PLATE_MAX_WIDTH = 20; // 盘子最大宽度(百分比)
const PLATE_COLORS = [
    ['#FF0000', '#FFF'],
    ['#FF7F00', '#FFF'],
    ['#FFFF00', 'rgb(107 107 107)'],
    ['#00FF00', '#FFF'],
    ['#0000FF', '#FFF'],
    ['#4B0082', '#FFF'],
    ['#9400D3', '#FFF']
]; // 盘子颜色
const MOVE_END_EVENT = new CustomEvent('moveend'); // 盘子移动结束事件
const PAUSE_PLAY_EVENT = new CustomEvent('pauseplay'); // 暂停播放事件
const plateElems = []; // 所有盘子的元素
const poleElems = [[], [], []]; // 记录每根柱子上的盘子元素
const selectId = (query) => {
    return document.querySelector(`#${query}`);
}

const PLAY_BTN = selectId('playBtn'); // 开始播放按钮
const PREV_BTN = selectId('prevBtn'); // 上一步按钮
const NEXT_BTN = selectId('nextBtn'); // 下一步按钮
const RESET_BTN = selectId('resetBtn'); // 重置按钮


// 一摞盘子
class PlatePile {
    constructor(plateNum, from, via, to) {
        this.plateNum = plateNum;
        this.from = from;
        this.via = via;
        this.to = to;
    }
}

const hanoiSteps = []; // 汉诺塔移动步骤

function solveHanoi(plateNum) {
    hanoiSteps.length = 0; // 清空移动步骤
    let stack = []; // 新建栈
    let initial = new PlatePile(plateNum, 0, 1, 2); // 初始的一摞盘子
    stack.push(initial); // 压入栈
    while (stack.length > 0) {
        let current = stack.pop(); // 弹出栈顶元素
        if (current.plateNum === 1) {
            hanoiSteps.push([current.from, current.to]);
        } else {
            stack.push(new PlatePile(current.plateNum - 1, current.via, current.from, current.to));
            stack.push(new PlatePile(1, current.from, current.via, current.to));
            stack.push(new PlatePile(current.plateNum - 1, current.from, current.to, current.via));
        }
    }
}

/**
 * 播放将盘子移动到to柱的动画
 * @param {Element} plateElem 盘子元素
 * @param {Number} to 目标柱下标
 */
function movePlate(plateElem, to) {
    const eventRemover = () => {
        plateElem.removeEventListener('transitionend', moveSteps);
        plateElem.removeEventListener('interruptmove', interrupt);
        document.body.dispatchEvent(MOVE_END_EVENT);
    };
    let step = Number(plateElem.getAttribute('movestate')); // 获取盘子的移动状态
    let sixth = 100 / 6;
    let startFrom = Number(plateElem.getAttribute('pole')); // 起始柱下标
    let plateId = Number(plateElem.getAttribute('plateId')); // 盘子编号
    // 如果有其他对当前盘子移动的操作正在进行，就中断掉
    plateElem.dispatchEvent(new CustomEvent('interruptmove', {
        detail: {
            plateId: plateId
        }
    }));
    if (startFrom === to) return; // 起始柱和目标柱相同，不移动
    let polePlateNum = poleElems[to].length; // 目标柱上已有盘子的数量
    const moveSteps = () => {
        switch (step) {
            case 0:
                plateElem.style.bottom = "70%";
                plateElem.setAttribute('movestate', 1);
                break;
            case 1:
                plateElem.style.bottom = "70%";
                plateElem.style.left = `${sixth * to * 2 + sixth}%`;
                plateElem.setAttribute('movestate', 2);
                break;
            case 2:
                plateElem.style.left = `${sixth * to * 2 + sixth}%`;
                plateElem.style.bottom = `${PLATE_MIN_HEIGHT + (PLATE_HEIGHT + 1) * polePlateNum}px`;
                plateElem.setAttribute('movestate', 0);
                break;
            default:
                eventRemover();
                return;
        }
        step++;
    };
    // 中断移动
    const interrupt = (event) => {
        if (plateId === event.detail.plateId) {
            console.log('interrupted');
            eventRemover();
            return;
        }
    };
    moveSteps();
    plateElem.addEventListener('transitionend', moveSteps);
    plateElem.addEventListener('interruptmove', interrupt);
    plateElem.setAttribute('pole', to); // 更新盘子所在柱
    poleElems[startFrom].pop();
    poleElems[to].push(plateElem);
}

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
        plateElem.setAttribute('plateid', i + 1); // 盘子编号
        plateElem.setAttribute('pole', 0); // 位于0号柱
        plateElem.setAttribute('movestate', 0); // 0代表静止，1代表提起来，2代表平移
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
        poleElems[0].push(plateElem); // 起始柱上有这么多盘子
    }
    solveHanoi(plateNum); // 计算移动步骤
}

let playState = false; // 播放状态
let currentPlayStep = 0; // 当前播放到第几步
const playPrev = () => { // 播放上一步
    let prevStep = currentPlayStep - 1;
    if (prevStep >= 0) {
        let [from, to] = hanoiSteps[prevStep]; // 取出上一次的移动步骤
        let topPlateInd = poleElems[to].length - 1; // 
        let topPlate = poleElems[to][topPlateInd]; // 取出上次移动的盘子
        movePlate(topPlate, from); // 把盘子移动回去
        currentPlayStep--;
    }
};
const playStep = () => {
    let lastStep = hanoiSteps.length - 1;
    if (currentPlayStep <= lastStep) {
        let [from, to] = hanoiSteps[currentPlayStep]; // 取出当前的移动步骤
        let topPlateInd = poleElems[from].length - 1;
        let topPlate = poleElems[from][topPlateInd]; // 取出起始柱顶上的盘子
        movePlate(topPlate, to); // 移动盘子
        currentPlayStep++;
    } else {
        document.body.removeEventListener('moveend', playStep);
        document.body.removeEventListener('pauseplay', pausePlay);
        alert('播放结束');
        console.log('Play end.');
    }
};
const pausePlay = () => {
    document.body.removeEventListener('pauseplay', pausePlay);
    document.body.removeEventListener('moveend', playStep);
    console.log('Play paused.');
};

/**
 * 自动播放汉诺塔整个过程
 */
function autoPlay() {
    document.body.addEventListener('moveend', playStep);
    document.body.addEventListener('pauseplay', pausePlay);
    playStep();
}

PLAY_BTN.addEventListener('click', () => {
    playState = !playState;
    if (playState) {
        PLAY_BTN.value = '暂停';
        autoPlay();
    } else {
        PLAY_BTN.value = '播放';
        document.body.dispatchEvent(PAUSE_PLAY_EVENT);
    }
});

NEXT_BTN.addEventListener('click', () => {
    if (playState) {
        document.body.dispatchEvent(PAUSE_PLAY_EVENT); // 停止播放
        playState = !playState;
        PLAY_BTN.value = '播放';
    }
    playStep();
});

PREV_BTN.addEventListener('click', () => {
    if (playState) {
        document.body.dispatchEvent(PAUSE_PLAY_EVENT); // 停止播放
        playState = !playState;
        PLAY_BTN.value = '播放';
    }
    playPrev();
});


RESET_BTN.addEventListener('click', () => {
    if (playState) {
        document.body.dispatchEvent(PAUSE_PLAY_EVENT); // 停止播放
        playState = !playState;
        PLAY_BTN.value = '播放';
    }
    currentPlayStep = 0;
    let plates = document.querySelectorAll('.plate');
    plates.forEach(plate => {
        plate.remove();
    });
    poleElems.forEach(pole => {
        pole.length = 0;
    });
    plateElems.length = 0;
    hanoiSteps.length = 0;
    initialize(6);
});


initialize(6);