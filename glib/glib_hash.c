/*************************************************************************
> FileName: hash.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月12日 星期四 11时04分36秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <glib.h>

/* 键值 */
gint hash_compare (gconstpointer n1, gconstpointer n2)
{
    return (!strcmp((char*)n1, (char*)n2));
}

/* 存储位置 */
guint hash_function (gconstpointer key)
{
    return (((char*)key)[0] + ((char*)key)[1]);
}

void print_names (gpointer key, gpointer value, gpointer data)
{
    g_print ("key: %s, value: %s\n", (gchar*)key, (gchar*)value);
}

int main (int argc, char* argv[])
{
    int index;
    GHashTable* htable;

    htable = g_hash_table_new (hash_function, hash_compare);

    g_hash_table_insert (htable, "aaa", "111");
    g_hash_table_insert (htable, "bbb", "222");
    g_hash_table_insert (htable, "ccc", "333");
    g_hash_table_insert (htable, "ddd", "444");
    g_hash_table_insert (htable, "ddd", "55");
    g_hash_table_insert (htable, "ddc", "55");

    g_print ("aaa: %s\n", (gchar*)g_hash_table_lookup(htable, "aaa"));
    g_print ("bbb: %s\n", (gchar*)g_hash_table_lookup(htable, "bbb"));
    g_print ("ccc: %s\n", (gchar*)g_hash_table_lookup(htable, "ccc"));
    g_print ("ddd: %s\n", (gchar*)g_hash_table_lookup(htable, "ddd"));
    g_print ("==========\n");

    g_hash_table_foreach(htable, (GHFunc)print_names, NULL);
    return 0;
}
