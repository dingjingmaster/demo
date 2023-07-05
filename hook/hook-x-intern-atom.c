/*************************************************************************
> FileName: hook-x-intern-atom.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 05 Jul 2023 11:36:36 AM CST
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

typedef uint64_t (*XInternAtomPtr) (void* disp, const char* name, bool exists);

uint64_t XInternAtom (void* disp, const char* name, bool exists)
{
    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 
    XInternAtomPtr old = (XInternAtomPtr) dlsym (RTLD_NEXT, "XInternAtom");
    if (old == NULL) {
        return -1;
    }
    loge("OK1 files");

    return old(disp, name, exists);
}
