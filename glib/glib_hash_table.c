/*************************************************************************
> FileName: gtk2_hash_table.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 14时00分25秒
 ************************************************************************/

#include <glib.h>

int main(int argc, char *argv[]) 
{
    GHashTableIter iter;
    gpointer key, value;
    GHashTable *hashTable;
                    
    hashTable = g_hash_table_new(g_str_hash, g_str_equal);
                                                   
    g_hash_table_insert(hashTable, "justin", "justin's message!!");
    g_hash_table_insert(hashTable, "momor", "momor's message!!");
    g_hash_table_insert(hashTable, "caterpillar", "caterpillar's message!!");
                                    
    g_hash_table_iter_init (&iter, hashTable);
    while(g_hash_table_iter_next(&iter, &key, &value)) {
        g_print("key\t: %s\nvalue\t: %s\n\n", key , value);
    }
                                            
    g_hash_table_destroy(hashTable);
                                                
    return 0; 
}
