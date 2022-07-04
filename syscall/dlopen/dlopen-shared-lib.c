/*************************************************************************
> FileName: dlopen-shared-lib.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 04 Jul 2022 07:24:03 AM CST
 ************************************************************************/
#include <stdio.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

void* libc_handle = NULL;
int (*open_ptr) (const char*, int) = NULL;
int (*close_ptr) (int) = NULL;
ssize_t (*read_ptr) (int, void*, size_t) = NULL;

static bool inited = false;

_Noreturn void die (const char* fmt, ...)
{
    va_list va;
    va_start (va, fmt);
    vprintf (fmt, va);
    _exit (0);
}

static void find_original_function ()
{
    if (inited) return;

    printf ("libc path: %s\n", LIBC);

    libc_handle = dlopen (LIBC, RTLD_LAZY);

    if (libc_handle == NULL) {
        die ("cannot open libc.so\n");
    }

    open_ptr = dlsym (libc_handle, "open");
    if (open_ptr == NULL) {
        die ("cannot find open()\n");
    }

    close_ptr = dlsym (libc_handle, "close");
    if (close_ptr == NULL) {
        die ("cannot find close()\n");
    }

    read_ptr = dlsym (libc_handle, "read");
    if (read_ptr == NULL) {
        die ("cannot find read()\n");
    }

    inited = true;
}

int open (const char* pathName, int flag)
{
    find_original_function();

    printf ("start open()\n");
    int fd = (*open_ptr) (pathName, flag);
    printf ("end open()\n");

    return fd;
}

int close (int fd)
{
    find_original_function();

    printf ("start close()\n");
    int ret = (*close_ptr) (fd);
    printf ("end close()\n");

    return ret;
}

ssize_t read (int fd, void* buf, size_t count)
{
    find_original_function();

    printf ("start read()\n");
    ssize_t ret = (*read_ptr) (fd, buf, count);
    printf ("end read()\n");

    return ret;
}
