#!/bin/bash

build='aarch64-gcc10'
target='aarch64-none-linux-gnu'

sysroot="/data/build/$build/sysroot"
gccBase="/data/build/$build/gcc-10.3"

export PATH=$gccBase/bin:$PATH

../configure \
    --prefix=/usr \
    --disable-werror \
    --with-sysroot=$sysroot \
    --host=$target \
    --build=$(build-aux/config.guess) \
    CC=$gccBase/bin/aarch64-none-linux-gnu-gcc \
    CXX=$gccBase/bin/aarch64-none-linux-gnu-g++ \
    CFLAGS=-I$sysroot/usr/include \
    CXXFLAGS=-I$sysroot/usr/include \


make -j$(nproc)
make DESTDIR=$sysroot install
