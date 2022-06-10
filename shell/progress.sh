#!/bin/bash

progressBar=''

for i in {0..100..2}
do 
    printf "进度:[%-50s]%d%%\r" $progressBar $i
    sleep 0.5
    progressBar=#${progressBar}
done

echo ""
