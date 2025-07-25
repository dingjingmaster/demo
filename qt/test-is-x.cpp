/*************************************************************************
> FileName: test-is-x.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年07月25日 星期五 08时57分11秒
 ************************************************************************/
#include <QDebug>
#include <QString>

#include <fcntl.h>
#include <iconv.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>

bool check_proc_exists_by_name(const char *procName)
{
    DIR* dir = opendir("/proc");
    if (NULL == dir) {
        qWarning() << "[utils] opendir";
        return false;
    }
    bool isFound = false;
    char buf[128] = {0};
    char procPath[PATH_MAX] = {0};
    struct dirent* entry = NULL;

    QString bufR = QString("/%1").arg(procName).toLower();
    qInfo() << bufR;

    while (NULL != (entry = readdir(dir))) {
        int proc = atoi(entry->d_name);
        if (proc <= 0) {
            continue;
        }
        int r = snprintf(buf, sizeof(buf) - 1, "/proc/%d/exe", proc);
        if (r > 0) {
            buf[r] = 0;
	    memset(procPath, 0, sizeof(procPath));
            ssize_t t = readlink(buf, procPath, sizeof(procPath) - 1);
            if (t > 0) {
                QString procPathQ(procPath);
		qInfo() << procPathQ;
                if (procPathQ.toLower().endsWith(bufR)) {
                    qInfo() << "[utils] proc name: " << procName << " pid: " << proc;
                    isFound = true;
                }
            }
        }
    }
    closedir(dir);

    return isFound;
}

int main ()
{
    qInfo() << "x: " << check_proc_exists_by_name("x");
    qInfo() << "x: " << check_proc_exists_by_name("xorg");

    return 0;
}
