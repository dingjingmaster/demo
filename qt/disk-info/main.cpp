#include <QCoreApplication>
#include <QDebug>
#include <QStorageInfo>

#include <glib.h>
#include <glib/gprintf.h>

#define GIB 2^30

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QStorageInfo disk ("/data");

    qDebug() << "total:" << (disk.bytesTotal() / (GIB)) << "  --  " << g_format_size(disk.bytesTotal())
             << "  ==  " << g_format_size_full(disk.bytesTotal(), G_FORMAT_SIZE_DEFAULT)
             << "  ==  " << g_format_size_full(disk.bytesTotal(), G_FORMAT_SIZE_LONG_FORMAT)
             << "  ==  " << g_format_size_full(disk.bytesTotal(), G_FORMAT_SIZE_IEC_UNITS)
             << "  ==  " << g_format_size_full(disk.bytesTotal(), G_FORMAT_SIZE_BITS);

    qDebug() << "free:" << (disk.bytesFree() / (GIB)) << "  --  " << g_format_size(disk.bytesTotal())
             << "  ==  " << g_format_size_full(disk.bytesFree(), G_FORMAT_SIZE_DEFAULT)
             << "  ==  " << g_format_size_full(disk.bytesFree(), G_FORMAT_SIZE_LONG_FORMAT)
             << "  ==  " << g_format_size_full(disk.bytesFree(), G_FORMAT_SIZE_IEC_UNITS)
             << "  ==  " << g_format_size_full(disk.bytesFree(), G_FORMAT_SIZE_BITS);

    return a.exec();
}
