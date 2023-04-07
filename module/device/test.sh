#!/bin/bash

curDir=$(dirname $(realpath -- ${0}))
echo $curDir

cd ${curDir} > /dev/null 2>&1

sudo rmmod device >/dev/null 2>&1
sudo dmesg -C && sudo insmod device.ko && sudo dmesg 

cd - > /dev/null 2>&1
