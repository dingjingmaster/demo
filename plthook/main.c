/*************************************************************************
> FileName: main.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月21日 星期三 17时17分25秒
 ************************************************************************/
#include <stdio.h>
#include <dlfcn.h>
#include "./plthook/plthook.h"


int main (int argc, char* argv[])
{
    if (argc != 3) {
        printf ("Usage: %s <so path> <func>\n", argv[0]);
        return 1;
    }

    unsigned int pos = 0; /* This must be initialized with zero. */
    void **addr = NULL;
    void* handle = NULL;
    void* address = NULL;
    const char *name = NULL;
    plthook_t *plthook = NULL;

    handle = dlopen(argv[1], RTLD_LAZY);
    if (!handle) {
        printf ("dlopen error: %s\n", dlerror());
        return 1;
    }

    address = dlsym(handle, argv[2]);
    if (NULL == address) {
        printf ("dlsym error: %s\n", dlerror());
        return 2;
    }

    if (0 != plthook_open_by_address(&plthook, address)) {
        printf("plthook_open error: %s\n", plthook_error());
        return 3;
    }

    while (plthook_enum(plthook, &pos, &name, &addr) == 0) {
        printf("%p(%p) %s\n", addr, *addr, name);
    }
    plthook_close(plthook);

    return 0;
}
