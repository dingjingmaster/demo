#include "window.h"

#include <QWidget>
#include <QMetaEnum>

Window::Window(QWidget *parent) : QWidget(parent),
    mIconSize(32, 32)
{
    mUi.setupUi(this);
    mUi.easingCurvePicker->setIconSize(mIconSize);
    mUi.easingCurvePicker->setMinimumHeight(mIconSize.height() + 50);
    mUi.buttonGroup->setId(mUi.lineRadio, 0);
    mUi.buttonGroup->setId(mUi.circleRadio, 1);

    QEasingCurve dummy;
    mUi.periodSpinBox->setValue(dummy.period());
    mUi.amplitudeSpinBox->setValue(dummy.amplitude());
    mUi.overshootSpinBox->setValue(dummy.overshoot());

    connect(mUi.easingCurvePicker, &QListWidget::currentRowChanged, this, &Window::curveChanged);
    connect(mUi.buttonGroup, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(pathChanged(QAbstractButton *)));
    connect(mUi.periodSpinBox, SIGNAL(valueChanged(double)), this, SLOT(periodChanged(double)));
    connect(mUi.amplitudeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(amplitudeChanged(double)));
    connect(mUi.overshootSpinBox, SIGNAL(valueChanged(double)), this, SLOT(overshootChanged(double)));
    createCurveIcons();

    QPixmap pix(QLatin1String(":/pic.png"));
    mItem = new PixmapItem(pix);
    mScene.addItem(mItem);
    mUi.graphicsView->setScene(&mScene);

    mAnim = new Animation(mItem, "pos");
    mAnim->setEasingCurve(QEasingCurve::OutBounce);
    mUi.easingCurvePicker->setCurrentRow(int(QEasingCurve::OutBounce));

    startAnimation();
}

Window::~Window()
{

}

QEasingCurve createEasingCurve(QEasingCurve::Type curveType)
{
    QEasingCurve curve(curveType);

    if (curveType == QEasingCurve::BezierSpline) {
        curve.addCubicBezierSegment(QPointF(0.4, 0.1), QPointF(0.6, 0.9), QPointF(1.0, 1.0));
    } else if (curveType == QEasingCurve::TCBSpline) {
        curve.addTCBSegment(QPointF(0.0, 0.0), 0, 0, 0);
        curve.addTCBSegment(QPointF(0.3, 0.4), 0.2, 1, -0.2);
        curve.addTCBSegment(QPointF(0.7, 0.6), -0.2, 1, 0.2);
        curve.addTCBSegment(QPointF(1.0, 1.0), 0, 0, 0);
    }

    return curve;
}

void Window::createCurveIcons()
{
    QPixmap pix(mIconSize);
    QPainter painter(&pix);
    QLinearGradient gradient(0,0, 0, mIconSize.height());
    gradient.setColorAt(0.0, QColor(240, 240, 240));
    gradient.setColorAt(1.0, QColor(224, 224, 224));
    QBrush brush(gradient);
    const QMetaObject &mo = QEasingCurve::staticMetaObject;
    QMetaEnum metaEnum = mo.enumerator(mo.indexOfEnumerator("Type"));
    // Skip QEasingCurve::Custom
    for (int i = 0; i < QEasingCurve::NCurveTypes - 1; ++i) {
        painter.fillRect(QRect(QPoint(0, 0), mIconSize), brush);
        QEasingCurve curve = createEasingCurve((QEasingCurve::Type) i);
        painter.setPen(QColor(0, 0, 255, 64));
        qreal xAxis = mIconSize.height()/1.5;
        qreal yAxis = mIconSize.width()/3;
        painter.drawLine(0, xAxis, mIconSize.width(),  xAxis);
        painter.drawLine(yAxis, 0, yAxis, mIconSize.height());

        qreal curveScale = mIconSize.height()/2;

        painter.setPen(Qt::NoPen);

        // start point
        painter.setBrush(Qt::red);
        QPoint start(yAxis, xAxis - curveScale * curve.valueForProgress(0));
        painter.drawRect(start.x() - 1, start.y() - 1, 3, 3);

        // end point
        painter.setBrush(Qt::blue);
        QPoint end(yAxis + curveScale, xAxis - curveScale * curve.valueForProgress(1));
        painter.drawRect(end.x() - 1, end.y() - 1, 3, 3);

        QPainterPath curvePath;
        curvePath.moveTo(start);
        for (qreal t = 0; t <= 1.0; t+=1.0/curveScale) {
            QPoint to;
            to.setX(yAxis + curveScale * t);
            to.setY(xAxis - curveScale * curve.valueForProgress(t));
            curvePath.lineTo(to);
        }
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.strokePath(curvePath, QColor(32, 32, 32));
        painter.setRenderHint(QPainter::Antialiasing, false);
        QListWidgetItem *item = new QListWidgetItem;
        item->setIcon(QIcon(pix));
        item->setText(metaEnum.key(i));
        mUi.easingCurvePicker->addItem(item);
    }
}

void Window::startAnimation()
{
    mAnim->setStartValue(QPointF(0, 0));
    mAnim->setEndValue(QPointF(100, 100));
    mAnim->setDuration(2000);
    mAnim->setLoopCount(-1); // forever
    mAnim->start();
}

void Window::curveChanged(int row)
{
    QEasingCurve::Type curveType = (QEasingCurve::Type)row;
    mAnim->setEasingCurve(createEasingCurve(curveType));
    mAnim->setCurrentTime(0);

    bool isElastic = curveType >= QEasingCurve::InElastic && curveType <= QEasingCurve::OutInElastic;
    bool isBounce = curveType >= QEasingCurve::InBounce && curveType <= QEasingCurve::OutInBounce;
    mUi.periodSpinBox->setEnabled(isElastic);
    mUi.amplitudeSpinBox->setEnabled(isElastic || isBounce);
    mUi.overshootSpinBox->setEnabled(curveType >= QEasingCurve::InBack && curveType <= QEasingCurve::OutInBack);
}

void Window::pathChanged(QAbstractButton *button)
{
    const int index = mUi.buttonGroup->id(button);
    mAnim->setPathType(Animation::PathType(index));
}

void Window::periodChanged(double value)
{
    QEasingCurve curve = mAnim->easingCurve();
    curve.setPeriod(value);
    mAnim->setEasingCurve(curve);
}

void Window::amplitudeChanged(double value)
{
    QEasingCurve curve = mAnim->easingCurve();
    curve.setAmplitude(value);
    mAnim->setEasingCurve(curve);
}

void Window::overshootChanged(double value)
{
    QEasingCurve curve = mAnim->easingCurve();
    curve.setOvershoot(value);
    mAnim->setEasingCurve(curve);
}

