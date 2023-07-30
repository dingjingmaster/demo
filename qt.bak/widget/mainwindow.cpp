#include "mainwindow.h"
#include <QX11Info>
#include <QDebug>
#include <QMouseEvent>

#include <KWindowSystem>
#include <NETWM>
#include <QX11Info>
#include <xcb/xcb.h>

#include <QApplication>
#include "xatom-helper/xatom-helper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setProperty("useStyleWindowManager", false);
    //setContextMenuPolicy(Qt::CustomContextMenu);
    //installEventFilter(this);
    //setAttribute(Qt::WA_DeleteOnClose);
    //setAttribute(Qt::WA_TranslucentBackground);
    //setMouseTracking(true);

    //if (QX11Info::isPlatformX11()) {
    //    XatomHelper::getInstance()->setUKUIDecoraiontHint(this->winId(), true);
    //    MotifWmHints hints;
    //    hints.flags = MWM_HINTS_FUNCTIONS; //|MWM_HINTS_DECORATIONS;
    //    hints.functions = MWM_FUNC_ALL;
    //    hints.decorations = MWM_DECOR_BORDER;
    //    XatomHelper::getInstance()->setWindowMotifHint(this->winId(), hints);
    //}
}

MainWindow::~MainWindow()
{
}

void MainWindow::actionEvent(QActionEvent *event)
{

}

void MainWindow::changeEvent(QEvent *event)
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{

}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{

}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{

}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{

}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{

}

void MainWindow::dropEvent(QDropEvent *event)
{

}

void MainWindow::enterEvent(QEvent *event)
{

}

void MainWindow::focusInEvent(QFocusEvent *event)
{

}

bool MainWindow::focusNextPrevChild(bool next)
{

}

void MainWindow::focusOutEvent(QFocusEvent *event)
{

}

void MainWindow::hideEvent(QHideEvent *event)
{

}

void MainWindow::inputMethodEvent(QInputMethodEvent *event)
{

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{

}

void MainWindow::leaveEvent(QEvent *event)
{
    //qDebug() << "leave event";

}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug() << "mouse double click event";

}

void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    qDebug() << "mouse move event";

    const QPoint native = e->globalPos();
    qreal  dpiRatio = qApp->devicePixelRatio();
    qDebug() << __FUNCTION__ << " --- 1";
    if (QX11Info::isPlatformX11()) {
        //if (m_is_dragging)
        //    return;
        auto connection = QX11Info::connection();
        qDebug() << __FUNCTION__ << " --- 2";

        if (m_is_dragging) {
            qDebug()<<"x11 move start";
            xcb_ungrab_pointer(connection, XCB_TIME_CURRENT_TIME);
            NETRootInfo(connection, NET::WMMoveResize).moveResizeRequest(winId(), native.x() * dpiRatio, native.y() * dpiRatio, NET::Move);
            qDebug()<<"x11 move end";

            if (e->source() == Qt::MouseEventSynthesizedByQt) {
                if (!this->mouseGrabber()) {
                    this->grabMouse();
                    this->releaseMouse();
                }
            }

        }

        xcb_button_release_event_t* event = new xcb_button_release_event_t;
        memset(event, 0x00, sizeof(xcb_button_release_event_t));
        event->response_type = XCB_BUTTON_RELEASE; // | XCB_BUTTON_PRESS;
        event->event = winId();
        event->time = QX11Info::getTimestamp();
        event->same_screen = 1;
        event->root = QX11Info::appRootWindow();
        event->root_x = native.x();
        event->root_y = native.y();
        event->event_x = 0;
        event->event_y = 0;
        event->child = 0;
        event->state = 0;
        event->detail = XCB_BUTTON_INDEX_1;

        xcb_send_event(connection, false, winId(), XCB_EVENT_MASK_BUTTON_RELEASE, (char *) event);
        delete event;

        xcb_flush(connection);

//        m_is_dragging = true;

        m_timer.start();
    } else {
        auto topLevel = topLevelWidget();

        auto shellSurface = KWayland::Client::ShellSurface::fromWindow(topLevel->windowHandle());
        if (!shellSurface)
            return;
        shellSurface->requestMove(m_seat, m_serial);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    qDebug() << "mouse press event";
    endDrag();
    //NOTE: We have to skip the border for resize event.
    auto pos = mapFromGlobal(e->globalPos());
    if (!rect().adjusted(10, 10, -10, -10).contains(pos)) {
        //qDebug()<<"skip move event";
        return;
    }

    m_is_dragging = true;
    m_start_point = e->pos();
    m_timer.start();

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "mouse release event";
    m_is_dragging = false;
    m_start_point = QPoint(0, 0);
    m_timer.stop();


    if (QX11Info::isPlatformX11()) {
    auto connection = QX11Info::connection();
    xcb_button_release_event_t* event = new xcb_button_release_event_t;
    memset(event, 0x00, sizeof(xcb_button_release_event_t));
    event->response_type = XCB_BUTTON_PRESS;
    event->event = winId();
    event->time = QX11Info::getTimestamp();
    event->same_screen = 1;
    event->root = QX11Info::appRootWindow();
    event->event_x = 0;
    event->event_y = 0;
    event->child = 0;
    event->state = 0;
    event->detail = XCB_BUTTON_INDEX_1;

    xcb_send_event(connection, false, winId(), XCB_EVENT_MASK_BUTTON_PRESS, (char *) event);
    delete event;

    xcb_flush(connection);
    }


}

void MainWindow::moveEvent(QMoveEvent *event)
{
    qDebug() << "move event";

}

void MainWindow::resizeEvent(QResizeEvent *event)
{

}

void MainWindow::tabletEvent(QTabletEvent *event)
{
}

void MainWindow::endDrag()
{
    m_is_dragging = false;
    m_start_point = QPoint(0, 0);
    m_timer.stop();
}


