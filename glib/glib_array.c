/*************************************************************************
> FileName: gtk2_array.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 14时07分18秒
 ************************************************************************/

#include <glib.h>

int main(int argc, char *argv[]) 
{
    GPtrArray *array;
    gint i;
    GString *text;
                    
    array = g_ptr_array_new();
    for (i = 0; i < 5; i++) {
        text = g_string_sized_new(5);
        g_string_printf(text, "TEST %d", i);
        g_ptr_array_add(array, text);
    }

    for (i = 0; i < 5; i++) {
        text = g_ptr_array_index(array, i);
        g_printf("%s\n", text->str);
    }
                                
    g_ptr_array_free(array, TRUE);
                                    
    return 0;
}
