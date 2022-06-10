/*************************************************************************
> FileName: gio-get_current_dir.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年03月17日 星期三 15时03分32秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

int main (int argc, char** argv)
{
    printf ("%s\n", g_get_current_dir());

    return 0;
}
