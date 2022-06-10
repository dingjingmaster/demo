/*************************************************************************
> FileName: gtk2_avl_tree.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 14时02分34秒
 ************************************************************************/

#include <glib.h>

gint key_compare_func(gconstpointer a, gconstpointer b) 
{
    return g_strcmp0(a, b);
}

gboolean traverse_func(gpointer key, gpointer value, gpointer data)
{
    g_print("key\t: %s\nvalue\t: %s\n\n", key, value);

    return FALSE;
}

int main(int argc, char *argv[]) 
{
    GTree *tree;
            
    tree = g_tree_new(key_compare_func);
    g_tree_insert(tree, "justin", "justin's message!!");
    g_tree_insert(tree, "momor", "momor's message!!");
    g_tree_insert(tree, "caterpillar", "caterpillar's message!!");
                         
    g_tree_foreach(tree, traverse_func, NULL);
                             
    g_tree_destroy(tree);
                                 
    return 0; 
}
