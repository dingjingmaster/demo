#!/bin/bash

declare -A dic

dic=(["key1"]="value1" ["key2"]="value2" ["key3"]="value3")

dic+=(["key33"]="v33")

arr=(1 2 3 4 5 6 7 8 9)

for key in ${arr[*]}
do 
    dic+=([$key]="$key")
done

for key in $(echo ${!dic[*]})
do
        echo "$key : ${dic[$key]}"
done

echo ${dict[*]}
