#!/bin/bash
# 杨辉三角/帕斯卡三角
# Written by SomeBottle 20220607
# 在输出左方填充空格
function LeftPad() {
    local i=0
    local len="$1" # 需要填充空格的长度
    for ((i = 0; i < $len; i++)); do
        echo -n " " # -n 选项取消末尾的换行符
    done
}

# 阶乘运算
function Factorial() {
    local order="$1" # 阶乘的阶数
    local result=1   # 阶乘的结果
    for ((i = order; i > 0; i--)); do
        result=$((result * i))
    done
    echo -n $result # 利用echo返回值，末尾不要带换行符
}

# 计算组合数C(n,m)，从n中选出m项
# 组合数公式C(n,m)= (n!) / (m!)*(n-m)!
function CmbNum() {
    local factorialM=$(Factorial $2)                          # m的阶乘
    local factorialN=$(Factorial $1)                          # n的阶乘
    local factorialNMinusM=$(Factorial $(($1 - $2)))          # (n-m)的阶乘
    echo -n $((factorialN / (factorialM * factorialNMinusM))) # 利用echo返回值，末尾不要带换行符
}

# 初始化层数
pileHeight=0
while true; do
    # 读入用户输入
    read -p "Please input the Height of the Pascal Triangle: " pileHeight
    # 判断是数字且大于0
    if [[ "$pileHeight" =~ ^[0-9]+$ && "$pileHeight" -gt 0 ]]; then
        break
    else
        echo "Please input a positive integer (>0) !"
    fi
done

# 打印部分
maxSupport=5                                       # 最大支持5位数，必须要保证maxSupport+1为偶数
spaceOffset=$(((maxSupport + 1) / 2))              # 左边空格的偏移量
spaceNum=$((pileHeight * spaceOffset))             # 每一层前面的空格数
LeftPad $((spaceNum))                              # 填充首行空格
printf "%-$((maxSupport + 1))d" "1"                # 输出第一层
echo -ne "\n"                                      # 每行之后换行
for ((i = 1; i <= pileHeight - 1; i++)); do        # 每层，从第二层开始
    LeftPad $((spaceNum - i * spaceOffset))        # 在左边填充空格，形成三角形
    for ((j = 0; j <= i; j++)); do                 # 每行
        calcNum=$(CmbNum $i $j)                    # 计算组合数
        printf "%-$((maxSupport + 1))d" "$calcNum" # 输出组合数
    done
    printf "\n" # 每行之后换行
done
