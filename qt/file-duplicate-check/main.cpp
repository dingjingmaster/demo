#include <QDebug>
#include <gio/gio.h>
#include "file-dumplicate-check.h"

int main(int argc, char *argv[])
{
    //
    QString fileUri1 = "file:///home/dingjing/a.txt";
    g_autoptr(GFile) file1 = g_file_new_for_uri(fileUri1.toUtf8().constData());

    QString fileUri2 = "file:///home/dingjing/a-link.txt";
    g_autoptr(GFile) file2 = g_file_new_for_uri(fileUri2.toUtf8().constData());

    QString fileUri3 = "file:///home/dingjing/a-hard-link-啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊.txt";
    g_autoptr(GFile) file3 = g_file_new_for_uri(fileUri3.toUtf8().constData());


    QString fileUri21 = "file:///home/dingjing/b.txt";
    g_autoptr(GFile) file21 = g_file_new_for_uri(fileUri21.toUtf8().constData());

    QString fileUri22 = "file:///home/dingjing/b-link.txt";
    g_autoptr(GFile) file22 = g_file_new_for_uri(fileUri22.toUtf8().constData());

    QString fileUri23 = "file:///home/dingjing/b-hard-link.txt";
    g_autoptr(GFile) file23 = g_file_new_for_uri(fileUri23.toUtf8().constData());


    qDebug() << "file: " << fileUri1 << " --- md5: |" << getFileMD5 (file1) << "|";
    qDebug() << "file: " << fileUri2 << " --- md5: |" << getFileMD5 (file2) << "|";
    qDebug() << "file: " << fileUri3 << " --- md5: |" << getFileMD5 (file3) << "|";

    qDebug() << "file: " << fileUri1 << " --- size: |" << getFileSize (file21) << "|";
    qDebug() << "file: " << fileUri2 << " --- size: |" << getFileSize (file22) << "|";
    qDebug() << "file: " << fileUri3 << " --- size: |" << getFileSize (file23) << "|";

    qDebug() << "file1: " << fileUri1 << " -- file2: " << fileUri21 << " -- dumplicate: " << "|" << fileIsDumplicate(fileUri1, fileUri21);
    qDebug() << "file1: " << fileUri2 << " -- file2: " << fileUri22 << " -- dumplicate: " << "|" << fileIsDumplicate(fileUri2, fileUri21);
    qDebug() << "file1: " << fileUri3 << " -- file2: " << fileUri23 << " -- dumplicate: " << "|" << fileIsDumplicate(fileUri3, fileUri23);

    qDebug() << "file1: " << fileUri1 << " -- file2: " << fileUri1 << " -- dumplicate: " << "|" << fileIsDumplicate(fileUri1, fileUri1);
    qDebug() << "file1: " << fileUri2 << " -- file2: " << fileUri2 << " -- dumplicate: " << "|" << fileIsDumplicate(fileUri2, fileUri2);
    qDebug() << "file1: " << fileUri3 << " -- file2: " << fileUri3 << " -- dumplicate: " << "|" << fileIsDumplicate(fileUri3, fileUri3);

    qDebug() << "file1: " << fileUri21 << " -- file2: " << fileUri21 << " -- dumplicate: " << "|" << fileIsDumplicate(fileUri21, fileUri21);

    Q_UNUSED(argc)
    Q_UNUSED(argv)

    return 0;
}
