#include <stdio.h>
#include <stdlib.h>
#include <sys/syslog.h>
#include <syslog.h>
#include <QFileDialog>
#include <QDebug>

#ifdef __cplusplus
extern "C" 
{
#endif

// _ZNK11QFileDialog13selectedFilesEv
QStringList _ZNK11QFileDialog13selectedFilesEv (void* p)
{
    printf("HOOK IN, obj: %p\n", p);
#if 1

    void* pThis = (QFileDialog*) p;

    if (!pThis) return QStringList();

    QFileDialog* dlg = (QFileDialog*) pThis;

    auto urls = dlg->selectedUrls();

    QStringList ls;
    for (auto u : urls) {
        ls << u.path();
    }

    qDebug() << "urls: " << urls;
#endif

    return ls;
}

#ifdef __cplusplus
}
#endif
