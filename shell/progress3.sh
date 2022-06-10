#!/bin/bash
i=0
str=""
arry=("|" "/" "-" "\\")
while [ $i -le 100 ]
do
    let index=i%4
    printf "%3d%% %c%-20s%c\r" "$i" "${arry[$index]}" "$str" "${arry[$index]}"
    sleep 0.2
    let i=i+5
    str+="*"
done
echo ""
