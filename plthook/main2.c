/*************************************************************************
> FileName: main2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月21日 星期三 16时52分41秒
 ************************************************************************/
#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "./plthook/plthook.h"

typedef int (*OldFunc) (const char* path, int flags, ...);
int hook_open (const char* path, int flags, ...)
{
    printf("Hook open: %s\n", path);
    return 100;
}

extern int test_libc2(int, int);

int main (int argc, char* argv[])
{
    void* handle = NULL;
    void* address = NULL;
    OldFunc oldFunc = NULL;
    plthook_t* plthook = NULL;

    handle = dlopen("libtest1.so", RTLD_LAZY);
    if (!handle) {
        printf ("dlopen error: %s\n", dlerror());
        return 1;
    }

    address = dlsym(handle, "test_libc2");
    if (NULL == address) {
        printf ("dlsym error: %s\n", dlerror());
        return 2;
    }

    if (0 != plthook_open_by_address(&plthook, address)) {
        printf("plthook_open error: %s\n", plthook_error());
        return 3;
    }

    if (0 != plthook_replace(plthook, "open", (void*) hook_open, (void**) &oldFunc)) {
        printf ("plthook_replace error: %s\n", plthook_error());
        return 4;
    }

    // 还是调用的原来的 open 函数
    int ret = open("/asdsad/sd.so", O_RDONLY);
    printf ("open: %d\n", ret);

    // 此函数中的 open 函数已经被替换了
    ret = test_libc2(1, 2);
    printf ("open2: %d\n", ret);

    plthook_close(plthook);

    return 0;
}

