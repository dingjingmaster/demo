#!/bin/bash

build='aarch64-gcc10'
target='aarch-none-linux-gnu'

sysroot="/data/build/$build/sysroot"
gccBase="/data/build/$build/gcc-10.3"

export PATH=$gccBase/bin:$PATH


../configure \
    --prefix=$sysroot 	    \
    --with-sysroot=$sysroot \
    -release                \
    -opensource             \
    -confirm-license        \
    -qpa xcb                \
    -no-kms                 \
    -qt-pcre                \
    -nomake examples -no-opengl \
    -xplatform linux-aarch64-gnu-g++ \
    CROSS_COMPILE=$gccBase/aarch64-none-linux-gnu


make -j$(nproc)
make install
