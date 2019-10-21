/*************************************************************************
> FileName: syslog_demo.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年10月09日 星期三 19时54分14秒
 ************************************************************************/

#include <stdio.h>
#include <syslog.h>


int main (int argc, char* argv[]) {
    openlog("syslog demo", LOG_NDELAY|LOG_PID, LOG_USER);
    setlogmask (LOG_UPTO(LOG_DEBUG));

    syslog (LOG_INFO, "syslog test .....");
    syslog (LOG_INFO, "syslog test .....");
    syslog (LOG_INFO, "syslog test .....");
    syslog (LOG_INFO, "syslog test .....");
    syslog (LOG_INFO, "syslog test .....");
    syslog (LOG_INFO, "syslog test .....");

    closelog();

    return 0;
}

