#!/bin/bash

build='x86_64-gcc10'
target='x86_64-none-linux-gnu'

sysroot="/data/build/$build/sysroot/"

[[ ! -d $sysroot/usr ]] && mkdir $sysroot/usr

cd ..

export INSTALL_HDR_PATH=$sysroot/usr/include
make mrproper
make headers_install
find user/include -type f ! -name '*.h' -delete
cp -rv usr/include $sysroot/usr

cd -

