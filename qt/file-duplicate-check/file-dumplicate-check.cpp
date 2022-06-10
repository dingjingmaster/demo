#include "file-dumplicate-check.h"

#include <QFile>
#include <QDebug>
#include <QProcess>
#include <QCryptographicHash>

#include <fcntl.h>
#include <gio/gio.h>
#include <sys/stat.h>
#include <sys/types.h>


QString getFileMD5(GFile* file)
{
    g_return_val_if_fail(G_IS_FILE(file), "");

    g_return_val_if_fail(g_file_query_exists(file, nullptr), "");

    g_autofree char* path = g_file_get_path(file);

    QProcess pro;
    QStringList args;

    args << path;

    pro.setProgram("md5sum");
    pro.setArguments(args);

    pro.start();
    pro.waitForFinished(-1);

    QString res = pro.readAllStandardOutput();
    QString md5 = res.split(" ").at(0);

    return md5;
}

goffset getFileSize(GFile* file)
{
    g_return_val_if_fail(G_IS_FILE(file), 0);

    g_return_val_if_fail(g_file_query_exists(file, nullptr), 0);

    g_autoptr(GFileInfo) fileInfo = g_file_query_info(file, "standard::*", G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, nullptr, nullptr);

    g_return_val_if_fail(G_IS_FILE_INFO(fileInfo), 0);

    return g_file_info_get_size(fileInfo);
}

bool fileIsDumplicate(QString uri1, QString uri2)
{
    g_return_val_if_fail(uri1 != uri2, true);

    g_autoptr(GFile) file1 = g_file_new_for_uri(uri1.toUtf8().constData());
    g_autoptr(GFile) file2 = g_file_new_for_uri(uri2.toUtf8().constData());

    g_return_val_if_fail(G_IS_FILE(file1), false);
    g_return_val_if_fail(G_IS_FILE(file2), false);

    g_return_val_if_fail(g_file_query_exists(file1, nullptr) && g_file_is_native(file1), false);
    g_return_val_if_fail(g_file_query_exists(file2, nullptr) && g_file_is_native(file2), false);

    g_return_val_if_fail(G_FILE_TYPE_REGULAR == g_file_query_file_type(file1, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, nullptr), false);
    g_return_val_if_fail(G_FILE_TYPE_REGULAR == g_file_query_file_type(file2, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, nullptr), false);

    // check is hard link
    g_autofree char* path1 = g_file_get_path(file1);
    g_autofree char* path2 = g_file_get_path(file2);

    int             ret1 = 0;
    int             ret2 = 0;
    goffset         fileSize1 = 0;
    goffset         fileSize2 = 0;

    QProcess        pro;
    QStringList     args;
    QString         fileMD51;
    QString         fileMD52;
    struct stat     statBuf1;
    struct stat     statBuf2;

    // hard link not compare
    int fd1 = open(path1, O_RDONLY);
    int fd2 = open(path2, O_RDONLY);

    if (-1 == fd1 || -1 == fd2) {
        goto error;
    }

    ret1 = fstat(fd1, &statBuf1);
    ret2 = fstat(fd2, &statBuf2);
    if (-1 == ret1 || -1 == ret2
        || statBuf1.st_nlink > 1 || statBuf2.st_nlink > 1) {
        goto error;
    }

    // compare file size
    fileSize1 = getFileSize(file1);
    fileSize2 = getFileSize(file2);

    g_return_val_if_fail(fileSize1 == fileSize2, false);

    // compare md5
    fileMD51 = getFileMD5(file1);
    fileMD52 = getFileMD5(file2);

    g_return_val_if_fail(fileMD51 == fileMD52, false);

    // diff file's content
    args.clear();
    args << path1 << path2;

    pro.setProgram("diff");
    pro.setArguments(args);

    pro.start();
    pro.waitForFinished(-1);

    return pro.readAll().isEmpty();

error:
    if (-1 != fd1) close(fd1);
    if (-1 != fd2) close(fd2);

    return false;
}
