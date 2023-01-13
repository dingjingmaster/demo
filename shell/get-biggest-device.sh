#!/bin/bash

#
# 获取电脑上最大的分区挂载点
# 注意：
#   1. 不能是外接设备
#   2. 不能是网络设备
#   3. 不能是USB设备
#

biggestMountDir=`lsblk -nbpro "PATH,HOTPLUG,MOUNTPOINT,SIZE" | awk '{if ($2 == 0) print $1"\t"$2"\t"$3"\t"$4}' | sort -n -r -k 4 | awk -F'\t' '{print $3"\t"$4}' | grep -e "^/" | head -1 | cut -f 1`

echo "最大的挂载点:$biggestMountDir"

