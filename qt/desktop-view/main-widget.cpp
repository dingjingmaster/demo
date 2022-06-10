#include "main-widget.h"
#include "desktop-view.h"
#include <QStandardItemModel>

MainWidget::MainWidget(QWidget *parent) : QMainWindow(parent), mDesktopView (new DesktopView(this))
{
    setContentsMargins(0, 0, 0, 0);

    mModel = new QFileSystemModel(this);
    setCentralWidget(mDesktopView);

    mDesktopView->setModel(mModel);
    mDesktopView->setRootIndex(mModel->setRootPath("/home/dingjing/"));
}
