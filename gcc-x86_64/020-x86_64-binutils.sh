#!/bin/bash

build='x86_64-gcc10'
target='x86_64-none-linux-gnu'

sysroot="/data/build/$build/sysroot/"

sed '6009s/$add_dir//' -i ../ltmain.sh
sed -e "s/AR=\"x86_64-none-linux-gnu-ar/$sysroot\/tools\/bin\/x86_64-none-linux-gnu-ar/g" -i bfd/libtool

../configure                   \
    --prefix=/usr              \
    --build=$(../config.guess) \
    --host=$target             \
    --disable-nls              \
    --enable-shared            \
    --enable-gprofng=no        \
    --disable-werror           \
    --enable-64-bit-bfd        \
    --with-sysroot=$sysroot \
    CC=$sysroot/tools/bin/x86_64-none-linux-gnu-gcc \
    CXX=$sysroot/tools/bin/x86_64-none-linux-gnu-g++ \
    LD=$sysroot/tools/bin/x86_64-none-linux-gnu-ld \
    AS=$sysroot/tools/bin/x86_64-none-linux-gnu-as \
    AR=$sysroot/tools/bin/x86_64-none-linux-gnu-ar \
    CFLAGS=-I$sysroot/usr/include \
    CXXFLAGS=-I$sysroot/usr/include \
    LDFLAGS=-I$sysroot/usr/lib \
    --enable-default-hash-style=gnu

make
make DESTDIR=$sysroot install

