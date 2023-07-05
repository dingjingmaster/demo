/*************************************************************************
> FileName: hook-xcb-property.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 05 Jul 2023 11:03:47 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "c-log.h"

#ifndef RTLD_NEXT
#define RTLD_NEXT (void*)-1
#endif

typedef int (*XcbChangeProperty) (void*, uint8_t, int, int, int, uint8_t, uint32_t, const void*);

int xcb_change_property (void* conn, uint8_t mode, int window, int property, int type, uint8_t format, uint32_t dataLen, const void* data)
{
    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 
    XcbChangeProperty old = (XcbChangeProperty) dlsym (RTLD_NEXT, "xcb_change_property");
    if (old == NULL) {
        return 0;
    }
    loge("%s", __FUNCTION__);

    return old(conn, mode, window, property, type, format, dataLen, data);
}
