set(HEADERS
        alloca.h
        afunix.h
        crt_externs.h
        dirent.h
        float.h
        fstab.h
        grp.h
        inttypes.h
        limits.h
        locale.h
        mach/mach_time.h
        malloc.h
        memory.h
        mntent.h
        poll.h
        pwd.h
        sched.h
        spawn.h
        stdatomic.h
        stdint.h
        stdlib.h
        string.h
        strings.h
        sys/auxv.h
        sys/event.h
        sys/filio.h
        sys/inotify.h
        sys/mkdev.h
        sys/mntctl.h
        sys/mnttab.h
        sys/mount.h
        sys/param.h
        sys/resource.h
        sys/select.h
        sys/statfs.h
        sys/stat.h
        sys/statvfs.h
        sys/sysctl.h
        sys/time.h
        sys/times.h
        sys/types.h
        sys/uio.h
        sys/vfs.h
        sys/vfstab.h
        sys/vmount.h
        sys/wait.h
        termios.h
        unistd.h
        values.h
        wchar.h
        xlocale.h)

include(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

# 检查文件是否存在
start_config_file(${CMAKE_BINARY_DIR}/config.h "")
foreach (h ${HEADERS})
    check_system_header_exists(${h} absPath)
    if (${absPath})
        string_split_and_take_last(${h} res)
        message(STATUS "Check ${h} -- EXISTS")
        append_config_file(${CMAKE_BINARY_DIR}/config.h "${h}")
    else ()
        message(STATUS "Check ${h} -- NOT EXISTS")
    endif ()
endforeach ()
stop_config_file(${CMAKE_BINARY_DIR}/config.h "")
