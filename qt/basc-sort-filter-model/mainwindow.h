#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QSortFilterProxyModel;
class QTreeView;
QT_END_NAMESPACE

class Window : public QWidget
{
    Q_OBJECT
public:
    Window();
    void setSourceModel(QAbstractItemModel *model);

private slots:
    void filterRegularExpressionChanged(int);
    void filterColumnChanged(int);
    void sortChanged();

private:
    QSortFilterProxyModel *proxyModel;

    QGroupBox *sourceGroupBox;
    QGroupBox *proxyGroupBox;
    QTreeView *sourceView;
    QTreeView *proxyView;
    QCheckBox *filterCaseSensitivityCheckBox;
    QCheckBox *sortCaseSensitivityCheckBox;
    QLabel *filterPatternLabel;
    QLabel *filterSyntaxLabel;
    QLabel *filterColumnLabel;
    QLineEdit *filterPatternLineEdit;
    enum Syntax {
        RegularExpression,
        Wildcard,
        FixedString
    };

    QComboBox *filterSyntaxComboBox;
    QComboBox *filterColumnComboBox;
};
#endif // MAINWINDOW_H
