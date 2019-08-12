#!/bin/bash

index=1
while true
do
    echo "lock1"$index
    ((index=$index+1))
done
