#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QFileSystemModel>
#include <QMainWindow>
#include <QWidget>

class DesktopView;
class QStandardItemModel;

class MainWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);

private:
    DesktopView*            mDesktopView = nullptr;
    QFileSystemModel*       mModel = nullptr;

};

#endif // MAINWIDGET_H
