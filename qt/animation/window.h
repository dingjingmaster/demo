#ifndef WINDOW_H
#define WINDOW_H

#include "animation.h"

#include <QAbstractButton>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMainWindow>
#include <ui_form.h>

class PixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY (QPointF pos READ pos WRITE setPos)
public:
    PixmapItem (const QPixmap& pixmap) : QGraphicsPixmapItem (pixmap)
    {
    }
};

class Window : public QWidget
{
    Q_OBJECT
public:
    Window(QWidget *parent = nullptr);
    ~Window();

private Q_SLOTS:
    void curveChanged(int row);
    void pathChanged(QAbstractButton *button);
    void periodChanged(double);
    void amplitudeChanged(double);
    void overshootChanged(double);

private:
    void createCurveIcons();
    void startAnimation();

    Ui::Form mUi;
    QGraphicsScene mScene;
    PixmapItem *mItem;
    Animation *mAnim;
    QSize mIconSize;
};
#endif // WINDOW_H
