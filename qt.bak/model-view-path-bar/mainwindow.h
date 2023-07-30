#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QHash>
#include <memory>
#include <QLineEdit>

class PathBarModel : public QStringListModel
{
    Q_OBJECT
public:
    explicit PathBarModel(QObject* parent = nullptr);

Q_SIGNALS:
    void updated ();

public Q_SLOTS:
    void setRootUri (QString& uri, bool force = false);

private:
    QString mCurrentUri = nullptr;


};

class PathEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit PathEdit (QWidget* parent = nullptr);

public Q_SLOTS:
    void setUri (const QString& uri);

protected:
    void focusInEvent (QFocusEvent* e) override;
    void focusOutEvent (QFocusEvent* ) override;
    void keyPressEvent (QKeyEvent* e) override;
    void mousePressEvent (QMouseEvent*) override;

private:
    PathBarModel*  mModel;

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
