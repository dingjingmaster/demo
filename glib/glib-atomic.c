/*************************************************************************
> FileName: glib-atomic.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 15 Mar 2023 09:53:58 AM CST
 ************************************************************************/
#include <glib.h>

int main (int argc, char* argv[])
{
    gint a = 0;

    // __atomic_store 调用 由gcc提供：https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html

    printf ("%d\n", g_atomic_int_get(&a));

    g_atomic_int_set(&a, 10);
    printf ("%d\n", g_atomic_int_get(&a));
}
