#!/bin/bash

set +e

build='x86_64-gcc10'
target='x86_64-none-linux-gnu'

sysroot="/data/build/$build/sysroot/"

[[ ! -d $sysroot/usr/lib ]] && mkdir -p $sysroot/usr/lib
[[ ! -d $sysroot/usr/bin ]] && mkdir -p $sysroot/usr/bin
[[ ! -d $sysroot/lib ]] && cd $sysroot && ln -s usr/lib lib && cd -
[[ ! -d $sysroot/bin ]] && cd $sysroot && ln -s usr/bin bin && cd -
[[ ! -d $sysroot/sbin ]] && cd $sysroot && ln -s usr/bin sbin && cd -

cd ..
case $(uname -m) in
    i?86)   ln -sfv ld-linux.so.2 $sysroot/lib/ld-lsb.so.3
    ;;
    x86_64) ln -sfv ../lib/ld-linux-x86-64.so.2 $sysroot/lib64
            ln -sfv ../lib/ld-linux-x86-64.so.2 $sysroot/lib64/ld-lsb-x86-64.so.3
    ;;
esac
cd -

echo "rootsbindir=/usr/sbin" > configparms

../configure \
      --disable-werror                    \
      --prefix=/usr                       \
      --host=$target                      \
      --build=$(../scripts/config.guess)  \
      --enable-kernel=4.19                \
      --with-headers=$sysroot/usr/include \
      --disable-nscd                      \
      libc_cv_slibdir=/usr/lib

make -j$(nproc)
make DESTDIR=$sysroot install
sed '/RTLDLIST=/s@/usr@@g' -i $sysroot/usr/bin/ldd

echo 'int main(){}' | $target-gcc -xc -
readelf -l a.out | grep ld-linux
