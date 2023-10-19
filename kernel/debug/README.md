# Linux内核环境搭建

1. 编译内核源码(打开debug)
2. 配置 busybox (build as static binary)
3. 制作rootfs
    1. dd 创建 .img 文件
    2. mkfs.ext4 将其格式化
    3. 挂载 .img 文件
    4. `busybox install` 到.img挂载点
    5. 补充挂载点内容 `mkdir proc dev etc home mnt`
    6. 复制默认配置 `cp examples/bootfloppy/etc/* etc/`
    7. 卸载 .img
4. 启动qemu
    1. `-kernel` 指定编译好的内核
    2. `-hda` 指定硬盘
    3. `-append "root=/dev/sda"` 只是根文件系统 `"console=ttyS0"` 把QEMU的输入输出重定向到当前终端上
    4. `-nographic` 不使用图形输出窗口
    5. `-s` 是 `-gdb tcp::1234` 缩写，监听1234端口，在GDB中通过target remote localhost:1234连接
```
qemu-system-x86_64 -kernel ~/linux-4.14.191/arch/x86_64/boot/bzImage  -hda ~/busybox-1.32.0/rootfs.img  -append "root=/dev/sda console=ttyS0" -s -S  -smp 1 -nographic
```
5. 添加共享磁盘
    1. 创建另一个 .img，格式化并将其添加到qemu启动中`-hdb ~/shadisk/ext4.img`
    2. 进入qemu虚拟机中挂载 `/dev/sdb`
    3. 在宿主机挂载 .img 即可实现共享
