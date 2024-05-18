#!/bin/bash

build='aarch64-gcc10'
target='aarch64-none-linux-gnu'

sysroot="/data/build/$build/sysroot"
gccBase="/data/build/$build/gcc-10.3"

export PATH=$gccBase/bin:$PATH
export QEMU_LD_PREFIX=$sysroot

../configure --prefix=/usr \
    --host=$target \
    --build=$(build-aux/config.guess) \
    --enable-install-program=hostname \
    --with-sysroot=$sysroot \
    CC=$gccBase/bin/aarch64-none-linux-gnu-gcc \
    CXX=$gccBase/bin/aarch64-none-linux-gnu-g++ \
    CFLAGS=-I$sysroot/usr/include \
    CXXFLAGS=-I$sysroot/usr/include \
    --enable-no-install-program=kill,uptime

make
make DESTDIR=$sysroot install
mv -v $sysroot/usr/bin/chroot $sysroot/usr/sbin
mkdir -pv $sysroot/usr/share/man/man8
mv -v $sysroot/usr/share/man/man1/chroot.1 $sysroot/usr/share/man/man8/chroot.8
sed -i 's/"1"/"8"/' $sysroot/usr/share/man/man8/chroot.8

