#!/bin/bash

set -e

curdir=$(realpath -- $(dirname -- $0))
export RTE_TARGET=x86_64-native-linux-gcc
export RTE_SDK=${curdir}/dpdk-stable-19.08.2

# DPDK通过hugetlbfs访问HugePages
# 为DPDK工具分配大页
echo never | sudo tee /sys/kernel/mm/transparent_hugepage/enabled
#echo 4096 | sudo tee /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages
echo 4096 | sudo tee /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages
#echo 4 | sudo tee /sys/kernel/mm/hugepages/hugepages-1048576kB/nr_hugepages

[[ $# -ge 1 ]] && sudo bash -c "${@:1}"
