/*************************************************************************
> FileName: host-address.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 09 Aug 2024 11:05:31 AM CST
 ************************************************************************/
#include <QDebug>
#include <QLocale>
#include <QSysInfo>
#include <QStorageInfo>

int main (int argc, char* argv[])
{
    auto ls = QStorageInfo::mountedVolumes();

    for (auto s : ls) {
        qDebug() << "path: " << s.rootPath();
    }

    qInfo() << "filter ===>";
    for (auto s : ls) {
        if (s.isRoot() || s.isReadOnly() || !s.isValid() 
	    || "/opt" == s.rootPath() || "/root" == s.rootPath() || "/var" == s.rootPath() || "/run" == s.rootPath()
	    || s.rootPath().startsWith("/run/") || "/data" == s.rootPath() || "/home" == s.rootPath()
	    || s.rootPath().contains("/var/lib/andsec/mount/")) {
            continue;
        }

        qInfo() << s.rootPath();
    }
}
