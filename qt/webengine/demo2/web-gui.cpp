#include "web-gui.h"

#include <QDebug>
#include <QMutex>
#include <QScreen>
#include <QX11Info>
#include <QCloseEvent>
#include <QApplication>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QNetworkProxyFactory>
#include <QWebEngineCookieStore>

#include "../lib/Configure.h"

#include <X11/Xlib.h>


#define WEB_URL_WORKBENCH				"/workbench"
#define WEB_URL_LOGIN					"/desktop"



WebGui* WebGui::gInstance = NULL;


WebGui::WebGui(QWidget *parent)
    : QWidget{parent},
    core(new WebExportCore(Configure::getConfigValue ("client","id"))),
    mMainLayout(new QHBoxLayout),
    mWebChannel(new QWebChannel(this)),
    mWebEngineView(new QWebEngineView(this))
{
    setMouseTracking(true);
    setMinimumSize(120, 120);
    setContentsMargins (0, 0, 0, 0);
    setWindowFlag(Qt::FramelessWindowHint);
    QNetworkProxyFactory::setUseSystemConfiguration(false);

    mMainLayout->addWidget(mWebEngineView);

    mWebChannel->registerObject("iand", this->core);  // 向QWebChannel注册用于Qt和Web交互的对象。

    setLayout(mMainLayout);
}

bool WebGui::eventFilter(QObject* watched, QEvent *ev)
{
    if (watched == mWebEngineView) {
        switch (ev->type()) {
        case QEvent::MouseButtonPress: {
            mousePressEvent(dynamic_cast<QMouseEvent*>(ev));
            return false;
        }
        case QEvent::MouseButtonRelease: {
            mouseReleaseEvent(dynamic_cast<QMouseEvent*>(ev));
            return false;
        }
        case QEvent::MouseMove: {
            mouseMoveEvent(dynamic_cast<QMouseEvent*>(ev));
            return false;
        }
        default:
            break;
        }
    }

    return true;
}

WebGui* WebGui::getInstance()
{
    static int inited = 0;
    static QMutex mutex;

    if (!inited) {
        mutex.lock();
        if (!gInstance) {
            gInstance = new WebGui(nullptr);
        }
        mutex.unlock();
    }

    return gInstance;
}

bool WebGui::loadPage(WebPage page)
{
    QUrl url = getBaseUrl(page);
    if (url.isEmpty()) {
        return false;
    }

    mWebEngineView->load(url);

    // 此处设置

    return true;
}

void WebGui::resizeEvent(QResizeEvent* e)
{
    QWidget::resizeEvent (e);
}

void WebGui::mouseMoveEvent(QMouseEvent* e)
{
    QPoint globalPos = e->globalPos();
    QRect rect = this->rect();
    QPoint tl = mapToGlobal (rect.topLeft());
    QPoint rb = mapToGlobal (rect.bottomRight());

    qreal dpiRatio = qApp->devicePixelRatio ();
    //    qDebug() << "pressed: " << (mIsPress ? "true" : "false");
    if (!mIsPress) {
        region (globalPos);
    }
    else {
        if (NONE != mDirection) {
            QRect rMove (tl, rb);
            switch (mDirection) {
            case LEFT: {
                if (rb.x () - globalPos.x () <= this->minimumWidth ()) {
                    rMove.setX (tl.x ());
                }
                else {
                    rMove.setX (globalPos.x ());
                }
                //                    qDebug() << "pressed: " << (mIsPress ? "true" : "false") << " left";
                break;
            }
            case RIGHT: {
                rMove.setWidth (globalPos.x () - tl.x ());
                //                    qDebug() << "pressed: " << (mIsPress ? "true" : "false") << " right";
                break;
            }
            case UP: {
                if (rb.y () - globalPos.y () <= this->minimumHeight ()) {
                    rMove.setY (tl.y ());
                }
                else {
                    rMove.setY (globalPos.y ());
                }
                //                    qDebug() << "pressed: " << (mIsPress ? "true" : "false") << " up";
                break;
            }
            case DOWN: {
                rMove.setHeight (globalPos.y () - tl.y ());
                //                    qDebug() << "pressed: " << (mIsPress ? "true" : "false") << " down";
                break;
            }
            case LEFT_TOP: {
                if (rb.x () - globalPos.x () <= this->minimumWidth ()) {
                    rMove.setX (tl.x ());
                }
                else {
                    rMove.setX (globalPos.x ());
                }

                if (rb.y () - globalPos.y () <= this->minimumHeight ()) {
                    rMove.setY (tl.y ());
                }
                else {
                    rMove.setY (globalPos.y ());
                }
                //                    qDebug() << "pressed: " << (mIsPress ? "true" : "false") << " left_top";
                break;
            }
            case RIGHT_TOP: {
                rMove.setWidth (globalPos.x () - tl.x ());
                rMove.setY (globalPos.y ());
                //                    qDebug() << "pressed: " << (mIsPress ? "true" : "false") << " right_top";
                break;
            }
            case LEFT_BOTTOM: {
                rMove.setX (globalPos.x ());
                rMove.setHeight (globalPos.y () - tl.y ());
                //                    qDebug() << "pressed: " << (mIsPress ? "true" : "false") << " left_bottom";
                break;
            }
            case RIGHT_BOTTOM: {
                rMove.setWidth (globalPos.x () - tl.x ());
                rMove.setHeight (globalPos.y () - tl.y ());
                //                    qDebug() << "pressed: " << (mIsPress ? "true" : "false") << " right_bottom";
                break;
            }
            default: {
                break;
            }
            }
            this->setGeometry(rMove);
            return;
        }
    }

    if (mDrag) {
        if (QX11Info::isPlatformX11 ()) {
            Display *display = QX11Info::display ();
            Atom netMoveResize = XInternAtom (display, "_NET_WM_MOVERESIZE", False);
            XEvent xEvent;
            const auto pos = QCursor::pos ();

            memset (&xEvent, 0, sizeof (XEvent));
            xEvent.xclient.type = ClientMessage;
            xEvent.xclient.message_type = netMoveResize;
            xEvent.xclient.display = display;
            xEvent.xclient.window = this->winId ();
            xEvent.xclient.format = 32;
            xEvent.xclient.data.l[0] = pos.x () * dpiRatio;
            xEvent.xclient.data.l[1] = pos.y () * dpiRatio;
            xEvent.xclient.data.l[2] = 8;
            xEvent.xclient.data.l[3] = Button1;
            xEvent.xclient.data.l[4] = 0;

            XUngrabPointer (display, CurrentTime);
            XSendEvent (display, QX11Info::appRootWindow (QX11Info::appScreen ()), False,
                       SubstructureNotifyMask | SubstructureRedirectMask, &xEvent);
            //XFlush(display);
            XEvent xevent;
            memset (&xevent, 0, sizeof (XEvent));

            xevent.type = ButtonRelease;
            xevent.xbutton.button = Button1;
            xevent.xbutton.window = this->winId ();
            xevent.xbutton.x = e->pos ().x () * dpiRatio;
            xevent.xbutton.y = e->pos ().y () * dpiRatio;
            xevent.xbutton.x_root = pos.x () * dpiRatio;
            xevent.xbutton.y_root = pos.y () * dpiRatio;
            xevent.xbutton.display = display;

            XSendEvent (display, this->effectiveWinId (), False, ButtonReleaseMask, &xevent);
            XFlush (display);

            if (e->source () == Qt::MouseEventSynthesizedByQt) {
                if (!WebGui::mouseGrabber ()) {
                    this->grabMouse ();
                    this->releaseMouse ();
                }
            }
            mDrag = false;
        }
        else {
            move ((QCursor::pos () - mOffset) * dpiRatio);
        }
    }
    QWidget::mouseMoveEvent(e);
}

