#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdarg.h>
#include <unistd.h>
#include <dlfcn.h>
#include <dirent.h>
#include <syslog.h>
#include <errno.h>

typedef int (*OpenPointer)(const char *pathname, int flags, ...);
int open (const char *pathname, int flags, ...)
{
    printf("open IN, file: '%s'!!!!!!!!!!!!!!\n", pathname);

    int mode = 0;

    va_list ap;
    va_start(ap, flags);
    mode = va_arg(ap, mode_t);
    va_end(ap);

    // handle = dlopen("libc.so.6", RTLD_LAZY);
    OpenPointer oldOpen = (OpenPointer) dlsym (-1, "open");
    if (oldOpen == NULL) {
        printf("dlsym open error\n");
        return -1;
    }

    if (mode == 0) {
        int ret = oldOpen(pathname, flags);
    } else {
        int ret = oldOpen(pathname, flags, mode);
    }


    return 0;
}

