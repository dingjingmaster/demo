/*************************************************************************
> FileName: dlinfo.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月21日 星期三 18时29分10秒
 ************************************************************************/
#define _GNU_SOURCE
#include <link.h>
#include <stdio.h>
#include <dlfcn.h>

int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf ("Usage: %s <so file>\n", argv[0]);
        return 0;
    }

    void* handle = dlopen (argv[1], RTLD_LAZY | RTLD_NOLOAD);
    if (handle) {
        struct link_map* lmap = NULL;
        if (0 == dlinfo(handle, RTLD_DI_LINKMAP, &lmap)) {
            for (struct link_map* l = lmap; l; l = l->l_next) {
                printf ("%s: l_addr: %p, l_ld: %p\n", l->l_name, l->l_addr, l->l_ld);
            }
        }
        dlclose(handle);
    }


    return 0;
}
