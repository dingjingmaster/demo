#include "header-button.h"
#include "res.h"

#include <unistd.h>

#include <QFile>
#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>

HeaderButton::HeaderButton(QWidget *parent, Type type)
    : QPushButton{parent}
{
    setFlat(true);
    setMouseTracking(true);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("main-header-button");

    setFixedSize(42, 42);
    setContentsMargins(3, 3, 3, 3);

    onTypeChanged(type);

    connect (this, &HeaderButton::typeChanged, this, &HeaderButton::onTypeChanged);
}

HeaderButton::Type HeaderButton::getType()
{
    return mType;
}

void HeaderButton::setType(Type type)
{
    if (mType != type) {
        Q_EMIT typeChanged(type);
        mType = type;
    }
}

void HeaderButton::enterEvent(QEvent *event)
{
    setCursor(Qt::PointingHandCursor);
}

void HeaderButton::leaveEvent(QEvent *event)
{
    setCursor(Qt::ArrowCursor);
}

void HeaderButton::onTypeChanged(Type type)
{
    QString path = nullptr;

    switch (type) {
    case MIN: {
        path = ":min.png";
//        path = "window-minimize-symbolic";
        break;
    }
    case MAX: {
        path = ":max.png";
//        path = "window-maximize-symbolic";
        break;
    }
    case CLOSE: {
        path = ":close.png";
//        path = "window-close-symbolic";
        break;
    }
    case RESTORE: {
        path = ":max.png";
//        path = "window-restore-symbolic";
        break;
    }
    default:
        break;
    }
    
//    setIcon(QIcon(transColor(QIcon::fromTheme(path).pixmap (width(), height()))));
    setIcon(QIcon(transColor(QIcon(path).pixmap (width(), height()))));

    update();
}

QPixmap HeaderButton::transColor (const QPixmap &icon)
{
    if (icon.isNull())  return {};

    QImage oldp = icon.toImage();

    QImage newp(icon.width(), icon.height(), QImage::Format_ARGB32);

    for (auto i = 0; i < oldp.width(); ++i) {
        for (auto j = 0; j < oldp.height(); ++j) {
            QColor c = QColor(oldp.pixel(i, j));
//            if (c.red () != 0 || c.green () != 0 || c.blue () != 0)
//                usleep (1);
            //qDebug() << "r: " << c.red() << " g: " << c.green () << " b: " << c.blue ();
                        
            if ((c.alpha() > 0) && (c.red() > 0) && (c.green() > 0) && (c.blue() > 0)) {
                newp.setPixel(i, j, qRgb(255, 255, 255));
            } else {
                newp.setPixel(i, j, qRgba(0, 0, 0, 0));
            }
        }
    }

    return QPixmap::fromImage(newp);
}

void HeaderButton::paintEvent(QPaintEvent* event)
{
    QPushButton::paintEvent (event);
}
