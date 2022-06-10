/*************************************************************************
> FileName: gdk-demo2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Вс 22 дек 2019 19:28:48
 ************************************************************************/
#include <stdio.h>
#include <gdk/gdk.h>

/**
 * 单例对象
 * GdkDisplayManager - 维护一系列打开的GdkDisplays
 * GDK_IS_X11_DISPLAY()验明是哪个后端
 *
 * GdkDisplayManager* gdk_display_manager_get()
 * GdkDisplay* gdk_display_get_default_display()
 * void gdk_display_manager_set_default_display()
 * GList* gdk_display_manager_list_displays()
 * GdkDisplay* gdk_display_manager_open_display()
 *
 * 信号：display-opened
 *
 * 信号：closed
 *      monitor-added
 *      monitor-removed
 *      opened
 *      seat-added
 *      seat-removed
 */

int main (int argc, char* argv[])
{
    GdkDisplay* dsp = NULL;
    GdkScreen* screen = NULL;
    GdkDisplayManager* dspm = NULL;

    // 必须先调用
    gdk_init (&argc, &argv);

    dspm = gdk_display_manager_get ();
    if (NULL == dspm) {
        printf ("error1\n");
        exit (1);
    }

    dsp = gdk_display_manager_get_default_display (dspm);
    if (NULL == dsp) {
        printf ("error2\n");
        exit (1);
    }

    /* GdkDisplay相关操作 */
    // 获取显示器名字，名字属于GDK
    printf ("name:%s\n", gdk_display_get_name(dsp));

    // 获取屏幕数量，已废弃
    printf ("display number:%d\n", gdk_display_get_n_screens(dsp));

    // GdkScreen* gdk_display_get_default_screen (GdkDisplay* display);
    screen = gdk_display_get_screen (dsp, 0);
    if (NULL == screen) {
        printf ("error3\n");
        exit(1);
    }

    // gdk_display_sync() 刷新窗口系统中排队的所有请求，等待所有请求被处理完，确保显示与程序的当前状态同步

    // 关系窗口与后台的连接
    //gdk_display_close (dsp);

    // 获取要处理的下一个 GdkEvent 用于显示，必要时从窗口系统获取事件
    // GdkEvent* gdk_display_get_event (GdkDisplay* display)
    // 获取显示的事件队列中的第一个GdkEvent事件副本，不从队列中删除该事件
    // GdkEvent* gdk_display_peek_event (GdkDisplay* display)

    // 追加事件到显示队列前边
    // void gdk_display_put_event (GdkDisplay* display, const GdkEvent* event);
    // gdk_display_has_pending()是否屏幕有等待处理的事件

    // 双击时间
    // void gdk_display_set_double_click_time (GdkDisplay* display, guint msec);

    // 获取当前指针位置和mask
    // void gdk_display_get_pointer (GdkDisplay* display, GdkScreen** screen, gint* x, gint* y, HdkModifierType* mask);

    // 获取连接到显示器的设备链表，无须释放链表
    // GList* gdk_display_list_devices (GdkDisplay* display);

    // 获取窗口中指针位置，如果指针不再则为NULL
    // GdkWindow* gdk_display_get_window_at_pointer (GdkDisplay* display, gint* x, gint* y);
    // 返回鼠标指针下的窗口，没有则为NULL
    gint x = 0;
    gint y = 0;
    GdkWindow* window = NULL;
    //window = gdk_display_get_window_at_pointer (dsp, &x, &y);
    //if (NULL == window) {
    //    printf ("error5\n");
    //    exit(-1);
    //}
    //printf ("鼠标位置 x:%d, y:%d\n", x, y);

    // 鼠标是否支持多色彩
    printf ("color:%s\n", gdk_display_supports_cursor_color(dsp)?"支持":"不支持");
    printf ("alpha:%s\n", gdk_display_supports_cursor_alpha(dsp)?"支持":"不支持");

    // 鼠标大小
    printf ("cursor size:%d\n", gdk_display_get_default_cursor_size(dsp));

    gdk_display_get_maximal_cursor_size (dsp, &x, &y);
    printf ("cursor max width:%d, max height:%d\n", x, y);

    // 存储粘贴板
    // void gdk_display_store_clipboard (GdkDisplay* display, GdkWindow* clip_wind, guint32 time, const GdkAtom* targets, gint n_targets)

    // 是否可用于显示图形的窗口
    printf ("是否可显示图形:%s\n", gdk_display_supports_shapes(dsp)?"是":"否");

    // 是否可用于修改显示窗口的输入形状
    printf ("显示窗口的输入形状:%s\n", gdk_display_supports_input_shapes(dsp)?"是":"否");

    // 返回启用应用程序的上下文
    // GdkAppLaunchContext* gdk_display_get_app_launch_context(GdkDisplay* display);

    // 获取GdkSeat
    // GdkSeat* gdk_display_get_default_seat (GdkDisplay* display);

    // GList* gdk_display_list_seats (GdkDisplay* display);

    // 返回属于display监视器的数量
    // int gdk_display_get_n_monitors (GdkDisplay* display);

    // 返回监视器
    // GdkMonitor* gdk_display_get_monitor(GdkDisplay, int monitor_num);

    // 获取主监视器
    // GdkMonitor* gdk_display_get_primary_monitor (GdkDisplay* display);

    // 获取鼠标位置处监视器
    // GdkMonitor* gdk_display_get_monitor_at_pointer


    return 0;
}
