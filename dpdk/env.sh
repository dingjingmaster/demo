#!/bin/bash

export RTE_TARGET=x86_64-native-linux-gcc
export RTE_SDK=/home/dpdk/code/dpdk-stable-19.08.2

# DPDK通过hugetlbfs访问HugePages
# 为DPDK工具分配大页
echo 4096 | sudo tee /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages
echo 4 | sudo tee /sys/kernel/mm/hugepages/hugepages-1048576kB/nr_hugepages
