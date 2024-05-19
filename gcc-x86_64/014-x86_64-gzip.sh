#!/bin/bash

build='x86_64-gcc10'
target='x86_64-none-linux-gnu'

sysroot="/data/build/$build/sysroot/"

../configure --prefix=/usr   \
            --host=$target                 \
            --build=$(build-aux/config.guess)

make
make DESTDIR=$sysroot install

