#!/bin/bash

build='x86_64-gcc10'
target='x86_64-none-linux-gnu'

sysroot="/data/build/$build/sysroot/"

sed -i s/mawk// ../configure

mkdir build
pushd build
../../configure \
    --prefix=/usr \
    --disable-werror \
    --host=$target \
    --build=$(build-aux/config.guess) \
    --with-sysroot=$sysroot \

make -C include
make -C progs tic
popd

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
    --enable-widec

make -j$(nproc)
make DESTDIR=$sysroot TIC_PATH=$(pwd)/build/progs/tic install

ln -sv libncursesw.so $sysroot/usr/lib/libncurses.so
sed -e 's/^#if.*XOPEN.*$/#if 1/' -i $sysroot/usr/include/curses.h
