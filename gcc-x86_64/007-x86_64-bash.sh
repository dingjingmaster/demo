#!/bin/bash

build='x86_64-gcc10'
target='x86_64-none-linux-gnu'

sysroot="/data/build/$build/sysroot/"

../configure \
    --prefix=/usr                      \
    --build=$(sh support/config.guess) \
    --host=$target                     \
    --without-bash-malloc

make
make DESTDIR=$sysroot install
ln -sv bash $sysroot/bin/sh
