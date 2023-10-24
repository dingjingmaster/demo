# LFS

## LFS遵循的Linux标准
- POSIX.1-2008
- Filesystem Hierarchy Standard(FHS) Version 3.0
- Linux Standard Base(LSB) Version5.0 (2015)

https://refspecs.linuxfoundation.org/lsb.shtml
https://refspecs.linuxfoundation.org/FHS_3.0/fhs/index.html
https://pubs.opengroup.org/onlinepubs/9699919799/

## LSB 标准要求的软件包
|||
|:---|:---|
|LSB核心包|`bash`, `bc`, `binutils`, `coreutils`, `diffutils`, `file`, `findutils`, `gawk`, `grep`, `gzip`, `m4`, `man-db`, `ncurses`, `procps`, `psmisc`, `sed`, `shadow`, `tar`, `util-linux`, `zlib`|
|LSB桌面包|`无`|
|LSB运行时语言|`perl`,`python`|
|LSB Imaging||

其中 LSB 要求的，由BLFS提供的软件包,核心：`at`,`batch`,`cpio`,`ed`,`fcrontab`,`lsb-tools`,`nspr`,`nss`,`pam`,`pax`,`sendmail(或postfix,或exim)`,`time`

## 开始构建
### 第一步
创建Linux目录树结构
```shell
mkdir -pv etc var home root usr/{bin,lib,sbin}

# x86_64还需要执行
mkdir lib/lib64
```

创建软连接
```
ln -sv usr/lib lib
ln -sv usr/bin bin
ln -sv usr/sbin sbin
```

添加配置:`etc/profile`
```
#!/bin/bash

set +h
umask 022
LFS=/data/LFS/lfs-12.0
LC_ALL=POSIX
LFS_TGT=$(uname -m)-lfs-linux-gnu
PATH=$LFS/tools/bin:$PATH
CONFIG_SITE=$LFS/usr/share/config.site

export LFS LC_ALL LFS_TAT PATH CONFIG_SITE
```

创建临时二进制目录：
```shell
mkdir tools
```

> bash 设置 `+h`，bash 会计算命令的hash值，并和文件系统中对应的命令路径关联并缓存，这样每次执行命令时候，先计算执行命令的hash值，得到结果后去找缓存的命令开始执行，避免查找PATH，优化了速度。`+h`之后关闭了bash的hash功能，要求每次执行命令都从PATH中查找。

### 第二步
1. 构建一个交叉编译环境和与之相关的库
2. 使用这个交叉编译工具链构建一些工具，并使用它们和宿主系统的构建环境分离
3. 进入 chroot 环境，完成剩余的构建工作

> 交叉编译工具的优势是，任何用它构建的程序都不依赖于宿主环境。<br/>交叉编译器:`gcc-multilib`

#### 交叉编译环境和依赖库

1. 编译 binutils 并安装到 `$LFS/tools` 下，构建交叉编译环境
2. 构建并安装交叉工具链中的 `gcc`、 `linux`、 `glibc`、`c++标准库`

#### 继续构建
1. 使用LFS中的编译环境重新构建 binutils
2. 重新构建 gcc
3. 开始进入 chroot 环境

#### 具体过程

