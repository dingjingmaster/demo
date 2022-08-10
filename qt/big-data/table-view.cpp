#include "table-view.h"

#include <QScrollBar>
#include <QWheelEvent>

TableView::TableView(QWidget *parent):QTableView(parent)
{
    verticalScrollBar()->setMaximum(100);
    connect (verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onVerscrollBarChange(int)));
}
void TableView::wheelEvent(QWheelEvent *event)
{
    QPoint delta = event->angleDelta();

    int row = 0;
    if (delta.y () < 0) {
        row = 2;
    } else {
        row = -2;
    }

   emit rowchanged(row);
}
void TableView::onVerscrollBarChange(int value)
{
    int tem = this->verticalScrollBar()->maximum();
    double ratio = double(value)/tem;
    emit verticalbarValue(ratio);
}
