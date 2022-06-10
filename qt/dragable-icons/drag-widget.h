#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QFrame>

QT_BEGIN_NAMESPACE
class QDropEvent;
class QDragMoveEvent;
class QDragEnterEvent;
QT_END_NAMESPACE

class DragWidget : public QFrame
{
    Q_OBJECT
public:
    DragWidget(QWidget *parent = nullptr);
    ~DragWidget();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};
#endif // DRAGWIDGET_H
