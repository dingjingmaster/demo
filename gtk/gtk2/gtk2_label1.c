/*************************************************************************
> FileName: gtk2_label1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 13时29分24秒
 ************************************************************************/

#include <gtk/gtk.h>

int main(int   argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *frame;
    GtkWidget *label;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkLabel");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    vbox = gtk_vbox_new(FALSE, 5);

    frame = gtk_frame_new("簡單標記");
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span foreground='blue' size='x-large'>良葛格學習筆記</span>");
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

    frame = gtk_frame_new("多行");
    label = gtk_label_new("良葛格學習筆記\ncaterpillar.onlyfun.net\n Gossip@Openhome");
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

    frame = gtk_frame_new("自動換行");
    label = gtk_label_new("You might think you know but actually you don't. "       \
                            "You might think you know but actually you don't. "     \
                            "You might think you know but actually you don't. "     \
                            "You might think you know but actually you don't.");
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

    frame = gtk_frame_new("靠右對齊");
    label = gtk_label_new("良葛格學習筆記\ncaterpillar.onlyfun.net\n"               \
                            "Gossip@Openhome");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);
                                                                                                                        
    frame = gtk_frame_new("自動換行、左右對齊");
    label = gtk_label_new("You might think you know but actually you don't. "       \
                            "You might think you know but actually you don't. "     \
                            "You might think you know but actually you don't. "     \
                            "You might think you know but actually you don't.");
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_FILL);
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
                                                                                                                                                     
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}


