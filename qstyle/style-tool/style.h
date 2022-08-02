#ifndef STYLE_H
#define STYLE_H

#include <QDebug>
#include <QCommonStyle>
#include <QStyleOption>
#include <QStyleOptionComplex>
#include <QPalette>

#define LOG     qDebug() << " "

QDebug operator<< (QDebug debug, const QSize size);
QDebug operator<< (QDebug debug, const QPalette pe);
QDebug operator<< (QDebug debug, const QIcon::Mode pe);
QDebug operator<< (QDebug debug, QStyle::StyleHint sh);
QDebug operator<< (QDebug debug, const QStyleOption pe);
QDebug operator<< (QDebug debug, const Qt::Orientation pe);
QDebug operator<< (QDebug debug, const QStyle::ContentsType);
QDebug operator<< (QDebug debug, const QStyle::SubControl pe);
QDebug operator<< (QDebug debug, const QStyle::SubElement pe);
QDebug operator<< (QDebug debug, const QStyleOptionComplex pe);
QDebug operator<< (QDebug debug, const QStyle::PixelMetric pe);
QDebug operator<< (QDebug debug, const QStyle::ComplexControl cc);
QDebug operator<< (QDebug debug, const QStyle::ControlElement ce);
QDebug operator<< (QDebug debug, const QStyle::PrimitiveElement pe);
QDebug operator<< (QDebug debug, const QSizePolicy::ControlType pe);

/**
 * @brief QCommonStyle <- QStyle <- QObject
 *  通过打日志的方式输出指定widget的绘制过程进行分析
 */
class Style : public QCommonStyle
{
    Q_OBJECT
public:
    Style();
    
public:
    void drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = nullptr) const  override;
    void drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    void drawItemPixmap(QPainter *painter, const QRect &rectangle, int alignment, const QPixmap &pixmap) const override;
    void drawItemText(QPainter *painter, const QRect &rectangle, int alignment, const QPalette &palette, bool enabled, const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const override;
    void drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *option) const  override;
    QStyle::SubControl hitTestComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, const QPoint &position, const QWidget *widget = nullptr) const  override;
    QRect itemPixmapRect(const QRect &rectangle, int alignment, const QPixmap &pixmap) const override;
    QRect itemTextRect(const QFontMetrics &metrics, const QRect &rectangle, int alignment, bool enabled, const QString &text) const override;
    int layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, const QStyleOption *option = nullptr, const QWidget *widget = nullptr) const override;
    int pixelMetric(QStyle::PixelMetric metric, const QStyleOption *option = nullptr, const QWidget *widget = nullptr) const override;
    void polish(QWidget *widget) override;
    void polish(QApplication *application) override;
    void polish(QPalette &palette) override;
    QSize sizeFromContents(QStyle::ContentsType type, const QStyleOption *option, const QSize &contentsSize, const QWidget *widget = nullptr) const override;
    QIcon standardIcon(QStyle::StandardPixmap standardIcon, const QStyleOption *option = 0, const QWidget *widget = 0) const override;
    QPalette standardPalette() const override;
    int styleHint(QStyle::StyleHint hint, const QStyleOption *option = nullptr, const QWidget *widget = nullptr, QStyleHintReturn *returnData = nullptr) const override;
    QRect subControlRect(QStyle::ComplexControl control, const QStyleOptionComplex *option, QStyle::SubControl subControl, const QWidget *widget = nullptr) const override;
    QRect subElementRect(QStyle::SubElement element, const QStyleOption *option, const QWidget *widget = nullptr) const override;
    void unpolish(QWidget *widget) override;
    void unpolish(QApplication *application) override;
};

#endif // STYLE_H
