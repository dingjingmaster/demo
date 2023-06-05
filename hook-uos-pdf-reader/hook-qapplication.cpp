/*************************************************************************
> FileName: hook-qapplication.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 05 Jun 2023 10:31:21 AM CST
 ************************************************************************/

#include <stdio.h>
#include <dlfcn.h>

#include <QStyle>
#include <QString>


typedef QStyle* (*QApplicationSetStylePtr) (const QString& style);

extern "C"
QStyle* _ZN12QApplication8setStyleERK7QString(const QString& style)
{
    printf ("HOOK setStyle HOOK!\n");

    QApplicationSetStylePtr st = (QApplicationSetStylePtr) dlsym ((void*) -1, "_ZN12QApplication8setStyleERK7QString");
    if (NULL == st) {
        printf ("HOOK setStyle HOOK error!\n");
        return NULL;
    }

    printf ("style: %s\n", style.toUtf8().constData());

    printf ("HOOK setStyle TO hook-qstyle!\n");

    return st ("HOOK hook-qstyle");
}

extern "C"
QStyle* _ZN13QStyleFactory6createERK7QString (const QString& style)
{
    printf ("HOOK create Style HOOK!\n");

    QApplicationSetStylePtr st = (QApplicationSetStylePtr) dlsym ((void*) -1, "_ZN12QApplication8setStyleERK7QString");
    if (NULL == st) {
        printf ("HOOK create Style HOOK error!\n");
        return NULL;
    }

    printf ("style: %s\n", style.toUtf8().constData());

    printf ("HOOK create Style TO hook-qstyle!\n");

    return st ("HOOK hook-qstyle");
}
