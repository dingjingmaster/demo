/*************************************************************************
> FileName: gdk-demo1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Вс 22 дек 2019 18:57:42
 ************************************************************************/
#include <stdio.h>
#include <gdk/gdk.h>

int main (int argc, char* argv[])
{
    /**
     * 初始化GDK库并连接到窗口系统，初始化失败则结束程序
     * GTK+在gtk_init()中初始化GDK
     */
    gdk_init (&argc, &argv);

    // 获取命令行参数中指定的显示名称 --display xx 指定参数
    printf ("display arg name:%s\n", gdk_get_display_arg_name());

    // GTK+在打开第一个 Gtkwindow 后自动调用此函数
    //gdk_notify_startup_complete();

    // 后端名称，默认是全部，可用x11、win32、quartz、broadway、wayland；*表示所有
    //gdk_set_allowed_backends ("*");

    printf ("program class:%s\n", gdk_get_program_class());

    // 获取显示的名字，此名字来源于 --display 选项传入
    printf ("display:%s\n", gdk_get_display());
    // 等价于
    printf ("display:%s\n", gdk_display_get_name(gdk_display_get_default()));

    // 刷新所有显示连接的输出缓冲区，并等待，直到处理完所有请求
    // 新版已废弃，无需这样做
    gdk_flush ();

    // 查看显示器宽度
    // 已废弃int gdk_monitor_get_scale_factor (GdkMonitor *monitor);
    printf ("width:%d\n", gdk_screen_width());
    // 返回宽度毫米，不一定准确
    printf ("width mm:%d\n", gdk_screen_width_mm());

    // 查看显示器高度
    // 已废弃 gdk_monitor_get_scale_factor (GdkMonitor *monitor);
    printf ("height:%d\n", gdk_screen_height());
    // 返回高度毫米，不一定准确
    printf ("height mm:%d\n", gdk_screen_height_mm());

    gdk_beep();
    /* 都已废弃 */
    // gdk_pointer_grab
    // gdk_pointer_ungrab
    // gdk_pointer_is_grabbed
    // gdk_set_double_click_time
    // gdk_keyboard_grab
    // gdk_keyboard_ungrab
    // gdk_beep
    // gdk_error_trap_push
    // gdk_error_trap_pop
    // gdk_error_trap_pop_ignored

    /* 一些返回值 */

    return 0;
}