void WebGui::mousePressEvent(QMouseEvent* e)
{
    QWidget::mousePressEvent (e);

    if (e->isAccepted()) {
        return;
    }

    switch (e->button()) {
        case Qt::LeftButton: {
            mDrag = true;
            mIsPress = true;
            mOffset = mapFromGlobal (QCursor::pos());
            if (NONE != mDirection) {
                mouseGrabber();
            }
            else {
                mDragPos = e->globalPos() - frameGeometry().topLeft();
            }
            break;
        }
        case Qt::RightButton:
        default: {
            QWidget::mousePressEvent (e);
        }
    }
}

void WebGui::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton || e->button() == Qt::RightButton) {
        mDrag = false;
        mIsPress = false;
        if (NONE != mDirection) {
            releaseMouse();
            setCursor (QCursor(Qt::ArrowCursor));
        }
    }
    else {
        QWidget::mouseReleaseEvent (e);
    }
}

void WebGui::showEvent(QShowEvent* ev)
{
    mWebEngineView->page()->setWebChannel(mWebChannel);

    mWebEngineView->show();

    QWidget::showEvent(ev);
}

bool WebGui::deleteDir(const QString &path)
{
    if (path.isEmpty()){
        return false;
    }

    QDir dir(path);
    if (!dir.exists()) {
        return true;
    }

    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    foreach (QFileInfo file, fileList) {
        //遍历文件信息
        if (file.isFile()) {
            // 是文件，删除
            file.dir().remove(file.fileName());
        }
        else { // 递归删除
            deleteDir(file.absoluteFilePath());
        }
    }

    return dir.rmpath(dir.absolutePath()); // 删除文件夹
}

QUrl WebGui::getBaseUrl(WebPage page)
{
    QString port = Configure::getConfigValue("server", "port");
    QString address = Configure::getConfigValue("server", "address");

    if ("" == port || port.isEmpty() || "" == address || address.isEmpty()) {
        return QUrl();
    }

    QString baseUrl = QString("https://%1:%2").arg(address).arg(port);

    switch (page) {
    case WP_WORKBENCH: {
        return baseUrl + WEB_URL_WORKBENCH;
    }
    }

    return baseUrl + WEB_URL_LOGIN;
}

void WebGui::region(const QPoint &cursorGlobalPoint)
{
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());
    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    //    qDebug() << "region -- tl: " << tl << " rb: " << rb << " x: " << x << " b: " << y;

    if (tl.x() + 3 >= x && x >= tl.x() - 3 && tl.y() + 3 >= y && y >= tl.y() - 3) {
        mDirection = LEFT_TOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
    }
    else if (x >= rb.x() - 3 && x <= rb.x() + 3 && y >= rb.y() - 3 && y <= rb.y() + 3) {
        mDirection = RIGHT_BOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if (x <= tl.x() + 3 && x >= tl.x() - 3 && y >= rb.y() - 3 && y <= rb.y() + 3) {
        mDirection = LEFT_BOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if (x <= rb.x() + 3 && x >= rb.x() - 3 && y >= tl.y() + 3 && y <= tl.y() - 3) {
        mDirection = RIGHT_TOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if (x <= tl.x() + 3 && x >= tl.x() - 3) {
        mDirection = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if (x <= rb.x() + 3 && x >= rb.x() - 3) {
        mDirection = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if (y >= tl.y() - 3 && y <= tl.y() + 3) {
        mDirection = UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if (y <= rb.y() + 3 && y >= rb.y() - 3) {
        mDirection = DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else {
        mDirection = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}
