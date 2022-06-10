/*************************************************************************
> FileName: gtk2_assistant.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 13时49分16秒
 ************************************************************************/

#include <gtk/gtk.h>

GtkWidget* gtk_assistant_page_new(GtkWidget *assistant, GtkWidget *widget, gchar *title, gchar *image, GtkAssistantPageType type) 
{
    gtk_assistant_append_page(GTK_ASSISTANT(assistant), widget);
    gtk_assistant_set_page_title(GTK_ASSISTANT(assistant), widget, title);
    gtk_assistant_set_page_side_image(GTK_ASSISTANT(assistant), widget, gdk_pixbuf_new_from_file(image, NULL));
    gtk_assistant_set_page_type(GTK_ASSISTANT(assistant), widget, type);
    gtk_assistant_set_page_complete(GTK_ASSISTANT(assistant), widget, TRUE);
                            
    return assistant;
}

int main(int argc, char *argv[]) 
{
    GtkWidget *assistant;
    GtkWidget *label;

    gtk_init (&argc, &argv);

    assistant = gtk_assistant_new();
    gtk_window_set_title(GTK_WINDOW(assistant), "GtkAssistant");
    gtk_window_set_default_size(GTK_WINDOW(assistant), 300, 180);

    gtk_assistant_page_new(assistant, gtk_label_new("簡介"), "精靈開始", "caterpillar.jpg", GTK_ASSISTANT_PAGE_INTRO);
    gtk_assistant_page_new(assistant, gtk_label_new("說明內容或元件"), "精靈第二頁", "caterpillar.jpg", GTK_ASSISTANT_PAGE_PROGRESS);
    gtk_assistant_page_new(assistant, gtk_label_new("說明內容或元件"), "精靈第三頁", "caterpillar.jpg", GTK_ASSISTANT_PAGE_PROGRESS);
    gtk_assistant_page_new(assistant, gtk_label_new("說明內容或元件"), "精靈結束", "caterpillar.jpg", GTK_ASSISTANT_PAGE_CONFIRM);
                                                                                   
    g_signal_connect(GTK_OBJECT(assistant), "cancel", G_CALLBACK(gtk_main_quit), NULL);
                                                    
    gtk_widget_show_all(assistant);
                                                        
    gtk_main(); 

    return 0;
}
