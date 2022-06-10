/*************************************************************************
> FileName: local.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2022年01月24日 星期一 14时56分10秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <locale.h>
#define GETTEXT_PACKAGE "A"
#include <glib/gi18n-lib.h>

int main (int argc, char* argv[])
{
    const gchar* const* l = g_get_language_names();

    int i = 0;
    for (const char* t = l[i]; t != NULL; i += 1, t = l[i]) {
        if (t) printf ("%s\n", t);
    }

    return 0;
}
