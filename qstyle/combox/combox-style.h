#ifndef COMBOXSTYLE_H
#define COMBOXSTYLE_H

#include <qglobal.h>
#include <QProxyStyle>

class ComboxStyle : public QProxyStyle 
{
    Q_OBJECT
public:
    ComboxStyle();
    
public:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc, const QWidget *widget) const override;
};

#endif // COMBOXSTYLE_H
