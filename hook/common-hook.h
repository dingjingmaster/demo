/*************************************************************************
> FileName: hook-common.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 24 Jul 2023 08:30:05 AM CST
 ************************************************************************/
#ifndef _HOOK_COMMON_H
#define _HOOK_COMMON_H
#include <errno.h>
#include <dlfcn.h>
#include <stdint.h>
#include <syslog.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

/**
 * @fixme
 *  Dynamic libraries need to be divided into graphical and non-graphical interfaces
 */

#ifndef RTLD_NEXT
#define RTLD_NEXT (void*) -1
#endif


typedef bool (*ProcessIsBrowserPtr) (const char* progress);


static void* gLibCommon = NULL;
static pthread_mutex_t gLocker = PTHREAD_MUTEX_INITIALIZER;


static inline void init_hook_common ()
{
    if (!gLibCommon) {
        pthread_mutex_lock (&gLocker);
        if (!gLibCommon) {
            gLibCommon = dlopen (HOOK_COMMON"/libcommon-hook.so", RTLD_NOW);
        }
        pthread_mutex_unlock (&gLocker);
    }
}

static inline void* get_system_function_ptr (const char* functionName)
{
    init_hook_common();
    return dlsym (RTLD_NEXT, functionName);
}

static inline void* get_common_function_ptr (const char* functionName)
{
    init_hook_common();
    // FIXME:// Add a cache ?
    return dlsym (gLibCommon, functionName);
}

/**
 * @brief
 *  If the parameter @progress is null, disable all!
 */
static inline void forbid_drag_file_to_progress (void* display/*Display*/, int64_t window/*Window*/, const char* progress)
{
    init_hook_common();

    //void common_forbid_drag_file_to_progress (Display* display, Window window, const char* programName)
    void (*ptr) (void*, int64_t, const char*) =
            (void(*)(void*, int64_t, const char*)) get_common_function_ptr ("common_forbid_drag_file_to_progress");
    if (NULL == ptr) {
        syslog (LOG_ERR, "common_forbid_drag_file_to_progress get error, It's impossible");
        return;
    }

    ptr (display, window, progress);
}


#endif
