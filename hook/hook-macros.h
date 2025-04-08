/*************************************************************************
> FileName: hook-macros.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月08日 星期二 14时24分52秒
 ************************************************************************/
#ifndef _HOOK_MACROS_H
#define _HOOK_MACROS_H

#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <linux/limits.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif


#if !(defined (C_STMT_START) && defined (C_STMT_END))
#define C_STMT_START  do
#if defined (_MSC_VER) && (_MSC_VER >= 1500)
#define C_STMT_END \
    __pragma(warning(push)) \
    __pragma(warning(disable:4127)) \
    while(0) \
    __pragma(warning(pop))
#else
#define C_STMT_END    while (0)
#endif
#endif

#define C_LIKELY(expr) (expr)
#define C_UNLIKELY(expr) (expr)


#define c_assert(x) \
C_STMT_START \
{ \
    assert(x); \
} \
C_STMT_END

#define c_malloc(ptr, size) \
C_STMT_START \
{ \
    if (C_LIKELY(size > 0)) { \
        ptr = malloc (size); \
        c_assert(ptr); \
        memset (ptr, 0, size); \
    } \
    else { \
        c_assert(false); \
    } \
} \
C_STMT_END

#define c_malloc_type(ptr, type, count) \
C_STMT_START \
{ \
    if (C_LIKELY(count > 0)) { \
        ptr = (type*) malloc (sizeof (type) * count); \
        c_assert(ptr); \
        memset (ptr, 0, sizeof (type) * count); \
    } \
    else { \
        exit(-errno); \
    } \
} \
C_STMT_END


#define c_free(ptr) \
C_STMT_START \
{ \
    if (C_LIKELY(ptr)) { \
        free (ptr); \
        ptr = NULL; \
	} \
} \
C_STMT_END

#define c_return_if_fail(x) \
C_STMT_START \
{ \
    if (C_UNLIKELY(!(x))) { \
        return; \
    } \
} \
C_STMT_END

#define c_return_val_if_fail(x, val) \
C_STMT_START \
{ \
    if (C_UNLIKELY(!(x))) { \
        return val; \
    } \
} \
C_STMT_END

static inline void* c_malloc0 (uint64_t size)
{
    void* ptr = NULL;

    c_malloc(ptr, size);

    return ptr;
}




#endif
