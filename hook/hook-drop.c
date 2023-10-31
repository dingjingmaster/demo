#include <syslog.h>

void _ZN7QWidget14dragLeaveEventEP15QDragLeaveEvent(void* pThis)
{
    syslog (LOG_ERR, "drag");
}

void _ZN7QWidget9dropEventEP10QDropEvent (void* pThis)
{
    syslog(LOG_ERR, "drop");
}

void _ZN17QAbstractItemView9dropEventEP10QDropEvent(void* pThis)
{
    syslog(LOG_ERR, "drop");
}
