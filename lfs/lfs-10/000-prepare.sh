#!/bin/bash
# 'https://mirrors.aliyun.com/lfs/lfs-packages/10.0/?spm=a2c6h.25603864.0.0.9bef3436AXWsJw'

curDir=$(dirname -- $(realpath -- $0))
lfsDir=${curDir}/fs
srcDir=${curDir}/src
toolsDir=${curDir}/tools
cpuNum=$(nproc)

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
    ${lfsDir}/usr/sbin \
    ${lfsDir}/usr/share 

case $(uname -m) in
    x86_64) mkdir -pv ${lfsDir}/lib64 ;;
esac

cd ${lfsDir}
ln -sf usr/lib lib
ln -sf usr/bin bin
ln -sf usr/sbin sbin
cd ${curDir}

# cross-compiler
mkdir -pv ${toolsDir}

# 源码目录
function download_source_code()
{
    mkdir -p ${srcDir}
    wget --input-file=${curDir}/wget-list --continue --directory-prefix=${srcDir} 

    pushd ${srcDir}
        md5sum -c ${curDir}/md5sums
    popd
}

set +h

umask 022

LC_ALL=POSIX
LFS=${lfsDir}
LFS_TGT=$(uname -m)-lfs-linux-gnu
#PATH=/usr/bin
#if [ ! -L /bin ]; then PATH=/bin:$PATH; fi
#PATH=$LFS/tools/bin:$PATH
CONFIG_SITE=$LFS/usr/share/config.site
export LFS LC_ALL LFS_TGT CONFIG_SITE # PATH


#### compile
# binutils
# 汇编、操作二进制文件和目标文件的一组程序
function cross_binutils()
{
    cd ${srcDir}
    [[ ! -d ./binutils-2.35 ]] && tar xf ./binutils-2.35.tar.xz
    cd ./binutils-2.35
    ./configure --prefix=${toolsDir} \
        --with-sysroot=$LFS \
        --target=$LFS_TGT \
        --disable-nls \
        --enable-gprofng=no \
        --disable-werror \
        --enable-default-hash-style=gnu
    make -j${cpuNum}
    make install
    cd ${srcDir}
}

function cross_gcc()
{
    cd ${srcDir}
    [[ ! -d ./mpfr ]] && tar xf ./mpfr-4.1.0.tar.xz && mv mpfr-4.1.0 mpfr
    [[ ! -d ./gmp  ]] && tar xf ./gmp-6.2.0.tar.xz  && mv gmp-6.2.0  gmp
    [[ ! -d ./mpc  ]] && tar xf ./mpc-1.1.0.tar.gz  && mv mpc-1.1.0  mpc
    [[ ! -d ./gcc-10.2.0 ]] && tar xf ./gcc-10.2.0.tar.xz

    cd ./gcc-10.2.0

    case $(uname -m) in
        x86_64)
            sed -e '/m64=/s/lib64/lib/' \
                -i.orig gcc/config/i386/t-linux64
        ;;
    esac

    ./configure                  \
    --target=$LFS_TGT         \
    --prefix=$toolsDir        \
    --with-glibc-version=2.39 \
    --with-sysroot=$LFS       \
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
    make -j${cpuNum}
    make install
    cd ${srcDir}
}

function linux_kernel_header()
{
    cd ${srcDir}

    [[ ! -d ./linux-5.8.3 ]] && tar xf ./linux-5.8.3.tar.xz

    cd ./linux-5.8.3

    make mrproper
    make headers
    find usr/include -type f ! -name '*.h' -delete
    cp -rv usr/include $LFS/usr

    cd ${srcDir}
}

function compile_glibc()
{
    cd ${srcDir}

    [[ ! -d ./glibc-2.32 ]] && tar xf ./glibc-2.32.tar.xz

    cd ./glibc-2.32
    mkdir ./build
    cd ./build
    patch -Np1 -i ../glibc-2.32-fhs-1.patch
    echo "rootsbindir=/usr/sbin" > configparms
    ../configure                         \
      --prefix=/usr                      \
      --host=$LFS_TGT                    \
      --build=$(../scripts/config.guess) \
      --enable-kernel=4.19               \
      --with-headers=$LFS/usr/include    \
      --disable-nscd                     \
      --disable-werror                   \
      libc_cv_slibdir=/usr/lib

    make -j${cpuNum}
    make DESTDIR=$LFS install
    sed '/RTLDLIST=/s@/usr@@g' -i $LFS/usr/bin/ldd

    cd ${srcDir}
}

# stdc++
function compile_cpp()
{
    cd ${srcDir}
    cross_gcc

    cd ./gcc-10.2.0
    ./libstdc++-v3/configure            \
        --host=$LFS_TGT                 \
        --build=$(./config.guess)       \
        --prefix=/usr                   \
        --disable-multilib              \
        --disable-nls                   \
        --disable-libstdcxx-pch         \
        --with-gxx-include-dir=${toolsDir}/$LFS_TGT/include/c++/10.2.0
    make -j${cpuNum}
    make DESTDIR=$LFS install
    rm -fv $LFS/usr/lib/lib{stdc++{,exp,fs},supc++}.la

    cd ${srcDir}
}

function compile_m4()
{
    cd ${srcDir}
    [[ ! -d ./m4-1.4.18 ]] && tar xf ./m4-1.4.18.tar.xz
    cd ./m4-1.4.18
    patch -Np1 -i ../0001-c-stack.patch
    ./bootstrap
    ./configure --prefix=/usr \
        --host=$LFS_TGT \
        --build=$(build-aux/config.guess)
    make
    make DESTDIR=$LFS install
    cd ${srcDir}
}


# main
#download_source_code
#cross_binutils
#cross_gcc
#linux_kernel_header
#compile_glibc
#compile_cpp
compile_m4
