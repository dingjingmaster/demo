#!/bin/bash

rdlogger_start() 
{
    # rd.debug implies rd.log=console if rd.log(=.*)? isn't present
    rd_logmask=0

    #[ "$rd_logmask" -gt 0 ] || return

    mkfifo /run/initramfs/rdlogger.pipe

    rdlogger </run/initramfs/rdlogger.pipe >/dev/console 2>&1 &
    printf %s "$!" >/run/initramfs/rdlogger.pid

    exec >/run/initramfs/rdlogger.pipe 2>&1
    #[ -n "$rd_debug" ] && set -x

    # messages would be otherwise lost if we don't unset quiet. this does,
    # however, mean that passing rd.log=console will negate the effects of
    # 'quiet' for initramfs console output.
    unset quiet
}

rdlogger_stop() 
{
    local i=0 pid

    [ -e /run/initramfs/rdlogger.pipe ] || return

    [ -n "$rd_debug" ] && { set +x; } 2>/dev/null

    # this nudges rdlogger to exit
    exec 0<>/dev/console 1<>/dev/console 2<>/dev/console

    # wait up to 1 second for a graceful shutdown
    until [ ! -e /run/initramfs/rdlogger.pipe ] || [ "$i" -eq 10 ]; do
        sleep 0.1
        i="$(( i + 1 ))"
    done

    if [ "$i" -eq 10 ]; then
        # racy! the logger might still go away on its own
        read -r pid </run/initramfs/rdlogger.pid 2>/dev/null
        if [ -n "$pid" ]; then
            kill "$pid" 2>/dev/null
        fi
    fi
}

rdlogger() 
{
    local line

    # establish logging FDs. Either redirect to an appropriate target or to
    # /dev/null. This way, log methods can simply write and the associated FD
    # will Do The Right Thing™.

    # rd.log=console
    if bitfield_has_bit "$rd_logmask" "$_rdlog_cons"; then
        exec 4>/dev/console
    else
        exec 4>/dev/null
    fi

    # rd.log=kmsg
    if [ -c /dev/kmsg ] && bitfield_has_bit "$rd_logmask" "$_rdlog_kmsg"; then
        exec 5>/dev/kmsg
    else
        exec 5>/dev/null
    fi

    # rd.log=file
    if bitfield_has_bit "$rd_logmask" "$_rdlog_file"; then
        exec 6>/run/initramfs/init.log
    else
        exec 6>/dev/null
    fi

    while read -r line; do
        # rd.log=console
        printf '%s\n' "$line" >&4

        # rd.log=kmsg
        log_kmsg '%s' "$line" >&5

        # rd.log=file
        printf '%s\n' "$line" >&6
    done

    # EOF, shutting down...
    exec 4>&- 5>&- 6>&-
    rm -f /run/initramfs/rdlogger.pipe /run/initramfs/rdlogger.pid
}

rdlogger_start
