#!/bin/bash

set +e

build='aarch64-gcc10'
target='aarch-none-linux-gnu'

sysroot="/data/build/$build/sysroot"
gccBase="/data/build/$build/gcc-10.3"

export PATH=$gccBase/bin:$PATH

[[ ! -d $sysroot/usr ]] && mkdir $sysroot/usr

cd ..

export INSTALL_HDR_PATH=$sysroot/usr/include
make mrproper
make ARCH=arm headers_install
find user/include -type f ! -name '*.h' -delete
cp -rv usr/include $sysroot/usr

cd -

