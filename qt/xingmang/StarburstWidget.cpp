/*************************************************************************
> FileName: StarburstWidget.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月07日 星期五 09时37分33秒
 ************************************************************************/

#include "StarburstWidget.h"
#include <QPainter>
#include <QFileDialog>
#include <QGuiApplication>
#include <cmath>

StarburstWidget::StarburstWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(400, 400);
}

QImage StarburstWidget::generateStarburst(int size, int petals, double sigma)
{
    QImage img(size, size, QImage::Format_RGB888);
    img.fill(Qt::black);

    const double centerX = size * 0.5;
    const double centerY = size * 0.5;
    const double scale = size * 0.1;  // 控制图案大小

    for (int py = 0; py < size; ++py) {
        uchar* line = img.scanLine(py);
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

            // 写入像素（灰度）
            uchar val = static_cast<uchar>(intensity * 255);
            line[px * 3 + 0] = val;
            line[px * 3 + 1] = val;
            line[px * 3 + 2] = val;
        }
    }
    return img;
}

void StarburstWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int size = qMin(width(), height());
    QImage star = generateStarburst(size, 5, 3.0);

    painter.drawImage((width() - size)/2, (height() - size)/2, star);

    // 右键保存
    if (QGuiApplication::mouseButtons() & Qt::RightButton) {
        QString path = QFileDialog::getSaveFileName(this, "保存星芒", "starburst.png", "PNG Images (*.png)");
        if (!path.isEmpty()) star.save(path);
    }
}
