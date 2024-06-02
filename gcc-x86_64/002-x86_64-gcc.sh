#!/bin/bash

set -e

build='x86_64-gcc10'
target='x86_64-none-linux-gnu'

sysroot="/data/build/$build/sysroot/"
gccBase="/data/build/$build/sysroot/tools"

PATH=/usr/bin:$gccBase/bin
export PATH

cd ..
case $(uname -m) in
  x86_64)
    sed -e '/m64=/s/lib64/lib/' -i.orig gcc/config/i386/t-linux64
 ;;
esac
cd -

../configure                  \
    --target=$target          \
    --prefix=$sysroot/tools   \
    --with-glibc-version=2.34 \
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

make -j$(nproc)
make DESTDIR=${sysroot}/tools install

cd ..
cat gcc/limitx.h gcc/glimits.h gcc/limity.h > $sysroot/usr/include/limits.h
cd -
