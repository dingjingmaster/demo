#!/bin/bash

progs=(
    "/home/dingjing/a.sh"
    "/home/dingjing/b.sh"
)

for p in "${progs[@]}"; do
    echo "start $p ..."
    $p &>/dev/null 2>&1 &
done

while true; do
    for p in "${progs[@]}"; do
        pgrep -f "$p" >/dev/null 2>&1
        if [ $? -ne 0 ]; then
            $p &>/dev/null 2>&1 &
            echo "$p start ..."
        fi
        sleep 1
    done
    sleep 1
done
