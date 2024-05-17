#!/bin/bash

../configure                                    \
    --target=x86_64-none-linux-gnu              \
    --prefix=/data/xc/gcc-10.3.0-x86_64-x86_64  \
    --with-glibc-version=2.39                   \
    --with-sysroot=x86_64-none-linux-gnu        \
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
