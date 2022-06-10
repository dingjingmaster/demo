/*************************************************************************
> FileName: gs-music.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年08月31日 星期二 16时44分19秒
 ************************************************************************/
#include <gio/gio.h>
#include <gst/gst.h>
#include <gtk/gtk.h>

int main (int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    gst_init (&argc, &argv);

    GstElement* gstElement = gst_element_factory_make ("demo", "music");
    g_return_val_if_fail (gstElement, -1);

    g_autoptr(GstBus) bus = gst_element_get_bus (gstElement);
    g_return_val_if_fail (bus, -1);

    gtk_main();

    return 0;
}
