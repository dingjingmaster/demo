#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QTableView; //forward declaration
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTableView *tableView;
public:
    MainWindow(QWidget *parent = nullptr);
public slots:
    void showWindowTitle(const QString &title);
};

#endif // MAINWINDOW_H
