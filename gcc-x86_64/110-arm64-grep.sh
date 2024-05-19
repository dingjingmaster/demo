#!/bin/bash

build='aarch64-gcc10'
target='aarch64-none-linux-gnu'

sysroot="/data/build/$build/sysroot"
gccBase="/data/build/$build/gcc-10.3"

export PATH=$gccBase/bin:$PATH
export QEMU_LD_PREFIX=$sysroot

../configure \
    --prefix=/$sysroot/usr  \
    --disable-extensions \
    --with-sysroot=$sysroot \
    CC=$gccBase/bin/aarch64-none-linux-gnu-gcc \
    CXX=$gccBase/bin/aarch64-none-linux-gnu-g++ \
    LD=$gccBase/bin/aarch64-none-linux-gnu-ld \
    CFLAGS=-I$sysroot/usr/include \
    CXXFLAGS=-I$sysroot/usr/include \
    LDFLAGS=-I$sysroot/usr/lib \
    --host=$target \
    --build=$(build-aux/config.guess)

make
make DESTDIR=$sysroot install

