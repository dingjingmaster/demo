#!/bin/bash

data=/data/
dataSize=$(df -Bk | grep "/data" | awk '{print $2}' | sed 's/.$//')
dataAvai=$(df -Bk | grep "/data" | awk '{print $4}' | sed 's/.$//')

echo "data 盘总大小: ${dataSize} KB, 可用大小: ${dataAvai} KB"
