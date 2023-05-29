#!/bin/bash

function install_package ()
{
    echo "1"
    packages=""
    [[ $# -ne 1 ]] && return 0
    echo "2"

    pack=$(ls -1 $1 | xargs)
    for p in $pack;
    do
        case "$p" in
            *.deb) 
                pname=$(echo $p | awk -F'_' '{print $1}')
                case "$pname" in
                    *-dev) continue ;;
                    *) [[ 0 -eq $(dpkg -l | grep -w $pname | wc -l) ]] && echo "will install $pname.." && packages="${packages} ${p}" ;;
                esac
            ;;
            *) continue ;;
        esac
    done

    [[ ${#packages} -gt 1 ]] && echo "dpkg -i ${packages}" && dpkg -i "${packages}"
}

install_package

echo "==========="
install_package /home/dingjing/


