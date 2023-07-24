/*************************************************************************
> FileName: hook-for-nautilus.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 10 May 2023 05:35:59 PM CST
 ************************************************************************/
#include <syslog.h>

void nautilus_file_operations_copy_move (const void           *item_uris,
                                    const char                *target_dir,
                                    int                       copy_action,
                                    void                      *parent_view,
                                    void* dbus_data,
                                    void* done_callback,
                                    void* done_callback_data)
{
    syslog (LOG_ERR, "nautilus hook!");
}
