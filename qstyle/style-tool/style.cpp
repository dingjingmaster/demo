#include "style.h"

#include <QApplication>
#include <QStyleOption>
#include <QStyleOptionComplex>

Style::Style() 
{
    
}

void Style::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    LOG << "ComplexControl:" << control << " QStyleOptionComplex: " << *option;
    QCommonStyle::drawComplexControl(control, option, painter, widget); 
}

void Style::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    LOG << "ControlElement:" << element << " QStyleOption: " << option;
    QCommonStyle::drawControl(element, option, painter, widget); 
}

void Style::drawItemPixmap(QPainter *painter, const QRect &rectangle, int alignment, const QPixmap &pixmap) const
{
    LOG << "QPainter:" << painter << " QRect: " << rectangle << " alignment: " << alignment;
    QCommonStyle::drawItemPixmap(painter, rectangle, alignment, pixmap); 
}

void Style::drawItemText(QPainter *painter, const QRect &rectangle, int alignment, const QPalette &palette, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    LOG << "QPainter:" << painter << " QRect: " << rectangle << " alignment: " << alignment << " enabled: " << enabled << " text: " << text << " textRole: " << textRole;
    QCommonStyle::drawItemText(painter, rectangle, alignment, palette, enabled, text, textRole);
}

void Style::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    LOG << "PrimitiveElement:" << element << " QStyleOption: " << option << " QPainter: " << painter;
    
    QCommonStyle::drawPrimitive(element, option, painter, widget);
}

QPixmap Style::generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *option) const
{
    LOG << "QIcon::Mode:" << iconMode << " QStyleOption: " << option;
    
    return QCommonStyle::generatedIconPixmap(iconMode, pixmap, option); 
}

QCommonStyle::SubControl Style::hitTestComplexControl(ComplexControl control, const QStyleOptionComplex *option, const QPoint &position, const QWidget *widget) const
{
    LOG << "ComplexControl: " << control << " QStyleOptionComplex: " << option << " QPoint: " << position;
    
    return QCommonStyle::hitTestComplexControl(control, option, position, widget);
}

QRect Style::itemPixmapRect(const QRect &rectangle, int alignment, const QPixmap &pixmap) const
{
    LOG << "QRect: " << rectangle << " alignment: " << alignment;
    
    return QCommonStyle::itemPixmapRect(rectangle, alignment, pixmap); 
}

QRect Style::itemTextRect(const QFontMetrics &metrics, const QRect &rectangle, int alignment, bool enabled, const QString &text) const
{
    LOG << "QFont Metrics  QRect: " << rectangle << " alignment: " << alignment << " enable: " << enabled << " textL: " << text;
    
    return QCommonStyle::itemTextRect(metrics, rectangle, alignment, enabled, text); 
}

int Style::layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, const QStyleOption *option, const QWidget *widget) const
{
    LOG << "QSizePolicy::ControlType: " << control1 << " QSizePolicy::ControlType: " << control2 << " Qt::Orientation: " << orientation << " QStyleOption: " << option;
    
    return QCommonStyle::layoutSpacing(control1, control2, orientation, option, widget); 
}

int Style::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    LOG << "PixelMetric: " << metric << " QStyleOption: " << option;
    
    return QCommonStyle::pixelMetric(metric, option, widget); 
}

void Style::polish(QWidget *widget)
{
    LOG << "Widget: " << widget->metaObject ()->className ();
    
    QCommonStyle::polish(widget);
}

void Style::polish(QApplication *application)
{
    LOG << "Widget: " << application->applicationName ();
    
    QCommonStyle::polish(application);
}

void Style::polish(QPalette &palette)
{
    //LOG << "QPalette: " << palette;
    QCommonStyle::polish(palette);
}

QSize Style::sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &contentsSize, const QWidget *widget) const
{
    LOG << "ContentType: " << type << " QStyleOption: " << option; // << " QSize: " << contentsSize;
    
    return QCommonStyle::sizeFromContents(type, option, contentsSize, widget);
}

QIcon Style::standardIcon(StandardPixmap standardIcon, const QStyleOption *option, const QWidget *widget) const
{
    LOG << "StandardPixmap: " << standardIcon;
    
    return QCommonStyle::standardIcon(standardIcon, option, widget); 
}

QPalette Style::standardPalette() const
{
    return QCommonStyle::standardPalette(); 
}

