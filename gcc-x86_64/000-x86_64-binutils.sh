#!/bin/bash

build='x86_64-gcc10'
target='x86_64-none-linux-gnu'

sysroot="/data/build/$build/sysroot/"

../configure \
    --prefix=$sysroot/tools \
    --with-sysroot=$sysroot \
    --target=$target        \
    --disable-nls           \
    --enable-gprofng=no     \
    --disable-werror        \
    --enable-default-hash-style=gnu

make -j$(nproc)
make install
