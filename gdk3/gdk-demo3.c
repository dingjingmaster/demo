/*************************************************************************
> FileName: gdk-demo3.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Вс 22 дек 2019 20:37:27
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <gdk/gdk.h>

int main (int argc, char* argv[])
{
    GdkWindow* win = NULL;
    GdkWindowAttr wattr;

    wattr.title = "测试";
    wattr.x = 100;
    wattr.y = 100;
    wattr.width = 100;
    wattr.height = 100;
    cairo_t* cairo = NULL;
    wattr.window_type = GDK_WINDOW_TOPLEVEL;


    gdk_init(&argc, &argv);

    win = gdk_window_new (NULL, &wattr, GDK_WA_TITLE | GDK_WA_X | GDK_WA_Y | GDK_WA_CURSOR);
    if (NULL == win) {
        printf ("error1");
        exit(-1);
    }
    cairo = gdk_cairo_create(win);


#if 1

    gdk_window_fullscreen (win);
    gdk_window_show (win);

    //gdk_window_destroy (win);
#endif

    return 0;
}
