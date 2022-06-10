#!/bin/bash

total_stdy="$(($(stty size|cut -d' ' -f1)))"
total_stdx="$(($(stty size|cut -d' ' -f2)))"

head="Progress bar: "
total=$[${total_stdx} - ${#head}*2]

i=0
loop=100
while [ $i -lt $loop ]
do
    let i=i+1
    
    per=$[${i}*${total}/${loop}]
    remain=$[${total} - ${per}]
    printf "\r\e[${total_stdy};0H${head}\e[42m%${per}s\e[47m%${remain}s\e[00m" "" ""
    sleep 0.1
done

echo ""

