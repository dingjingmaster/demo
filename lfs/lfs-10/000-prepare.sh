#!/bin/bash
# 'https://mirrors.aliyun.com/lfs/lfs-packages/10.0/?spm=a2c6h.25603864.0.0.9bef3436AXWsJw'

curDir=$(dirname -- $(realpath -- $0))
lfsDir=${curDir}/fs
srcDir=${curDir}/src

export LFS=${lfsDir}

mkdir -pv ${lfsDir} \
    ${lfsDir}/home \
    ${lfsDir}/dev \
    ${lfsDir}/proc \
    ${lfsDir}/sys \
    ${lfsDir}/var \
    ${lfsDir}/boot \
    ${lfsDir}/efi \
    ${lfsDir}/media \
    ${lfsDir}/mnt \
    ${lfsDir}/share \
    ${lfsDir}/tmp \
    ${lfsDir}/etc \
    ${lfsDir}/opt \
    ${lfsDir}/run \
    ${lfsDir}/srv \
    ${lfsDir}/usr \
    ${lfsDir}/usr/bin \
    ${lfsDir}/usr/lib \
    ${lfsDir}/usr/share \

cd ${lfsDir}
ln -sf usr/lib lib
ln -sf usr/bin bin
ln -sf usr/bin sbin
cd ${curDir}

# 源码目录
mkdir -p ${srcDir}
wget --input-file=${curDir}/wget-list --continue --directory-prefix=${srcDir} 

pushd ${srcDir}
    md5sum -c ${curDir}/md5sum
popd