int Style::styleHint(QStyle::StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const
{
    LOG << "StyleHint: " << hint << "QStyleHintReturn: " << returnData;
    
    return QCommonStyle::styleHint(hint, option, widget, returnData); 
}

QRect Style::subControlRect(ComplexControl control, const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const
{
    LOG << "ComplexControl: " << control << " QStyleOptionComplex: " << option << " SubControl: " << subControl;
    return QCommonStyle::subControlRect(control, option, subControl, widget);
}

QRect Style::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    LOG << "SubElement: " << element << " QStyleOption: " << option;
    return QCommonStyle::subElementRect(element, option, widget); 
}

void Style::unpolish(QWidget *widget)
{
    LOG << "QWidget: " << widget->metaObject()->className ();
    
    QCommonStyle::unpolish(widget);
}

void Style::unpolish(QApplication *application)
{
    LOG << "app name: " << application->applicationName();
    QCommonStyle::unpolish(application);
}

QDebug operator<<(QDebug debug, QStyle::StyleHint sh) 
{
    return debug;
}

QDebug operator<<(QDebug debug, const QSize size)
{
    debug << "[width: " << size.width () << " height: " << size.height () << "]";  
    return debug;
}

QDebug operator<<(QDebug debug, const QPalette pe)
{
    return debug;
}

QDebug operator<<(QDebug debug, const QIcon::Mode pe)
{
    switch (pe) {
    case QIcon::Normal:
        debug << "[IconMode: " << "Normal]";
        break;
    case QIcon::Active:
        debug << "[IconMode: " << "Active]";
        break;
    case QIcon::Disabled:
        debug << "[IconMode: " << "Disable]";
        break;
    case QIcon::Selected:
        debug << "[IconMode: " << "Selected]";
        break;
    }
    
    return debug;
}

QDebug operator<<(QDebug debug, const QStyleOption pe)
{
    return debug;
}

QDebug operator<<(QDebug debug, const Qt::Orientation pe)
{
    switch (pe) {
    case Qt::Horizontal:
        debug << "[orientation: " << "Horizontal]";
        break;
    case Qt::Vertical:
        debug << "[orientation: " << "Vertical]";
        break;
    }
        
    return debug;
}

QDebug operator<<(QDebug debug, const QStyle::ContentsType pe)
{
    switch (pe) {
    case QStyle::CT_PushButton:
        LOG << "[ContentsType: CT_PushButton]";
        break;
    case QStyle::CT_CheckBox:
        LOG << "[ContentsType: CT_CheckBox]";
        break;
    case QStyle::CT_RadioButton:
        break;
    case QStyle::CT_ToolButton:
        break;
    case QStyle::CT_ComboBox:
        break;
    case QStyle::CT_Splitter:
        break;
    case QStyle::CT_ProgressBar:
        break;
    case QStyle::CT_MenuItem:
        break;
    case QStyle::CT_MenuBarItem:
        break;
    case QStyle::CT_MenuBar:
        break;
    case QStyle::CT_Menu:
        break;
    case QStyle::CT_TabBarTab:
        break;
    case QStyle::CT_Slider:
        break;
    case QStyle::CT_ScrollBar:
        break;
    case QStyle::CT_LineEdit:
        break;
    case QStyle::CT_SpinBox:
        break;
    case QStyle::CT_SizeGrip:
        break;
    case QStyle::CT_TabWidget:
        break;
    case QStyle::CT_DialogButtons:
        break;
    case QStyle::CT_HeaderSection:
        break;
    case QStyle::CT_GroupBox:
        break;
    case QStyle::CT_MdiControls:
        break;
    case QStyle::CT_ItemViewItem:
        break;
    case QStyle::CT_CustomBase:
        break;
    }

    return debug;
}

QDebug operator<<(QDebug debug, const QStyle::SubControl pe)
{
	
    return debug;
    
}

QDebug operator<<(QDebug debug, const QStyle::SubElement pe)
{

    return debug;
}

QDebug operator<<(QDebug debug, const QStyleOptionComplex pe)
{

    return debug;
}

QDebug operator<<(QDebug debug, const QStyle::PixelMetric pe)
{

    return debug;
}

QDebug operator<<(QDebug debug, const QStyle::ComplexControl cc)
{
    return debug;
}

QDebug operator<<(QDebug debug, const QStyle::ControlElement ce)
{
    return debug;
}

QDebug operator<<(QDebug debug, const QStyle::PrimitiveElement pe)
{
    return debug;
}

QDebug operator<<(QDebug debug, const QSizePolicy::ControlType pe)
{
    return debug;
}
