#!/usr/bin/env bash

for i in {0..999}
do
	echo "$i..."
	gio mount -u /media/kylin/3F4D25377BD9BAF4 && gio mount -d /dev/nvme0n1p10
done
