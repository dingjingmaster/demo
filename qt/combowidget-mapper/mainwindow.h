#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class QDataWidgetMapper;
class QLabel;
class QLineEdit;
class QPushButton;
class QStandardItemModel;
class QStringListModel;
class QTextEdit;
QT_END_NAMESPACE

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);

private slots:
    void updateButtons(int row);

private:
    void setupModel();

    QLabel *nameLabel;
    QLabel *addressLabel;
    QLabel *typeLabel;
    QLineEdit *nameEdit;
    QTextEdit *addressEdit;
    QComboBox *typeComboBox;
    QPushButton *nextButton;
    QPushButton *previousButton;

    QStandardItemModel *model;
    QStringListModel *typeModel;
    QDataWidgetMapper *mapper;
};

#endif // MAINWINDOW_H
