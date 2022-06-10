#!/bin/bash

# @param1 当前用户名
# @param2 当前共享的文件夹路径 <可选>

function parseopts() 
{
    local opt= optarg= i= shortopts=$1
    local -a longopts=() unused_argv=()

    shift
    while [[ $1 && $1 != '--' ]]; do
        longopts+=("$1")
        shift
    done
    shift

    longoptmatch() {
        local o longmatch=()
        for o in "${longopts[@]}"; do
            if [[ ${o%:} = "$1" ]]; then
                longmatch=("$o")
                break
            fi
            [[ ${o%:} = "$1"* ]] && longmatch+=("$o")
        done

        case ${#longmatch[*]} in
            1)
                opt=${longmatch%:}
                if [[ $longmatch = *: ]]; then
                    return 1
                else
                    return 0
                fi ;;
            0)
                return 255 ;;
            *)
                return 254 ;;
        esac
    }

    while (( $# )); do
        case $1 in
            --) # explicit end of options
                shift
                break
                ;;
            -[!-]*) # short option
                for (( i = 1; i < ${#1}; i++ )); do
                    opt=${1:i:1}

                    # option doesn't exist
                    if [[ $shortopts != *$opt* ]]; then
                        OPTRET=(--)
                        return 1
                    fi

                    OPTRET+=("-$opt")
                    # option requires optarg
                    if [[ $shortopts = *$opt:* ]]; then
                        if (( i < ${#1} - 1 )); then
                            OPTRET+=("${1:i+1}")
                            break
                        elif (( i == ${#1} - 1 )) && [[ "$2" ]]; then
                            OPTRET+=("$2")
                            shift
                            break
                        # parse failure
                        else
                            OPTRET=(--)
                            return 1
                        fi
                    fi
                done
                ;;
            --?*=*|--?*) # long option
                IFS='=' read -r opt optarg <<< "${1#--}"
                longoptmatch "$opt"
                case $? in
                    0)
                        if [[ $optarg ]]; then
                            OPTRET=(--)
                            return 1
                        else
                            OPTRET+=("--$opt")
                        fi
                        ;;
                    1)
                        if [[ $optarg ]]; then
                            OPTRET+=("--$opt" "$optarg")
                        elif [[ "$2" ]]; then
                            OPTRET+=("--$opt" "$2" )
                            shift
                        else
                            printf "%s: 配置选项 '--%s' 需要参数!\n" "${0##*/}" "$opt"
                            OPTRET=(--)
                            return 1
                        fi
                        ;;
                    254)
                        OPTRET=(--)
                        return 1
                        ;;
                    255)
                        # parse failure
                        printf "%s: 未定义的配置选项 '%s'\n" "${0##*/}" "--$opt"
                        OPTRET=(--)
                        return 1
                        ;;
                esac
                ;;
            *) # non-option arg encountered, add it as a parameter
                unused_argv+=("$1")
                ;;
        esac
        shift
    done

    # add end-of-opt terminator and any leftover positional parameters
    OPTRET+=('--' "${unused_argv[@]}" "$@")
    unset longoptmatch

    return 0
}

function cleanup()
{
    exit ${1:-$?}
}

function usage() 
{
    cat <<EOF
使用: ${0##*/} [配置选项]

  配置选项:
   -u, --username <用户名>      将用户添加到 sambashare 组 
   -p, --path <samba共享路径>   将当前目录及其祖宗目录都赋予可执行权限
   -h, --help                   显示此帮助信息并退出

EOF
}

function add_user_sambashare()
{
    #printf "name: $1\n"
    if [[ "x$1" != "x" && "" != $(cat /etc/passwd | awk -F':' '{print $1}' | grep "$1") 
        && "" == $(groups "$1" | grep "sambashare") ]]; then
        usermod -a -G sambashare "$1"
        #printf "usermod -a -G sambashare $1\n"
    fi
}

function add_execute_dir()
{
    printf "path: $1\n"
    if [[ "x$1" != "x" && "x/" == "x${1:0:1}"  ]]; then
        printf "is a absolute path: $1\n"
        path="$1"
        while [ "x$path" != "x/" ];
        do
            if [ "x$path" != "x/home" ]; then
                continue
            fi
            chmod o+x "$path"
            printf "chmod o+x $path\n"
            path=$(dirname ${path})
        done
    fi
}



### main
trap 'cleanup 130' INT
trap 'cleanup 143' TERM

if [ "$#" -lt 1 ]; then
    usage
    cleanup 1
fi

_opt_short='u:p:h'
_opt_long=('username:' 'path:' 'help')

parseopts "$_opt_short" "${_opt_long[@]}" -- "$@" || exit 1
set -- "${OPTRET[@]}"
unset _opt_short _opt_long OPTRET

while :; do
    case $1 in
        -u|--username)
            shift
            # 添加用户到 sambashare 组
            add_user_sambashare "$1"
            ;;
        -p|--path)
            shift
            # 修改当前目录及祖宗目录并添加可执行权限
            add_execute_dir "$1"
            ;;
        -h|--help)
            usage
            cleanup 0
            ;;
        --)
            shift
            break 2
            ;;
    esac
    shift
done


