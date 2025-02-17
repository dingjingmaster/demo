#!/bin/bash

curDir=$(cd $(dirname $0) && pwd)
systemVersion=""
andsecDeps=""
andsecPlatform=""

case $(uname -m) in
	x86_64)
		andsecPlatform="amd64"
		;;
	*)
		echo "Not supported architecture!" && exit 1
		;;
esac

[[ -f /etc/issue ]] && systemVersion=$(cat /etc/issue|awk 'NF > 0 {print tolower($1$2)}')
[[ "${systemVersion}" == "" ]] && echo "Not supported system!" && exit 1


echo ""
echo "CPU               : ${andsecPlatform}"
echo "Current Directory : ${curDir}"
echo "System Version    : ${systemVersion}"
echo ""


# 针对 ubuntu 类型系统, 使用 dpkg 包管理器
if [[ "${systemVersion}" =~ "^ubuntu" ]]; then
	# 安装依赖
	[[ -d "${curDir}/deps/${systemVersion}" ]] && dpkg -i "${curDir}/deps/${systemVersion}/*"
	# 安装 andsec
	[[ -d "${curDir}/andsec/${systemVersion}" ]] && dpkg -i "${curDir}/andsec/andsec_*_${andsecPlatform}.deb"
else
	echo "Not supported system!"
fi

