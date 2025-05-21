/*************************************************************************
> FileName: main.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月21日 星期三 17时17分25秒
 ************************************************************************/
#include <stdio.h>
#include "./plthook/plthook.h"

extern int test_libc(int a, int b);

int main (int argc, char* argv[])
{
    plthook_t *plthook;
    unsigned int pos = 0; /* This must be initialized with zero. */
    const char *name;
    void **addr;

    void* tmp = test_libc;

    printf("%p\n", tmp);

    if (plthook_open(&plthook, "libtest.so") != 0) {
        printf("plthook_open error: %s\n", plthook_error());
        return -1;
    }

    while (plthook_enum(plthook, &pos, &name, &addr) == 0) {
        printf("%p(%p) %s\n", addr, *addr, name);
    }
    plthook_close(plthook);

    return 0;
}
