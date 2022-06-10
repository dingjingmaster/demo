#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QProxyStyle>
#include <QMainWindow>

#include <QVariantAnimation>

class ProgressBarStyle : public QProxyStyle
{
    Q_OBJECT
public:
    static ProgressBarStyle* getStyle();

    void polish(QWidget *widget);
    int pixelMetric(PixelMetric m, const QStyleOption *opt, const QWidget *widget) const;
    void drawPrimitive(PrimitiveElement elem, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
    QRect subElementRect(SubElement sr, const QStyleOption *opt, const QWidget *widget) const;

private:
    ProgressBarStyle ();
    ~ProgressBarStyle ();

private:
    static ProgressBarStyle* gInstance;
    QVariantAnimation* mAnimation = NULL;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
