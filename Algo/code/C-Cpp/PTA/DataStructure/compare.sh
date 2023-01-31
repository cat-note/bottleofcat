#!/bin/bash
# 比较两个程序的输出，直至出现不同的地方
# 本脚本主要适用于Bash/Zsh

# 用法：./compare.sh <execProgram1> <execProgram2> <refreshProgram> <inputFile>
# 用法示例：./compare.sh ./a.out ./b.out 'node randomRank7-41.js 10 5' input.txt

PROGRAM_1=$1
PROGRAM_2=$2
REFRESH_PROGRAM=$3
INPUT_FILE=$4

DIFF=

while [[ $DIFF =~ ^[[:space:]]*$ ]]; do
    $REFRESH_PROGRAM # 生成新的输入
    DIFF=$(diff <(cat $INPUT_FILE | $PROGRAM_1) <(cat $INPUT_FILE | $PROGRAM_2) -y --suppress-common-lines)
done

echo "$DIFF"