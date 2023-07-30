#include "desktop-view.h"

#include <QDebug>
#include <QPainter>
#include <QAbstractItemView>

DesktopView::DesktopView(QWidget *parent) : QAbstractItemView(parent)
{
    setAttribute(Qt::WA_AlwaysStackOnTop);
    setAttribute(Qt::WA_TranslucentBackground);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

QModelIndex DesktopView::indexAt(const QPoint &point) const
{
    return QModelIndex();
}

void DesktopView::scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint)
{

}

int DesktopView::verticalOffset() const
{
    return 10;
}

int DesktopView::horizontalOffset() const
{
    return 10;
}

bool DesktopView::isIndexHidden(const QModelIndex &index) const
{
    return false;
}

void DesktopView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags)
{

}

QRegion DesktopView::visualRegionForSelection(const QItemSelection &selection) const
{
    return QRegion();
}

QModelIndex DesktopView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    return QModelIndex();
}

// view 里最重要的方法,不实现则会为空窗口
void DesktopView::paintEvent(QPaintEvent *e)
{
    // 在窗口上绘制图形
    QPainter paint (viewport());

    // 获取要绘制的图形信息
    QStyleOptionViewItem po = viewOptions();

    // 循环遍历 模型 大小
    for (int r = 0; r < model()->rowCount(); ++r) {
        for (int c = 0; c < model()->columnCount(); ++c) {
            QModelIndex i = model()->index(r, c);
            QRect rect = visualRect(i);     // 获取索引 i 指向的矩形位置和大小
            po.rect = visualRect(i);

            qDebug() << i.data(Qt::DisplayRole);

            // 处理项目被选择的情形
            if (selectionModel()->isSelected(i)) {
                po.state |= QStyle::State_Selected;
            }

            // 使用代理绘制自定义项
            if (itemDelegate()) {
                itemDelegate()->paint(&paint, po, i);
            }

            // 绘制视图单元的轮廓线
            paint.save();
            paint.setPen(QPen(QColor(111, 1, 1)));
            paint.drawLine(rect.bottomLeft(), rect.bottomRight());
            paint.drawLine(rect.bottomRight(), rect.topRight());
            paint.restore();
        }
    }

    QAbstractItemView::paintEvent(e);
}

// 每个项目大小， 用于 paintEvent
QRect DesktopView::visualRect(const QModelIndex &index) const
{
    return QRect(index.column() * 110 + 20, index.row() * 33 + 20, 100, 33);
}
