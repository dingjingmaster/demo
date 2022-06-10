/*************************************************************************
> FileName: gtk2_string.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 14时05分16秒
 ************************************************************************/

#include <glib.h>

int main(int argc, char *argv[]) 
{
    GString *string1, *string2;
    gboolean is_eq;
                
    string1 = g_string_sized_new(16);
    g_string_printf(string1, "This is %s speaking!", "caterpillar");
    g_print("%s\n", string1->str);
                            
    string2 = g_string_new("This is caterpillar speaking!");
    is_eq = g_string_equal(string1, string2);
    g_printf("equal: %s\n", is_eq ? "TRUE" : "FALSE");
                                        
    g_string_ascii_up(string1);
    g_printf("Upper: %s\n", string1->str);
    g_string_ascii_down(string1);
    g_printf("Down: %s\n", string1->str);
                                                        
    g_string_free(string1, FALSE);
    g_string_free(string2, FALSE);
                                                                
    return 0;
}
