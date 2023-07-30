#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <KWayland/Client/shell.h>
#include <KWayland/Client/seat.h>
#include <KWayland/Client/registry.h>
#include <KWayland/Client/connection_thread.h>
#include <KWayland/Client/pointer.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    virtual void actionEvent(QActionEvent *event);
    virtual void changeEvent(QEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragLeaveEvent(QDragLeaveEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void enterEvent(QEvent *event);
    virtual void focusInEvent(QFocusEvent *event);
    virtual bool focusNextPrevChild(bool next);
    virtual void focusOutEvent(QFocusEvent *event);
    virtual void hideEvent(QHideEvent *event);
    virtual void inputMethodEvent(QInputMethodEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void moveEvent(QMoveEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void tabletEvent(QTabletEvent *event);

protected:
    void endDrag();

private:
    bool m_is_dragging = false;
    bool m_prepared_to_drag = false;
    bool isDragable(QWidget *widget);
    bool dragable = true;
    QPoint m_start_point;
    QTimer m_timer;

    KWayland::Client::ConnectionThread *m_connection = nullptr;
    KWayland::Client::Registry *m_registry = nullptr;
    KWayland::Client::Seat *m_seat = nullptr;
    KWayland::Client::Pointer *m_pointer = nullptr;
    int m_serial = 0;
};

#endif // MAINWINDOW_H
