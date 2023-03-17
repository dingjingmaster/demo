/*************************************************************************
> FileName: glib-date-time.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 17 Mar 2023 03:37:01 PM CST
 ************************************************************************/
#include <glib.h>

int main (int argc, char* argv[])
{
    g_autoptr(GDateTime) tim = g_date_time_new_now_local ();

    g_autofree char* timStr = g_strdup_printf ("%04d%02d%02d%d%02d%2d.%3d",
            g_date_time_get_year(tim),
            g_date_time_get_month(tim),
            g_date_time_get_day_of_month(tim),
            g_date_time_get_hour(tim),
            g_date_time_get_minute(tim),
            g_date_time_get_second(tim),
            g_date_time_get_microsecond(tim));

    printf ("%s\n", timStr);

    return 0;
}
