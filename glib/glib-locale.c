/*************************************************************************
> FileName: glib-version.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 15 Mar 2023 08:44:50 AM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>


int main (int argc, char* argv[])
{
    const char** locale = g_get_language_names();

    for (int i = 0; locale[i]; ++i) {
        printf ("\t%s", locale[i]);
    }
    printf ("\n");

    return 0;
}
