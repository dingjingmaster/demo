
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <dlfcn.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>
#include <dirent.h>
#include <syslog.h>


typedef int (*OpenPointer)(const char *pathname, int flags, ...);
int open (const char *pathname, int flags, ...)
{
    printf("open IN, file: '%s'!!!!!!!!!!!!!!\n", pathname);

    int mode = 0;

    va_list ap;
    va_start(ap, flags);
    mode = va_arg(ap, mode_t);
    va_end(ap);

    OpenPointer oldOpen = (OpenPointer) dlsym ((void*) -1, "open");
    if (oldOpen == NULL) {
        printf("dlsym open error\n");
        return -1;
    }

    if (mode == 0) {
        oldOpen(pathname, flags);
    } else {
        oldOpen(pathname, flags, mode);
    }

    errno = EACCES;

    return -1;
}


typedef int (*OpenatPointer)(int dirfd, const char *pathname, int flags, ...);
int openat (int dirfd, const char *pathname, int flags, ...)
{
    printf("open IN, file: '%s'!!!!!!!!!!!!!!\n", pathname);

    int mode = 0;

    va_list ap;
    va_start(ap, flags);
    mode = va_arg(ap, mode_t);
    va_end(ap);

    OpenatPointer oldOpen = (OpenatPointer) dlsym ((void*) -1, "openat");
    if (oldOpen == NULL) {
        printf("dlsym open error\n");
        return -1;
    }

    if (mode == 0) {
        oldOpen(dirfd, pathname, flags);
    } else {
        oldOpen(dirfd, pathname, flags, mode);
    }

    errno = EACCES;

    return -1;
}

int openat64 (int dirfd, const char *pathname, int flags, ...)
{
    printf("open IN, file: '%s'!!!!!!!!!!!!!!\n", pathname);

    int mode = 0;

    va_list ap;
    va_start(ap, flags);
    mode = va_arg(ap, mode_t);
    va_end(ap);

    OpenatPointer oldOpen = (OpenatPointer) dlsym ((void*) -1, "openat64");
    if (oldOpen == NULL) {
        printf("dlsym open error\n");
        return -1;
    }

    if (mode == 0) {
        oldOpen(dirfd, pathname, flags);
    } else {
        oldOpen(dirfd, pathname, flags, mode);
    }

    errno = EACCES;

    return -1;
}


