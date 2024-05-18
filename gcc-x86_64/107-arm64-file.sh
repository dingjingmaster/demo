#!/bin/bash

build='aarch64-gcc10'
target='aarch64-none-linux-gnu'

sysroot="/data/build/$build/sysroot"
gccBase="/data/build/$build/gcc-10.3"

export PATH=$gccBase/bin:$PATH
export QEMU_LD_PREFIX=$sysroot

mkdir build
pushd build
../../configure \
    --disable-bzlib      \
    --disable-libseccomp \
    --disable-xzlib      \
    --with-sysroot=$sysroot \
    CC=$gccBase/bin/aarch64-none-linux-gnu-gcc \
    CXX=$gccBase/bin/aarch64-none-linux-gnu-g++ \
    CFLAGS=-I$sysroot/usr/include \
    CXXFLAGS=-I$sysroot/usr/include \
    --disable-zlib
make
popd

../configure --prefix=/usr \
    --with-sysroot=$sysroot \
    CC=$gccBase/bin/aarch64-none-linux-gnu-gcc \
    CXX=$gccBase/bin/aarch64-none-linux-gnu-g++ \
    CFLAGS=-I$sysroot/usr/include \
    CXXFLAGS=-I$sysroot/usr/include \
    --host=$target \
    --build=$(./config.guess)

make FILE_COMPILE=$(pwd)/build/src/file
make DESTDIR=$sysroot install
rm -v $sysroot/usr/lib/libmagic.la

