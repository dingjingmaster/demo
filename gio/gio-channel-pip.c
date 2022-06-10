/*************************************************************************
> FileName: gio-channel-pip.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 14时50分53秒
 ************************************************************************/

#include<gtk/gtk.h>
#include<errno.h>
#include<unistd.h>

void setup_child(gint input[]);
void setup_parent(gint output[]);
void value_changed_callback(GtkSpinButton *spinButton, GIOChannel *channel);
gboolean iochannel_read(GIOChannel *channel, GIOCondition condition, GtkLabel *label);

int main(int argc, char *argv[]) 
{
    gint parent_to_child[2];

    if(pipe(parent_to_child) == -1) { // 開啟Pipe
        g_error("Error: %s\n", g_strerror(errno));
        return 1;
    }

    // fork 子程序
    switch(fork()) {
    case -1:
        g_error("錯誤: %s\n", g_strerror(errno));
        break;
    case 0: // 這是子程序
        gtk_init(&argc, &argv);
        setup_child(parent_to_child);
        break;
    default: // 這是父程序
        gtk_init(&argc, &argv);
        setup_parent(parent_to_child);
    }

    gtk_main();

    return 0;
}

// GtkSpinButton的callback
void value_changed_callback(GtkSpinButton *spinButton, GIOChannel *channel) 
{
    GIOStatus ret_value;
    gint value;
    GString *text;
    gsize length;

    value = gtk_spin_button_get_value_as_int(spinButton);
    text = g_string_new("");
    g_string_sprintf(text, "%d\n", value);

    // 寫入資料至 GIOChannel
    ret_value = g_io_channel_write_chars(channel, text->str, -1, &length, NULL);
    if(ret_value == G_IO_STATUS_ERROR) {
        g_error("錯誤: 無法寫入 GIOChannel ！\n");
    } else {
        g_io_channel_flush(channel, NULL);
    }
}

gboolean iochannel_read(GIOChannel *channel, GIOCondition condition, GtkLabel *label) 
{
    GIOStatus ret_value;
    gchar *message;
    gsize length;

    if(condition & G_IO_HUP) {
        g_error("錯誤: Pipe 已中斷！\n");
    }

    ret_value = g_io_channel_read_line(channel, &message, &length, NULL, NULL);

    if(ret_value == G_IO_STATUS_ERROR) {
        g_error("錯誤: 無法讀取！\n");
    }

    message[length-1] = 0;
    gtk_label_set_text(label, message);

    return TRUE;
}

void setup_parent(gint output[]) 
{
    GtkWidget *window;
    GtkWidget *spinButton;
    GIOChannel *channel_write;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Parent");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 50);

    spinButton = gtk_spin_button_new_with_range(0.0, 100.0, 1.0);
    gtk_container_add(GTK_CONTAINER(window), spinButton);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 關閉不用的 Pipe 輸出
    close(output[0]);

    // 建立 GIOChannel
    channel_write = g_io_channel_unix_new(output[1]);
    if(channel_write == NULL) {
        g_error("錯誤: 無法建立 GIOChannels！\n");
    }

    g_signal_connect(G_OBJECT(spinButton), "value_changed", G_CALLBACK(value_changed_callback), (gpointer) channel_write);

    gtk_widget_show_all(window);
}

void setup_child(gint input[]) 
{
    GtkWidget *window;
    GtkWidget *label;
    GIOChannel *channel_read;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Child");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 50);

    label = gtk_label_new("0");
    gtk_container_add(GTK_CONTAINER(window), label);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 關閉不必要的 Pipe 輸入
    close(input[1]);

    channel_read = g_io_channel_unix_new(input[0]);
    if(channel_read == NULL) {
        g_error("錯誤: 無法建立 GIOChannels！\n");
    }

    if(!g_io_add_watch(channel_read, G_IO_IN | G_IO_HUP, (GIOFunc) iochannel_read, (gpointer) label)) {
        g_error("錯誤: 無法對 GIOChannel 加入 watch！\n");
    }

    gtk_widget_show_all(window);
}
