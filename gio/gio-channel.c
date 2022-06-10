/*************************************************************************
> FileName: gio-channel.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 14时46分24秒
 ************************************************************************/

#include <glib.h>

void handle_error(GError *error) 
{
    if(error != NULL) {
        g_printf(error->message);
        g_clear_error(&error);
    }
}

int main(int argc, char *argv[]) 
{
    gchar *src, *dest; 
    GIOChannel *ioChannel;
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

    ioChannel = g_io_channel_new_file(src, "r", &error);
    handle_error(error);
    g_io_channel_read_to_end(ioChannel, &content, &length, NULL);
    g_io_channel_close(ioChannel);
                                                        
    ioChannel = g_io_channel_new_file(dest, "w", &error);
    handle_error(error);
    g_io_channel_write_chars(ioChannel, content, -1, &length, NULL);
    g_io_channel_close(ioChannel);
                                                                        
    g_free(content);
                                                                            
    return 0;
}
