#!/bin/bash

set -e

build='aarch64-gcc10'
target='aarch64-none-linux-gnu'

sysroot="/data/build/$build/sysroot"
gccBase="/data/build/$build/gcc-10.3"

PATH=/usr/bin:$gccBase/bin:${gccBase}/aarch64-none-linux-gnu/bin
export PATH

[[ ! -d $sysroot/usr/lib ]] && mkdir -p $sysroot/usr/lib
[[ ! -d $sysroot/usr/bin ]] && mkdir -p $sysroot/usr/bin
[[ ! -d $sysroot/lib ]] && cd $sysroot && ln -s usr/lib lib && cd -
[[ ! -d $sysroot/bin ]] && cd $sysroot && ln -s usr/bin bin && cd -
[[ ! -d $sysroot/sbin ]] && cd $sysroot && ln -s usr/bin sbin && cd -

cd ..
ln -sfv ../lib/ld-linux-aarch64.so.2 $sysroot/lib64
#ln -sfv ../lib/ld-linux-aarch64.so.2 $sysroot/lib64/ld-linux-aarch64.so
cd -

#echo "rootsbindir=/usr/sbin" > configparms

../configure \
    --enable-debug \
    --prefix=/usr                       \
    --host=$target                      \
    --build=$(../scripts/config.guess)  \
    --enable-kernel=4.19                \
    --disable-werror                    \
    --with-headers=$sysroot/usr/include \
    --disable-nscd                      \
    libc_cv_slibdir=/usr/lib            \
    CC=$gccBase/bin/aarch64-none-linux-gnu-gcc       \
    CXX=$gccBase/bin/aarch64-none-linux-gnu-g++      \
    CFLAGS=-I$sysroot/usr/include       \
    CXXFLAGS=-I$sysroot/usr/include           

make -d -j$(nproc)
#make DESTDIR=$sysroot install
#sed '/RTLDLIST=/s@/usr@@g' -i $sysroot/usr/bin/ldd

#echo 'int main(){}' | $target-gcc -xc -
#readelf -l a.out | grep ld-linux
