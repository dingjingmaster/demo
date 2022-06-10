/*************************************************************************
> FileName: gtk3_theme.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月10日 星期二 10时09分19秒
 ************************************************************************/

#include <gtk/gtk.h>  
#include <stdio.h>
  
int main(int argc , char **argv)  
{
    if (argc < 2) {
        printf("input 1 or 2\n");
        return -1;
    }

    gtk_init(&argc, &argv);
    g_type_class_unref (g_type_class_ref (GTK_TYPE_IMAGE_MENU_ITEM));

    if (0 == g_strcmp0("1", argv[1])) {
        g_object_set (gtk_settings_get_default (), "gtk-theme-name", "Adwaita", NULL);
    } else {
        g_object_set (gtk_settings_get_default (), "gtk-theme-name", "Adwaita-dark", NULL);
    }

    gtk_main();

	return 0;  
}  
