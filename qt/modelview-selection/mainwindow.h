#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QTreeView; //forward declarations
class QStandardItemModel;
class QItemSelection;
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTreeView *treeView;
    QStandardItemModel *standardModel;
private slots:
    void selectionChangedSlot(const QItemSelection &newSelection, const QItemSelection &oldSelection);
public:
    MainWindow(QWidget *parent = nullptr);
};

#endif // MAINWINDOW_H
