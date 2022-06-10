/*************************************************************************
> FileName: gtk3_recent_manager.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年10月07日 星期三 10时51分57秒
 ************************************************************************/
#include <stdio.h>
#include <gtk/gtk.h>

int main (int argc, char* argv[])
{
    GtkRecentManager* manager = gtk_recent_manager_get_default();
    if (NULL == manager) {
        printf ("error\n");
        return -1;
    }

    GList* recentItems = gtk_recent_manager_get_items(manager);
    for (GList* l = recentItems; NULL != l; l = l->next) {
        GtkRecentInfo* info = l->data;
        printf ("===================================================\n");
        printf ("display name:%s\n", gtk_recent_info_get_uri_display (info));
        printf ("display name:%s\n", gtk_recent_info_get_short_name(info));
        printf ("===================================================\n\n");

        gtk_recent_info_unref (info);
    }
    g_list_free (recentItems);

    return 0;
}
