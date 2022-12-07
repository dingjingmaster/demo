#!/bin/bash

sudo mkdir -p /sys/fs/cgroup/cpu/test/

sudo echo '10000' > /sys/fs/cgroup/cpu/test/cpu.cfs_period_us
sudo echo '5000'  > /sys/fs/cgroup/cpu/test/cpu.cfs_quota_us
