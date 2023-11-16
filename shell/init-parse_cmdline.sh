#!/bin/bash

startswith() {
    local word="$1" prefix="$2"

    case "$word" in
        $prefix*)
            return 0
            ;;
    esac

    return 1
}

endswith() {
    local word="$1" suffix="$2"

    case "$word" in
        *$suffix)
            return 0
            ;;
    esac

    return 1
}

parse_cmdline_item() {
    local key="$1" value="$2"

    case "$key" in
        rw | ro)
            rwopt="$key"
            ;;
        fstype)
            # The kernel understands 'rootfstype', but mkinitcpio has (without
            # documentation) supported 'fstype' instead. Ensure we support both
            # for backwards compat, but make fstype legacy.
            rootfstype="$value"
            ;;
        fsck.mode)
            case "$value" in
                force)
                    forcefsck=y
                    ;;
                skip)
                    fastboot=y
                    ;;
                *)
                    err "unknown fsck.mode parameter: '$value'"
                    ;;
            esac
            ;;
        rd.debug)
            rd_debug=y
            ;;
        rd.log)
            if [ -n "$value" ]; then
                set_log_option "$value"
            else
                rd_logmask="$(( _rdlog_kmsg | _rdlog_cons ))"
            fi
            ;;
        [![:alpha:]_]* | [[:alpha:]_]*[![:alnum:]_]*)
            # invalid shell variable, ignore it
            ;;
        *)
            # valid shell variable
            eval "$key"='${value:-y}'
            ;;
    esac
}

process_cmdline_param() {
    local item_callback="$1" key="$2" value="$3"

    # maybe unquote the value
    # busybox ash supports string indexing
    # shellcheck disable=SC3057
    if startswith "$value" "[\"']" && endswith "$value" "${value:0:1}"; then
        value="${value#?}" value="${value%?}"
    fi

    #echo "'$item_callback' '$key' '$value'"
    "$item_callback" "$key" "$value"
}



function parse_cmdline() 
{
    local item_callback="${1:-parse_cmdline_item}"
    local cmdline word quoted key value

    set -f
    read -r cmdline
    # shellcheck disable=SC2086
    set -- $cmdline
    set +f

    for word; do
        if [ -n "$quoted" ]; then
            value="$value $word"
        else
            case "$word" in
                *=*)
                    key="${word%%=*}"
                    value="${word#*=}"

                    if startswith "$value" "[\"']"; then
                        # busybox ash supports string indexing
                        # shellcheck disable=SC3057
                        quoted="${value:0:1}"
                    fi
                    ;;
                '#'*)
                    break
                    ;;
                *)
                    key="$word"
                    ;;
            esac
        fi

        if [ -n "$quoted" ]; then
            if endswith "$value" "$quoted"; then
                unset quoted
            else
                continue
            fi
        fi

        echo "process_cmdline_param '$item_callback' '$key' '$value'"
        process_cmdline_param "$item_callback" "$key" "$value"
        unset key value
    done

    if [ -n "$key" ]; then
        echo "process_cmdline_param '$item_callback' '$key' '$value'"
        process_cmdline_param "$item_callback" "$key" "$value"
    fi
}

parse_cmdline < /proc/cmdline
