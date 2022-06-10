/*************************************************************************
> FileName: g_hash.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年05月09日 星期六 16时08分42秒
 ************************************************************************/
#include <stdio.h>
#include <gmodule.h>

void insert (GHashTable*tab, char* str)
{
    if (!g_hash_table_contains (tab, str)) {
        g_hash_table_insert (tab, str, NULL);
    }
}

void func (gpointer key, gpointer value, gpointer udata)
{
    printf ("%s\n", key);
}

int main (int argc, char* argv[])
{

    GHashTable* tab = g_hash_table_new (g_str_hash, g_str_equal);

    insert (tab, "aaaa");
    insert (tab, "aaaa");
    insert (tab, "aaaa");
    insert (tab, "aaaa");
    insert (tab, "aaaa");
    insert (tab, "bbbb");
    insert (tab, "bbbb");
    insert (tab, "cccc");

    g_hash_table_foreach (tab, func, NULL);



    return 0;
}
