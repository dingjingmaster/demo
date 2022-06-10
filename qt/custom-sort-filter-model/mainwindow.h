#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QCheckBox;
class QComboBox;
class QDateEdit;
class QGroupBox;
class QLabel;
class QLineEdit;
class QTreeView;
QT_END_NAMESPACE
class MySortFilterProxyModel;
class FilterWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

    void setSourceModel(QAbstractItemModel *model);

private slots:
    void textFilterChanged();
    void dateFilterChanged();

private:
    MySortFilterProxyModel *proxyModel;

    QGroupBox *sourceGroupBox;
    QGroupBox *proxyGroupBox;
    QTreeView *sourceView;
    QTreeView *proxyView;
    QLabel *filterPatternLabel;
    QLabel *fromLabel;
    QLabel *toLabel;
    FilterWidget *filterWidget;
    QDateEdit *fromDateEdit;
    QDateEdit *toDateEdit;
};
#endif // MAINWINDOW_H
