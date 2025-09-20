/*************************************************************************
> FileName: glib_format_size.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年09月20日 星期六 13时35分08秒
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <stdint.h>

int main (void)
{
    printf("INT32_MAX ptr: %s\n", g_format_size(INT32_MAX * sizeof(void*)));

    return 0;
}
