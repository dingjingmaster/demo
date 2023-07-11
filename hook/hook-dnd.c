/*************************************************************************
> FileName: hook-dnd.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 06 Jul 2023 03:39:41 PM CST
 ************************************************************************/
#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "c-log.h"
#include "c-log.c"

#ifndef RTLD_NEXT
#define RTLD_NEXT (void*)-1
#endif


static void* gLibCommon = NULL;

static inline void init_hook_common ()
{
    if (gLibCommon) return;

    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 

    errno = 0;
    gLibCommon = dlopen ("/usr/lib/libcommon-dnd.so", RTLD_NOW);
    if (!gLibCommon) {
        loge ("lib: %s, error: %s", "/usr/lib/libcommon-dnd.so", strerror (errno));
    }
}


typedef void (*XdndDropPtr) (void* event);
typedef bool (*CheckEventPtr) (void* event, const char* pname, int evType, const char* evName);
typedef void (*XNextEventPtr) (void* disp /*Display*/, void* event /*XEvent - ret*/);
void XNextEvent (void* disp, void* event)
{
    init_hook_common();
    XNextEventPtr old = (XNextEventPtr) dlsym (RTLD_NEXT, "XNextEvent");
    if (old == NULL) {
        loge ("");
        return;
    }

    CheckEventPtr checkEvent = (CheckEventPtr) dlsym (gLibCommon, "check_event");
    if (NULL == checkEvent) {
        loge ("");
        return;
    }

    // ClientMessage
    if (!checkEvent(event, "browser", 33, "XdndDrop")) {
        old(disp, event);
        return;
    }
    else {
        XdndDropPtr xdndDrop = (XdndDropPtr) dlsym (gLibCommon, "xdnd_drop");
        xdndDrop (event);
    }
    
    return;
}

typedef int (*XChangePropertyPtr) (void*, int64_t, int64_t, int64_t, int, int, const unsigned char* data, int nes);
int XChangeProperty (void* display, int64_t window, int64_t property, int64_t type, int format, int mode, const unsigned char* data, int npositions)
{
    init_hook_common();
    XChangePropertyPtr old = (XChangePropertyPtr) dlsym (RTLD_NEXT, "XChangeProperty");
    if (old == NULL) {
	loge("");
        return 0;
    }

    void (* p) (void*, int64_t, void*) = (void*) dlsym (gLibCommon, "list_property_by_name");
    if (p == NULL) {
	loge("");
        return 0;
    }

    p (display, window, "browser");

    logw("XChangeProperty: %s", data ? data : "<null>");

    return old(display, window, property, type, format, mode, data, npositions);
}
