#!/bin/bash

set -e

targetVer=""
targetPkg=""
targetArch="$(uname -m)"
curDir=$(realpath -- $(dirname -- $0))


if [ "x86_64" == "$targetArch" ]; then
	targetArch="x86"
elif [ "X86_64" == "$targetArch" ]; then
	targetArch="x86"
elif [ "aarch" == "$targetArch" ]; then
	targetArch="arm"
elif [ "aarch64" == "$targetArch" ]; then
	targetArch="arm"
elif [ "arm64" == "$targetArch" ]; then
	targetArch="arm"
fi

while IFS= read -r file; do 
	fname="${file##*/}"
	name1="${fname%.*}"
	name2="${name1%_*}"
	arch=$(echo $name1 | awk -F'_' '{print $2}')
	ver=$(echo $name2 | awk -F'-' '{print $2}')
	if [ "$arch" != $targetArch ]; then
		continue
	fi
	if [ "" == "$targetPkg" ]; then
		targetPkg="$file"
		targetVer="$ver"
		continue
	fi
	if dpkg --compare-versions "$targetVer" lt "$ver"; then
		targetPkg=$file
		targetVer=$ver
	fi
done < <(find $curDir -maxdepth 1 -type f -name "andbox-*.deb")

echo "包路径: $targetPkg"
echo "  版本: $targetVer"
echo "  架构: $targetArch"

echo "开始安装..."
pkexec dpkg -i $targetPkg


echo ''
read -n 1 -s -r -p $"安装完成, 按任意键退出..."
