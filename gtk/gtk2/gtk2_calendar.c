/*************************************************************************
> FileName: gtk2_calendar.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 13时51分28秒
 ************************************************************************/

#include <gtk/gtk.h>

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *calendar;
    guint year = 1975;
    guint month = 5 - 1; // 5 月
    guint day = 26;
    guint mark_day = 19;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkCalendar");

    calendar = gtk_calendar_new();
    gtk_calendar_select_month(GTK_CALENDAR(calendar), month, year);
    gtk_calendar_select_day(GTK_CALENDAR(calendar), day);
    gtk_calendar_mark_day(GTK_CALENDAR(calendar), mark_day);
                                                            
    gtk_container_add(GTK_CONTAINER(window), calendar);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
                                                                                         
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
