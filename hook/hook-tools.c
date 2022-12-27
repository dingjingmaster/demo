/*************************************************************************
> FileName: hook-tools.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 27 Dec 2022 09:54:39 AM CST
 ************************************************************************/
#include <glib.h>
#include <fcntl.h>
#include <stdio.h>
#include <dlfcn.h>
#include <syslog.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/uio.h>

char* get_process_name ()
{
#define BUF_SIZE (1024)
    g_autofree char* path = g_strdup_printf ("/proc/%d/exe", getpid());
    g_autofree char* name = (char*) g_malloc0 (BUF_SIZE);
    readlinkat (AT_FDCWD, path, name, BUF_SIZE - 1);

	return g_strdup(name);
}

int get_ascii_str_start (char* s, int len)
{
    for (int i = 0; i < len; ++i) {
        if (s[i] >= 32 && s[i] <= 127) {
            return i;
        }
    }

    return len;
}


/////////////////////////////////////////

#if 1
typedef int (OpenPtr) (const char* pathName, int flags, ...);
int open (const char* pathName, int flags, ...)
{
    va_list ap;
    va_start (ap, flags);
    int mode = va_arg (ap, mode_t);
    va_end (ap);

    OpenPtr* openPtr = (OpenPtr*) dlsym (RTLD_NEXT, "open");
    if (NULL == openPtr) {
        syslog (LOG_ERR, "dlsym open error");
        return -1;
    }

    if (0 == mode) {
        if (!g_str_has_prefix (pathName, "/dev/")) {
            syslog (LOG_ERR, "[Hook Tools]: open (%s, %d);", pathName, flags);
        }
        return openPtr (pathName, flags);
    } else {
        if (!g_str_has_prefix (pathName, "/dev/")) {
            syslog (LOG_ERR, "[Hook Tools]: open (%s, %d, %d);", pathName, flags, mode);
        }
        return openPtr (pathName, flags, mode);
    }
}

typedef int (Open64Ptr) (const char* pathName, int flags, ...);
int open64 (const char* pathName, int flags, ...)
{
    va_list ap;
    va_start (ap, flags);
    int mode = va_arg (ap, mode_t);
    va_end (ap);

    Open64Ptr* openPtr = (Open64Ptr*) dlsym (RTLD_NEXT, "open64");
    if (NULL == openPtr) {
        syslog (LOG_ERR, "dlsym open error");
        return -1;
    }

    if (0 == mode) {
        syslog (LOG_ERR, "[Hook Tools]: open64 (%s, %d);", pathName, flags);
        return openPtr (pathName, flags);
    } else {
        syslog (LOG_ERR, "[Hook Tools]: open64 (%s, %d, %d);", pathName, flags, mode);
        return openPtr (pathName, flags, mode);
    }
}
#endif

typedef int (CreatPtr) (const char* pathName, mode_t mode);
int creat (const char* pathName, mode_t mode)
{
    CreatPtr* creatPtr = (CreatPtr*) dlsym (RTLD_NEXT, "creat");
    if (NULL == creatPtr) {
        syslog (LOG_ERR, "dlsym open error");
        return -1;
    }

    syslog (LOG_ERR, "[Hook Tools]: creat (%s, %d);", pathName, mode);

    return creatPtr (pathName, mode);
}


typedef ssize_t (ReadvPtr) (int fd, const struct iovec* iov, int iovcnt);
ssize_t readv (int fd, const struct iovec* iov, int iovcnt)
{
    ReadvPtr* readPtr = (ReadvPtr*) dlsym (RTLD_NEXT, "readv");
    if (NULL == readPtr) {
        syslog (LOG_ERR, "dlsym open error");
        return -1;
    }

    syslog (LOG_ERR, "[Hook Tools]: readv (%d, %p, %d);", fd, iov, iovcnt);
    for (int i = 0; i < iovcnt; ++i) {
        syslog (LOG_ERR, "[Hook Tools]: readv - iov[%d]='%s', len=%lu", i, (char*) iov[i].iov_base, iov[i].iov_len);
    }

    return readPtr (fd, iov, iovcnt);
}


typedef ssize_t (WritevPtr) (int fd, const struct iovec* iov, int iovcnt);
ssize_t writev (int fd, const struct iovec* iov, int iovcnt)
{
    WritevPtr* writePtr = (WritevPtr*) dlsym (RTLD_NEXT, "writev");
    if (NULL == writePtr) {
        syslog (LOG_ERR, "dlsym open error");
        return -1;
    }

    syslog (LOG_ERR, "[Hook Tools]: writev (%d, %p, %d);", fd, iov, iovcnt);
    for (int i = 0; i < iovcnt; ++i) {
        syslog (LOG_ERR, "[Hook Tools]: writev - iov[%d]='%s', len=%lu", i, &((char*) iov[i].iov_base)[get_ascii_str_start ((char*)iov[i].iov_base, iov[i].iov_len)], iov[i].iov_len);
    }

    return writePtr (fd, iov, iovcnt);
}

