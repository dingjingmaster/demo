#include <gtk/gtk.h>

// 全局变量
GtkWidget *window;
GtkStatusIcon *status_icon;

// 回调函数声明
static void toggle_window(GtkWidget *widget, gpointer data);
static void quit_app(GtkWidget *widget, gpointer data);
static gboolean on_window_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
static void status_icon_popup_menu(GtkStatusIcon *status_icon, guint button, guint activate_time, gpointer data);

// 创建主窗口
static void create_main_window(void) {
    // 创建主窗口
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "两列布局示例");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "delete-event", G_CALLBACK(on_window_delete_event), NULL);

    // 创建主布局
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // 创建可调节的分隔面板
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(main_box), paned, TRUE, TRUE, 0);

    // 创建左侧面板
    GtkWidget *left_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_size_request(left_panel, 200, -1);
    gtk_widget_set_hexpand(left_panel, TRUE);
    gtk_widget_set_vexpand(left_panel, TRUE);

    // 创建右侧面板
    GtkWidget *right_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_size_request(right_panel, 200, -1);
    gtk_widget_set_hexpand(right_panel, TRUE);
    gtk_widget_set_vexpand(right_panel, TRUE);

    // 添加示例标签
    GtkWidget *left_label = gtk_label_new("左侧面板");
    GtkWidget *right_label = gtk_label_new("右侧面板");

    gtk_box_pack_start(GTK_BOX(left_panel), left_label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(right_panel), right_label, TRUE, TRUE, 0);

    // 将面板添加到分隔面板
    gtk_paned_add1(GTK_PANED(paned), left_panel);
    gtk_paned_add2(GTK_PANED(paned), right_panel);
}

// 创建系统托盘
static void create_tray(void) {
    // 创建状态图标
    status_icon = gtk_status_icon_new_from_icon_name("system-run-symbolic");
    gtk_status_icon_set_tooltip_text(status_icon, "两列布局示例");
    
    // 连接右键点击事件
    g_signal_connect(status_icon, "popup-menu", G_CALLBACK(status_icon_popup_menu), NULL);
    
    // 连接左键点击事件
    g_signal_connect(status_icon, "activate", G_CALLBACK(toggle_window), NULL);
}

// 创建托盘菜单
static void status_icon_popup_menu(GtkStatusIcon *status_icon, guint button, guint activate_time, gpointer data) {
    GtkWidget *menu = gtk_menu_new();
    
    // 添加显示/隐藏菜单项
    GtkWidget *show_item = gtk_menu_item_new_with_label("显示/隐藏");
    g_signal_connect(show_item, "activate", G_CALLBACK(toggle_window), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), show_item);
    
    // 添加退出菜单项
    GtkWidget *quit_item = gtk_menu_item_new_with_label("退出");
    g_signal_connect(quit_item, "activate", G_CALLBACK(quit_app), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), quit_item);
    
    gtk_widget_show_all(menu);
    gtk_menu_popup(GTK_MENU(menu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}

// 回调函数实现
static void toggle_window(GtkWidget *widget, gpointer data) {
    if (gtk_widget_get_visible(window)) {
        gtk_widget_hide(window);
    } else {
        gtk_widget_show_all(window);
    }
}

static void quit_app(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

static gboolean on_window_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    gtk_widget_hide(window);
    return TRUE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    create_main_window();
    create_tray();

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
} 