/*************************************************************************
> FileName: hook-x.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 06 Jul 2023 09:22:13 AM CST
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


typedef uint64_t    (*XInternAtomPtr)               (void* disp /*Display*/,
                                                     const char* name,
                                                     bool exists);
typedef void        (*XNextEventPtr)                (void* disp /*Display*/,
                                                     void* event /*XEvent - ret*/);
typedef char*       (*XGetAtomNamePtr)              (void* display/*Display*/,
                                                     int64_t atom /*Atom*/);
typedef int         (*XChangePropertyPtr)           (void* display/*Diaplay*/,
                                                     int64_t /*Window*/,
                                                     int64_t property/*Atom*/,
                                                     int64_t type/*Atom*/,
                                                     int format,
                                                     int mode,
                                                     const unsigned char* data,
                                                     int nes);
typedef int         (*XGetWindowPropertyPtr)        (void* display/*Display*/,
                                                     int64_t window/*Window*/,
                                                     int64_t property/*Atom*/,
                                                     long offset,
                                                     long length,
                                                     bool del,
                                                     int64_t reqType/*Atom*/,
                                                     int64_t* retType/*Atom*/,
                                                     int* retFormatType,
                                                     unsigned long* retNElem,
                                                     unsigned long* retByte,
                                                     unsigned char** retProp);

static void* gLibCommon = NULL;

static inline void init_hook_common ()
{
    if (gLibCommon) return;

    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 

    errno = 0;
    gLibCommon = dlopen ("/usr/lib/libcommon-x.so", RTLD_NOW);
    if (!gLibCommon) {
        loge ("lib: %s, error: %s", "/usr/lib/libcommon-x.so", strerror (errno));
    }
}

uint64_t XInternAtom (void* disp, const char* name, bool exists)
{
    init_hook_common();
    XInternAtomPtr old = (XInternAtomPtr) dlsym (RTLD_NEXT, "XInternAtom");
    if (old == NULL) {
        loge ("");
        return -1;
    }
    logi("XInternAtom: %s", (name ? name : "null"));

    return old(disp, name, exists);
}


typedef void (*PrintEventPtr) (void* event);
void XNextEvent (void* disp, void* event)
{
    init_hook_common();
    XNextEventPtr old = (XNextEventPtr) dlsym (RTLD_NEXT, "XNextEvent");
    if (old == NULL) {
        loge ("");
        return;
    }
    old(disp, event);

    PrintEventPtr printEvent = (PrintEventPtr) dlsym (gLibCommon, "print_event_XClientMessageEvent");
    if (NULL == printEvent) {
        loge ("");
        return;
    }

    // print
    printEvent (event);
    
    return;
}

int XChangeProperty (void* display, int64_t window, int64_t property, int64_t type, int format, int mode, const unsigned char* data, int npositions)
{
    init_hook_common();
    XChangePropertyPtr old = (XChangePropertyPtr) dlsym (RTLD_NEXT, "XChangeProperty");
    if (old == NULL) {
        loge ("");
        return 0;
    }

    XGetAtomNamePtr atomName = (XGetAtomNamePtr) dlsym (RTLD_NEXT, "XGetAtomName");
    if (atomName == NULL) {
	    loge ("XGetAtomName error");
        return 0;
    }

    logi("XChangeProperty: '%s', property: '%s', type: '%s'", (data ? (char*)data : "<null>"), atomName(display, property), atomName(display, type));

    return old(display, window, property, type, format, mode, data, npositions);
}

int XGetWindowProperty (void* display/*Display*/, int64_t window/*Window*/, int64_t property/*Atom*/, long offset, long length, bool del, int64_t reqType/*Atom*/, int64_t* retType/*Atom*/, int* retFormatType, unsigned long* retNElem, unsigned long* retByte, unsigned char** retProp)
{
    init_hook_common();
    XGetWindowPropertyPtr old = (XGetWindowPropertyPtr) dlsym (RTLD_NEXT, "XGetWindowProperty");
    if (old == NULL) {
        loge ("");
        return 0;
    }

    XGetAtomNamePtr atomName = (XGetAtomNamePtr) dlsym (RTLD_NEXT, "XGetAtomName");
    if (atomName == NULL) {
	    loge ("XGetAtomName error");
        loge ("");
        return 0;
    }

    int ret = old (display, window, property, offset, length, del, reqType, retType, retFormatType, retNElem, retByte, retProp);

    logi ("XGetWindowProperty -- property: '%s', prop: '%s'", atomName(display, property), (*retProp ? (char*)*retProp : "<null>"));

    return ret;
}


