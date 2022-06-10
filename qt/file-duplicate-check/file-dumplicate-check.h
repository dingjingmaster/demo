#ifndef FILEDUMPLICATECHECK_H
#define FILEDUMPLICATECHECK_H

#include <QString>
#include <gio/gio.h>

// fixme:// 硬链接检测
QString getFileMD5 (GFile* uri);
goffset getFileSize (GFile* uri);

bool fileIsDumplicate(QString uri1, QString uri2);



#endif // FILEDUMPLICATECHECK_H
