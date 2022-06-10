/*************************************************************************
> FileName: gio-uri.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年06月30日 星期三 19时43分59秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

int main (int argc, char* argv[])
{
    char*  uri = "file:///data/code/c_library/demo/gio/aaaa s类似ss %%^^#$@$%&*(&!@!!";
    g_autofree gchar* encodeUrl = g_uri_escape_string (uri, ":/", 1);
    g_autoptr(GFile)    file = g_file_new_for_uri(encodeUrl);

    printf ("exist:%s\n", g_file_query_exists(file, NULL) ? "true" : "false");
    printf("uri:%s\n", g_file_get_uri(file));
    printf("path:%s\n", g_file_get_path(file));
    
    g_autoptr(GFileInfo) fileInfo = g_file_query_info (file, "*", G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, NULL);

    printf("display name:%s\n", g_file_info_get_display_name(fileInfo));

    return 0;
}
