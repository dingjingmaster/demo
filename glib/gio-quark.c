/*************************************************************************
> FileName: gio-quark.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 11 Jan 2023 02:42:02 PM CST
 ************************************************************************/
#include <stdio.h>

#include <glib.h>
#include <gmodule.h>

int main (int argc, char* argv[])
{
    const char* c1 = "dingjing";
    const char* c2 = "丁敬dingjing+-~!@#$%^&*()?><:{}/\\";
    GQuark q1;

    q1 = g_quark_from_string (c1);
    printf ("str: '%s' -- quark: %u --- str: '%s'\n", c1, q1, g_quark_to_string(q1));

    printf ("str: '%s' -- g_intern_string: '%s'\n", c1, g_intern_string (c1));
    printf ("str: '%s' -- g_intern_string: '%s'\n", c2, g_intern_string (c2));

    return 0;
}
