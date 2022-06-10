#include "clipboard-utils.h"

#include <QDebug>
#include <unistd.h>
#include <QApplication>
#include <QClipboard>
#include <QMetaObject>
#include <QMetaProperty>
#include <QMimeData>
#include <QUrl>
#include <QMutex>
#include <iostream>

#include <X11/X.h>
#include <X11/Xlib.h>

static QStringList gClipboardFileUrls;
static QMutex gClipboardFileUrlsMutex;
static QList<quint64> gClipbordFileinode;
static QAtomicInt gRemoteCurrentCount = 0;


ClipboardUtils::ClipboardUtils(QObject *parent) : QObject (parent)
{
    qApp->clipboard ()->installEventFilter (this);

    connect (QApplication::clipboard (), &QClipboard::changed, this, [=] () {
        qDebug() << qApp->clipboard()->mimeData();;
        qDebug() << qApp->clipboard ()->text ();
        //onClipboardDataChanged();

        qDebug() << "\n\n\n\n\n\n\n";
//        const QMetaObject* metaObject = QApplication::clipboard ()->metaObject();

//        for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i) {
//            qDebug() << "static property name ===>" << QString::fromLatin1(metaObject->property(i).name());
//        }

//        QList<QByteArray> dp = QApplication::clipboard ()->dynamicPropertyNames();
//        for (auto s = dp.constBegin (); s != dp.constEnd (); ++s) {
//            qDebug() << "dynamicPropertyNames ===>" << *s;
//        }
    });

//    connect (QApplication::clipboard (), &QClipboard::dataChanged, this, &ClipboardUtils::onClipboardDataChanged);

    connect (QApplication::clipboard (), &QClipboard::findBufferChanged, this, [=] () {
    });

    connect (QApplication::clipboard (), &QClipboard::selectionChanged, this, [=] () {
    });

    onClipboardDataChanged();
}

bool ClipboardUtils::eventFilter(QObject *obj, QEvent *ev)
{
    qDebug() << ev;

    return false;

    Q_UNUSED (obj);
}

QStringList ClipboardUtils::getUrlsByX11()
{
#if 0
    QAtomicInt currentCount = remoteCurrentCount;
    const QMimeData *mimedata = qApp->clipboard()->mimeData();
    if (!mimedata) {
        qWarning() << "the clipboard mimedata is invalid!";
        return QStringList ();
    }
//    if (!isRemoteUri) {
//        qWarning() << "current action is not RemoteAction ,error action " << !isRemoteUri;
//        return QStringList ();
//    }

    //使用x11创建一个窗口去阻塞获取URl
    Display *display = XOpenDisplay(nullptr);
    unsigned long color = BlackPixel(display, DefaultScreen(display));
    Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0,0, 1,1, 0, color, color);

    char *result = nullptr;
    unsigned long ressize = 0, restail = 0;
    int resbits;
    Atom bufid = XInternAtom(display, "CLIPBOARD", False),
        fmtid = XInternAtom(display, "text/uri-list", False),
        propid = XInternAtom(display, "XSEL_DATA", False),
        incrid = XInternAtom(display, "INCR", False);
    XEvent event;

    QList<QUrl> urls;
    QString results;
    QAtomicInteger<bool> isCanceled = false;

    XSelectInput (display, window, PropertyChangeMask);
    XConvertSelection(display, bufid, fmtid, propid, window, CurrentTime);
    do {
        XNextEvent(display, &event);
        qInfo () << "event-type:" << event.type;
    } while (event.type != SelectionNotify || event.xselection.selection != bufid);
    if (event.xselection.property) {
        XGetWindowProperty(display, window, propid, 0, LONG_MAX/4, True, AnyPropertyType,
                           &fmtid, &resbits, &ressize, &restail, (unsigned char**)&result);
        if (fmtid != incrid) {
            qInfo() << QString(result);
            urls += QUrl::fromStringList(QString(result).split("\n"));
        }

        XFree(result);

        if (fmtid == incrid) {
            do {
                do {
                    XNextEvent(display, &event);
                    if (event.type == SelectionNotify) {
                        isCanceled = true;
                        break;
                    }
                } while (event.type != PropertyNotify || event.xproperty.atom != propid || event.xproperty.state != PropertyNewValue);
                if (isCanceled)
                    break;
                XGetWindowProperty(display, window, propid, 0, LONG_MAX/4, True, AnyPropertyType, &fmtid, &resbits, &ressize, &restail, (unsigned char**)&result);
                if (QString(result) != "/")
                    results += QString(result);
                XFree(result);
            } while (ressize > 0);
        }
    }
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    if (isCanceled) {
        qWarning() << "user cancel remote download !";
        return QStringList ();
    }
    urls += QUrl::fromStringList(results.split("\n"));

    QStringList clipboardFileUrls;
    for (QUrl url : urls) {
        //链接文件的inode不加入clipbordFileinode，只用url判断clip，避免多个同源链接文件的逻辑误判
        if (url.toString() == "copy")
            continue;

        QString path = url.path();
        path = path.replace(QRegExp("/*/"),"/");
        if (path.isEmpty() || path == "/")
            continue;
        QUrl temp;
        temp.setScheme("file");
        temp.setPath(path);
        clipboardFileUrls.append (temp.toString ());
    }
    qInfo() << results << urls << clipboardFileUrls;

    if (isRemoteUri && currentCount == remoteCurrentCount) {
        QMutexLocker lk(&gClipboardFileUrlsMutex);
        gClipboardFileUrls.clear();
        gClipboardFileUrls = clipboardFileUrls;
        gRemoteCurrentCount = 0;
    }

    return clipboardFileUrls;
#endif
    return QStringList();
}

void ClipboardUtils::onClipboardDataChanged()
{
    const QMimeData *mimeData = qApp->clipboard()->mimeData();
    if (!mimeData || mimeData->formats().isEmpty()) {
        qWarning() << "get null mimeData from QClipBoard or remote formats is null!";
        return;
    }

    qDebug() << mimeData->formats ();

    if (mimeData->hasFormat("uos/remote-copy")) {
        qInfo() << "clipboard use other !";
        isRemoteUri = true;
        remoteCurrentCount++;

        qDebug() << "=====>" << getUrlsByX11();
        return;
    } else {
        qDebug() << "has no uos/remote-copy";
    }
}
