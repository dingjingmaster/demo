/*************************************************************************
> FileName: gio-file-read-write.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 14时36分55秒
 ************************************************************************/

#include <glib.h>

handle_error(GError *error) 
{
    if(error != NULL) {
        g_printf(error->message);
        g_clear_error(&error);
    }
}

int main(int argc, char *argv[]) 
{
    gchar *src, *dest; 
    gchar *content;
    gsize length;
    GError *error = NULL;
                        
    if(argc >= 3) {
        src = argv[1];
        dest = argv[2];
    } else {
         return 1;
    }
                                
    if (!g_file_test(src, G_FILE_TEST_EXISTS)) {
        g_error("Error: File does not exist!");
    }

    g_file_get_contents(src, &content, &length, &error);
    handle_error(error);
    g_print("%s\n", content);
                                                
    g_file_set_contents(dest, content, -1, &error);
    handle_error(error);
                                                        
    g_free (content);
                                                            
    return 0;
}
