/*************************************************************************
> FileName: StarburstWidget.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月07日 星期五 09时37分33秒
 ************************************************************************/

#include "StarburstWidget.h"

#include <QDebug>
#include <QTimer>
#include <QPainter>
#include <QFileDialog>
#include <QGuiApplication>
#include <cmath>

static float start = 0;
static float step = 1;

StarburstWidget::StarburstWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(40, 40);

    QTimer* tm = new QTimer(this);
    tm->connect(tm, &QTimer::timeout, [&] () {
        if (start >= 0 && start < (360 - step)) {
            start += step;
        }
        else {
            start = 0;
            if (step < (350 - step)) {
                step += 1;
            }
            else {
                start = 0;
                step = 1;
            }
        }
        qInfo() << "update -- start: " << start << ", step: " << step;
        update(); 
    });
    tm->setInterval(10);
    tm->start();
}

QImage StarburstWidget::generateStarburst(int size, int petals, double sigma)
{
    QImage img(size, size, QImage::Format_ARGB32);
    img.fill(Qt::transparent);

    const double centerX = size * 0.5;
    const double centerY = size * 0.5;
    const double scale = size * 0.1;  // 控制图案大小

    for (int py = 0; py < size; ++py) {
        QRgb* line = (QRgb*) img.scanLine(py);
        for (int px = 0; px < size; ++px) {
            double x = (px - centerX) / scale;
            double y = (py - centerY) / scale;
            double r = std::sqrt(x*x + y*y);
            double theta = std::atan2(y, x);

            // 星芒函数：|cos(nθ)| × 高斯衰减
            double angleTerm = std::abs(std::cos(petals * theta));
            double radialTerm = std::exp(-r*r / (2 * sigma * sigma));
            double intensity = angleTerm * radialTerm;

            // 提升亮度 + 底噪
            intensity = qBound(0.0, intensity * 1.3 + 0.05, 1.0);

            QColor c;
            float sx = start + intensity * 225 * (360 - start)/ 225;
            c.setHsvF(sx / 360.0, 1.0, 1.0);
            line[px] = qRgba(c.red(), c.green(), c.blue(), c.alpha());
        }
    }
    return img;
}

void StarburstWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int size = qMin(width(), height());
    QImage star = generateStarburst(size, 3, 3.0);

    painter.drawImage((width() - size)/2, (height() - size)/2, star);
}
