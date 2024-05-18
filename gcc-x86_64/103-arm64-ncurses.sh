#!/bin/bash

build='aarch64-gcc10'
target='aarch64-none-linux-gnu'

sysroot="/data/build/$build/sysroot"
gccBase="/data/build/$build/gcc-10.3"

export PATH=$gccBase/bin:$PATH
export QEMU_LD_PREFIX=$sysroot

sed -i s/mawk// ../configure

mkdir build
pushd build
../../configure \
    --prefix=/usr \
    --disable-werror \
    --host=$target \
    --build=$(build-aux/config.guess) \
    --with-sysroot=$sysroot \
    CC=$gccBase/bin/aarch64-none-linux-gnu-gcc \
    CXX=$gccBase/bin/aarch64-none-linux-gnu-g++ \
    CFLAGS=-I$sysroot/usr/include \
    CXXFLAGS=-I$sysroot/usr/include \

make -C include
make -C progs tic
popd

sed -e "s/^\$TIC_PATH/QEMU_LD_PREFIX=${sysroot} qemu-aarch64 \$TIC_PATH/" -i build/misc/run_tic.sh

../configure \
    --prefix=/usr                \
    --host=$target               \
    --build=$(./config.guess)    \
    --mandir=/usr/share/man      \
    --with-manpage-format=normal \
    --with-shared                \
    --without-normal             \
    --with-cxx-shared            \
    --without-debug              \
    --without-ada                \
    --disable-stripping          \
    --with-sysroot=$sysroot \
    CC=$gccBase/bin/aarch64-none-linux-gnu-gcc \
    CXX=$gccBase/bin/aarch64-none-linux-gnu-g++ \
    CFLAGS=-I$sysroot/usr/include \
    CXXFLAGS=-I$sysroot/usr/include \
    --enable-widec

make -j$(nproc)
make DESTDIR=$sysroot TIC_PATH=$(pwd)/build/progs/tic install

ln -sv libncursesw.so $sysroot/usr/lib/libncurses.so
sed -e 's/^#if.*XOPEN.*$/#if 1/' -i $sysroot/usr/include/curses.h
