#!/bin/bash

build='x86_64-gcc10'
target='x86_64-none-linux-gnu'

sysroot="/data/build/$build/sysroot/"

../configure \
    --prefix=/usr \
    --disable-werror \
    --with-sysroot=$sysroot \
    --host=$target \
    --build=$(build-aux/config.guess) \


make -j$(nproc)
make DESTDIR=$sysroot install
