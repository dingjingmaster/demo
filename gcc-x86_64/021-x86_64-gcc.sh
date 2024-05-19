#!/bin/bash

build='x86_64-gcc10'
target='x86_64-none-linux-gnu'

sysroot="/data/build/$build/sysroot/"

case $(uname -m) in
  x86_64)
    sed -e '/m64=/s/lib64/lib/' \
        -i.orig ../gcc/config/i386/t-linux64
  ;;
esac

sed '/thread_header =/s/@.*@/gthr-posix.h/' \
    -i ../libgcc/Makefile.in ../libstdc++-v3/include/Makefile.in

../configure                                       \
    --build=$(../config.guess)                     \
    --host=$target             \
    --target=$target             \
    LDFLAGS_FOR_TARGET=-L$PWD/$target/libgcc      \
    --prefix=/usr                                  \
    --with-build-sysroot=$sysroot                  \
    --with-sysroot=$sysroot \
    CC=$sysroot/tools/bin/x86_64-none-linux-gnu-gcc \
    CXX=$sysroot/tools/bin/x86_64-none-linux-gnu-g++ \
    LD=$sysroot/tools/bin/x86_64-none-linux-gnu-ld \
    AR=$sysroot/tools/bin/x86_64-none-linux-gnu-ar \
    AS=$sysroot/tools/bin/x86_64-none-linux-gnu-as \
    CFLAGS=-I$sysroot/usr/include \
    CXXFLAGS=-I$sysroot/usr/include \
    LDFLAGS=-I$sysroot/usr/lib \
    --enable-default-pie                           \
    --enable-default-ssp                           \
    --disable-nls                                  \
    --disable-multilib                             \
    --disable-libatomic                            \
    --disable-libgomp                              \
    --disable-libquadmath                          \
    --disable-libsanitizer                         \
    --disable-libssp                               \
    --disable-libvtv                               \
    --enable-languages=c,c++

make -j$(nproc)
make DESTDIR=$sysroot install
ln -sv gcc $LFS/usr/bin/cc
