#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
}

MainWindow::~MainWindow()
{
}


PathBarModel::PathBarModel(QObject *parent) : QStringListModel(parent)
{
    //

}

void PathBarModel::setRootUri(QString &uri, bool force)
{
    mCurrentUri = uri;
    // 列出当前目录文件
}
