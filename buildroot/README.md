## 源配置
```
BACKUP_SITE="http://sources.buildroot.net"
KERNEL_MIRROR="https://mirror.bjtu.edu.cn/kernel/"
GNU_MIRROR="http://mirrors.nju.edu.cn/gnu/"
LUAROCKS_MIRROR="https://luarocks.cn"
CPAN_MIRROR="http://mirrors.nju.edu.cn/CPAN/"
```

## `Please port gnulib freadahead.c to your platform`
```
cd buildroot/output/build/host-m4-1.4.17 && 

sed -i 's/IO_ftrylockfile/IO_EOF_SEEN/' lib/*.c
echo "#define _IO_IN_BACKUP 0x100" >> lib/stdio-impl.h
```

