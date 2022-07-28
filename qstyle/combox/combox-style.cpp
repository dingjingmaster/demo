#include "combox-style.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QRect>
#include <QStyleOptionComplex>

ComboxStyle::ComboxStyle()
{
    
}

void ComboxStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    switch (element) {
    case PE_IndicatorArrowDown: {
        painter->save();
        painter->translate (option->rect.x(), option->rect.y());
        QPainterPath drawTriangle;      // 画三角
        drawTriangle.moveTo(0, 0);
        drawTriangle.lineTo(option->rect.width()/2, option->rect.height());
        drawTriangle.lineTo(option->rect.width(), 0);
        drawTriangle.lineTo(0, 0);
        painter->setPen(QPen(QColor("#128bf1"), 2));
        painter->drawPath(drawTriangle);
        painter->restore();
    } 
        break;
    default:
        QProxyStyle::drawPrimitive(element, option, painter, widget); 
        break;
    }
}

void ComboxStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    switch (control) {
    case QStyle::CC_ComboBox: {
        if (const QStyleOptionComboBox* cbOption = qstyleoption_cast <const QStyleOptionComboBox*> (option)) {
            painter->save();
            QRect rect = subControlRect (CC_ComboBox, option, SC_ComboBoxFrame, widget).adjusted (+1, +1, -1, -1);
            painter->setBrush(QColor("#128bf1"));
            painter->drawRect(rect);            
            
            rect = subControlRect (CC_ComboBox, option, SC_ComboBoxEditField, widget).adjusted (+2, +2, -2, -2);
            QLinearGradient gradient (rect.topLeft(), rect.bottomRight());
            gradient.setColorAt(0.0, QColor("#fa709a"));
            gradient.setColorAt(1.0, QColor("#fee140"));
            painter->setPen(Qt::NoPen);
            painter->setBrush(gradient);
            painter->drawRect(rect);
            
            rect = subControlRect (CC_ComboBox, option, SC_ComboBoxArrow, widget).adjusted(+1, +1, -1, -1);
            painter->setPen(Qt::transparent);
            painter->translate(option->rect.x(), option->rect.y());
            
            QLinearGradient grandient2 (rect.topLeft(), rect.bottomRight());
            grandient2.setColorAt (0.0, QColor("#84fab0"));
            grandient2.setColorAt (1.0, QColor("#8fd3f4"));
            
            painter->setBrush (grandient2);
            painter->drawRect(rect);
            painter->restore();
            
            QStyleOption arrowOpt (*cbOption);
            arrowOpt.rect = rect.adjusted (+rect.width() * 0.3, +rect.height() * 0.3,
                                           -rect.width() * 0.3, -rect.height() * 0.3);
            drawPrimitive (PE_IndicatorArrowDown, &arrowOpt, painter);
            
            if (option->activeSubControls == SC_ComboBoxEditField
                    || option->activeSubControls == SC_ComboBoxArrow) {
                painter->save();
                QColor s(0, 0, 0, 63);
                painter->setBrush (s);
                painter->drawRect(widget->rect());
                painter->restore();
            }
        }
    }
        break;
    default:
        QProxyStyle::drawComplexControl(control, option, painter, widget);
        break;
    }
}

QRect ComboxStyle::subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc, const QWidget *widget) const
{
    switch (cc) {
    case CC_ComboBox: {
        switch (sc) {
        case SC_ComboBoxEditField:
            return QRect(0, 0, opt->rect.width() * 0.7, opt->rect.height()).adjusted(+2, +2, -2, -2);
        case SC_ComboBoxFrame:
            return QRect(0, 0, opt->rect.width() * 0.7, opt->rect.height());
        case SC_ComboBoxArrow:
            return QRect(opt->rect.width() * 0.7, 0, opt->rect.width() * 0.3, opt->rect.height());
        default:
            QProxyStyle::subControlRect(cc, opt, sc, widget); 
            break;
        }
    }
        break;
    default:
        QProxyStyle::subControlRect(cc, opt, sc, widget); 
        break;
    }
}
