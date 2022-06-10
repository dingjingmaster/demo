#include "dialog.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QPushButton>

static QPixmap drawSymbolicColoredPixmap (const QPixmap& source);

Dialog::Dialog(QWidget *parent) : QDialog(parent)
{
    setMouseTracking(true);
    setContentsMargins(0, 0, 0, 0);
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(QSize(mFixWidth, mFixHeight));
}

Dialog::~Dialog()
{
}

void Dialog::paintEvent(QPaintEvent*)
{
    QPushButton btn;
    QPainter painter (this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(pen);

    // paint header icon
    painter.save();
    painter.drawPixmap(mMarginl, mMargint, mHeaderIconSize, mHeaderIconSize, QIcon::fromTheme("dialog-warning-symbolic").pixmap(mHeaderBtnSize, mHeaderBtnSize));
    painter.restore();

    // paint mini
//    pen(QColor(192,192,192), 1);
    painter.setPen(pen);
    painter.drawPixmap(width() - mMarginl * 2 - mHeaderBtnSize * 2, mMargint, mHeaderBtnSize, mHeaderBtnSize, drawSymbolicColoredPixmap(QIcon::fromTheme("window-minimize-symbolic").pixmap(mHeaderBtnSize, mHeaderBtnSize)));


    // paint close
    painter.save();
    painter.drawPixmap(width() - mMarginl - mHeaderBtnSize, mMargint, mHeaderBtnSize, mHeaderBtnSize, drawSymbolicColoredPixmap(QIcon::fromTheme("window-close-symbolic").pixmap(mHeaderBtnSize, mHeaderBtnSize)));
    painter.restore();
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    // cloas button
    QPoint pos = event->pos();
    if ((pos.x() >= width() - mMarginl - mHeaderBtnSize)
               && (pos.x() <= width() - mMarginl)
               && (pos.y() >= mMargint)
               && (pos.y() <= mMargint + mHeaderBtnSize)) {
        setCursor(Qt::PointingHandCursor);
    } else if ((pos.x() >= width() - 2 * mMarginl - 2 * mHeaderBtnSize)
               && (pos.x() <= width() - 2 * mMarginl - mHeaderBtnSize)
               && (pos.y() >= mMargint)
               && (pos.y() <= mMargint + mHeaderBtnSize)) {
        setCursor(Qt::PointingHandCursor);
    } else {
        setCursor(Qt::ArrowCursor);
        QWidget::mouseMoveEvent(event);
    }
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    // close button
    QPoint pos = event->pos();
    if ((pos.x() >= width() - mMarginl - mHeaderBtnSize)
               && (pos.x() <= width() - mMarginl)
               && (pos.y() >= mMargint)
               && (pos.y() <= mMargint + mHeaderBtnSize)) {
    }

    QWidget::mouseReleaseEvent(event);
}

static QPixmap drawSymbolicColoredPixmap (const QPixmap& source)
{
    // 18, 32, 69
    QPushButton      mBtn;
    QColor baseColor = mBtn.palette().color(QPalette::Text).light(150);
    QImage img = source.toImage();

    for (int x = 0; x < img.width(); ++x) {
        for (int y = 0; y < img.height(); ++y) {
            auto color = img.pixelColor(x, y);
            color.setRed(baseColor.red());
            color.setGreen(baseColor.green());
            color.setBlue(baseColor.blue());
            img.setPixelColor(x, y, color);
        }
    }

    return QPixmap::fromImage(img);
}
