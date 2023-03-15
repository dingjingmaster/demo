/*************************************************************************
> FileName: glib-version.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 15 Mar 2023 08:44:50 AM CST
 ************************************************************************/
#include <glib.h>


int main (int argc, char* argv[])
{
    printf ("version: %u.%u.%u\n", glib_major_version, glib_minor_version, glib_micro_version);

    return 0;
}
