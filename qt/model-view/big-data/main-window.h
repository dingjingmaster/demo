#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <qglobal.h>

class TableView;
class TableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    TableView*              mView;
    TableModel*             mModel;
};
#endif // MAIN-WINDOW_H
