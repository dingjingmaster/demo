#include "main-header.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QApplication>

MainHeader::MainHeader (QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);
    setFixedHeight(mMaxHeight);
    setObjectName("main-header");
    setContentsMargins(0, 0, 0, 0);
    setWindowFlag (Qt::FramelessWindowHint);
    setAttribute(Qt::WidgetAttribute::WA_StyledBackground);

    mMainLayout = new QHBoxLayout;
    mMainLayout->setContentsMargins(0, 0, 0, 6);

    mHeaderName = new QLabel;
    mHeaderName->setObjectName("main-header-title");
    mHeaderName->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    mLeftLayout = new QHBoxLayout;
    mRightLayout = new QHBoxLayout;
    mRightLayout->setSpacing(3);

    // title
    mLeftLayout->addWidget(mHeaderName);

    // button
    mMinBtn = new HeaderButton(nullptr, HeaderButton::MIN);
    mMaxBtn = new HeaderButton(nullptr, HeaderButton::MAX);
    mCloseBtn = new HeaderButton(nullptr, HeaderButton::CLOSE);
    mMinBtn->setObjectName("main-header-button");
    mMaxBtn->setObjectName("main-header-button");
    mCloseBtn->setObjectName("main-header-button-close");
    connect(mMinBtn,    &QPushButton::clicked, this, &MainHeader::windowMin);
    connect(mMaxBtn,    &QPushButton::clicked, this, &MainHeader::windowMax);
    connect(mCloseBtn,  &QPushButton::clicked, this, &MainHeader::windowClose);

    connect (mCloseBtn, &QPushButton::clicked, this, [=] () {
        QApplication::exit(0);
    });

    connect (mMinBtn, &QPushButton::clicked, this, [=] () {
        setWindowState (Qt::WindowMinimized);
    });

    connect (mMaxBtn, &QPushButton::clicked, this, [=] () {
        if (isMaximized()) {
            setWindowState (Qt::WindowNoState);
        }
        else {
            setWindowState (Qt::WindowMaximized);
        }
    });

    mRightLayout->addWidget(mMinBtn);
    mRightLayout->addWidget(mMaxBtn);
    mRightLayout->addWidget(mCloseBtn);

    // main layout
    mMainLayout->addItem(mLeftLayout);
    mMainLayout->addStretch();
    mMainLayout->addItem(mRightLayout);
    setLayout(mMainLayout);

    connect (this, &MainHeader::setCloseButtonEnable, this, [=] (bool b) {mCloseBtn->setEnabled (b);});
}

void MainHeader::onApplyData()
{
    mCloseBtn->setEnabled(false);
    mMaxBtn->setEnabled(false);
    mMinBtn->setEnabled(false);
}

void MainHeader::onApplyDataFinished()
{
    mCloseBtn->setEnabled(true);
    mMaxBtn->setEnabled(true);
    mMinBtn->setEnabled(true);
}

//void MainHeader::mouseMoveEvent(QMouseEvent* ev)
//{
//    if (mDrag) {
//        QPoint dist = ev->globalPos() - mMouseStartPoint;
//        qDebug() << ev->globalPos() << " -- " << dist << (mWindowTopLeftPoint + dist);
//        QWidget* parentt = qobject_cast<QWidget*>(parent());
//        if (parentt) {
//            parentt->move(mWindowTopLeftPoint + dist);
//        }
//    }
//}
//
//void MainHeader::mousePressEvent(QMouseEvent* ev)
//{
//    if (ev->button() == Qt::LeftButton) {
//        mDrag = true;
//        QWidget* parentt = qobject_cast<QWidget*>(parent());
//        if (parentt) {
//            mMouseStartPoint = ev->globalPos();
//            mWindowTopLeftPoint = parentt->frameGeometry().topLeft();
//        }
//    }
//}
//
//void MainHeader::mouseReleaseEvent(QMouseEvent* ev)
//{
//    if (ev->button() == Qt::LeftButton) {
//        mDrag = false;
//    }
//}

void MainHeader::mouseDoubleClickEvent(QMouseEvent *)
{
    qDebug() << "double clicked!";

    Q_EMIT windowMax();
}

//void MainHeader::setTitle(QString &title)
//{
//    mHeaderName->setText(title);
//}

void MainHeader::setTitle(const char *title)
{
    mHeaderName->setText(title);
}

void MainHeader::hideMin()
{
    mMinBtn->hide();
}

void MainHeader::showMin()
{
    mMinBtn->show();
}

void MainHeader::hideMax()
{
    mMaxBtn->hide();
}

void MainHeader::showMax()
{
    mMaxBtn->show();
}

