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
		auto tp = s.fileSystemType();
        if (s.isReadOnly() || !s.isValid() || !s.isReady() || tp.contains("tmpfs")) {
            continue;
        }

        qInfo() << s.rootPath() << " tp: " << tp;
    }
}
