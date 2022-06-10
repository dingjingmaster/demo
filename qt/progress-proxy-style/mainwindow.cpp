#include "mainwindow.h"

#include <qdrawutil.h>
#include <QProgressBar>
#include <QStyleOptionProgressBar>
#include <QPen>
#include <QPainter>
#include <QWidget>
#include <QTimer>

//#include <private/qstylehelper_p.h>
//#include <private/qdrawhelper_p.h>
//#include <private/qwidget_p.h>

//#include <private/qproxystyle_p.h>
//#include <private/qstyleanimation_p.h>

ProgressBarStyle* ProgressBarStyle::gInstance = NULL;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QProgressBar* pb = new QProgressBar(this);

    //
    pb->setStyle (ProgressBarStyle::getStyle ());

#if 1
    pb->setRange (0, 0);
#else
    pb->setRange (0, 100);
#endif

    pb->setValue (50);
}

MainWindow::~MainWindow()
{

}

ProgressBarStyle* ProgressBarStyle::getStyle()
{
    if (!gInstance) {
        gInstance = new ProgressBarStyle;
    }

    return gInstance;
}

void ProgressBarStyle::polish(QWidget *widget)
{
    if (QProgressBar* p = qobject_cast<QProgressBar*>(widget)) {
        connect (mAnimation, &QVariantAnimation::valueChanged, widget, [=] (const QVariant&) {
                p->update ();
            }, Qt::UniqueConnection);
    }
}

int ProgressBarStyle::pixelMetric(PixelMetric m, const QStyleOption *opt, const QWidget *widget) const
{
    int ret = 0;

    switch (m) {
    case PM_ProgressBarChunkWidth:
        ret = 9;
        break;
    default:
        QProxyStyle::pixelMetric (m, opt, widget);
        break;
    }

    return ret;
}

void ProgressBarStyle::drawPrimitive(PrimitiveElement elem, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    switch (elem) {
    case PE_IndicatorProgressChunk: {
        bool vertical = false;
        if (const QStyleOptionProgressBar *pb = qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
            vertical = !(pb->state & QStyle::State_Horizontal);
        }
        if (!vertical) {
            painter->fillRect(option->rect.x(), option->rect.y() + 3, option->rect.width() -2, option->rect.height() - 6, option->palette.brush(QPalette::Highlight));
        } else {
            painter->fillRect(option->rect.x() + 2, option->rect.y(), option->rect.width() -6, option->rect.height() - 2, option->palette.brush(QPalette::Highlight));
        }
    }
    break;
    default:
        QProxyStyle::drawPrimitive (elem, option, painter, widget);
        break;
    }
}

void ProgressBarStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    QRect rect = option->rect;
    QColor outline = QColor().red ();
    QColor highlight = option->palette.color (QPalette::Highlight);

    switch (element) {
    case CE_ProgressBar:
        if (const QStyleOptionProgressBar *pb = qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
            QStyleOptionProgressBar subopt = *pb;
            subopt.rect = subElementRect(SE_ProgressBarGroove, pb, widget);

            // 外框和背景
            QProxyStyle::drawControl(CE_ProgressBarGroove, &subopt, painter, widget);

            // 修改这里
            subopt.rect = subElementRect(SE_ProgressBarContents, pb, widget);

            drawControl(CE_ProgressBarContents, &subopt, painter, widget);
            if (pb->textVisible) {
                subopt.rect = subElementRect(SE_ProgressBarLabel, pb, widget);
                drawControl(CE_ProgressBarLabel, &subopt, painter, widget);
            }
        }
        break;
    case CE_ProgressBarGroove:
        if (option->rect.isValid()) {
            qDrawShadePanel(painter, option->rect, option->palette, true, 1, &option->palette.brush(QPalette::Window));
        }
        break;
    case CE_ProgressBarContents:
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->translate(0.5, 0.5);
        if (const QStyleOptionProgressBar *bar = qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
            bool vertical = false;
            bool inverted = false;
            bool indeterminate = (bar->minimum == 0 && bar->maximum == 0);
            bool complete = bar->progress == bar->maximum;

            vertical = !(bar->state & QStyle::State_Horizontal);
            inverted = bar->invertedAppearance;
            if (vertical) {
                rect = QRect(rect.left(), rect.top(), rect.height(), rect.width());
                QTransform m = QTransform::fromTranslate(rect.height()-1, -1.0);
                m.rotate(90.0);
                painter->setTransform(m, true);
            }

            int maxWidth = rect.width();
            const auto progress = qMax(bar->progress, bar->minimum);
            const auto totalSteps = qMax(Q_INT64_C(1), qint64(bar->maximum) - bar->minimum);
            const auto progressSteps = qint64(progress) - bar->minimum;
            const auto progressBarWidth = progressSteps * maxWidth / totalSteps;
            int width = indeterminate ? maxWidth : progressBarWidth;

            bool reverse = (!vertical && (bar->direction == Qt::RightToLeft)) || vertical;
            if (inverted) {
                reverse = !reverse;
            }

            float step = 0;
            QRect progressBar;
            QColor highlightedoutline = highlight.darker(140);
            if (qGray(outline.rgb()) > qGray(highlightedoutline.rgb())) {
                outline = highlightedoutline;
            }

            if (!indeterminate) {
                QColor innerShadow(Qt::black);
                innerShadow.setAlpha(35);
                painter->setPen(innerShadow);
                if (!reverse) {
                    progressBar.setRect(rect.left(), rect.top(), width - 1, rect.height() - 1);
                    if (!complete) {
                        painter->drawLine(progressBar.topRight() + QPoint(2, 1), progressBar.bottomRight() + QPoint(2, 0));
                        painter->setPen(QPen(highlight.darker(140)));
                        painter->drawLine(progressBar.topRight() + QPoint(1, 1), progressBar.bottomRight() + QPoint(1, 0));
                    }
                } else {
                    progressBar.setRect(rect.right() - width - 1, rect.top(), width + 2, rect.height() - 1);
                    if (!complete) {
                        painter->drawLine(progressBar.topLeft() + QPoint(-2, 1), progressBar.bottomLeft() + QPoint(-2, 0));
                        painter->setPen(QPen(highlight.darker(140)));
                        painter->drawLine(progressBar.topLeft() + QPoint(-1, 1), progressBar.bottomLeft() + QPoint(-1, 0));
                    }
                }
            } else {
                progressBar.setRect(rect.left(), rect.top(), rect.width() - 1, rect.height() - 1);
            }

            if (indeterminate || bar->progress > bar->minimum) {

                QColor highlightedGradientStartColor = highlight.lighter(120);
                QColor highlightedGradientStopColor  = highlight;
                QLinearGradient gradient(rect.topLeft(), QPoint(rect.bottomLeft().x(), rect.bottomLeft().y()));
                gradient.setColorAt(0, highlightedGradientStartColor);
                gradient.setColorAt(1, highlightedGradientStopColor);

                painter->setBrush(gradient);

                if (!complete && !indeterminate) {
                    painter->setClipRect(progressBar.adjusted(-1, -1, -1, 1));
                }

                // 等待状态的进度条
                if (indeterminate) {
                    int ww = width / 5 * 2;
                    int endPoint = (rect.width () - ww);
                    painter->setBrush (QBrush(highlight));
                    if (QAbstractAnimation::Running == mAnimation->state ()) {
                        step = qobject_cast<QVariantAnimation*>(mAnimation)->currentValue ().toFloat () / 100 * endPoint * 2;
                        step = (step >= endPoint) ? (2 * endPoint - step) : step;
                    } else {
                        mAnimation->stop ();
                        mAnimation->start ();
                    }
                    painter->drawRect (step, 0, ww, rect.height ());
                } else {
                    if (QAbstractAnimation::Running == mAnimation->state ()) {
                        mAnimation->stop ();
                    }

                    if (bar->progress > bar->minimum) {
                        int progress = bar->progress;
                        if (bar->progress > bar->maximum) {
                            progress = (bar->progress / (bar->maximum - bar->minimum) + bar->minimum);
                        }
                        painter->drawRect (0, 0, progress, rect.height ());
                    }
                }
            }
        }
        painter->restore();
        break;
    case CE_ProgressBarLabel:
        if (const QStyleOptionProgressBar *pb = qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
            const bool vertical = !(pb->state & QStyle::State_Horizontal);
            if (!vertical) {
                QPalette::ColorRole textRole = QPalette::NoRole;
                if ((pb->textAlignment & Qt::AlignCenter) && pb->textVisible && ((qint64(pb->progress) - qint64(pb->minimum)) * 2 >= (qint64(pb->maximum) - qint64(pb->minimum)))) {
                    textRole = QPalette::HighlightedText;
                    //Draw text shadow, This will increase readability when the background of same color
                    QRect shadowRect(pb->rect);
                    shadowRect.translate(1,1);
                    QColor shadowColor = (pb->palette.color(textRole).value() <= 128) ? QColor(255,255,255,160) : QColor(0,0,0,160);
                    QPalette shadowPalette = pb->palette;
                    shadowPalette.setColor(textRole, shadowColor);
                    QProxyStyle:: drawItemText(painter, shadowRect, Qt::AlignCenter | Qt::TextSingleLine, shadowPalette, pb->state & State_Enabled, pb->text, textRole);
                }
                QProxyStyle::drawItemText(painter, pb->rect, Qt::AlignCenter | Qt::TextSingleLine, pb->palette, pb->state & State_Enabled, pb->text, textRole);
            }
        }
        break;
    default:
        QProxyStyle::drawControl (element, option, painter, widget);
        break;
    }
}

QRect ProgressBarStyle::subElementRect(SubElement sr, const QStyleOption *opt, const QWidget *widget) const
{
    QRect r;
    switch (sr) {
    case SE_ProgressBarGroove:
    case SE_ProgressBarContents:
    case SE_ProgressBarLabel:
        if (const QStyleOptionProgressBar *pb = qstyleoption_cast<const QStyleOptionProgressBar *>(opt)) {
            int textw = 0;
            const bool vertical = !(pb->state & QStyle::State_Horizontal);
            if (!vertical) {
                if (pb->textVisible) {
                    textw = qMax(pb->fontMetrics.horizontalAdvance(pb->text), pb->fontMetrics.horizontalAdvance(QLatin1String("100%"))) + 6;
                }
            }

            if ((pb->textAlignment & Qt::AlignCenter) == 0) {
                if (sr != SE_ProgressBarLabel) {
                    r.setCoords(pb->rect.left(), pb->rect.top(), pb->rect.right() - textw, pb->rect.bottom());
                } else {
                    r.setCoords(pb->rect.right() - textw, pb->rect.top(), pb->rect.right(), pb->rect.bottom());
                }
            } else {
                r = pb->rect;
            }
            r = visualRect(pb->direction, pb->rect, r);
        }
        break;
    default:
        r = QProxyStyle::subElementRect(SE_ProgressBarGroove, opt, widget);
        break;
    }

    return r;
}

ProgressBarStyle::ProgressBarStyle()
{
    mAnimation = new QVariantAnimation (this);
    mAnimation->setStartValue (0);
    mAnimation->setEndValue (100);
    mAnimation->setDuration (1000);
}

ProgressBarStyle::~ProgressBarStyle()
{
    delete mAnimation;
}


