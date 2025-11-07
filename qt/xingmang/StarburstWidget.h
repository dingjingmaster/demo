/*************************************************************************
> FileName: StarburstWidget.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月07日 星期五 09时37分21秒
 ************************************************************************/
#ifndef _STARBURSTWIDGET_H
#define _STARBURSTWIDGET_H

#include <QWidget>
#include <QImage>

class StarburstWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StarburstWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QImage generateStarburst(int size, int petals = 5, double sigma = 3.0);
};

#endif
