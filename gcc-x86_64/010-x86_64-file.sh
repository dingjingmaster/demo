#!/bin/bash

build='x86_64-gcc10'
target='x86_64-none-linux-gnu'

sysroot="/data/build/$build/sysroot/"

mkdir build
pushd build
../../configure \
    --disable-bzlib      \
    --disable-libseccomp \
    --disable-xzlib      \
    --disable-zlib
make
popd

../configure --prefix=/usr --host=$target --build=$(./config.guess)

make FILE_COMPILE=$(pwd)/build/src/file
make DESTDIR=$sysroot install
rm -v $sysroot/usr/lib/libmagic.la

