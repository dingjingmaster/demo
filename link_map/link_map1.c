/*************************************************************************
> FileName: link_map1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月17日 星期二 10时36分38秒
 ************************************************************************/
#include <link.h>
#include <stdio.h>


int main (void)
{
    struct link_map* lMap = NULL;

    for (lMap = _r_debug.r_map; lMap != NULL; lMap = lMap->l_next) {
        printf ("name=%s, laddr=0x%lx, ld=0x%lx\n", lMap->l_name, lMap->l_addr, lMap->l_ld);
    }

    return 0;
}