> 镜像站：[https://mirrors.aliyun.com/lfs/lfs-packages](https://mirrors.aliyun.com/lfs/lfs-packages/)

1. 安装交叉工具链中的 binutils
[https://sourceware.org/pub/binutils/releases/binutils-2.41.tar.xz](https://sourceware.org/pub/binutils/releases/binutils-2.41.tar.xz)

```shell
mkdir build && cd build
../configure --prefix=$LFS/tools \
             --with-sysroot=$LFS \
             --target=$LFS_TGT   \
             --disable-nls       \
             --enable-gprofng=no \
             --disable-werror
make && make install
```
2. 安装 `gmp`，`mpfr`，`mpc`，这三源码解压到 gcc 的源码目录中，以便 gcc 构建过程中自动使用它们
```shell
# 1. 解压 gcc
# 2. 进入 gcc 目录
# 3. 依次解压 `mpfr` `gmp` `mpc` 并放在 gcc 源码根目录
# 4. 执行：sed -e '/m64=/s/lib64/lib/'  -i.orig gcc/config/i386/t-linux64
#   对于 x86_64 设置存放64位库的默认位置为 lib
```
开始编译安装
```shell
mkdir build && cd build
../configure                  \
    --target=$LFS_TGT         \
    --prefix=$LFS/tools       \
    --with-glibc-version=2.38 \
    --with-sysroot=$LFS       \
    --with-newlib             \
    --without-headers         \
    --enable-default-pie      \
    --enable-default-ssp      \
    --disable-nls             \
    --disable-shared          \
    --disable-multilib        \
    --disable-threads         \
    --disable-libatomic       \
    --disable-libgomp         \
    --disable-libquadmath     \
    --disable-libssp          \
    --disable-libvtv          \
    --disable-libstdcxx       \
    --enable-languages=c,c++

make && make install

cd ..

# 将 LFS 中缺少的头文件补全
cat gcc/limitx.h gcc/glimits.h gcc/limity.h > `dirname $($LFS_TGT-gcc -print-libgcc-file-name)`/include/limits.h
```

3. 安装 LinuxAPI 头文件
解压缩 linux-6.4.10
```shell
cd linux-6.4.10
make mrproper # 清空所有编译

make headers

# 安装内核头文件
find usr/include -type f ! -name '*.h' -delete
cp -rv usr/include $LFS/usr
```

4. 安装 glibc

```shell
# patch
mkdir -v build
cd       build

echo "rootsbindir=/usr/sbin" > configparms

../configure                             \
      --prefix=/usr                      \
      --host=$LFS_TGT                    \
      --build=$(../scripts/config.guess) \
      --enable-kernel=4.14               \
      --with-headers=$LFS/usr/include    \
      libc_cv_slibdir=/usr/lib

make && make DESTDIR=$LFS install

# 改正 ldd 脚本中硬编码的可执行文件加载器路径
sed '/RTLDLIST=/s@/usr@@g' -i $LFS/usr/bin/ldd
```
验证新工具链的各个基本功能
```shell
echo 'int main(){}' | $LFS_TGT-gcc -xc -
readelf -l a.out | grep ld-linux

# 如果一切正常则输出如下：
# [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]
# 如果什么也没有输出或者不像上一行的输出则有问题，需要解决
```
> 接下来，构建各软件包的过程可以作为对工具链是否正常构建的额外检查。如果 一些软件包，特别是第二遍的 Binutils 或者 GCC 不能构建，说明在之前安装 Binutils，GCC，或者 Glibc 时出了问题。


5. 构建stdc++
>  libstdc++ 是 GCC 源代码的一部分。您应该先解压 GCC 源码包并切换到解压出来的 gcc-13.2.0 目录。 

先进入 gcc 目录(之前步骤编译过的)
```
mkdir -v build-c++
cd       build-c++
make && make DESTDIR=$LFS install
```

移除对交叉编译有害的 libtool 档案文件
```shell
rm -v $LFS/usr/lib/lib{stdc++,stdc++fs,supc++}.la
```

> 以上完成了交叉编译环境的第一阶段构建
6. m4
```
mkdir build && cd build
../configure --prefix=/usr   \
            --host=$LFS_TGT \
            --build=$(build-aux/config.guess)

make && make DESTDIR=$LFS install
```

7. ncurses

保证在配置时候有限查找 gawk 命令：
```
sed -i s/mawk// configure
```

然后执行,下面代码在宿主机构建 tic 程序
```shell
mkdir build
pushd build
  ../configure
  make -C include
  make -C progs tic
popd
```

构建 ncurses:
```shell
./configure --prefix=/usr                \
            --host=$LFS_TGT              \
            --build=$(./config.guess)    \
            --mandir=/usr/share/man      \
            --with-manpage-format=normal \
            --with-shared                \
            --without-normal             \
            --with-cxx-shared            \
            --without-debug              \
            --without-ada                \
            --disable-stripping          \
            --enable-widec

make            

# 安装
make DESTDIR=$LFS TIC_PATH=$(pwd)/build/progs/tic install
echo "INPUT(-lncursesw)" > $LFS/usr/lib/libncurses.so
```
`TIC_PATH=$(pwd)/build/progs/tic`：我们需要传递刚刚构建的，可以在宿主系统运行的 tic 程序的路径，这样才能正确创建终端数据库。

`echo "INPUT(-lncursesw)" > $LFS/usr/lib/libncurses.so`：我们很快将会构建一些需要 libncurses.so 库的软件包

8. bash

```shell
./configure --prefix=/usr                      \
            --build=$(sh support/config.guess) \
            --host=$LFS_TGT                    \
            --without-bash-malloc
```

`--without-bash-malloc`：该选项禁用 Bash 自己的内存分配 (malloc) 函数，因为已知它会导致段错误。这样，Bash 就会使用 Glibc 的更加稳定的 malloc 函数。

```shell
make && make DESTDIR=$LFS install
```

为那些使用 sh 命令运行 shell 的程序考虑，创建一个链接：
```shell
ln -sv bash $LFS/bin/sh
```

9. 安装 coreutils

```shell
./configure --prefix=/usr                     \
            --host=$LFS_TGT                   \
            --build=$(build-aux/config.guess) \
            --enable-install-program=hostname \
            --enable-no-install-program=kill,uptime \
            gl_cv_macro_MB_CUR_MAX_good=y
```

`--enable-install-program=hostname`：该选项表示构建 hostname 程序并安装它 —— 默认情况下它被禁用，但 Perl 测试套件需要它。
`gl_cv_macro_MB_CUR_MAX_good=y`：该选项用于绕过该软件包内嵌的 gnulib 中一项导致无法进行交叉编译的问题。

安装：
```shell
make && make DESTDIR=$LFS install
```

将程序移动到它们最终安装时的正确位置。在临时环境中这看似不必要，但一些程序会硬编码它们的位置，因此必须进行这步操作：
```shell
mv -v $LFS/usr/bin/chroot              $LFS/usr/sbin
mkdir -pv $LFS/usr/share/man/man8
mv -v $LFS/usr/share/man/man1/chroot.1 $LFS/usr/share/man/man8/chroot.8
sed -i 's/"1"/"8"/'                    $LFS/usr/share/man/man8/chroot.8
```

10. diffutils
```shell
./configure --prefix=/usr   \
            --host=$LFS_TGT \
            --build=$(./build-aux/config.guess)

make
make DESTDIR=$LFS install
```

11. file
```shell
mkdir build
pushd build
  ../configure --disable-bzlib      \
               --disable-libseccomp \
               --disable-xzlib      \
               --disable-zlib
make
popd

./configure --prefix=/usr --host=$LFS_TGT --build=$(./config.guess)
make FILE_COMPILE=$(pwd)/build/src/file
make DESTDIR=$LFS install
rm -v $LFS/usr/lib/libmagic.la
```
12. findutils
```
./configure --prefix=/usr                   \
            --localstatedir=/var/lib/locate \
            --host=$LFS_TGT                 \
            --build=$(build-aux/config.guess)

make
make DESTDIR=$LFS install
```

13. gawk
```shell
sed -i 's/extras//' Makefile.in
./configure --prefix=/usr   \
            --host=$LFS_TGT \
            --build=$(build-aux/config.guess)
make && make DESTDIR=$LFS install            
```

14. grep
```shell
./configure --prefix=/usr   \
            --host=$LFS_TGT \
            --build=$(./build-aux/config.guess)

make
make DESTDIR=$LFS install
```

15. gzip
```shell
./configure --prefix=/usr --host=$LFS_TGT
make
make DESTDIR=$LFS install
```

16. make
```shell
./configure --prefix=/usr   \
            --without-guile \
            --host=$LFS_TGT \
            --build=$(build-aux/config.guess)

make
make DESTDIR=$LFS install
```

17. patch
```shell

./configure --prefix=/usr   \
            --host=$LFS_TGT \
            --build=$(build-aux/config.guess)

make
make DESTDIR=$LFS install
```

18. sed

```shell
./configure --prefix=/usr   \
            --host=$LFS_TGT \
            --build=$(./build-aux/config.guess)
make
make DESTDIR=$LFS install
```

19. tar
```shell
./configure --prefix=/usr                     \
            --host=$LFS_TGT                   \
            --build=$(build-aux/config.guess)

make
make DESTDIR=$LFS install
```

20. xz

```shell
./configure --prefix=/usr                     \
            --host=$LFS_TGT                   \
            --build=$(build-aux/config.guess) \
            --disable-static                  \
            --docdir=/usr/share/doc/xz-5.4.4
make
make DESTDIR=$LFS install
rm -v $LFS/usr/lib/liblzma.la
```

> 接下来安装 binutils，第二遍编译

21. binutils
```
# 
sed '6009s/$add_dir//' -i ltmain.sh
```

编译
```shell
mkdir -v build
cd       build

../configure                   \
    --prefix=/usr              \
    --build=$(../config.guess) \
    --host=$LFS_TGT            \
    --disable-nls              \
    --enable-shared            \
    --enable-gprofng=no        \
    --disable-werror           \
    --enable-64-bit-bfd

make
make DESTDIR=$LFS install

#  移除对交叉编译有害的 libtool 档案文件，同时移除不必要的静态库：
rm -v $LFS/usr/lib/lib{bfd,ctf,ctf-nobfd,opcodes,sframe}.{a,la}
```
22. gcc

```
# 在 x86_64 上构建时，修改存放 64 位库的默认路径为 “lib”: 
case $(uname -m) in
  x86_64)
    sed -e '/m64=/s/lib64/lib/' -i.orig gcc/config/i386/t-linux64
  ;;
esac

#  覆盖 libgcc 和 libstdc++ 头文件的构建规则，以允许在构建它们时启用 POSIX 线程支持： 
sed '/thread_header =/s/@.*@/gthr-posix.h/' \
    -i libgcc/Makefile.in libstdc++-v3/include/Makefile.in
```

编译
```shell


../configure                                       \
    --build=$(../config.guess)                     \
    --host=$LFS_TGT                                \
    --target=$LFS_TGT                              \
    LDFLAGS_FOR_TARGET=-L$PWD/$LFS_TGT/libgcc      \
    --prefix=/usr                                  \
    --with-build-sysroot=$LFS                      \
    --enable-default-pie                           \
    --enable-default-ssp                           \
    --disable-nls                                  \
    --disable-multilib                             \
    --disable-libatomic                            \
    --disable-libgomp                              \
    --disable-libquadmath                          \
    --disable-libsanitizer                         \
    --disable-libssp                               \
    --disable-libvtv                               \
    --enable-languages=c,c++

make
make DESTDIR=$LFS install

# 许多程序运行cc而不是gcc，无论安装哪种GNU C编译器，运行CC可以将使用编译器的选择权留给系统管理员
ln -sv gcc $LFS/usr/bin/cc
```

` --with-build-sysroot=$LFS`：通常，指定 --host 即可保证使用交叉编译器构建 GCC，这个交叉编译器知道它应该在 $LFS 中查找头文件和库。但是 GCC 构建系统使用其他一些工具，它们不知道这个位置。因此需要该选项，使得这些工具在 $LFS 中查找需要的文件，而不是在宿主系统中查找。
` --target=$LFS_TGT`：我们正在交叉编译 GCC，因此不可能用编译得到的 GCC 二进制程序构建用于目标系统的运行库 (libgcc 和 libstdc++) —— 这些二进制程序无法在宿主系统运行。GCC 构建系统在默认情况下会试图使用宿主系统提供的 C 和 C++ 编译器来绕过这个问题。但是，用不同版本的 GCC 构建 GCC 运行库不受支持，所以使用宿主系统的编译器可能导致构建失败。该选项保证使用第一遍构建的 GCC 编译运行库。
`LDFLAGS_FOR_TARGET=...`：允许 libstdc++ 使用即将构建的 libgcc 共享库，而不是第一遍构建 GCC 时得到的静态库。这对于 C++ 异常处理支持是必要的。
`--disable-libsanitizer`：禁用 GCC 清理检查运行库。它们对于临时安装的 GCC 并不必要。为了在没有为目标安装 libcrypt 的情况下构建 GCC，必须使用该选项。在 第一遍的 GCC 中，--disable-libstdcxx 隐式地指定了该选项，但这里我们必须显式指定它。


> 接下来就可以进入 chroot 环境了

## LSF中为什么要编译两次coreutils和gcc
原因是为了构建一个自举系统。

自举系统是指用自身的工具链来构建自身的系统。在构建一个新的Linux发行版或者移植到新的硬件平台时候，通常会使用一个已经存在的Linux系统作为构建环境。而为了创建一个独立的、完整的Linux环境，需要使用该环境中的工具链来编译核心工具集和编译器。

第一次编译是为了构建一个基本的核心工具集，包括一些最基本的命令和实用程序，如：ls、cp、mv等。这些工具将在后续的编译过程中使用。

一旦基本的核心工具集构建完成，就可以使用它们来编译第二次编译所需的工具，其中包括GCC编译器。第二次编译使用的是第一次编译生成的核心工具集和编译器，以及它们的头文件和库文件。

通过两次编译过程，可以确保生成的核心工具集和编译器是互相兼容的，并且可以在最终的Linux系统中正常运行。这种自举的方式可以提供一个稳定和可靠的构建过程，确保系统的一致性和正确性。

需要注意的是，一旦完成了自举过程，就可以使用生成的核心工具集和编译器来编译其它软件包，而不再需要进行多次编译。自举只是在构建系统的初始阶段进行的过程。

## 使用 chroot 切换到LFS

1. 将整个 LFS 文件系统所有者修改为 root，避免切换后LFS文件系统中文件UID没有对应的用户
```shell
chown -R root:root $LFS/{usr,lib,var,etc,bin,sbin,tools}
case $(uname -m) in
  x86_64) chown -R root:root $LFS/lib64 ;;
esac
```

2. 与当前机器的内核通信
```
# 创建挂载点
mkdir -pv $LFS/{dev,proc,sys,run}
```

依次挂载
```shell
mount -v --bind /dev $LFS/dev
mount -v --bind /dev/pts $LFS/dev/pts
mount -vt proc proc $LFS/proc
mount -vt sysfs sysfs $LFS/sys
mount -vt tmpfs tmpfs $LFS/run

if [ -h $LFS/dev/shm ]; then
  mkdir -pv $LFS/$(readlink $LFS/dev/shm)
else
  mount -t tmpfs -o nosuid,nodev tmpfs $LFS/dev/shm
fi
```

切换：
```shell
chroot "$LFS" /usr/bin/env -i   \
    HOME=/root                  \
    TERM="$TERM"                \
    PS1='(lfs chroot) \u:\w\$ ' \
    PATH=/usr/bin:/usr/sbin     \
    /bin/bash --login
```
 通过传递 -i 选项给 env 命令，可以清除 chroot 环境中的所有环境变量。随后，只重新设定 HOME，TERM，PS1，以及 PATH 变量。参数 TERM=$TERM 将 chroot 环境中的 TERM 变量设为和 chroot 环境外相同的值。一些程序需要这个变量才能正常工作，例如 vim 和 less。如果需要设定其他变量，例如 CFLAGS 或 CXXFLAGS，也可以在这里设定。
从现在开始，就不再需要使用 LFS 环境变量，因为所有工作都被局限在 LFS 文件系统内。这是由于 chroot 命令启动 Bash 时，已经将根目录 (/) 设置为 $LFS。
> 注意 /tools/bin 不在 PATH 中。这意味着不再使用交叉工具链。

> 注意 bash 的提示符会包含 I have no name!。这是正常的，因为现在还没有创建 /etc/passwd 文件。 

## 进入 LFS 文件系统之后

1. 创建剩余文件

```shell
mkdir -pv /{boot,home,mnt,opt,srv}

mkdir -pv /etc/{opt,sysconfig}
mkdir -pv /lib/firmware
mkdir -pv /media/{floppy,cdrom}
mkdir -pv /usr/{,local/}{include,src}
mkdir -pv /usr/local/{bin,lib,sbin}
mkdir -pv /usr/{,local/}share/{color,dict,doc,info,locale,man}
mkdir -pv /usr/{,local/}share/{misc,terminfo,zoneinfo}
mkdir -pv /usr/{,local/}share/man/man{1..8}
mkdir -pv /var/{cache,local,log,mail,opt,spool}
mkdir -pv /var/lib/{color,misc,locate}

ln -sfv /run /var/run
ln -sfv /run/lock /var/lock

install -dv -m 0750 /root
install -dv -m 1777 /tmp /var/tmp

#  历史上，Linux 曾在 /etc/mtab 维护已经挂载的文件系统的列表。现代内核在内部维护该列表，并通过 /proc 文件系统将它展示给用户。为了满足一些仍然使用 /etc/mtab 的工具，执行以下命令，创建符号链接： 
ln -sv /proc/self/mounts /etc/mtab

# 创建一个基本的 /etc/hosts 文件，一些测试套件，以及 Perl 的一个配置文件将会使用它： 
cat > /etc/hosts << EOF
127.0.0.1  localhost $(hostname)
::1        localhost
EOF

# 为了使得 root 能正常登录，而且用户名 “root” 能被正常识别，必须在文件 /etc/passwd 和 /etc/groups 中写入相关的条目
# 执行以下命令创建 /etc/passwd 文件：
cat > /etc/passwd << "EOF"
root:x:0:0:root:/root:/bin/bash
bin:x:1:1:bin:/dev/null:/usr/bin/false
daemon:x:6:6:Daemon User:/dev/null:/usr/bin/false
messagebus:x:18:18:D-Bus Message Daemon User:/run/dbus:/usr/bin/false
uuidd:x:80:80:UUID Generation Daemon User:/dev/null:/usr/bin/false
nobody:x:65534:65534:Unprivileged User:/dev/null:/usr/bin/false
EOF

# 创建 /etc/group 文件
cat > /etc/group << "EOF"
root:x:0:
bin:x:1:daemon
sys:x:2:
kmem:x:3:
tape:x:4:
tty:x:5:
daemon:x:6:
floppy:x:7:
disk:x:8:
lp:x:9:
dialout:x:10:
audio:x:11:
video:x:12:
utmp:x:13:
usb:x:14:
cdrom:x:15:
adm:x:16:
messagebus:x:18:
input:x:24:
mail:x:34:
kvm:x:61:
uuidd:x:80:
wheel:x:97:
users:x:999:
nogroup:x:65534:
EOF

# 设置 root 密码
```

这里创建的用户组并不属于任何标准 —— 它们一部分是为了满足第 9 章中 Udev 配置的需要，另一部分借鉴了一些 Linux 发行版的通用惯例。另外，某些测试套件需要特定的用户或组。Linux Standard Base (LSB，可以在 https://refspecs.linuxfoundation.org/lsb.shtml 查看) 标准只推荐以组 ID 0 创建用户组 root，以及以组 ID 1 创建用户组 bin。组 ID 5 被几乎所有发行版分配给 tty 组，而且 /etc/fstab 为 devpts 文件系统直接指定了数值 5。其他组名和组 ID 由系统管理员自由分配，因为好的程序不会依赖组 ID 的数值，而是使用组名。

编号 65534 被内核用于 NFS 和用户命名空间，以表示未映射的用户或组 (它们存在于 NFS 服务器或上一级用户命名空间，但是在当前机器或命名空间中“不存在”)。我们为 nobody 和 nogroup 分配该编号，以避免出现未命名的编号。但是其他发行版可能用不同方式处理这个编号，因此需要移植的程序不能依赖于这里给出的分配方式。

 为了移除 “I have no name!” 提示符，需要打开一个新 shell。由于已经创建了文件 /etc/passwd 和 /etc/group，用户名和组名现在就可以正常解析了：
 ```shell
 exec /usr/bin/bash --login
 ```

login、agetty 和 init 等程序使用一些日志文件，以记录登录系统的用户和登录时间等信息。然而，这些程序不会创建不存在的日志文件。初始化日志文件，并为它们设置合适的访问权限：
```
touch /var/log/{btmp,lastlog,faillog,wtmp}
chgrp -v utmp /var/log/lastlog
chmod -v 664  /var/log/lastlog
chmod -v 600  /var/log/btmp
```
文件 /var/log/wtmp 记录所有的登录和登出，文件 /var/log/lastlog 记录每个用户最后登录的时间，文件 /var/log/faillog 记录所有失败的登录尝试，文件 /var/log/btmp 记录所有错误的登录尝试。

> 此处如果在编译中执行 `./configure`， 先检查是否是没有执行软连接创建 /usr/bin/bash -> /usr/bin/sh
> 许多脚本中使用sh解析器

2. 安装 gettext

```shell
./configure --disable-shared

make

# 安装
cp -v gettext-tools/src/{msgfmt,msgmerge,xgettext} /usr/bin
```

3. 安装 bison

```shell
./configure --prefix=/usr \
            --docdir=/usr/share/doc/bison-3.8.2

make
make install
```

4. 安装 perl
```shell
sh Configure -des                                        \
             -Dprefix=/usr                               \
             -Dvendorprefix=/usr                         \
             -Duseshrplib                                \
             -Dprivlib=/usr/lib/perl5/5.38/core_perl     \
             -Darchlib=/usr/lib/perl5/5.38/core_perl     \
             -Dsitelib=/usr/lib/perl5/5.38/site_perl     \
             -Dsitearch=/usr/lib/perl5/5.38/site_perl    \
             -Dvendorlib=/usr/lib/perl5/5.38/vendor_perl \
             -Dvendorarch=/usr/lib/perl5/5.38/vendor_perl
```

`-des`：这是三个选项的组合：-d 对于所有配置项目使用默认值；-e 确保所有配置任务完成；-s 使得配置脚本不输出不必要的信息。

5. 安装python

```shell
./configure --prefix=/usr   \
            --enable-shared \
            --without-ensurepip

make
make install
```

`--enable-shared`：该选项防止安装静态库。
`--without-ensurepip`：该选项禁止构建 Python 软件包安装器，它在当前阶段没有必要。

6. texinfo

```shell
./configure --prefix=/usr
make
make install
```

7. util-linux

```shell
mkdir -pv /var/lib/hwclock

./configure ADJTIME_PATH=/var/lib/hwclock/adjtime    \
            --libdir=/usr/lib    \
            --runstatedir=/run   \
            --docdir=/usr/share/doc/util-linux-2.39.1 \
            --disable-chfn-chsh  \
            --disable-login      \
            --disable-nologin    \
            --disable-su         \
            --disable-setpriv    \
            --disable-runuser    \
            --disable-pylibmount \
            --disable-static     \
            --without-python

make
make install
```

`ADJTIME_PATH=/var/lib/hwclock/adjtime`：该选项根据 FHS 的规则，设定硬件时钟信息记录文件的位置。对于临时工具，这并不是严格要求的，但是这样可以防止在其他位置创建该文件，导致这个文件在安装最终的 Util-linux 软件包时不被覆盖或移除。
`--libdir=/usr/lib`：该选项确保 .so 符号链接直接指向同一目录 (/usr/lib) 中的共享库文件。
`--disable-*`：这些选项防止产生关于一些组件的警告，这些组件需要一些 LFS 之外，或当前尚未安装的软件包。
`--without-python`：该选项禁用 Python，防止构建系统尝试构建不需要的语言绑定。
`runstatedir=/run`：该选项正确设定 uuidd 和 libuuid 使用的套接字的位置。

8. 清理
在现代 Linux 系统中，libtool 的 .la 文件仅用于 libltdl。LFS 中没有库通过 libltdl 加载，而且已知一些 .la 文件会导致 BLFS 软件包出现异常。现在删除这些文件：
```shell
find /usr/{lib,libexec} -name \*.la -delete
```

9. 备份LFS

备份时候退出 lfs 的 chroot 环境

10. 升级

下面是您在升级时必须注意的重点，特别是升级正在运行的系统时：
- 如果需要升级 Linux 内核 (例如，从 5.10.17 升级到 5.10.18 或 5.11.1)，则不需要重新构建其他任何软件包。因为内核态与用户态的接口十分清晰，系统仍然能够继续正常工作。特别地，在升级内核时，不需要 (也不应该，详见下一项说明) 一同更新 Linux API 头文件。重新引导系统即可使用升级后的内核。 
- 如果需要升级 Linux API 头文件或 Glibc (例如从 Glibc-2.31 升级到 Glibc-2.32)，最安全的方法是重新构建 LFS。尽管您或许能按依赖顺序重新构建所有软件包，但我们不推荐这样做。 
- 如果更新了一个包含共享库的软件包，而且共享库的名称发生改变，那么所有动态链接到这个库的软件包都需要重新编译，以链接到新版本的库。(注意软件包的版本和共享库的名称没有关系。) 例如，考虑一个软件包 foo-1.2.3 安装了名为 libfoo.so.1 的共享库，如果您把该软件包升级到了新版本 foo-1.2.4，它安装了名为 libfoo.so.2 的共享库。那么，所有链接到 libfoo.so.1 的软件包都要重新编译以链接到 libfoo.so.2。注意，您不能删除旧版本的库，直到将所有依赖它的软件包都重新编译完成。 
- 如果一个软件包 (直接或间接地) 链接到同一共享库的旧名称和新名称 (例如，同时链接到 libfoo.so.2 和 libbar.so.1，而后者又链接到 libfoo.so.3)，这个软件包可能无法正常工作，这是由于不同版本的共享库可能对同一符号名提供不兼容的定义。这种情况可能由于在更新提供共享库的软件包后，重新编译一部分 (而非所有) 链接到旧的共享库的软件包而出现。为了避免这种问题，用户需要在共享库名称被更新时，尽快重新构建所有链接到该共享库的软件包。
- 如果更新了一个包含共享库的软件包，且共享库的名称没有改变，但是库文件的版本号降低了 (例如，库的名称保持 libfoo.so.1 不变，但是库文件名由 libfoo.so.1.25 变为 libfoo.so.1.24)，则需要删除旧版本软件包安装的库文件 (对于上述示例，需要删除 libfoo.so.1.25)。否则，ldconfig 命令 (可能是您通过命令行执行，也可能由一些软件包的安装过程自动执行) 会将符号链接 libfoo.so.1 的目标重设为旧版本的库文件，因为它版本号更大，看上去更“新”。在不得不降级软件包，或者软件包作者更改库文件版本号格式时，可能出现这种问题。 
- 如果更新了一个包含共享库的软件包，且共享库的名称没有改变，但是这次更新修复了一个严重问题 (特别是安全缺陷)，则要重新启动所有链接到该库的程序。在更新软件包的过程完成后，以 root 身份，运行以下命令，即可列出所有正在使用旧版本共享库的进程 (将 libfoo 替换成库名)： `grep -l 'libfoo.*deleted' /proc/*/maps | tr -cd 0-9\\n | xargs -r ps u`。如果正在使用 OpenSSH 访问系统，且它链接到了被更新的库，则需要重启 sshd 服务，登出并重新登录，然后再次运行上述命令，确认没有进程使用被删除的库文件。 
- 如果一个可执行程序或共享库被覆盖，正在使用该程序或库中的代码或数据的进程可能崩溃。正确的，不会导致进程崩溃的更新程序或共享库的方法是：先删除旧版本，再安装新版本。Coreutils 提供的 install 已经实现了这一过程，多数软件包使用该命令安装二进制文件和库。这意味着在更新软件包时通常不会遇到这个问题。然而，一些软件包 (如 BLFS 中的 Mozilla JS) 的安装过程会简单覆盖已经存在的文件并导致进程崩溃，因此在进行更新前，最好保存工作并关闭不需要的，正在运行的进程。

## 软件包管理技术

以下是几种常见的软件包管理方案
1. 安装到独立的目录，每次更新后创建软连接到 `/usr/bin` 或 `/usr/lib` 之下。一旦安装了比较多的软件包，这种模式就会变得无法管理。 
2. 符号链接风格的软件包管理：这是前一种软件包管理技术的变种。和前一种方式一样，将各个软件包同样安装在独立的目录中。但不是使用软件包的名称建立符号链接，而是将软件包中的每个文件符号链接到 /usr 目录树中对应的位置。这样就不需要修改环境变量。虽然这些符号链接可以由用户自己创建，但已经有许多包管理器能够自动化这一过程。一些流行的包管理器如 Stow、Epkg、Graft 和 Depot 使用这种管理方式。
3. 基于时间戳的方案： 在这种方案中，安装一个软件包之前，创建一个时间戳文件。在安装后，用一行简单的 find 命令，加上正确的参数，就能生成安装日志，包含在时间戳文件创建以后安装的所有文件。有一个采用这个方案的包管理器叫做 install-log。尽管这种方式很简单，但它有两个缺点。如果在安装过程中，某些文件没有以当前时间作为时间戳安装，它们就不能被包管理器跟踪。另外，只有每次只安装一个软件包时才能使用这种技术。如果在两个控制台中同时安装两个不同的软件包，它们的安装日志就不可靠了。 
4. 追踪安装脚本： 在这种方案中，安装脚本执行的命令被记录下来。有两种技巧可以用于记录：在安装前设置 LD_PRELOAD 环境变量，将其指向一个库以在安装过程中预加载它。在安装过程中，这个库附加在 cp、install、mv 等可执行文件上，跟踪修改文件系统的系统调用。如果要使用这种方法，所有需要跟踪的可执行文件必须是动态链接的，且没有设定 suid 和 sgid 位。预加载动态库可能在安装过程中导致不希望的副作用。因此，最好在实际使用前进行一些测试，以确保包管理器不会造成破坏，并且记录了所有应该记录的文件。第二种技巧是使用 strace，它能够记录安装脚本执行过程中的所有系统调用。 
5. 创建软件包档案：在这种方案中，软件包被伪装安装到一个独立的目录树中，就像软链接风格的软件包管理那样。在安装后，使用被安装的文件创建一个软件包档案。它可以被用来在本地机器甚至其他机器上安装该软件包。 大多数商业发行版的包管理器采用这种策略。例如 RPM、pkg-utils、debian 的apt、gentoo的portage等
6. 基于用户的软件包管理：这种架构是 LFS 特有的，由 Matthias Benkmann 提出，可以在 Hints Project 查阅。在该架构中，每个软件包都由一个单独的用户安装到标准位置。只要检查文件所有者，就能找出属于一个软件包的所有文件。

## LFS可引导

1. 创建 /etc/fstab 文件
2. 在文件系统中安装内核、内核模块、内核符号文件、内核文档、内核配置、配置内核模块加载顺序`/etc/modprobe.d/`
3. 制作 grub 可引导盘(非必需)
    ```shell
    grub-mkrescure --output=grub-img.iso
    xorriso -as cdrecord -v dev=/dev/cdrw blank=as_needed grub-img.iso
    # xorriso 包含在 libisoburn 中

    GRUB 命名惯例：分区名的命名形式为(hdn,m)，这里的n表示硬盘驱动器号，m表示分区编号。硬盘驱动器号从0开始，分区对于主分区来说从1开始
    例如：分区sda1在GRUB中的名字是(hd0,1)，而sdb3名字是(hd1,3)
    GRUB与Linux不同，不认为CD-ROM驱动器属于硬盘驱动器。例如，如果在hdb上有一个CD-ROM驱动器，而hdc上有第二个硬盘驱动器，则第二个硬盘驱动器仍然命名为hd1
    ```
4. grub 引导系统，GRUB的工作方式是，将数据写入硬盘的第一个物理磁道，启动时候第一个物理磁道的程序从引导分区加载GRUB模块，默认在/boot/grub中查找模块。 grub-install
5. 创建GRUB配置文件
    ```shell
    cat > /boot/grub/grub.cfg << "EOF"
    # Begin /boot/grub/grub.cfg
    set default=0
    set timeout=5

    insmod part_gpt
    insmod ext2
    set root=(hd0,2)

    menuentry "GNU/Linux, Linux 6.4.12-lfs-12.0" {
        linux   /boot/vmlinuz-6.4.12-lfs-12.0 root=/dev/sda2 ro
    }
    EOF
    ```
    需要注意的是，set root 指定的磁盘分区是相对于内核镜像来的，上述 set root 的方法在多块设备的时候可能会出错，因此需要使用UUID替换
    search --set=root --fs-uuid <内核所在文件系统的 UUID>
    并把 root=/dev/sda2 设置为UUID
