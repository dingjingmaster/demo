/*************************************************************************
> FileName: hook-x-event.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 05 Jul 2023 02:27:23 PM CST
 ************************************************************************/
#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>
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

#ifndef RTLD_NEXT
#define RTLD_NEXT (void*)-1
#endif




typedef void (*XNextEventPtr) (void* disp, void* event);

void XNextEvent (void* disp, void* event)
{
    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 
    XNextEventPtr old = (XNextEventPtr) dlsym (RTLD_NEXT, "XNextEvent");
    if (old == NULL) {
        return ;
    }

    old(disp, event);
    int* p = (int*) event;

    const char* str = NULL;

    switch (*p) {
        case 2  :       str = "KeyPress";           break;
        case 3  :       str = "KeyRelease";         break;
        case 4  :       str = "ButtonPress";        break;
        case 5  :       str = "ButtonRelease";      break;
        case 6  :       str = "MotionNotify";       break;
        case 7  :       str = "EnterNotify";        break;
        case 8  :       str = "LeaveNotify";        break;
        case 9  :       str = "FocusIn";            break;
        case 10 :       str = "FocusOut";           break;
        case 11 :       str = "KeymapNotify";       break;
        case 12 :       str = "Expose";             break;
        case 13 :       str = "GraphicsExpose";     break;
        case 14 :       str = "NoExpose";           break;
        case 15 :       str = "VisibilityNotify";   break;
        case 16 :       str = "CreateNotify";       break;
        case 17 :       str = "DestroyNotify";      break;
        case 18 :       str = "UnmapNotify";        break;
        case 19 :       str = "MapNotify";          break;
        case 20 :       str = "MapRequest";         break;
        case 21 :       str = "ReparentNotify";     break;
        case 22 :       str = "ConfigureNotify";    break;
        case 23 :       str = "ConfigureRequest";   break;
        case 24 :       str = "GravityNotify";      break;
        case 25 :       str = "ResizeRequest";      break;
        case 26 :       str = "CirculateNotify";    break;
        case 27 :       str = "CirculateRequest";   break;
        case 28 :       str = "PropertyNotify";     break;
        case 29 :       str = "SelectionClear";     break;
        case 30 :       str = "SelectionRequest";   break;
        case 31 :       str = "SelectionNotify";    break;
        case 32 :       str = "ColormapNotify";     break;
        case 33 :       str = "ClientMessage";      break;
        case 34 :       str = "MappingNotify";      break;  
        case 35 :       str = "GenericEvent";       break;
        case 36 :       str = "LASTEvent";          break;
        default :       str = "unknown";            break;
    }

    loge("XNextEvent %d - %s", (*p), str);
}

typedef int (*XChangePropertyPtr) (void*, int64_t, int64_t [], int64_t, int, int, const unsigned char* data, int nes);
int XChangeProperty (void* display, int64_t window, int64_t property, int64_t type, int format, int mode, const unsigned char* data, int npositions)
{
    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 
    XChangePropertyPtr old = (XChangePropertyPtr) dlsym (RTLD_NEXT, "XChangeProperty");
    if (old == NULL) {
        return 0;
    }

    loge("XChangeProperty: %s", data ? data : "<null>");

    return old(display, window, property, type, format, mode, data, npositions);
}
