#!/bin/bash

set -e

#
# 使用此脚本之前, 自觉判断 /proc/kallsyms 是否存在
#
# 依赖 /proc/kallsyms 判断符号是否存在, 需要 3 个参数:
#  参数1: 指明要执行的操作
#         S 表示第一个检查的符号是否存在;
#         C 表示后续检查的符号是否存在;
#         E 表示最后一个检查的符号是否存在
#  参数2: 指明要检查的符号类型
#         T 表示代码段全局符号
#         t 表示代码段局部符号
#         具体情况参看 /proc/kallsyms 第二列字段
#  参数3: 指明检查的具体符号
#

[[ 3 -ne $# ]] && echo 'Invalid params\n' && exit 1

kallsymsConfigFile='./kallsyms-config.h'

function check_type_and_symbol()
{
    symbolType="$1"
    symbolName="$2"

    cat /proc/kallsyms | grep -w "$symbolType" | grep -w "$symbolName" > /dev/null 2>&1
    res=$?

    if [ 0 -eq "$res" ]; then
        echo "Check $1 $2 OK"
        echo -e "#define KALLSYMS_${symbolName^^}                               1" \
            >> "${kallsymsConfigFile}"
    else
        echo "Check $1 $2 Not Exist!"
        echo -e "#define KALLSYMS_${symbolName^^}                               0" \
            >> "${kallsymsConfigFile}"
    fi
}

if [ 'S' == "$1" ]; then
    echo -e "#ifndef _KERNEL_CONFIG_KALLSYMBOL_IS_DEFINED_" >  "${kallsymsConfigFile}"
    echo -e "#define _KERNEL_CONFIG_KALLSYMBOL_IS_DEFINED_" >> "${kallsymsConfigFile}"
    echo -e "" >> "${kallsymsConfigFile}"
    check_type_and_symbol "$2" "$3"
elif [ 'E' == "$1" ]; then
    check_type_and_symbol "$2" "$3"
    echo -e "" >> "${kallsymsConfigFile}"
    echo "#endif" >> "${kallsymsConfigFile}"
elif [ 'C' == "$1" ]; then
    check_type_and_symbol "$2" "$3"
else
    echo "Invalid params1\n"
    exit -1
fi
