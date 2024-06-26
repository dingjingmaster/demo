#!/bin/bash

build='x86_64-gcc10'
target='x86_64-none-linux-gnu'

sysroot="/data/build/$build/sysroot/"

../libstdc++-v3/configure \
    --prefix=/usr/                  \
    --host=$target                  \
    --build=$(../config.guess)      \
    --disable-multilib              \
    --disable-nls                   \
    --disable-libstdcxx-pch         \
    --with-gxx-include-dir=$sysroot/tools/$target/include/c++/10.4.0

make -j$(nproc)
make DESTDIR=$sysroot install
#rm -fv $sysroot/usr/lib/lib{stdc++{,exp,fs},supc++}.la
