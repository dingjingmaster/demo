/**
 * gtk 窗口创建流程：
 *  1. 通过 gtk_application_new() 函数初始化一个窗口
 *  2. 回调函数关联 activate 事件
 *  3. 在与 activate 关联的回调函数里创建窗口并设置窗口参数和显示操作
 *  4. 通过g_application_run() 函数发射 activate 事件并激活对应的回调函数
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * 任何 gtk+ 程序都必须包含这一头文件，该头文件里面声明了 gtk+ 需要的函数、类型、宏
 *
 */
#include <gtk/gtk.h>

#ifdef __cplusplus
}
#endif

static void activate (GtkApplication* app, gpointer user_data) {
  GtkWidget *window;

  /* 将构建一个 GTK 窗口 */
  window = gtk_application_window_new (app);
  /* 设置窗口标题 需要使用 GTK_WINDOW() 宏函数进行转化 */
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  /* 设置窗口大小 */
  gtk_window_set_default_size (GTK_WINDOW (window),800, 500);
  /* 显示窗体 */
  gtk_widget_show_all (window);
}

/**
 * GtkApplication 对象并运行，
 * 使用 gtk_application_new() 进行初始化
 * 使用 g_signal_connect() 将信号和回调函数连接
 * g_application_run() 运行将会发射 "activate" 信号，该函数还会带入命令行的参数
 *
 */
int main (int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
