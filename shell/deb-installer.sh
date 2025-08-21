#!/bin/bash

set -e


targetVer=""
targetPkg=""
curDir=$(realpath -- $(dirname -- $0))


while IFS= read -r file; do 
	fname="${file##*/}"
	name1="${fname%.*}"
	name2="${name1%_*}"
	ver=$(echo $name2 | awk -F'-' '{print $2}')
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

echo "找到最新安装包: $targetPkg, 版本号: $targetVer"

echo "开始安装..."
pkexec dpkg -i $targetPkg


echo ''
read -n 1 -s -r -p $"安装完成, 按任意键退出..."
