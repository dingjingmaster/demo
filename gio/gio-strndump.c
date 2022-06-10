/*************************************************************************
> FileName: gio-strndump.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年11月04日 星期四 19时07分50秒
 ************************************************************************/
#include <gio/gio.h>

int main (int argc, char* argv[])
{
    char* str = "1636196340185173";

    g_autofree char* str1 = g_strndup (str, 10);

    printf ("%d\n", atoll(str1));

    return 0;
}
