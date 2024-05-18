#!/bin/bash

build='x86_64-gcc10'
target='x86_64-none-linux-gnu'

sysroot="/data/build/$build/sysroot/"

../configure --prefix=/usr \
    --host=$target \
    --build=$(build-aux/config.guess) \
    --enable-install-program=hostname \
    --enable-no-install-program=kill,uptime

make
make DESTDIR=$sysroot install
mv -v $sysroot/usr/bin/chroot $sysroot/usr/sbin
mkdir -pv $sysroot/usr/share/man/man8
mv -v $sysroot/usr/share/man/man1/chroot.1 $sysroot/usr/share/man/man8/chroot.8
sed -i 's/"1"/"8"/' $sysroot/usr/share/man/man8/chroot.8
