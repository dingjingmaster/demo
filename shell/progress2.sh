#!/bin/bash
i=0
str=""
arry=("\\" "|" "/" "-")
while [ $i -le 100 ]
do
    let index=i%4
    if [ $i -le 20 ]; then
        let color=44
        let bg=34
    elif [ $i -le 45 ]; then
        let color=43
        let bg=33
    elif [ $i -le 75 ]; then
        let color=41
        let bg=31
    else
        let color=42
        let bg=32
    fi
    printf "\033[${color};${bg}m%-s\033[0m %d %c\r" "$str" "$i" "${arry[$index]}"
    sleep 0.5 
    let i=i+1
    str+="#"
done
echo ""
