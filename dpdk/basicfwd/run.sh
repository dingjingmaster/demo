#!/bin/bash

qemu-system-x86_64 \
    -chardev socket,id=char0,path=/tmp/vhost-user1 \
    -netdev type=vhost-user,id=net0,chardev=char0,vhostforce \
    -device virtio-net-pci,netdev=net0
