#ifndef DESKTOPVIEW_H
#define DESKTOPVIEW_H

#include <QObject>
#include <QAbstractItemView>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QFocusEvent>
#include <QFocusEvent>
#include <QInputMethodEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QEvent>
#include <QStandardItemModel>

class DesktopView : public QAbstractItemView
{
    Q_OBJECT
public:
    DesktopView (QWidget* parent = nullptr);

public:
    virtual QModelIndex	indexAt(const QPoint &point) const override;
    virtual QRect visualRect(const QModelIndex &index) const override;
    virtual void scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint = EnsureVisible) override;

protected:
    virtual int verticalOffset() const override;
    virtual int	horizontalOffset() const override;
    virtual bool isIndexHidden(const QModelIndex &index) const override;
    virtual QRegion	visualRegionForSelection(const QItemSelection &selection) const override;
    virtual void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags) override;
    virtual QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;

protected:
    void paintEvent (QPaintEvent* e) override;

//    virtual void dragEnterEvent(QDragEnterEvent *event) override;
//    virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
//    virtual void dragMoveEvent(QDragMoveEvent *event) override;
//    virtual void dropEvent(QDropEvent *event) override;
//    virtual bool event(QEvent *event) override;
//    virtual bool eventFilter(QObject *object, QEvent *event) override;
//    virtual void focusInEvent(QFocusEvent *event) override;
//    virtual bool focusNextPrevChild(bool next) override;
//    virtual void focusOutEvent(QFocusEvent *event) override;
//    virtual void inputMethodEvent(QInputMethodEvent *event) override;
//    virtual void keyPressEvent(QKeyEvent *event) override;
//    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
//    virtual void mouseMoveEvent(QMouseEvent *event) override;
//    virtual void mousePressEvent(QMouseEvent *event) override;
//    virtual void mouseReleaseEvent(QMouseEvent *event) override;
//    virtual void resizeEvent(QResizeEvent *event) override;
//    virtual void timerEvent(QTimerEvent *event) override;
//    virtual bool viewportEvent(QEvent *event) override;
//    virtual QSize viewportSizeHint() const override;

};

#endif // DESKTOPVIEW_H
