#!/bin/bash

build='aarch64-gcc10'
target='aarch64-none-linux-gnu'

sysroot="/data/build/$build/sysroot"
gccBase="/data/build/$build/gcc-10.3"

export PATH=$gccBase/bin:$PATH
export QEMU_LD_PREFIX=$sysroot


../configure \
    --prefix=/usr                      \
    --build=$(sh support/config.guess) \
    --host=$target                     \
    --with-sysroot=$sysroot \
    CC=$gccBase/bin/aarch64-none-linux-gnu-gcc \
    CXX=$gccBase/bin/aarch64-none-linux-gnu-g++ \
    CFLAGS=-I$sysroot/usr/include \
    CXXFLAGS=-I$sysroot/usr/include \
    --without-bash-malloc

make
make DESTDIR=$sysroot install
ln -sv bash $sysroot/bin/sh
