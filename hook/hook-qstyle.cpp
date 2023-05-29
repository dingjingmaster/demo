/*************************************************************************
> FileName: hook-qstyle.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 22 May 2023 04:27:52 PM CST
 ************************************************************************/
#include <QWidget>
#include <QFileDialog>

#include <dlfcn.h>
#include <unistd.h>
#include <syslog.h>



#ifdef __cplusplus
extern "C" 
{
#endif

typedef void (*Polish) (QWidget* w);

void _ZN12QCommonStyle6polishEP7QWidget(QWidget* w)
{
    syslog (LOG_ERR, "hook polish");

    Polish old = (Polish) dlsym ((void*)-1, "_ZN12QCommonStyle6polishEP7QWidget");
    if (old == NULL) {
        syslog (LOG_ERR, "dlsym open error");
        return;
    }

    QFileDialog* fd = dynamic_cast<QFileDialog*>(w);
    if (!fd) {
        old(w);
        syslog (LOG_ERR, "NOT FILE DIALOG");
        return;
    }
    else {
        syslog (LOG_ERR, "IS FILE DIALOG");
        return;
    }
}

#ifdef __cplusplus
}
#endif
