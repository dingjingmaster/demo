#!/bin/bash

os_name=''
os_version=''

if [ -f /etc/os-release ];then
    os_name=$(cat /etc/os-release | grep '^ID=' | awk -F'=' '{print $2}' | tr '\n' ' ' | sed 's/ //g')
    shopt -s nocasematch
    if [ x"${os_name}" == x"uos" ];then
        os_name="uos"
        if [ -f /etc/os-version ];then
            os_version=$(cat /etc/os-version | grep '^MinorVersion=' | awk -F'=' '{print $2}' | tr '\n' ' ' | sed 's/ //g')
        fi
    elif [ x"${os_name}" == x"kylin" ];then
        os_name="kylin"
        os_version='v10'
    fi
    shopt -u nocasematch
fi

if [ x"${os_name}" == x"uos" ] && [ "$os_version" -ge "1043" ];then
    echo "uos 大于 1043 版本"
elif [ x"${os_name}" == x"uos" ] && [ "$os_version" -lt "1043" ];then
    echo "uos 小于 1043 版本"
elif [ x"${os_name}" == x"kylin" ] && [ x"$os_version" == x"v10" ];then
    echo "kylin"
else
    # 其它国产化发行版
    echo "os: ${os_name} -- version: ${os_version}"
fi

#echo "os: ${os_name} -- version: ${os_version}"
